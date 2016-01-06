 #define MCU8051 

 #ifdef MCU8051	 
 /*
  *	本代码段定义是与8051单片机硬件相关定义
  */

    #ifndef  uchar_8
 		#define uchar_8  unsigned char
	#endif
	#ifndef  uint_16
 		#define uint_16  unsigned int
	#endif

	#define GPIO_0(pin)  P0_##pin
	#define GPIO_1(pin)  P1_##pin

 #endif/* 8051定义结束 */


 /**		
  **		袁凯江水三千里，
  **		家书十五行。
  **		行行无别语，
  **		只道早还乡。
  **				-- 2016/01/04 01:16
  **/


 #ifndef __CONFIG_H__
 #define __CONFIG_H__
 	
	/*  串口0引脚  */
	#define RXD_0 GPIO_1(0) 					//接收数据的引脚
  	#define TXD_0 GPIO_1(1) 					//发送数据的引脚

	/*  串口1引脚  */
	#define RXD_1 GPIO_1(2) 					//接收数据的引脚
  	#define TXD_1 GPIO_1(3) 					//发送数据的引脚

	#define FATHER_PORT 0						//连接父节点端口
	#define SON_PORT(son)  son						//连接第一个孩子端口

  //#define TASK_WAIT 1 			//任务等待
  //#define TASK_OVER 0			//任务已执行

  	#define SON_CONNECTED 1		//子节点连接状态
  	#define SON_DISCONNECTED 0	//子节点断开状态

	#define SONS_AMOUNT   2		//允许连接子节点最大数量

 #endif //__CONFIG_H__