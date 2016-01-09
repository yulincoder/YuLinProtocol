/***********************************************************************************
 *	
 * 本部分code是接收来自父节点的命令并返回相应数据或者命令给父节点
 * 这段代码是task1
 *
 */

 #include "..\\include\\config.h" 
 #include "..\\include\\hardware_config.h"
 #include "..\\include\\serial.h" 
 #include "..\\include\\environment.h"

 #define TIMEOUT 100		//等待数据接收超时时间(ms)

 /* 向父节点发送数据队列所有内容 */
 void send_queue_to_father(const uchar_8 rate)
 {
 	//  while( !queue_estimate_empty() ){
	 // 	queue_read(); 		//将队列数据读到缓存区
	 // 	if( data_processed.dirty == 0 ){	
			print_stream(sizeof(data_processed.cmd),rate,FATHER_PORT,data_processed.cmd);			 
	  		data_processed.dirty = 1;		//缓冲区数据已脏
	//	}
	//  }
 }

 void rcv_father_data(const uchar_8 rate)
 {
 	uchar_8 i = 0;
	uchar_8 data_ack_cnt = 0;
	
		
 	if( outside_data_coming() ){

		for( i=0;i<9;i++){				
			serial_buffer[i] = serial_read_byte(0,rate);

			while( (!outside_data_coming()) && (i < MESSAGE_LENGTH - 1) );  //等待下一byte数据
		  	/*timeout_cnt = 0;	//	超时计数器
			while( (timeout_cnt < TIMEOUT) && (!outside_data_coming()) && (i < MESSAGE_LENGTH - 1) );  //等待下一byte数据
		*/}

		 
		for( i=0;i<MESSAGE_LENGTH;i++ ){
			if( serial_buffer[i] == hand_cmd[i] )
				data_ack_cnt ++;
		}

		if( data_ack_cnt == MESSAGE_LENGTH ){
			print_stream(sizeof(hand_ack),rate,FATHER_PORT,hand_ack);	//返回握手确认命令
		}else{
			data_ack_cnt = 0;

			for( i=0;i<MESSAGE_LENGTH;i++ ){
				if( serial_buffer[i] == data_request[i] )
					data_ack_cnt ++;
			}

			if( data_ack_cnt == MESSAGE_LENGTH ){
				//发送队列所有数据给父节点();
				if( !queue_estimate_empty() ){
					send_queue_to_father(rate);
				}else{
					print_stream(sizeof("queue emptied"),rate,FATHER_PORT,"queue emptied");	
				}
			}else{
				print_stream(sizeof("command error"),rate,FATHER_PORT,"command error");
			} 
		}  	  
   	 }
  }

 















