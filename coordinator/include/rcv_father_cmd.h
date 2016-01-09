 #ifndef __RCV_FATHER_CMD_H__
 #define __RCV_FATHER_CMD_H__

 
/* 从父节点接收数据 */
 extern void rcv_father_data(const uchar_8 rate);

/* 向父节点发送数据队列所有内容 */
 extern void send_queue_to_father(const uchar_8 rate);

 #endif //__RCV_FATHER_CMD_H__