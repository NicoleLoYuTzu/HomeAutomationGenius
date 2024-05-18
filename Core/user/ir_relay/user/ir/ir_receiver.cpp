/** \file	ir_receiver.cpp
	\brief	IR code receiver.
*/

#include <stdio.h>
#include <math.h>

#include "gpio.h"
#include "i2c.h"
#include "ssd1306.h"

#include "ir_receiver.h"
#include "ir_sender.h"
#include "ir_code.h"

bool g_bEnableRelay = false;

extern IR_CodeTable mod;
extern IR_CodeTable casio;

IR_CodeTable *pRelayTarget = &casio;

/**	\brief	Constructor.
 *
 */
IR_Receiver::IR_Receiver()
{
	m_bWaitToRelay = false;
}

/**	\brief	Process received IR code.
 *
 */
void	IR_Receiver::processing()
{
	static uint32_t m_lastSignalTime = 0;

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
			printf("%3lu (M) ", pulseWidth);
		else
			printf("%3lu (S)\r\n", pulseWidth);

		IR_State state = proc_IRsignal(pulseWidth);
		if ((state == NEC_DONE) || (state == NEC_DONE_REPEAT) || (state == DAIKIN_DONE))
			ShowIrCode(state);

		preSignalTick = signal.time;
		m_lastSignalTime = HAL_GetTick();
	}

	if (m_bWaitToRelay)
	{
		uint32_t elapsed = HAL_GetTick() - m_lastSignalTime;
		if (elapsed > 10)
		{
			uint32_t code = pRelayTarget->GetIrCode(m_pRelayBtnName);
			if (code == 0)
			{
				m_bWaitToRelay = false;
				ssd1306_SetCursor(0, 56);
				ssd1306_printf(Font_5x7, White, "No match button");
				ssd1306_UpdateScreen(&oled_i2c);
			}
			else
				ir_sender.SendCode(code);
		}
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

		int displayLine = 0;

		if (m_bWaitToRelay)
		{
			displayLine = 8 * 6;
			m_bWaitToRelay = false;
		}
		else
		{
			ssd1306_Fill(Black);

			showRelayState();

			repeatX = 0;
			repeatY = 16;

			if (g_bEnableRelay && strcmp(pController, "HDR player") == 0)
			{
				m_bWaitToRelay = true;
				m_pRelayBtnName = pBtnName;
			}
		}

		ssd1306_SetCursor(0, displayLine);
		ssd1306_printf(Font_5x7, White, "%-8s", pController);

		ssd1306_SetCursor(0, displayLine+8);
		ssd1306_printf(Font_5x7, White, "%03d %04X %-10s", ++nBtn, irCode, pBtnName);
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
				printf("\r\n");
				m_bitNo = 0;
				if (++m_byteNo == 4)
					m_state = (check_IRcode() >= 0) ? NEC_DONE : ST_START;
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
int		IR_Receiver::check_IRcode(void)
{
	if (m_datacode[2] + m_datacode[3] != 0xff)
		return -1;

	m_ircode = m_datacode[2];
	uint16_t customcode = m_datacode[0] | (m_datacode[1] << 8);

	bool bFound = IR_CodeTable::FindKey(customcode, m_ircode, &m_pControllerName, &m_pkeyname);

	if (bFound)
		return 1;

	if ((m_datacode[0] + m_datacode[1] == 0xff) ||
		(m_datacode[0] == m_datacode[1]))
	{
		m_ircode |= ((uint16_t) m_datacode[0] << 8);
		m_pControllerName = "Unknown";
		m_pkeyname = "Unknown";
		return 0;
	}

	return -1;
}

/**	\brief	Show Relay state.
 *
 */
void	showRelayState()
{
	ssd1306_SetCursor(14*Font_5x7.FontWidth, 0);
	ssd1306_printf(Font_5x7, White, "Relay %c", g_bEnableRelay ? 'v' : '_');
}
