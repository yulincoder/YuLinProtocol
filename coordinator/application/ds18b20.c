//文件名称:（FILE7）DS18B20驱动文件
//功能说明:	读出温度数据,并转换为BCD码。
//			本驱动只针对单总线上挂一只DS18B20的情况。
//硬件描述:	数据口P3^4,上拉4.7K电阻,未采用寄生电源供电
//使用晶体:	12M晶体
//创建日期:	2009-03-03
//软件版本:	v 1.0     
//==================================================================
 #include <REGX52.H>
 #include <intrins.h> 
 #include "..\\include\\config.h" 
 #include "..\\include\\hardware_config.h"
 #include "..\\include\\serial.h" 
 #include "..\\include\\environment.h"
 #include "..\\include\\req_son_data.h" 
 #include "..\\include\\rcv_father_cmd.h" 
 #include "..\\include\\serach_sons.h" 	
 	  		
//=====================常量=========================================
 #define		SkipROM		0xcc			//跳过ROM指令			
 #define		TempCvt		0x44			//温度转化命令
 #define		ReadTemp	0xbe			//读温度命令
 sbit		DQ18B20	 =  P3^3;			//数据口
//=====================变量=========================================
 uint_16		data		TempReg;		//温度值
 bit			bdata		NPflag;			//正负标志	
 uchar_8		data		IntgerH;		//BCD码表示
 uchar_8		data		IntgerM;
 uchar_8		data		IntgerL;		   
 uchar_8		data		DecimalH;
 uchar_8		data		DecimalMH;
 uchar_8		data		DecimalML;
 uchar_8		data		DecimalL;

 //extern volatile uchar SysSt; //系统状态
										//小数部分转换表
 uint_16		code		TempTab[] = { 0x0000,0x0625,0x1250,0x1875,
	 								  0x2500,0x3125,0x3750,0x4375,
	 								  0x5000,0x5625,0x6250,0x6875,
	 								  0x7500,0x8125,0x8750,0x9375
									};

//==================================================================
//函 数 名: _nNOP_()
//功能描述: 短延时函数
//输入参数: i
//输出参数:	无
//设 计 者：
//创建日期：2009-03-03
//==================================================================
 void	_nNOP_	(uint_16 i)
 {
	for(;i>0;i--)
	{
		_nop_();
	}
 }
//==================================================================
//函 数 名: Init18B20()
//功能描述: 18B20初始化函数
//输入参数: 无
//输出参数:	Init18B20()
//设 计 者：
//创建日期：2009-03-03
//==================================================================
 bit	Init18B20(void)
 {
	DQ18B20 = 0;				//数据线拉低
	EA = 0; //关中断，防止中断服务程序打乱操作DS18B20的时序
	_nNOP_(80);				//延时480us
	DQ18B20 = 1;				//数据线拉高
	_nNOP_(7);					//延时25us
	EA = 1;
	if(DQ18B20==1)				
		{
		DQ18B20 = 1;			//数据线拉高
		return(0);				//初始化失败返回0
		}
	else
		{
		EA = 0;
		_nNOP_(240);			//延时240us	
		EA = 1;
		DQ18B20 = 1;			//数据线拉高			
		return(1);				//初始化成功返回1
		}
	EA = 1;	  //开中断
 }
//==================================================================
//函 数 名: Write18B20()
//功能描述: 18B20写数据函数
//输入参数: Data
//输出参数:	无
//设 计 者：
//创建日期：2009-03-03
//==================================================================	
 void	Write18B20( uchar_8	Data)
 {
	uchar_8	data	i;
	EA = 0;
    for (i=8; i>0; i--)
   { 
	 DQ18B20 = 0;
     DQ18B20 = Data&0x01;
	 
     EA = 0;//关中断
     _nNOP_(10);
    // EA = 1; //开中断

     DQ18B20 = 1;
     Data>>=1;
   } 
   EA = 1;
 }
