4/13如何從單板電腦接 出來
Smart home system
Pinout PG10 右鍵改成GPIO EXTI10
Categories 的GPIO選 pull-up
User label改成IRDA

NVIC enable打勾
Generate code
Debug鐵鎚按下去
接下來要知道如何從startup code裡面攔截
去startup裏面找callback
從vector table 裡面找Interrupt
去查看.word 的資料找到EXTI15_10
用ctrl+H search

找到該函式 繼續追蹤被叫的函式ctrl+h
把該函式複製貼上到usermain.cpp

4/25
Tim6
去看clock最右邊看數值是多少
去改prescaler 一秒鐘trigger一次
老師timer的目標是 10 microsecond/ trigger一次
Timer6 的hz 假設72 算週期 1/72mega = 0.0138
照原本的timer trigger太快
所以prescaler
1 microsecond要十次才會變成10microsecond
第二個是9所以要扣1

4/27
Ioc connectivity Enable i2C1 mode
把ssd1306資料放入
Configuration debug release移除後先編譯
去ioc查看scl,sca 在哪個腳位
並對應到第一章講義CN7 CN10的哪個位置
￼
