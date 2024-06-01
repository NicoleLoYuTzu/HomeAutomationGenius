全域變數與外部變數
IR_STATE g_waitState;：全域變數，用於儲存當前的紅外線接收狀態。
IR_Receiver ir_receiver;：紅外線接收器物件。
IR_Sender ir_sender;：紅外線發射器物件。
extern IR_CodeTable hdrplayer;：外部變數，表示一個紅外線碼表，用於發送特定的紅外線控制碼。
usermain 函數
這是主功能函數，執行以下操作：

啟動定時器：設置定時器htim6以10微秒為間隔產生中斷。
初始化等待狀態：根據IR接收引腳的狀態設置初始等待狀態（MARK或SPACE）。
初始化OLED顯示：呼叫init_oled()函數初始化OLED顯示。
啟動PWM：在定時器htim3的通道3上啟動PWM。
主循環：不斷閃爍LED1並處理紅外線信號接收。
HAL_TIM_PeriodElapsedCallback 函數
這是定時器中斷回調函數，當定時器htim6溢出時被呼叫：

增加系統滴答計數：每10微秒增加一次計數。
處理紅外線發射：呼叫ir_sender.processing()函數以處理紅外線發射。
HAL_GPIO_EXTI_Callback 函數
這是外部中斷回調函數，根據觸發中斷的GPIO引腳執行不同操作：

IR接收引腳：更新紅外線接收器狀態並切換等待狀態。
按鈕引腳：發送特定的紅外線控制碼（例如“POWER”）。
init_oled 函數
初始化OLED顯示器，並在屏幕上顯示一些初始信息：

初始化OLED：使用I2C接口初始化OLED顯示。
顯示訊息：顯示“IR code Test”字樣和一些字符。
更新屏幕：將本地緩衝區的數據刷新到OLED顯示器上。
flash_LD1 函數
以1Hz頻率閃爍LED1：

檢查忙等待標誌：若為忙等待模式，使用HAL_Delay()函數等待0.5秒。
非忙等待模式：檢查系統滴答計數，若已經過0.5秒則切換LED1狀態。
__io_putchar 函數
重定向printf函數的輸出到USART3：

傳輸字符：使用UART傳輸函數將字符發送到串口。
