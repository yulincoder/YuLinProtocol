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
 	uchar_8 q1[9] = {"ha001003x"},
			q2[9] = {"world   "}; 
	
	uchar_8 a = 0;	
			 
	delay_for_debug();
	hardware_launch();
	environment_launch();
	
	queue_append(q1);
	queue_append(q2);	
	  

  //	print_stream(sizeof("system have be launched by 张特:\r\n"),BAUD_4800,0,"system have be launched by 张特:\r\n");


	/* 
	 * 相互握手已经成功 
	 * 目前技术债务: 需要确定初始化各个引脚，将其它函数内debug用的led全部清除，但是保留系统启动指示灯
	 * 				 然后继续调试
	 *				 完成数据传输任务，并封装API
	 */
	for( ;; ){
		
		chaeck_linked_worked();

		/* so trouble */
		/**/

		/* 父节点 */
		if( sons_status.linked_1 == SON_DISCONNECTED ){
			serach_sons(BAUD_4800);		
		}  
		temporary_rcv_sons(BAUD_4800);

		if( byte_time_out_cnt > 1000 ){
			byte_time_out_cnt = 0;
			print_stream(sizeof(hand_cmd),BAUD_9600,FATHER_PORT,hand_cmd);

		}   
	  	 
		/* 子节点 */   
		/*	 	
		if( sons_status.linked_father == SON_DISCONNECTED){ 
			rcv_father_data(BAUD_4800);
	 	} else{
			if( byte_time_out_cnt > 900 ){
			 byte_time_out_cnt = 0;	  
			 temporary_send_data(BAUD_4800,FATHER_PORT,data_request);
			}
		}  
		 */		
		
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
		for( j=0; j<20; j++ )
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

































