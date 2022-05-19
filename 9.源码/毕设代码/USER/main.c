#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "led.h"
#include "touch.h"
#include "gui.h"
#include "pic.h"
#include "timer.h"
#include "bsp_usart.h"
#include "loader.h"
#include "rc522_config.h"
#include "rc522_function.h"

u8 KeyValue[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // 卡A密钥
u8 ucArray_ID[4];  /*先后存放IC卡的类型和UID(IC卡序列号)*/
u8 ucStatusReturn; /*返回状态*/
u8 ucCardNum = 0;
char send_buff[526] = {0};
char destination[64]={0};
char cardnum[64]={0};
char departure[64]={0};
char Ride_time[64]={0};
char ticket[64]={0};

void main_page();
void buy_page();
void sure_page();
void quit_page();
void time_page();
void num_page();
void scan_page();
void money_page();

void money_page(){
	
LCD_Fill(0,0,lcddev.width,35,BLUE);
LCD_Fill(0,lcddev.height-35,lcddev.width,lcddev.height,BLUE);
Show_Str(180,300,WHITE,BLUE,"五邑大学毕设版",16,1);
Show_Str(30,10,WHITE, BLUE,"上一步",16,1);
Show_Str(400,10,WHITE, BLUE,"首页",16,1);
LCD_Fill(0,40,lcddev.width,280,GRAY );
LCD_DrawRectangle(25,5,80,30); 
LCD_DrawRectangle(390,5,440,30); 	
Show_Str(110,90,WHITE,BLUE,departure,16,1);	
Show_Str(330,90,WHITE,BLUE,destination,16,1);	
Show_Str(220,75,WHITE,BLUE,Ride_time,16,1);	
Show_Str(220,95,WHITE,BLUE,ticket,16,1);
LCD_DrawLine(180, 95, 300,95);
LCD_DrawLine(280, 85, 300,95);
	
Show_Str(390,70,WHITE, BLUE,"总金额",16,1);	
LCD_DrawRectangle(390,90,440,120); 	
Show_Str(390,130,WHITE, BLUE,"总手续费",16,1);		
LCD_DrawRectangle(390,150,440,180); 
Show_Str(400,160,WHITE, BLUE,"$0",16,1);	
Show_Str(390,190,WHITE, BLUE,"应支付金额",16,1);	
LCD_DrawRectangle(390,220,440,250); 


LCD_DrawRectangle(390,150,440,180); 
if(ticket=="成人票"){
	Show_Str(400,100,WHITE, BLUE,"$85",16,1);
	Show_Str(400,210,WHITE, BLUE,"$85",16,1);
}	
else{
	Show_Str(400,100,WHITE, BLUE,"$45",16,1);	
	Show_Str(400,220,WHITE, BLUE,"$45",16,1);	
}
	
	
while(1){
	LCD_Fill(180,210,280,240,BLUE);
	Show_Str(190,220,WHITE, BLUE,"确认并支付",16,1);
	delay_ms(200);
	LCD_Fill(180,210,280,240,YELLOW);
	Show_Str(190,220,WHITE, YELLOW,"确认并支付",16,1);
	delay_ms(200);
	
	tp_dev.scan(0); 
			if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
			{	
				if(tp_dev.x<lcddev.width&&tp_dev.y<lcddev.height)	// 在屏幕范围内
				{	
								
						if(tp_dev.x<84&&tp_dev.x>35&&tp_dev.y<30&&tp_dev.y>4)	//上一步
						{
								scan_page();
						}
						else if (tp_dev.x<456&&tp_dev.x>398&&tp_dev.y<28&&tp_dev.y>5)//首页
						{
								main_page();
						}	
						else if (tp_dev.x<280&&tp_dev.x>180&&tp_dev.y<240&&tp_dev.y>210)
						{
								 sure_page();
						}
				}
			}
	

}
}
void scan_page(){
	
LCD_Fill(0,0,lcddev.width,35,BLUE);
LCD_Fill(0,lcddev.height-35,lcddev.width,lcddev.height,BLUE);
Show_Str(180,300,WHITE,BLUE,"五邑大学毕设版",16,1);
Show_Str(30,10,WHITE, BLUE,"上一步",16,1);
Show_Str(400,10,WHITE, BLUE,"首页",16,1);
LCD_Fill(0,40,lcddev.width,280,GRAY );
LCD_DrawRectangle(25,5,80,30); 
LCD_DrawRectangle(390,5,440,30); 	
Show_Str(120,60,WHITE,BLUE,departure,16,1);	
Show_Str(320,60,WHITE,BLUE,destination,16,1);	
Show_Str(220,45,WHITE,BLUE,Ride_time,16,1);	
Show_Str(220,65,WHITE,BLUE,ticket,16,1);
LCD_DrawLine(180, 65, 300,65);
LCD_DrawLine(280, 55, 300,65);	
	
LCD_Fill(80,90,380,260,WHITE);	
LCD_Fill(165,110,295,140,BLUE);	
LCD_Fill(165,170,295,200,BLUE);	
LCD_Fill(330,220,370,240,BLUE);	
Show_Str(205,115,WHITE,BLUE,"校园卡",16,1);
Show_Str(205,175,WHITE,BLUE,"身份证",16,1);
Show_Str(335,223,WHITE,BLUE,"取消",16,1);

while(1){
			tp_dev.scan(0); 
			if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
			{	
				if(tp_dev.x<lcddev.width&&tp_dev.y<lcddev.height)	// 在屏幕范围内
				{	
								
						if(tp_dev.x<84&&tp_dev.x>35&&tp_dev.y<30&&tp_dev.y>4)	//上一步
						{
								num_page();
						}
						else if (tp_dev.x<456&&tp_dev.x>398&&tp_dev.y<28&&tp_dev.y>5)//首页
						{
								main_page();
						}	
						else if (tp_dev.x<300&&tp_dev.x>170&&tp_dev.y<140&&tp_dev.y>110)
						{
							LCD_Fill(80,90,380,260,WHITE);	
							LCD_Fill(165,110,295,200,BLUE);	
							Show_Str(185,140,WHITE,BLUE,"请刷校园卡",16,1);
							
							while(1){
									if ( ( ucStatusReturn = PcdRequest ( PICC_REQALL, ucArray_ID ) ) != MI_OK )                                    //寻卡
										{
												ucStatusReturn = PcdRequest ( PICC_REQALL, ucArray_ID );		                                                 //若失败再次寻卡
										}
										if ( ucStatusReturn == MI_OK  )
										{
											if ( PcdAnticoll ( ucArray_ID ) == MI_OK )                                                                   //防冲撞（当有多张卡进入读写器操作范围时，防冲突机制会从其中选择一张进行操作）
											{
													PcdSelect(ucArray_ID);
													ucCardNum = (ucArray_ID[0]<<24)|(ucArray_ID[1]<<16)|(ucArray_ID[2]<<8)|(ucArray_ID[3]);
													if(ucCardNum==219)
														sprintf(cardnum, "3120000922");
													else
														sprintf(cardnum, "0000000000");
													LCD_Fill(80,90,380,260,WHITE);	
													LCD_Fill(165,110,295,200,BLUE);	
													Show_Str(180,130,WHITE,BLUE,"正在处理证件",16,1);
													Show_Str(205,150,WHITE,BLUE,"请稍后",16,1);
													delay_ms(500);
													LCD_Fill(80,90,380,260,WHITE);	
													LCD_Fill(165,110,295,200,BLUE);
													Show_Str(180,130,WHITE,BLUE,"您的证件为：",16,1);
													Show_Str(180,150,WHITE,BLUE,cardnum,16,1);
													LCD_Fill(120,220,220,250,BLUE);	
													LCD_Fill(250,220,350,250,BLUE);	
													Show_Str(140,227,WHITE,BLUE,"继续使用",16,1);
													Show_Str(270,227,WHITE,BLUE,"更换证件",16,1);
												
															while(1){
																		tp_dev.scan(0); 
																				if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
																				{	
																					if(tp_dev.x<lcddev.width&&tp_dev.y<lcddev.height)	// 在屏幕范围内
																					{	
																												if(tp_dev.x<220&&tp_dev.x>120&&tp_dev.y<250&&tp_dev.y>220)	
																												{
																														money_page();
																												}
																												else if (tp_dev.x<350&&tp_dev.x>250&&tp_dev.y<250&&tp_dev.y>220)
																												{
																													scan_page();
																												}	
																					}
																				
																				}
															}
											}
											
										}
							}
							
							
						}
						else if (tp_dev.x<300&&tp_dev.x>170&&tp_dev.y<200&&tp_dev.y>170)
						{
							LCD_Fill(80,90,380,260,WHITE);	
							LCD_Fill(165,110,295,200,BLUE);	
							Show_Str(185,140,WHITE,BLUE,"请刷身份证",16,1);
							
							while(1){
									if ( ( ucStatusReturn = PcdRequest ( PICC_REQALL, ucArray_ID ) ) != MI_OK )                                    //寻卡
										{
												ucStatusReturn = PcdRequest ( PICC_REQALL, ucArray_ID );		                                                 //若失败再次寻卡
										}
										if ( ucStatusReturn == MI_OK  )
										{
											if ( PcdAnticoll ( ucArray_ID ) == MI_OK )                                                                   //防冲撞（当有多张卡进入读写器操作范围时，防冲突机制会从其中选择一张进行操作）
											{
													PcdSelect(ucArray_ID);
													ucCardNum = (ucArray_ID[0]<<24)|(ucArray_ID[1]<<16)|(ucArray_ID[2]<<8)|(ucArray_ID[3]);
													if(ucCardNum==219)
														sprintf(cardnum, "3120000922");
													else
														sprintf(cardnum, "0000000000");
													LCD_Fill(80,90,380,260,WHITE);	
													LCD_Fill(165,110,295,200,BLUE);	
													Show_Str(180,130,WHITE,BLUE,"正在处理证件",16,1);
													Show_Str(205,150,WHITE,BLUE,"请稍后",16,1);
													delay_ms(500);
													LCD_Fill(80,90,380,260,WHITE);	
													LCD_Fill(165,110,295,200,BLUE);
													Show_Str(180,130,WHITE,BLUE,"您的证件为：",16,1);
													Show_Str(180,150,WHITE,BLUE,cardnum,16,1);
													LCD_Fill(120,220,220,250,BLUE);	
													LCD_Fill(250,220,350,250,BLUE);	
													Show_Str(140,227,WHITE,BLUE,"继续使用",16,1);
													Show_Str(270,227,WHITE,BLUE,"更换证件",16,1);
												
															while(1){
																		tp_dev.scan(0); 
																				if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
																				{	
																					if(tp_dev.x<lcddev.width&&tp_dev.y<lcddev.height)	// 在屏幕范围内
																					{	
																												if(tp_dev.x<220&&tp_dev.x>120&&tp_dev.y<250&&tp_dev.y>220)	
																												{
																														money_page();
																												}
																												else if (tp_dev.x<350&&tp_dev.x>250&&tp_dev.y<250&&tp_dev.y>220)
																												{
																													scan_page();
																												}	
																					}
																				
																				}
															}
											}
											
										}
							}
							
									
						}
						else if (tp_dev.x<370&&tp_dev.x>330&&tp_dev.y<240&&tp_dev.y>220)
						{
									num_page();
						}
				}
			}
}
	
}
void num_page(){

u8 ticketNum[][31]={"30","29","28","27","26","25","24","23","22","21","20","19","18","17","16","15","14","13","12","11","10","9",
"8","7","6","5","4","3","2","1","0"};
u8 i=30;
u8 j=30;
LCD_Fill(0,0,lcddev.width,35,BLUE);
LCD_Fill(0,lcddev.height-35,lcddev.width,lcddev.height,BLUE);
Show_Str(180,300,WHITE,BLUE,"五邑大学毕设版",16,1);
Show_Str(30,10,WHITE, BLUE,"上一步",16,1);
Show_Str(400,10,WHITE, BLUE,"首页",16,1);
LCD_Fill(0,40,lcddev.width,280,GRAY );
LCD_DrawRectangle(25,5,80,30); 
LCD_DrawRectangle(390,5,440,30); 
LCD_DrawRectangle(25,50,320,270); 
Show_Str(80,100,WHITE, BLUE,"成人票",16,1);
Show_Str(80,160,WHITE, BLUE,"儿童票",16,1);
LCD_DrawRectangle(180,90,250,120); 
LCD_DrawRectangle(180,150,250,180); 
Show_Str(210,95,WHITE,BLUE,ticketNum[i],16,1);		
Show_Str(210,155,WHITE,BLUE,ticketNum[j],16,1);		

gui_circle(280, 105,WHITE,16, 0);
gui_circle(280, 170,WHITE,16, 0);
LCD_DrawLine(272, 105, 288, 105);
LCD_DrawLine(280, 97,280, 113);
LCD_DrawLine(272, 170, 288, 170);
LCD_DrawLine(280, 162,280, 178);

LCD_Fill(390,240,450,270,BLUE);
Show_Str(400,250,WHITE, BLUE,"下一步",16,1);	


while(1){
			tp_dev.scan(0); 
			if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
			{	
				if(tp_dev.x<lcddev.width&&tp_dev.y<lcddev.height)	// 在屏幕范围内
				{	
								
						if(tp_dev.x<84&&tp_dev.x>35&&tp_dev.y<30&&tp_dev.y>4)	//上一步
						{
								time_page();
						}
						else if (tp_dev.x<456&&tp_dev.x>398&&tp_dev.y<28&&tp_dev.y>5)//首页
						{
								main_page();
						}	
						else if (tp_dev.x<300&&tp_dev.x>260&&tp_dev.y<130&&tp_dev.y>80)
						{
							
							i--;
							if(i<0)
									i=0;	
							LCD_Fill(200,98,230,110,GRAY );
							Show_Str(210,95,WHITE,BLUE,ticketNum[i],16,1);	
							if(i<30){
										gui_circle(150, 105,WHITE,16, 0);
										LCD_DrawLine(142, 105, 158, 105);
							}
							delay_ms(200);
							
						}
						else if (tp_dev.x<300&&tp_dev.x>260&&tp_dev.y<200&&tp_dev.y>150)
						{
								j--;
								if(j<0)
										j=0;
								LCD_Fill(200,158,230,168,GRAY );
								Show_Str(210,155,WHITE,BLUE,ticketNum[j],16,1);		
								if(j<30){
										gui_circle(150, 170,WHITE,16, 0);	
										LCD_DrawLine(142, 170,158, 170);
								}
								delay_ms(200);
						}
						else if (tp_dev.x<170&&tp_dev.x>130&&tp_dev.y<130&&tp_dev.y>80)
						{
							i++;
							if(i>=30)
							{
								LCD_Fill(130,85,170,125,GRAY );
								i=30;
							}
							
							LCD_Fill(200,98,230,110,GRAY );
							Show_Str(210,95,WHITE,BLUE,ticketNum[i],16,1);	


							delay_ms(200);
						}
						else if (tp_dev.x<170&&tp_dev.x>130&&tp_dev.y<200&&tp_dev.y>150)
						{
							
														
							j++;
							if(j>=30)
							{
								LCD_Fill(130,150,170,190,GRAY );
								j=30;
							}

							LCD_Fill(200,158,230,168,GRAY );
							Show_Str(210,155,WHITE,BLUE,ticketNum[j],16,1);		

							delay_ms(200);
						}
						else if (tp_dev.x<459&&tp_dev.x>404&&tp_dev.y<276&&tp_dev.y>247)
						{
								if(i<30&&i>=0)
										sprintf(ticket, "成人票");
								if(j<30&&j>=0)
									  sprintf(ticket, "儿童票");
								scan_page();
						}
				}
			}
	}
	
}
void buy_page(){
u8 Labeled_departure,Labeled_destination;	
u8 i=1;
u8 j=1;
LCD_Fill(0,0,lcddev.width,35,BLUE);
LCD_Fill(0,lcddev.height-35,lcddev.width,lcddev.height,BLUE);
Show_Str(180,300,WHITE,BLUE,"五邑大学毕设版",16,1);
Show_Str(30,10,WHITE, BLUE,"上一步",16,1);
Show_Str(400,10,WHITE, BLUE,"首页",16,1);
LCD_Fill(0,40,lcddev.width,280,GRAY );
LCD_DrawRectangle(25,5,80,30); 
LCD_DrawRectangle(390,5,440,30); 
LCD_DrawRectangle(25,50,320,270); 
LCD_Fill(50,55,150,90,BLUE);
Show_Str(70,65,WHITE, BLUE,"热门车站",16,1);
Show_Str(220,65,WHITE, BLUE,"其他车站",16,1);

LCD_DrawRectangle(70,110,145,145); 	
Show_Str(90,120,WHITE, BLUE,"北京",16,1);	
	
LCD_DrawRectangle(200,110,275,145); 	
Show_Str(215,120,WHITE, BLUE,"上海",16,1);	
	
LCD_DrawRectangle(70,170,145,205); 	
Show_Str(90,180,WHITE, BLUE,"广州",16,1);	

Show_Str(340,50,WHITE, BLUE,"出发地",16,1);	
Show_Str(340,110,WHITE, BLUE,"目的地",16,1);	


LCD_Fill(390,240,450,270,BLUE);
Show_Str(400,250,WHITE, BLUE,"下一步",16,1);	

while(1){
		tp_dev.scan(0); 
		if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
		{	
			if(tp_dev.x<lcddev.width&&tp_dev.y<lcddev.height)	// 在屏幕范围内
			{
				
					if(tp_dev.x<84&&tp_dev.x>35&&tp_dev.y<30&&tp_dev.y>4)	//上一步
					{
							if(j==2)
								{
										LCD_Fill(335,68,410,110,GRAY );
										LCD_Fill(335,131,410,180,GRAY );
										j=1;
								}
					}
					else if (tp_dev.x<456&&tp_dev.x>398&&tp_dev.y<28&&tp_dev.y>5)//首页
					{
							main_page();
					}
				
					else if (tp_dev.x<142&&tp_dev.x>68&&tp_dev.y<83&&tp_dev.y>59&&i==2)//热门
					{
							LCD_Fill(43,55,310,237,GRAY );
							LCD_Fill(50,55,150,90,BLUE);
							Show_Str(70,65,WHITE, BLUE,"热门车站",16,1);
							Show_Str(220,65,WHITE, BLUE,"其他车站",16,1);
							LCD_DrawRectangle(70,110,145,145); 	
							Show_Str(90,120,WHITE, BLUE,"北京",16,1);	
	
							LCD_DrawRectangle(200,110,275,145); 	
							Show_Str(215,120,WHITE, BLUE,"上海",16,1);	
								
							LCD_DrawRectangle(70,170,145,205); 	
							Show_Str(90,180,WHITE, BLUE,"广州",16,1);	
						
							i=1;
						
					}
					else if (tp_dev.x<322&&tp_dev.x>197&&tp_dev.y<86&&tp_dev.y>61&&i==1)//其他
					{
							LCD_Fill(43,55,310,237,GRAY );
								LCD_Fill(190,55,310,90,BLUE);
							Show_Str(70,65,WHITE, BLUE,"热门车站",16,1);
							Show_Str(220,65,WHITE, BLUE,"其他车站",16,1);
							LCD_DrawRectangle(70,110,145,145); 	
							Show_Str(90,120,WHITE, BLUE,"江门",16,1);	
	
							LCD_DrawRectangle(200,110,275,145); 	
							Show_Str(215,120,WHITE, BLUE,"深圳",16,1);	
								
							LCD_DrawRectangle(70,170,145,205); 	
							Show_Str(90,180,WHITE, BLUE,"珠海",16,1);	
						
							LCD_DrawRectangle(200,170,275,205); 	
							Show_Str(215,180,WHITE, BLUE,"佛山",16,1);	
						
							i=2;
						
					}
					else if (tp_dev.x<153&&tp_dev.x>74&&tp_dev.y<145&&tp_dev.y>105&&i==1)//北京
					{
						
							if(j==1)
							{
									LCD_Fill(348,68,410,110,GRAY );
									LCD_DrawRectangle(340,75,395,100); 
									Show_Str(360,80,WHITE, BLUE,"北京",16,1);		
									j++;
									Labeled_departure=1;
									delay_ms(200);
							}
							else 
							{
								LCD_Fill(347,131,402,180,GRAY );
								LCD_DrawRectangle(345,139,400,166); 	
								Show_Str(360,143,WHITE, BLUE,"北京",16,1);	
									Labeled_destination=1;
							}							
								
					}
					else if (tp_dev.x<283&&tp_dev.x>208&&tp_dev.y<145&&tp_dev.y>105&&i==1)//上海
					{
							if(j==1)
							{
								LCD_Fill(348,68,410,110,GRAY );
								LCD_DrawRectangle(340,75,395,100); 	
								Show_Str(360,80,WHITE, BLUE,"上海",16,1);		
									j++;
									Labeled_departure=2;
									delay_ms(200);
							}
								else 
							{
								LCD_Fill(347,131,402,180,GRAY );
								LCD_DrawRectangle(345,139,400,166); 	
								Show_Str(360,143,WHITE, BLUE,"上海",16,1);	
									Labeled_destination=2;								
							}
					}	
					else if (tp_dev.x<151&&tp_dev.x>76&&tp_dev.y<209&&tp_dev.y>178&&i==1)//广州
					{
							if(j==1)
							{
								LCD_Fill(348,68,410,110,GRAY );
								LCD_DrawRectangle(340,75,395,100); 	
								Show_Str(360,80,WHITE, BLUE,"广州",16,1);		
									j++;
									Labeled_departure=3;
									delay_ms(200);			
							}
							else  
							{
								LCD_Fill(347,131,402,180,GRAY );
								LCD_DrawRectangle(345,139,400,166); 	
								Show_Str(360,143,WHITE, BLUE,"广州",16,1);
											Labeled_destination=3;
							}
					}
					else if (tp_dev.x<153&&tp_dev.x>74&&tp_dev.y<145&&tp_dev.y>105&&i==2)//江门
					{
							if(j==1)
							{
								LCD_Fill(348,68,410,110,GRAY );
								LCD_DrawRectangle(340,75,395,100); 	
								Show_Str(360,80,WHITE, BLUE,"江门",16,1);		
									j++;
									Labeled_departure=4;
									delay_ms(200);
							}
							else  
							{
								LCD_Fill(347,131,402,180,GRAY );
								LCD_DrawRectangle(345,139,400,166); 	
								Show_Str(360,143,WHITE, BLUE,"江门",16,1);	
										Labeled_destination=4;
							}
					}
					else if (tp_dev.x<283&&tp_dev.x>208&&tp_dev.y<145&&tp_dev.y>105&&i==2)//深圳
					{
							if(j==1)
							{
								LCD_Fill(348,68,410,110,GRAY );
								LCD_DrawRectangle(340,75,395,100); 	
								Show_Str(360,80,WHITE, BLUE,"深圳",16,1);		
								j++;
								Labeled_departure=5;
								delay_ms(200);	
							}
							else 
							{
								LCD_Fill(347,131,402,180,GRAY );
								LCD_DrawRectangle(345,139,400,166); 	
								Show_Str(360,143,WHITE, BLUE,"深圳",16,1);	
									Labeled_destination=5;
							}
					}	
					else if (tp_dev.x<151&&tp_dev.x>76&&tp_dev.y<209&&tp_dev.y>178&&i==2)//佛山
					{
							if(j==1)
							{
								LCD_Fill(348,68,410,110,GRAY );
								LCD_DrawRectangle(340,75,395,100); 	
								Show_Str(360,80,WHITE, BLUE,"珠海",16,1);		
								j++;
								Labeled_departure=6;
								delay_ms(200);
							}
							else  
							{
								LCD_Fill(347,131,402,180,GRAY );
								LCD_DrawRectangle(345,139,400,166); 	
								Show_Str(360,143,WHITE, BLUE,"珠海",16,1);		
								Labeled_destination=6;
							}
					}
					else if (tp_dev.x<283&&tp_dev.x>208&&tp_dev.y<209&&tp_dev.y>178&&i==2)//珠海
					{
							if(j==1)
							{
								LCD_Fill(348,68,410,110,GRAY );
								LCD_DrawRectangle(340,75,395,100); 	
								Show_Str(360,80,WHITE, BLUE,"佛山",16,1);		
									j++;
								Labeled_departure=7;
									delay_ms(200);
							}
							else 
							{
								LCD_Fill(347,131,402,180,GRAY );
								LCD_DrawRectangle(345,139,400,166); 	
								Show_Str(360,143,WHITE, BLUE,"佛山",16,1);		
								Labeled_destination=7;
							}
					}
					else if (tp_dev.x<459&&tp_dev.x>404&&tp_dev.y<276&&tp_dev.y>247)//下一步
					{
						if(Labeled_departure==1)  sprintf(departure, "北京");
							else if(Labeled_departure==2)  sprintf(departure, "上海");
								else if(Labeled_departure==3)		sprintf(departure, "广州");
									else if(Labeled_departure==4)		sprintf(departure, "江门");
										else if(Labeled_departure==5)		sprintf(departure, "深圳");
											else if(Labeled_departure==6)		sprintf(departure, "珠海");
												else if(Labeled_departure==7)		sprintf(departure, "佛山");
						
						if(Labeled_destination==1)  sprintf(destination, "北京");
							else if(Labeled_destination==2)  sprintf(destination, "上海");
								else if(Labeled_destination==3)		sprintf(destination, "广州");
									else if(Labeled_destination==4)		sprintf(destination, "江门");
										else if(Labeled_destination==5)		sprintf(destination, "深圳");
											else if(Labeled_destination==6)		sprintf(destination, "珠海");
												else if(Labeled_destination==7)		sprintf(destination, "佛山");
							time_page();
					}
				
			}
		}
}

}

void main_page(){
	
LCD_Clear(WHITE);
LCD_Fill(0,0,lcddev.width,35,BLUE);
LCD_Fill(0,lcddev.height-35,lcddev.width,lcddev.height,BLUE);
Show_Str(350,10,WHITE,BLUE,"欢迎使用自助售票机",16,1);
Show_Str(30,10,WHITE,BLUE,"无纸化终端",16,1);
Show_Str(180,300,WHITE,BLUE,"五邑大学毕设版",16,1);
LCD_Fill(0,40,lcddev.width,280,GRAY );	
	
LCD_Fill(90,50, 260, 155,BLUE);
LCD_Fill(270,50,370,155,BLUE);	
LCD_Fill(90,165, 190, 270,BLUE);
LCD_Fill(200,165,370,270,BLUE);	
	
Show_Str(200,90,WHITE,BLUE,"购票",16,1);
Show_Str(290,120,WHITE,BLUE,"会员服务",16,1);
Show_Str(110,240,WHITE,BLUE,"电话订票",16,1);	
Show_Str(260,240,WHITE,BLUE,"互联网取票",16,1);		
	
Gui_Drawbmp16(90,80,gImage_1);	
Gui_Drawbmp16(270,165,gImage_2);		
Gui_Drawbmp16(295,50,gImage_3);	
Gui_Drawbmp16(125,165,gImage_4);	

	
	while(1)
	{
	
		tp_dev.scan(0); 
		if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
		{	
			if(tp_dev.x<lcddev.width&&tp_dev.y<lcddev.height)	// 在屏幕范围内
			{	
				
				if(tp_dev.x<260&&tp_dev.x>90&&tp_dev.y<160&&tp_dev.y>38)	
					{
							 buy_page();
					}				
			}
		}
	}

}

void time_page(){
u8 i;	
	
LCD_Fill(0,0,lcddev.width,35,BLUE);
LCD_Fill(0,lcddev.height-35,lcddev.width,lcddev.height,BLUE);
Show_Str(180,300,WHITE,BLUE,"五邑大学毕设版",16,1);
Show_Str(30,10,WHITE, BLUE,"上一步",16,1);
Show_Str(400,10,WHITE, BLUE,"首页",16,1);
LCD_Fill(0,40,lcddev.width,280,GRAY );

Show_Str(100,45,WHITE,BLUE,"车次",16,1);
Show_Str(190,45,WHITE,BLUE,"出发地",16,1);
Show_Str(270,45,WHITE,BLUE,"目的地",16,1);	
Show_Str(350,45,WHITE,BLUE,"乘车时间",16,1);	
	
LCD_Fill(390,255,450,274,BLUE);
Show_Str(400,260,WHITE, BLUE,"下一步",16,1);
	
LCD_DrawLine(0,70,480,70);//画线	
LCD_DrawLine(0,130,480,130);//画线		
LCD_DrawLine(0,190,480,190);//画线		
LCD_DrawLine(0,250,480,250);//画线	

	
LCD_DrawRectangle(40,90,50,100); 
Show_Str(120,90,WHITE,BLUE,"1",16,1);	
Show_Str(190,90,WHITE,BLUE,departure,16,1);	
Show_Str(270,90,WHITE,BLUE,destination,16,1);	
Show_Str(370,90,WHITE,BLUE,"8:00",16,1);	

LCD_DrawRectangle(40,150,50,160); 
Show_Str(120,150,WHITE,BLUE,"2",16,1);	
Show_Str(190,150,WHITE,BLUE,departure,16,1);	
Show_Str(270,150,WHITE,BLUE,destination,16,1);	
Show_Str(370,150,WHITE,BLUE,"11:00",16,1);

LCD_DrawRectangle(40,210,50,220); 
Show_Str(120,210,WHITE,BLUE,"3",16,1);	
Show_Str(190,210,WHITE,BLUE,departure,16,1);	
Show_Str(270,210,WHITE,BLUE,destination,16,1);	
Show_Str(370,210,WHITE,BLUE,"18:00",16,1);
	
	
	while(1)
	{
	
		tp_dev.scan(0); 
		if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
		{	
			if(tp_dev.x<lcddev.width&&tp_dev.y<lcddev.height)	// 在屏幕范围内
			{	
				
									
					if(tp_dev.x<84&&tp_dev.x>35&&tp_dev.y<30&&tp_dev.y>4)	//上一步
					{
							buy_page();
					}
					else if (tp_dev.x<456&&tp_dev.x>398&&tp_dev.y<28&&tp_dev.y>5)//首页
					{
							main_page();
					}	
					else if (tp_dev.y<120&&tp_dev.y>60)
					{
						
						LCD_Fill(40,150,50,160,GRAY );
						LCD_Fill(40,210,50,220,GRAY );
						LCD_DrawRectangle(40,150,50,160);
						LCD_DrawRectangle(40,210,50,220); 						
						LCD_Fill(40,90,50,100,BLUE);
						i=1;
					}
					else if (tp_dev.y<178&&tp_dev.y>125)
					{
						
						LCD_Fill(40,90,50,100,GRAY );
						LCD_Fill(40,210,50,220,GRAY );
						LCD_DrawRectangle(40,90,50,100); 
						LCD_DrawRectangle(40,210,50,220); 						
						LCD_Fill(40,150,50,160,BLUE);
						i=2;
					}
					else if (tp_dev.y<250&&tp_dev.y>185)
					{
						
						LCD_Fill(40,90,50,100,GRAY );
						LCD_Fill(40,150,50,160,GRAY );
						LCD_DrawRectangle(40,90,50,100); 
						LCD_DrawRectangle(40,150,50,160); 						
						LCD_Fill(40,210,50,220,BLUE);
						i=3;
					}
					else if (tp_dev.x<454&&tp_dev.x>388&&tp_dev.y<280&&tp_dev.y>240)
					{
						if(i==1)
							sprintf(Ride_time, "8:00");
						else if(i==2)
							sprintf(Ride_time, "11:00");
						else if(i==3)
							sprintf(Ride_time, "18:00");
						num_page();
						
					}
			}
		}
	}


}

void sure_page(){

LCD_Fill(0,0,lcddev.width,35,BLUE);
LCD_Fill(0,lcddev.height-35,lcddev.width,lcddev.height,BLUE);
LCD_Fill(0,40,lcddev.width,280,GRAY );
Show_Str(180,300,WHITE,BLUE,"五邑大学毕设版",16,1);

LCD_Fill(35,38,400,280,BLUE);
Show_Str(190,100,WHITE,BLUE,"确认购买",16,1);
Show_Str(110,200,WHITE,BLUE,"是",16,1);
Show_Str(310,200,WHITE,BLUE,"否",16,1);

while(1)
	{
		tp_dev.scan(0); 
		if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
		{	
			if(tp_dev.x<lcddev.width&&tp_dev.y<lcddev.height)	// 在屏幕范围内
			{					
				if(tp_dev.x<150&&tp_dev.x>110&&tp_dev.y<220&&tp_dev.y>193)	
					{
							sprintf(send_buff, "[铁路12306]，订单%s，您已订购从%s开往%s的车票，火车将于%s开出，请提前30分钟到火车站取票并乘车！",cardnum,
														departure,destination,Ride_time);
							Wifi_SendString(send_buff);	
							quit_page();
					}	
				else if (tp_dev.x<345&&tp_dev.x>305&&tp_dev.y<350&&tp_dev.y>180)	
				{
					main_page();
				}
			}
		}
	}
	
}


void quit_page(){

LCD_Fill(0,0,lcddev.width,35,BLUE);
LCD_Fill(0,lcddev.height-35,lcddev.width,lcddev.height,BLUE);
LCD_Fill(0,40,lcddev.width,280,GRAY );
Show_Str(180,300,WHITE,BLUE,"五邑大学毕设版",16,1);

LCD_Fill(35,38,400,280,BLUE);
Show_Str(170,100,WHITE,BLUE,"您已成功购买该票",16,1);
Show_Str(300,260,WHITE,BLUE,"退出",16,1);
TIM3_Int_Init(7199,9999);	
	
while(1)
	{
		
		tp_dev.scan(0); 
		if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
		{	
			if(tp_dev.x<lcddev.width&&tp_dev.y<lcddev.height)	// 在屏幕范围内
			{					
				if(tp_dev.x<348&&tp_dev.x>285&&tp_dev.y<285&&tp_dev.y>240)	
					{
							RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, DISABLE);
							main_page();
					}	
			}
		}
	}

}


void InitializeSystem(){
	LED_OFF;
	delay_ms(50);
	PcdReset();
	PcdAntennaOff(); 
	PcdAntennaOn();  
	M500PcdConfigISOType( 'A' );
	LED_ON;
	delay_ms(50);	
	LED_OFF;
	delay_ms(50);
	LED_ON;
	delay_ms(50);	
	LED_OFF;
}

int main(void)
{	

	SystemInit();		//初始化RCC 设置系统主频为72MHZ
	delay_init(72);	     //延时初始化
	LED_Init();
	UART1_Init(115200);	
	UART3_Init(115200);	
	Wifi_Start();
	LCD_Init();	   //液晶屏初始化
	TP_Init(); 
	LCD_Clear(WHITE);
	RC522_Init();
	InitializeSystem();


 while (1)
  {
		main_page();
	}
	
}


