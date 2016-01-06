/*********************************************************************************
 *
 * 这部分code是遍历所有链路， 发现新连接的节点则添加节点
 * 发现原来连接节点断开，则kill it
 *
 */
 #include <REGX52.H>
 #include "..\\include\\config.h" 
 #include "..\\include\\hardware_config.h"
 #include "..\\include\\serial.h" 
 #include "..\\include\\environment.h"

 #define FREQUENCY_LINKED 3	//每一个遍历一次请求连接次数
 
 
 extern void delay_for_debug(void);

 uint_16 byte_time_out_cnt = 0;

 void send_hand_cmd( const uchar_8 linked_port )
 {
 	/*
	 * 这个延时函数是因为我发现不加他数据发送太快，串口助手
	 * 显示不过来(但是确实收到了)
	 */
 	delay_for_debug(); 
 	print_stream(sizeof(hand_cmd),1,linked_port,hand_cmd);	//发送握手包
//	
 }


 void link_son(const uchar_8 rate,const uchar_8 linked_port)
 {
 	uchar_8 i = 0;

 	for( i=0;i<FREQUENCY_LINKED;i++ ){
		send_hand_cmd(linked_port);

		timeout_cnt = 0;
		while( timeout_cnt < 50 ){

			if( outside_data_coming() ){

				for( i=0;i<9;i++){				
					serial_buffer[i] = serial_read_byte(rate,0);
		  	//		print_stream(sizeof("收到"),0,"收到");

					byte_time_out_cnt = 0;
					while( (byte_time_out_cnt < BYTE_TIME_OUT) && (!outside_data_coming()) && (i < MESSAGE_LENGTH - 1) );  //等待下一byte数据
				}
			}			
		}	
	}	
 }	


 void serach_sons( void )
 {
 	uchar_8 number_port = 0;

 	for( number_port=1;number_port<SONS_AMOUNT;number_port++){

		switch(number_port){
			//port 0为与父节点的链路
			case 1: break; 
		  	case 2: break;
			case 3: break;
			default : break;
		}
	}		
 }



