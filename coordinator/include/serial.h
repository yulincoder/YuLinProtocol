 //#include "..\\include\\config.h" 
 #ifndef __SERIAL_H__
 #define __SERIAL_H__

/* 串口0发送一个字符 */ 
 extern void serial_0_write_byte(const uchar_8 rate,uchar_8 input);

/* 串口发送byte数据  */
 extern void serial_write_byte(const uchar_8 rate,const uchar_8 serial_number,uchar_8 input);

/* 串口接收byte数据 */
 extern uchar_8 serial_read_byte(const uchar_8 rate,const uchar_8 serial_number);

/* RXD_0接收一个字符 */  
 extern uchar_8 serial_0_read_byte(const uchar_8 rate);
 
/* 外部数据流到来 */
 extern uchar_8 outside_data_coming(void); 

/* 输出字符流 */ 
 extern void print_stream(const uchar_8 data_size,const uchar_8 rate,const uchar_8 serial_number, uchar_8 * pstream);  
 
/* 握手命令缓存区 */ 
 extern uchar_8 idata hand_cmd_buffer[9];

/* 握手确认信号缓冲区 */
 extern uchar_8 idata hand_ack_buffer[9]; 

 #define BYTE_TIME_OUT 50		//接收一个byte的超时时间

 #define BAUD_4800 1			//波特率4800参数
 #define BAUD_9600 2			//波特率9600参数

 #endif //__SERIAL_H__