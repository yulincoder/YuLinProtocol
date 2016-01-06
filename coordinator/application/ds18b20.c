//�ļ�����:��FILE7��DS18B20�����ļ�
//����˵��:	�����¶�����,��ת��ΪBCD�롣
//			������ֻ��Ե������Ϲ�һֻDS18B20�������
//Ӳ������:	���ݿ�P3^4,����4.7K����,δ���ü�����Դ����
//ʹ�þ���:	12M����
//��������:	2009-03-03
//�����汾:	v 1.0     
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
 	  		
//=====================����=========================================
 #define		SkipROM		0xcc			//����ROMָ��			
 #define		TempCvt		0x44			//�¶�ת������
 #define		ReadTemp	0xbe			//���¶�����
 sbit		DQ18B20	 =  P3^3;			//���ݿ�
//=====================����=========================================
 uint_16		data		TempReg;		//�¶�ֵ
 bit			bdata		NPflag;			//������־	
 uchar_8		data		IntgerH;		//BCD���ʾ
 uchar_8		data		IntgerM;
 uchar_8		data		IntgerL;		   
 uchar_8		data		DecimalH;
 uchar_8		data		DecimalMH;
 uchar_8		data		DecimalML;
 uchar_8		data		DecimalL;

 //extern volatile uchar SysSt; //ϵͳ״̬
										//С������ת����
 uint_16		code		TempTab[] = { 0x0000,0x0625,0x1250,0x1875,
	 								  0x2500,0x3125,0x3750,0x4375,
	 								  0x5000,0x5625,0x6250,0x6875,
	 								  0x7500,0x8125,0x8750,0x9375
									};

//==================================================================
//�� �� ��: _nNOP_()
//��������: ����ʱ����
//�������: i
//�������:	��
//�� �� �ߣ�
//�������ڣ�2009-03-03
//==================================================================
 void	_nNOP_	(uint_16 i)
 {
	for(;i>0;i--)
	{
		_nop_();
	}
 }
//==================================================================
//�� �� ��: Init18B20()
//��������: 18B20��ʼ������
//�������: ��
//�������:	Init18B20()
//�� �� �ߣ�
//�������ڣ�2009-03-03
//==================================================================
 bit	Init18B20(void)
 {
	DQ18B20 = 0;				//����������
	EA = 0; //���жϣ���ֹ�жϷ��������Ҳ���DS18B20��ʱ��
	_nNOP_(80);				//��ʱ480us
	DQ18B20 = 1;				//����������
	_nNOP_(7);					//��ʱ25us
	EA = 1;
	if(DQ18B20==1)				
		{
		DQ18B20 = 1;			//����������
		return(0);				//��ʼ��ʧ�ܷ���0
		}
	else
		{
		EA = 0;
		_nNOP_(240);			//��ʱ240us	
		EA = 1;
		DQ18B20 = 1;			//����������			
		return(1);				//��ʼ���ɹ�����1
		}
	EA = 1;	  //���ж�
 }
//==================================================================
//�� �� ��: Write18B20()
//��������: 18B20д���ݺ���
//�������: Data
//�������:	��
//�� �� �ߣ�
//�������ڣ�2009-03-03
//==================================================================	
 void	Write18B20( uchar_8	Data)
 {
	uchar_8	data	i;
	EA = 0;
    for (i=8; i>0; i--)
   { 
	 DQ18B20 = 0;
     DQ18B20 = Data&0x01;
	 
     EA = 0;//���ж�
     _nNOP_(10);
    // EA = 1; //���ж�

     DQ18B20 = 1;
     Data>>=1;
   } 
   EA = 1;
 }
//==================================================================
//�� �� ��: Read18B20()
//��������: ��18B20���ֽ����ݺ���
//�������: ��
//�������:	TempReg[]
//�� �� �ߣ�
//�������ڣ�2009-03-03
//==================================================================
 uchar_8 Read18B20(void)	
 {
	uchar_8	data 	i=0;
	uchar_8	data 	Data = 0;
	EA = 0;	//���ж�
	for (i=8;i>0;i--)
	{   
		DQ18B20 = 0; 					//�������ź�
    	Data>>=1;
    	DQ18B20 = 1; 					//�������ź�
    	if(DQ18B20)
    	Data|=0x80;
    	
		EA = 0;//���ж�
     	_nNOP_(10);
     	//EA = 1; //���ж�
	}
	EA = 1; //���ж�
   return(Data);
 }
//==================================================================
//�� �� ��: GetTemp()
//��������: �õ��¶����ݺ���
//�������: ��
//�������:	
//�� �� �ߣ�
//�������ڣ�2009-03-03
//==================================================================
 void	GetTemp(void)
 {
	uchar_8 data	TempL,TempH;
	if(Init18B20()==0);					//��ʼ�����ɹ��������ʼ��
	Write18B20(	SkipROM );				//��������ROMָ��
	Write18B20(	TempCvt );				//�����¶�ת������

	EA = 0;
	_nNOP_(80);						//��ʱ800us
	EA = 1;

	if(Init18B20()==0);					//��ʼ�����ɹ��������ʼ��
	Write18B20(	SkipROM );				//��������ROMָ��
	Write18B20( ReadTemp);				//�������¶�����
	TempL = Read18B20();				//���ö�����,�������ֽ�
	TempH = Read18B20();
	TempReg = TempH;
	TempReg = TempReg<<8;
	TempReg = TempReg|TempL;
	if((TempReg&0x800)!=0x800)
		NPflag = 0;						//���¶�
	else
		{
		TempReg = ~TempReg+1;			//���¶�
		NPflag = 1;
		}
 }
//==================================================================
//�� �� ��: DataCovt()
//��������: ���������¶����ݲ��Ϊ������С�����֣���תΪASIC��
//�������: ��
//�������:	
//�� �� �ߣ�
//�������ڣ�2009-03-03
//==================================================================
 void	DataCovt(void)					//11λ��Чλ
 {
	uchar_8  data	Rwk1,Rwk2;
	uint_16  data	Rwk3;	
	Rwk1 = (TempReg&0x07f0)>>4;			//  ssss,s|xxx,xxxx,|xxxx
	Rwk2 = TempReg&0x000f;				//  ����λ| ����    | С��  ����������-55--125��Ϊ7λ��
	IntgerH = Rwk1/100;					//��������		
	IntgerM = Rwk1%100/10;
	IntgerL = Rwk1%10;
	Rwk3 = TempTab[Rwk2];
	DecimalL  = Rwk3&0x000f;			//С������
	DecimalML = (Rwk3&0x00f0)>>4;
	DecimalMH = (Rwk3&0x0f00)>>8;
	DecimalH  = (Rwk3&0xf000)>>12;
 }
//==================================================================
//�� �� ��: MnDS18B20()
//��������: DS18B20��������
//�������: ��
//�������:	��
//�� �� �ߣ�
//�������ڣ�2009-03-03
//==================================================================
 void    MnDS18B20(void)
 {
    static unsigned int hCnt = 0;//�ɼ����ڼ�����
    static unsigned char initFlag = 0;

	uchar_8 a = 0,b = 0 ,c = 0;
    
     if( hCnt>=6000 )
    {
        hCnt = 0;

            if( initFlag==0 )
            {
                Init18B20();                        //��ʼ��
                initFlag = 1;
            }   
            GetTemp();                            //�¶Ȳɼ�
            DataCovt();                            //�ɼ�ֵ�任ΪBCD��
			
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

