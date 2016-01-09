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

/* 发送握手包 */
 void send_hand_cmd(const uchar_8 rate, const uchar_8 linked_port )
 {
 	/*
	 * 这个延时函数是因为我发现不加他数据发送太快，串口助手
	 * 显示不过来(但是确实收到了)
	 */
 	delay_for_debug(); 
 	print_stream(sizeof(hand_cmd),rate,linked_port,hand_cmd);	//发送握手包
//	
 }


 void link_son(const uchar_8 rate,const uchar_8 linked_port)
 {
 	uchar_8 i = 0;

  	for( i=0;i<FREQUENCY_LINKED;i++ ){

		send_hand_cmd(rate,linked_port);	//发送握手包

		timeout_cnt = 0;
		while( timeout_cnt < 5 ){
			 		
			if( outside_data_coming( linked_port ) ){

				for( i=0;i<MESSAGE_LENGTH;i++){				
					serial_buffer[i] = serial_read_byte(rate,linked_port);

					byte_time_out_cnt = 0;
				//	while( (!outside_data_coming(linked_port)) && (i < MESSAGE_LENGTH - 1) );  //等待下一byte数据
				
					while( (byte_time_out_cnt < 1) && (!outside_data_coming(linked_port)) && (i < MESSAGE_LENGTH - 1) );  //等待下一byte数据
				}

				if( (serial_buffer[0]==hand_ack[0]) && (serial_buffer[1] ==hand_ack[1]) ){
				
					// 调试完需要删除
					print_stream(sizeof("握手成功"),rate,linked_port,"握手成功");

					goto out;
				}	
			}			
		}
	}  
	out: return;	
 }	


 void serach_sons( const uchar_8 rate )
 {
 	uchar_8 number_port = 0;

	// 调试完number_port需要从1开始
 	for( number_port=0;number_port<SONS_AMOUNT;number_port++){

		switch(number_port){

			//port 0为与父节点的链路
			case 0:	print_stream(sizeof("暂时查找父亲:"),rate,0,"暂时查找父亲:");link_son(rate,number_port); break; 
			case 1:	print_stream(sizeof("查找儿子1:"),rate,0,"查找儿子1:");link_son(rate,number_port); break; 
		  	case 2: print_stream(sizeof("查找儿子2:"),rate,0,"查找儿子2:");link_son(rate,number_port); break;
			case 3: break;
			default : break;
		}
		//调试时加这个函数，显示不会乱码
			delay_for_debug();
	}		
 }



