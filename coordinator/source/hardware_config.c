/****************************************************************************************
 *	硬件配置模块
 *
 */

 /* REGX52.H必须放在第一个include */
 #include <REGX52.H>			
 #include "..\\include\\config.h" 
 #include "..\\include\\hardware_config.h"
 #include "..\\include\\serial.h" 
 #include "..\\include\\environment.h"
 #include "..\\include\\req_son_data.h" 
 #include "..\\include\\rcv_father_cmd.h" 
 #include "..\\include\\serach_sons.h" 

 uint_16 debug_cnt = 0;

/* 系统滴答 */
 uint_16  sys_tick = 0;

/* 系统启动延时函数 */
/* void sys_launch_delay(void)
 {
	unsigned char 	i,
					j;

	for( i=0; i<200; i++ )
		for( j=0; j<210; j++ )
		;
	return;
 }*/

/* 加电自检 */
/* void power_on_self_test(void)
 {
 	//ROUTER_RCVI_PIN_1 = 0;
//	ROUTER_SEND_PIN_1 = 1;
	/* do something	*/
/*	return;
 } */

/*
 * 定时器配置
 */
 void timer_config(void)
 {
	 TMOD=0x12;					//T0方式2,T1方式1

	 T2MOD |= 0x00;				//定时器2工作模式寄存器 部分头文件中没有添加 sfr T2MOD  = 0xC9;
	 T2CON  = 0x30; 			//定时器2控制寄存器
	 SCON   = 0x50;  			//串口中断工作模式1
	 RCAP2H = 0xff; 			//定时器2自动装载的初值
	 RCAP2L = 0xdc;

	 /*
	  *		以下代码是对定时器0做配置，定时器0是用来产生串口的波特率，
	  * 因此下面这段配置就相当重要，TH0 = (256 - 192)，这是一个经过实
	  * 践调节选定的值，理论值为96。并且
	  */

	 TR0=0;
 	 TF0=0;
     TH0=(256-192);				/* 失之毫厘，谬以千里*/
     TL0=TH0;


	 TH1 = 0xFC;  				//T1定时器11.0592M 定时1ms
     TL1 = 0x66;;
	 TH2 = 0xff;	 			//定时器2产生9600波特率初值
	 TL2 = 0xdc;

	 ES  = 1;	 				//打开串口中断
	 ET1 = 1; 					//定时器1允许
   	 ET0 = 1; 					//定时器0允许
	 TR0 = 1; 					//定时器0打开
	 TR1 = 1; 					//定时器1打开
	 TR2 = 1; 					//定时器2打开

     ET0=1;

	 /* 设定定时器0为最高优先级，这个值改变将导致系统工作不稳定 */
	 IP = 0x02;	  				
     EA=1;
 }	 



/* 硬件初始化 */
 void hardware_init(void)
 {
	GPIO_1(0) = 0;
	GPIO_1(1) = 1;
	GPIO_1(2) = 1;
	GPIO_1(3) = 1;
	GPIO_1(4) = 1;
	GPIO_1(5) = 1;
	GPIO_1(6) = 1;
	GPIO_1(7) = 1;
	/* 定时器配置 */
	timer_config();

	return;
 }

 void timer_0_IRQ(void) interrupt 1 using 0 
 {
 	return;	
 }

 bit aaa = 0;
  void timer_1_IRQ(void) interrupt 3   
 {
 	TH1 = 0xFC;
    TL1 = 0x66;	
	

	//完了应该改回来
	//byte_time_out_cnt ++;
	//timeout_cnt ++;

 	sys_tick ++;
	/* 系统滴答计时1ms  */
	if( sys_tick >= 999 ){
		sys_tick = 0;

		byte_time_out_cnt ++;
		timeout_cnt ++;

		debug_cnt ++;
		aaa = ~aaa;	
 		GPIO_0(1) = aaa; 
	}		   
	return;	
 } 

/* 硬件环境启动 */
 void hardware_launch(void)
 {
    //power_on_self_test();
 	hardware_init();

	return;
 } 

 /* 本模块测试功能函数 */
/* void test_hardware_moudle(void)
 {
	//test_pin_6 = 0;
 	//test_pin_7 = 0;
 	/* do somthing */
 //	return;
 //}*/

