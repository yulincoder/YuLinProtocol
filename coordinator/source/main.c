/*---------------------------------------------------------------------------------
  --
  ---------------------------------------------------------------------------------
  --                                        
  --
  --
  --
  ---------------------------------------------------------------------------------
 */

/*************************************************************************************************************
 * 
 * YuLinProtocol,simulate Zigebee Protocol,
 * Because of unbeknown reasons , the begin of system may occur error.
 * frist sending a '0'. 
 * 
 */
 #include <REGX52.H>
 #include "..\\include\\config.h" 
 #include "..\\include\\hardware_config.h"
 #include "..\\include\\serial.h" 
 #include "..\\include\\environment.h"
 #include "..\\include\\req_son_data.h" 
 #include "..\\include\\rcv_father_cmd.h" 
 #include "..\\include\\serach_sons.h"
 #include "..\\application\\ds18b20.h"  	 


 void delay_for_debug(void);
	
 sbit test_led_7 = P1^7;

 void main(void)
 {
 	uchar_8 q1[9] = {"zhangte  "},
			q2[9] = {"world   "}; 
	
	uchar_8 a = 0;	
			 
	delay_for_debug();
	hardware_launch();
	environment_launch();
	
	queue_append(q1);
	queue_append(q2);	
	  
	/* oh my god , who will able to give me some lovely gree pot in github??  */
    /* i am so sorrow */
	print_stream(sizeof("system have be launched by 张特:\r\n"),BAUD_4800,0,"system have be launched by 张特:\r\n");

	send_hand_cmd(BAUD_4800,FATHER_PORT);

	GPIO_1(0) = 1;
	GPIO_1(1) = 1;

	GPIO_1(4) = 1;
	GPIO_1(6) = 1;

	/* 
	 * 相互握手已经成功 
	 * 目前技术债务: 需要确定初始化各个引脚，将其它函数内debug用的led全部清除，但是保留系统启动指示灯
	 * 				 然后继续调试
	 *				 完成数据传输任务，并封装API
	 */
	for( ;; ){

		//if(GPIO_1(0) == 1){
		//	GPIO_1(5) = 0;
		//}  
	//	GPIO_1(0) = 1;
		//hao hhhh
		//print_stream(sizeof("hello,world!"),BAUD_4800,0,"hello,world!");
	//	delay_for_debug(); 
	//	rcv_father_data(BAUD_4800);

	//	link_son(BAUD_4800,FATHER_PORT);
		//	send_queue_to_father(BAUD_4800);
		serach_sons(BAUD_4800);	
	//	link_son(BAUD_4800,FATHER_PORT);			
	//	delay_for_debug();
	//	send_hand_cmd(BAUD_4800,FATHER_PORT);
	//	serial_write_byte( BAUD_4800,FATHER_PORT,' ' );
		/*if( outside_data_coming() ){
		//	a = serial_0_read_byte(BAUD_4800);
 			a = serial_read_byte(BAUD_4800,FATHER_PORT);
			serial_write_byte( BAUD_4800,FATHER_PORT,a );
 		}  */
	} 
 }



/********************
 * 调试用延时函数
 */
 void delay_for_debug(void)
 {
	unsigned char 	i,
					j;

	for( i=0; i<250; i++ )
		for( j=0; j<120; j++ )
		;
 }



/* 	                                                                                                    
                          =   ==   => /                                                             
                         +# = #= +  $=>>  =                                                         
                     =+%>=$%=/#%+%>%+/ +>/+>+=                                                      
                    >=+$%#+#>%$#%##$=+$$/+$+=  >=                                                   
                  >>/%#%###########+#######>==+#>  /                                                
             =%+//>$%#%#########################%%#/==                         
              +$+##+#########%##################$$%+/=                        
             ==/+$#########=  =$#######>>#######$##>=                             
          //=/%#$#########                +######$%%++$+                       
         =>/+$############=                #########%%>=                        
           /$##############%=             %######$#$+>=/+                        
         =>>+%$###############%/>     =/$############$+%/=                  
        =>/%%$$#########################################/                 
         ==>+%#######################################$%/>=                    
        =>+##$$######################################++>=                   
         /+++$#####################################$%$%/+>                    
          >>>++####################################+>/+%=                                           
            >>%#####################################$%/=                                            
             >$##################################/+>/==>                                            
            /#%/>################################+>//                                        
            =   %+$%##$######################$$//+%%>>                        
                = >$#/$$#####################$+=                                 
                  >>/+=##############%/+#+$##%+>                                 
                      +%%###%%##%%%#% > +//= //%                               
                     >  +>=+>+=%>=##$=> = =                                    
                          =   =   #>//                                          
            >//=                =$                                                
               /%+             =%     >/+++/>                                     
                 =$%          >%  +###$+>====>=                                              
                   /#>       >%/##$>                                                                
                    =#+     >###/                                                                   
                     =#%   >#%=                                                         
                      =#+  #                                                   
                       %#>+/                                                 
                        ###                                                     
                        /#%                                                   
                         #/                                                 
                         /+    //=                                                
                         >%  %+                                                
                          # #+                                                
                          $+#                                                  
                           #%                                                                  
                           >$                                                                   
                            #                                                                       
                            /+                                                                      
                             #                                                                      
                             $                                                                      
                             %                                                                      
                             = 
*/

































