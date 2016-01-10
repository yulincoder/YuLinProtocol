/***********************************************************************************
 *	
 * 本部分code是接收来自父节点的命令并返回相应数据或者命令给父节点
 * 这段代码是task1
 *
 */
 #include <REGX52.H>
 #include "..\\include\\config.h" 
 #include "..\\include\\hardware_config.h"
 #include "..\\include\\serial.h" 
 #include "..\\include\\environment.h"


 void send_delay(const uchar_8 delay_time )
 {
 	uchar_8 i,
			j;

	for( i=0;i<delay_time;i++ ){
		for( j=0;j<20;j++ ){
			;
		}
	}	
 }

 /* 向父节点发送数据队列所有内容 */
 void send_queue_to_father(const uchar_8 rate)
 {
 	 if( !queue_estimate_empty() ){

		while( !queue_estimate_empty() ){

	  		queue_read(); 		//将队列数据读到缓存区
	 		if( data_processed.dirty == 0 ){	
				print_stream(sizeof(data_processed.cmd),rate,FATHER_PORT,data_processed.cmd);			 
	  			data_processed.dirty = 1;		//缓冲区数据已脏
			}

			send_delay(20);
	  }
	 }else{
			print_stream(sizeof("queue emptied"),rate,FATHER_PORT,"queue emptied");	
	 }
 	  
	  
 }

/* 从父节点接收数据 */
 void rcv_father_data(const uchar_8 rate)
 {
 	uchar_8 i = 0;
			
 	if( outside_data_coming( FATHER_PORT ) ){

		for( i=0;i<MESSAGE_LENGTH;i++){				
			serial_buffer[i] = serial_read_byte(rate,FATHER_PORT);

		//	while( (!outside_data_coming( FATHER_PORT )) && (i < MESSAGE_LENGTH - 1) );  //等待下一byte数据
		  	
			timeout_cnt = 0;	//	超时计数器
			while( (timeout_cnt < WAIT_DATA_TIMEOUT ) && (!outside_data_coming(FATHER_PORT)) && (i < MESSAGE_LENGTH - 1) );  //等待下一byte数据
		 }

		 // print_stream(sizeof(serial_buffer),rate,FATHER_PORT,serial_buffer);	 
		if( (serial_buffer[0] == hand_cmd[0]) && (serial_buffer[1] == hand_cmd[1])){

			rcv_father_hand_creat_host_id(serial_buffer);	//提取父节点id并生成本机id
			id_insert_data();
			//握手包数据
			hand_ack[8] = (hand_ack[0] + hand_ack[1] + hand_ack[2] + hand_ack[3] + hand_ack[4] + hand_ack[5] );
			print_stream(sizeof(hand_ack),rate,FATHER_PORT,hand_ack);	//返回握手确认命令
			print_stream(sizeof(hand_ack),rate,1,hand_ack);	//返回握手确认命令

			sons_status.linked_father = FATHER_CONNECTED;
			clean_serial_buffer();
		}else if( (serial_buffer[0] == data_request[0]) && (serial_buffer[1] == data_request[1])){
			//数据请求包
		  	send_queue_to_father(rate);	//发送队列所有数据
		}else{
			//print_stream(sizeof("command error"),rate,FATHER_PORT,"command error");	
		}	 	  	  
   	 } 
  }


 void temporary_rcv_sons(const uchar_8 rate)
 {
 	uchar_8 i = 0,
			f1 = 0;
	timeout_cnt = 0;	//	超时计数器
	if( outside_data_coming( PORT(1) ) ){
		f1 = 1;
		for( i=0;i<MESSAGE_LENGTH;i++){				
			serial_buffer[i] = serial_read_byte(rate,PORT(1));
			
			while(  (!outside_data_coming(PORT(1))) && (i < MESSAGE_LENGTH - 1) );  //等待下一byte数据
			if( timeout_cnt > WAIT_DATA_TIMEOUT + 500 ){
				goto out;
			}
		}	
	} 
	if( f1 == 1){
		f1 = 0;
		print_stream(sizeof(serial_buffer),BAUD_9600,0,serial_buffer);
	}
	out: f1 = 0;			
 }
extern void delay_for_debug(void);
 void  temporary_send_data(const uchar_8 rate,const uchar_8 number_port,uchar_8 *cmd )
 {
 			serial_write_byte(rate,number_port, *(cmd + 0) );
			delay_for_debug();  
			serial_write_byte(rate,number_port, *(cmd + 1) );
			delay_for_debug();
			serial_write_byte(rate,number_port, *(cmd + 2) );
			delay_for_debug();
			serial_write_byte(rate,number_port, *(cmd + 3) );
			delay_for_debug();
			serial_write_byte(rate,number_port, *(cmd + 4) );
			delay_for_debug();
			serial_write_byte(rate,number_port, *(cmd + 5) );
			delay_for_debug();
			serial_write_byte(rate,number_port, *(cmd + 6) );
			delay_for_debug();
			serial_write_byte(rate,number_port, *(cmd + 7) );
			delay_for_debug();
			serial_write_byte(rate,number_port, *(cmd + 8) );
			delay_for_debug();  
		
 		delay_for_debug();
	 	delay_for_debug();
		delay_for_debug();
		delay_for_debug();
		delay_for_debug();	
 }












