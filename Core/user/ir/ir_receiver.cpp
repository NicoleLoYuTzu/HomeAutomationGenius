/** \file	ir_receiver.cpp
	\brief	IR code receiver.
*/

#include <stdio.h>
#include <math.h>

#include "gpio.h"
#include "i2c.h"
#include "../../ssd1306/ssd1306.h"

#include "ir_receiver.h"
#include "ir_code.h"

IR_CodeTable mod("MOD", MOD, mod_codetable, sizeof(mod_codetable));

IR_CodeTable fan("FAN", FAN, fan_codetable, sizeof(fan_codetable));

IR_CodeTable hdrplayer("HDR player", HD_PLAYER, hdrplayer_codeTable, sizeof(hdrplayer_codeTable));

IR_CodeTable tochibatv("Toshiba TV", ToshibaTV, toshibatv_codeTable, sizeof(toshibatv_codeTable));

IR_CodeTable tochibavcr("Toshiba VCR", ToshibaVCR, toshibavcr_codeTable, sizeof(toshibavcr_codeTable));

IR_CodeTable pioneerdvd("Pioneer DVD", PioneerDVD, pioneerdvd_codeTable, sizeof(pioneerdvd_codeTable));

/**	\brief	Process received IR code.
 *
 */
void	IR_Receiver::processing()
{
	//	process signal from remote controller
	if (m_size > 0)
	{
		// toggle LD2
		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);

		IrSignal signal = get();

		// if pulse width > 10 ms, it may mean a new code
		uint32_t pulseWidth = signal.time - preSignalTick;
		if (pulseWidth > MAX_STATE_TIME)
		{
			pulseWidth = 0;
			printf("\r\n");
		}

		// falling edge = end of SPACE state
		// rising edge = end of MARK state
		// a line = 17 bytes, take about 1.62 ms for 115200 bps
		if (signal.state == SPACE)
			printf("%3lu (M)\r\n", pulseWidth);
		else
			printf("%3lu (S) ", pulseWidth);

		IR_State state = proc_IRsignal(pulseWidth);
		if ((state == NEC_DONE) || (state == NEC_DONE_REPEAT) || (state == DAIKIN_DONE))
			ShowIrCode(state);

		preSignalTick = signal.time;
	}
}

/**	\brief	Show received IR code.
 *
 */
void	IR_Receiver::ShowIrCode(IR_State state)
{
	static int nBtn = 0;
	static int repeatX = 0;
	static int repeatY = 32;
	static SSD1306_COLOR pixelColor = White;

	if (state == NEC_DONE)
	{
		uint16_t irCode = GetCode();
		const char *pController = GetController();
		const char *pBtnName = GetBtnName();

		ssd1306_Fill(Black);

		ssd1306_SetCursor(0, 0);
		ssd1306_printf(Font_5x7, White, "%-8s", pController);

		ssd1306_SetCursor(0, 8);
		ssd1306_printf(Font_5x7, White, "%03d %04X %-10s", ++nBtn, irCode, pBtnName);

		repeatX = 0;
		repeatY = 16;
	}
	else if (state == NEC_DONE_REPEAT)
	{
		ssd1306_DrawPixel(repeatX, repeatY, pixelColor);

		repeatX += 2;
		if (repeatX >= SSD1306_WIDTH)
		{
			repeatX = 0;
			repeatY += 2;
			if (repeatY >= SSD1306_HEIGHT)
			{
				repeatX = 0;
				repeatY = 16;
				pixelColor = (pixelColor == White) ? Black : White;
			}
		}
	}
	else if (state == DAIKIN_DONE)
	{
		ssd1306_Fill(Black);

		int y = 0;
		for (int i=0; i<19; i++)
		{
			if ((i % 7) == 0)
			{
				ssd1306_SetCursor(0, y);
				y += 8;
			}

			ssd1306_printf(Font_5x7, White, "%02X ", m_datacode[i]);
		}
	}

	ssd1306_UpdateScreen(&oled_i2c);

	ResetState();
}

