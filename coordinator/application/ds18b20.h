
 #ifndef __DS18B20_H__
 #define __DS18B20_H__

//=======================变量定义==================================
 extern	unsigned char	data	IntgerM;
 extern	unsigned char	data	IntgerL;
 extern	unsigned char	data	DecimalH;
 extern	unsigned char	data	DecimalMH;			
//=======================调用函数===================================

 extern  bit Init18B20(void);		//初始化
 extern  void	MnDS18B20(void);

 #endif //__DS18B20_H__