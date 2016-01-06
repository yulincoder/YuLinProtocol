/******************************************************************************************
 *  
 *	串口配置模块
 *
 *	[注]:由于未知原因，在系统启动之后发送的第一个数据会发生错误
 *       因此建议程序员在发送系统启动后，每一个将要使用的模拟串口应首
 *  	 先发送一个无关紧要的数据，以
 *  	 保证后面数据发送正确。
 */

 #include <REGX52.H>
 #include <intrins.h>
 #include "..\\include\\config.h"

 
/* 产生波特率定时器使能与否操作 */
 #define BAUD_TIMER_START(enable) TR0 = ##enable

 #define BAUD_4800 1
 #define BAUD_9600 2

 uchar_8 idata hand_cmd_buffer[9] = {0};
 uchar_8 idata hand_ack_buffer[9] = {0};

/* 等待一个byte数据传输结束 */
 #define WAIT_BYTE_OVER do{				 	 	\
 						while(!TF0);  	      	 \
     					TF0 = 0;		       	  \
												   \
					}while(0)


 #define TIMER_ENABLE() 	{				  	\
 								TL0=TH0;	     \
								TR0=1;		      \
								TF0=0;		       \
							}//使能T/C
 #define TIMER_DISABLE() 	{				 	\
 								TR0=0;		     \
								TF0=0;		      \
											       \
							}//禁止T/C   


 void set_baud_rate(uchar_8 rate)
 {
 	switch(rate){
		case 1: TH0 = (256-192);TL0 = TH0;break; 	//4800bps
		case 2: TH0 = (256-92) ;TL0 = TH0;break; 	//9600bps
		default:TH0 = (256-192);TL0 = TH0;break;
	}
	return;
 }

/* 系统进入传输模式 */    
 void into_serial_pattern(void)
 {
 	 ES  = 0;	            		/*打开串口中断*/
	 ET0 = 1;				 		/*定时器0允许*/
	 TR0 = 1; 		   		   		/*定时器0打开*/
	 ET1 = 0;   		   		    /*定时器1禁止*///暂时先打开 for debug
	 TR1 = 0; 		   		     	/*定时器1关闭*///暂时先打开 for debug
	 TR2 = 0; 						/*定时器2关闭*/
 }



/* 系统退出传输模式 */    
 void exit_serial_pattern(void)
 {
 	 ES  = 1;	            		/*打开串口中断*/
	 ET0 = 1;				 		/*定时器0允许*/
	 TR0 = 1; 		   		   		/*定时器0打开*/
	 ET1 = 1;   		   		    /*定时器1禁止*/
	 TR1 = 1; 		   		     	/*定时器1关闭*/
	 TR2 = 1; 						/*定时器2关闭*/
 }

  

/* 串口0发送一个字符 */ 
 void serial_0_write_byte(const uchar_8 rate,uchar_8 input)  
 {  
    uchar_8 i=8;
 
 	TXD_0=0;

	set_baud_rate(rate);		//设置波特率
 	into_serial_pattern();		//进入串口模式

 	TIMER_ENABLE();
 	WAIT_BYTE_OVER;
 
 	while(i--)
 	{
 		if( input&1 ){
			TXD_0=1;
 		}else{
			TXD_0=0;
 		}

 		WAIT_BYTE_OVER;

 		input>>=1;
 	}
 
 	TXD_0=1;	 //发送结束
 
 	WAIT_BYTE_OVER;
 	TIMER_DISABLE();

	exit_serial_pattern();		//退出串口模式
 }

 /* 串口1发送一个字符 */ 
 void serial_1_write_byte(const uchar_8 rate,uchar_8 input)  
 {  
    uchar_8 i=8;
 
 	TXD_1=0;

	set_baud_rate(rate);		//设置波特率
 	into_serial_pattern();		//进入串口模式

 	TIMER_ENABLE();
 	WAIT_BYTE_OVER;
 
 	while(i--)
 	{
 		if( input&1 ){
			TXD_1=1;
 		}else{
			TXD_1=0;
 		}

 		WAIT_BYTE_OVER;

 		input>>=1;
 	}
 
 	TXD_1=1;	 //发送结束
 
 	WAIT_BYTE_OVER;
 	TIMER_DISABLE();

	exit_serial_pattern();		//退出串口模式
 }