//==================================================================
//函 数 名: Read18B20()
//功能描述: 读18B20两字节数据函数
//输入参数: 无
//输出参数:	TempReg[]
//设 计 者：
//创建日期：2009-03-03
//==================================================================
 uchar_8 Read18B20(void)	
 {
	uchar_8	data 	i=0;
	uchar_8	data 	Data = 0;
	EA = 0;	//关中断
	for (i=8;i>0;i--)
	{   
		DQ18B20 = 0; 					//给脉冲信号
    	Data>>=1;
    	DQ18B20 = 1; 					//给脉冲信号
    	if(DQ18B20)
    	Data|=0x80;
    	
		EA = 0;//关中断
     	_nNOP_(10);
     	//EA = 1; //开中断
	}
	EA = 1; //开中断
   return(Data);
 }
//==================================================================
//函 数 名: GetTemp()
//功能描述: 得到温度数据函数
//输入参数: 无
//输出参数:	
//设 计 者：
//创建日期：2009-03-03
//==================================================================
 void	GetTemp(void)
 {
	uchar_8 data	TempL,TempH;
	if(Init18B20()==0);					//初始化不成功则继续初始化
	Write18B20(	SkipROM );				//发出跳过ROM指令
	Write18B20(	TempCvt );				//发出温度转化命令

	EA = 0;
	_nNOP_(80);						//延时800us
	EA = 1;

	if(Init18B20()==0);					//初始化不成功则继续初始化
	Write18B20(	SkipROM );				//发出跳过ROM指令
	Write18B20( ReadTemp);				//发出读温度命令
	TempL = Read18B20();				//调用读程序,读出两字节
	TempH = Read18B20();
	TempReg = TempH;
	TempReg = TempReg<<8;
	TempReg = TempReg|TempL;
	if((TempReg&0x800)!=0x800)
		NPflag = 0;						//正温度
	else
		{
		TempReg = ~TempReg+1;			//负温度
		NPflag = 1;
		}
 }
//==================================================================
//函 数 名: DataCovt()
//功能描述: 将读出的温度数据拆分为整数和小数部分，并转为ASIC码
//输入参数: 无
//输出参数:	
//设 计 者：
//创建日期：2009-03-03
//==================================================================
 void	DataCovt(void)					//11位有效位
 {
	uchar_8  data	Rwk1,Rwk2;
	uint_16  data	Rwk3;	
	Rwk1 = (TempReg&0x07f0)>>4;			//  ssss,s|xxx,xxxx,|xxxx
	Rwk2 = TempReg&0x000f;				//  符号位| 整数    | 小数  （整数部分-55--125，为7位）
	IntgerH = Rwk1/100;					//整数部分		
	IntgerM = Rwk1%100/10;
	IntgerL = Rwk1%10;
	Rwk3 = TempTab[Rwk2];
	DecimalL  = Rwk3&0x000f;			//小数部分
	DecimalML = (Rwk3&0x00f0)>>4;
	DecimalMH = (Rwk3&0x0f00)>>8;
	DecimalH  = (Rwk3&0xf000)>>12;
 }
//==================================================================
//函 数 名: MnDS18B20()
//功能描述: DS18B20驱动函数
//输入参数: 无
//输出参数:	无
//设 计 者：
//创建日期：2009-03-03
//==================================================================
 void    MnDS18B20(void)
 {
    static unsigned int hCnt = 0;//采集周期计数器
    static unsigned char initFlag = 0;

	uchar_8 a = 0,b = 0 ,c = 0;
    
     if( hCnt>=6000 )
    {
        hCnt = 0;

            if( initFlag==0 )
            {
                Init18B20();                        //初始化
                initFlag = 1;
            }   
            GetTemp();                            //温度采集
            DataCovt();                            //采集值变换为BCD码
			
			a =  IntgerM + 30;
			b =  IntgerL + 30;
			c =  DecimalH + 30;
	/*		serial_write_byte(0,0xff );
			serial_write_byte(0, a);
			serial_write_byte(0, b);         
			serial_write_byte(0, c);
			serial_write_byte(0,0x77 );	 */
     }else hCnt ++;
 }


