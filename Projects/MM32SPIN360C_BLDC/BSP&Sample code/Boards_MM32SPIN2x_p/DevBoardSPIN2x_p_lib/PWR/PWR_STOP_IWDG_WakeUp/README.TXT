实验器材:
	Mini Board
	
实验目的:
	学习IWDG wake Stop的使用
	
硬件资源:
	1,LED1(PB5),LED2(PB4),LED3(PB3),LED4(PA15) 
	2,WK_UP(PA0),KEY3(PB10),KEY4(PB11)
	3,UART1(PA9,PA10)

	
实验现象:
	上电后4个LED被点亮，持续闪烁5次后，进入Stop，而后过IWDG设定的6.25s，LED 1闪烁，说明MCU已从STOP模式唤醒 ，如此反复
	
注意事项:  

	