/* 串口发送byte数据  */
 void serial_write_byte(const uchar_8 rate,const uchar_8 serial_number,uchar_8 input)
 {

 /*
  * doc:
  *	"serial_number" is the number of serial,and "input" is the
  * will be sent date ,only byte, accross serial。
  */

  	switch( serial_number ){
  		case 0:	 serial_0_write_byte(rate,input);break;	   //串口0发送
		case 1:	 serial_1_write_byte(rate,input);break;	   //串口1发送

		default: break;
  	}

 }

/* RXD_0接收一个字符 */  
 uchar_8 serial_0_read_byte(const uchar_8 rate)  
 {  
    uchar_8 i;
 	uchar_8 read_byte=0;

	set_baud_rate(rate);		//设置波特率 
	into_serial_pattern();		//进入串口模式

   	TIMER_ENABLE();
   	WAIT_BYTE_OVER;

	/*
	 * 这段代码执行时间加上调用函数耗费的时间总共大约占一个byte传输时间的一半，
	 * 这样可以使得电平采样发生在每一个byte的中间
	 */
	 /* There are some codes deleted recently ,all of which are the function of _nop_().  */ 
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();


   	for(i=0;i<8;i++){
     	if(RXD_0){
 	  		read_byte |= (1<<i);
		}
 
     	WAIT_BYTE_OVER;
   	}
 
    WAIT_BYTE_OVER; //等待结束位
    TIMER_DISABLE();
 

	exit_serial_pattern();		//退出串口模式
 	return read_byte; 
}

/* RXD_1接收一个字符 */  
 uchar_8 serial_1_read_byte(const uchar_8 rate)  
 {  
    uchar_8 i;
 	uchar_8 read_byte=0;

	set_baud_rate(rate);		//设置波特率
	into_serial_pattern();		//进入串口模式

   	TIMER_ENABLE();
   	WAIT_BYTE_OVER;

	/*
	 * 这段代码执行时间加上调用函数耗费的时间总共大约占一个byte传输时间的一半，
	 * 这样可以使得电平采样发生在每一个byte的中间
	 */
	 /* There are some codes deleted recently ,all of which are the function of _nop_().  */ 


   	for(i=0;i<8;i++){
     	if(RXD_1){
 	  		read_byte |= (1<<i);
		}
 
     	WAIT_BYTE_OVER;
   	}
 
    WAIT_BYTE_OVER; //等待结束位
    TIMER_DISABLE();
 

	exit_serial_pattern();		//退出串口模式
 	return read_byte; 
}

/* 串口接收byte数据 */
 uchar_8 serial_read_byte(const uchar_8 rate,const uchar_8 serial_number)
 {
 /*
  * doc:
  *	"serial_number" is the number of serial,and "input" is the
  * will be sent date ,only byte, accross serial。
  */

  switch( serial_number ){
  	case 0:	 return serial_0_read_byte(rate);	   //串口0接收
	case 1:	 return serial_1_read_byte(rate);	   //串口1接收

	default: return 0;
  }
	 
 }


/* 输出数据流 */ 
 void print_stream(const uchar_8 data_size,const uchar_8 rate,const uchar_8 serial_number, uchar_8 * pstream)
 {
 	/**
	 **	现在是2015.12.31 23:36:57，马上跨年了，出去抽根烟。
	 **/

	/*
	 * 本函数由于发送过快将导致数据接收出错，所以不建议直接使用本函数
	 */
	 uchar_8 i =0;
	 for( i=0;i<data_size;i++ ){
	 	serial_write_byte(rate,serial_number, *(pstream+i) );
	 }
	 /*
	 * 这段代码不可以是因为它会打印从pstream开始到内存未定义区所有数据
 	while(pstream && *pstream){
 		serial_write_byte(serial_number, *pstream++ );
 	}*/
 }



 /* 外部数据流到来 */
 uchar_8 outside_data_coming(void)
 {
 	return (RXD_0 == 0);
 }  