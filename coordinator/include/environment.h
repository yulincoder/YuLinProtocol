 #ifndef __ENVIRONMENT_H__
 #define __ENVIRONMENT_H__

 #define QUEUE_BUFF_SIZE 10			//消息队列大小
 #define MESSAGE_LENGTH 9			//一则数据长度





/*  待处理数据cache */
 typedef struct  data_processed_type{
  		uchar_8 cmd[MESSAGE_LENGTH];
		uchar_8 dirty;				//数据是否为脏
 }data_processed_type;
 extern data_processed_type  data_processed;






 typedef struct data_queue_struct{	  
 	/*
	 *  这个消息队列存储由路由器发来的消息并向其它接收对象转发
	 */
	uchar_8  data_buff[QUEUE_BUFF_SIZE][MESSAGE_LENGTH]; 		
	//uchar_8  data_queue_length;				   //数据队长度
	uchar_8  data_queue_rear;					   //队尾
	uchar_8  data_queue_head;					   //对头
 }data_queue_type; 								     
 extern data_queue_type idata data_queue;					   //生成数据队列

 

 
  typedef struct sons_struct{
 	/*
	 *  router连接状态
	 */
	uchar_8 linked_father : 1;
 	uchar_8 linked_1 : 1;
	uchar_8 linked_2 : 1;
	uchar_8 linked_3 : 1;	
	uchar_8 : 0;
	
	uchar_8 linked_amount;	//已连接子节点数量
 }sons_status_type;
 extern sons_status_type idata sons_status;


 extern uchar_8 host_id[3];


 extern uchar_8 hand_cmd[9] ,
 				hand_ack[9],
				data_request[9],
		 		data_respond[9];




/* 清空队列 */
 extern void empty_queue(void);

/* 读出队头一帧数据到待处理数据cache */
 extern void queue_read(void);

/* 写入一帧数据 */
 extern void queue_append( uchar_8 *message );

/* 判断队是否为空 */
 extern bit queue_estimate_empty(void);

 extern void clean_serial_buffer(void);

/* 判断队是否为满 */
 extern bit queue_estimate_full(void);

/* 软件环境启动 */
 extern void environment_launch(void);

/* 根据收到的父节点握手命令提取出本机id */
 extern void rcv_father_hand_creat_host_id(uchar_8 * cmd);

/* 根据本机id生成本机命令 */
 extern void id_insert_data(void);

 /* 检查节点连接显示 */
 extern void chaeck_linked_worked(void);

/* 串口接收数据缓冲区 */
 extern uchar_8 idata serial_buffer[MESSAGE_LENGTH];

/* 等待接收返回数据计数器 */
  extern uint_16 timeout_cnt;

 #endif //__ENVIRONMENT_H__