/** \brief	Process IR signal.
	The IR signal is process by a state machine.
*/
IR_State	IR_Receiver::proc_IRsignal(int pulseWidth)
{
	m_pulseWidth = pulseWidth;

	switch(m_state)
	{
	case ST_START:
		if (about(16*NEC_TICK))			// 9 ms
			m_state = NEC_LEADING;
		else if (about(8*DAIKIN_TICK))	// 3.5 ms
			m_state = DAIKIN_LEADING;
		break;

	case NEC_LEADING:
		if (about(8*NEC_TICK))			// 4.5 ms
		{
			m_bitNo = 0;
			m_byteNo = 0;
			m_state = NEC_DATAFRAME;
		}
		else if (about(4*NEC_TICK))		// 2.25 ms
			m_state = NEC_REPEATCODE;
		else
			m_state = ST_START;
		break;

	case NEC_DATAFRAME:
		if (about(NEC_TICK)) 			// 0.56 ms
			m_state = NEC_DATABIT;
		else
			m_state = ST_START;	// frame error
		break;

	case NEC_DATABIT:
		if (about(NEC_TICK) || about(3*NEC_TICK))	// 0.56 ms or 1.68 ms
		{
			m_datacode[m_byteNo] >>= 1;
			if (about(NEC_TICK))
				m_datacode[m_byteNo] &= 0x7f;	// bit = 0
			else
				m_datacode[m_byteNo] |= 0x80;	// bit = 1
			m_state = NEC_DATAFRAME;
			if (++m_bitNo == 8)
			{
				m_bitNo = 0;
				if (++m_byteNo == 4)
					m_state = check_IRcode() ? NEC_DONE : ST_START;
			}
		}
		else
			m_state = ST_START;
		break;

	case NEC_REPEATCODE:
		if (about(NEC_TICK)) 			// 0.56 ms
			m_state = NEC_DONE_REPEAT;
		else
			m_state = ST_START;
		break;

	case DAIKIN_LEADING:
		if (about(4*DAIKIN_TICK))			// 1.76 ms
		{
			m_bitNo = 0;
			m_byteNo = 0;
			m_state = DAIKIN_DATAFRAME;
		}
		else
			m_state = ST_START;
		break;

	case DAIKIN_DATAFRAME:
		if (about(DAIKIN_TICK)) 			// 0.44 ms
			m_state = DAIKIN_DATABIT;
		else
			m_state = ST_START;	// frame error
		break;

	case DAIKIN_DATABIT:
		if (about(DAIKIN_TICK) || about(3*DAIKIN_TICK))	// 0.44 ms or 1.32 ms
		{
			m_datacode[m_byteNo] >>= 1;
			if (about(DAIKIN_TICK))
				m_datacode[m_byteNo] &= 0x7f;	// bit = 0
			else
				m_datacode[m_byteNo] |= 0x80;	// bit = 1
			m_state = DAIKIN_DATAFRAME;
			if (++m_bitNo == 8)
			{
				m_bitNo = 0;
				if (++m_byteNo == 19)
					m_state = DAIKIN_DONE;
			}
		}
		else
			m_state = ST_START;
		break;

	default:
		break;
	}

	return m_state;
}

/**	\brief	Check pulse width.
 *
 */
bool	IR_Receiver::about(int nTick)
{
	return abs(m_pulseWidth - nTick) < TIME_ERROR;
}

/** \brief	Check for IR code.

	The received IR code is checked against a code table that is
	retrieved from a real IR remote controller.
*/
bool	IR_Receiver::check_IRcode(void)
{
	if (m_datacode[2] + m_datacode[3] != 0xff)
		return false;

	m_ircode = m_datacode[2];
	uint16_t customcode = m_datacode[0] | (m_datacode[1] << 8);

	switch(customcode)
	{
	case MOD:
		return mod.FindKey(m_ircode, &m_pControllerName, &m_pkeyname);

	case FAN:
		return fan.FindKey(m_ircode, &m_pControllerName, &m_pkeyname);

	case HD_PLAYER:
		return hdrplayer.FindKey(m_ircode, &m_pControllerName, &m_pkeyname);

	default:
		if ((m_datacode[0] + m_datacode[1] == 0xff) ||
 			(m_datacode[0] == m_datacode[1]))
		{
			m_ircode |= ((uint16_t) m_datacode[0] << 8);
			m_pControllerName = "Unknown";
			m_pkeyname = "Unknown";
			return true;
		}
	}

	return false;
}
