/** \file	ir_receiver.cpp
	\brief	紅外線接收
*/

#include <stdio.h>
#include <string.h>
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
		/// - 每收到狀態變化，以 toggle LD2 指示
		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);

		IrSignal signal = get();

		/// - 信號寬度最大為 9 ms，如果信號間隔 > 10 ms，視為另一個新的 IR code
		uint32_t pulseWidth = signal.time - preSignalTick;
		if (pulseWidth > MAX_STATE_TIME)
		{
			pulseWidth = 0;
//			printf("\n");
		}

		/// - 降緣 = SPACE 狀態 (無 IR 信號) 結束
		/// - 升緣 = MARK 狀態 (有 IR 信號) 結束
		/// - 顯示一行 = 17 bytes, 以 115,200 bps 的傳輸速度計，費時約 1.62 ms
#if 0
		if (signal.state == SPACE)
			printf("%3lu (M) ", pulseWidth);
		else
			printf("%3lu (S)\n", pulseWidth);
#endif

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
//				ssd1306_UpdateScreen(&oled_i2c);
			}
			else
				ir_sender.SendCode(code);
		}
	}
}

/**	\brief	Show received IR code.
 *
 *	\param[in]	state	FSM 的狀態
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

		/// - 收到遙控器的 IR code 顯示於 OLED 的第一行
		int displayLine = 0;

		/// - 收到中繼傳輸的 IR code 顯示於 OLED 的第七行
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

			/// - 收到 HDR Player 遙控器的 IR code，且 Relay 設為 Enable 時，送出 CASIO 投影機遙控器同按鈕名稱的 IR code。
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
		/// - 收到 Repeat code 時，以 pixel (點) 顯示
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
	else if (state == DAIKIN_DONE)	/// - 大金冷氣遙控器不是 NEC 格式，程式還不能解碼
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

//	ssd1306_UpdateScreen(&oled_i2c);

	ResetState();
}

/** \brief	Process IR signal.
 *
 * \param[in]	pulseWidth 信號寬度 (單位 : 10 us)
 *
 * 根據 NEC 協定，使用有限狀態機 (Finite State Machine, FSM) 檢查訊號時間寬度
*/
IR_State	IR_Receiver::proc_IRsignal(int pulseWidth)
{
	m_pulseWidth = pulseWidth;

	switch(m_state)
	{
	case ST_START:
		/// - 前導碼 = 16T + 8T (9 + 4.5 ms)
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
		/// - 前導碼 = 9T + 4T 為 Repeat code
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
		/// - T + T = Logical 0
		/// - T + 3T = Logical 1
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
//				printf("\n");
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

/** \brief	收到合規的 IR code，搜尋已定義的遙控器及按鈕。
 *
*/
int		IR_Receiver::check_IRcode(void)
{
	/// - 第 2、3 byte 需互為補數，所以 IR_Base::m_ircode 只記第 2 byte
	if (m_datacode[2] + m_datacode[3] != 0xff)
		return -1;

	m_ircode = m_datacode[2];

	/// - 第 0、1 byte 組成 custom code
	uint16_t customcode = m_datacode[0] | (m_datacode[1] << 8);

	/// - IR_CodeTable::SearchControllers 搜尋所有定義的遙控器
	bool bFound = IR_CodeTable::SearchControllers(customcode, m_ircode, &m_pControllerName, &m_pkeyname);

	if (bFound)
		return 1;

	/// - 如果符合 NEC 格式，顯示未定義的 IR code
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
