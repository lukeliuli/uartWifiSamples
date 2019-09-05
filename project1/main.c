/********************************************************************
                           伟达电子工作室

实现功能:基于STC12C5A60S2单片机实现双串口或者双485对八个led的控制
使用芯片：STC12C5A60S2系列
晶振：11.0592MHZ
波特率：9600(有无校验位：无；数据位：8；停止位：1；)
数据格式：01/02/03/04+ff/00
编译环境：Keil
作者：fenglongmeng
qq 490022104 CALL18003710439
网站：wddz.taobao.com
【声明】此程序仅用于学习与参考，引用请注明版权和作者信息！     

*********************************************************************/
/********************************************************************
                           功能说明（串口1234通讯）

PC发送01FF串口1，灯全亮;串口1向PC发送01AA;
PC发送0100串口1，灯全灭;串口1向PC发送01BB;
PC发送02FF串口2，灯全亮;串口2向PC发送02AA;
PC发送0200串口2，灯全灭;串口2向PC发送02BB;
PC发送03FF串口3，灯全亮;串口3向PC发送03AA;
PC发送0300串口3，灯全灭;串口3向PC发送03BB;
PC发送04FF串口4，灯全亮;串口4向PC发送04AA;
PC发送0400串口4，灯全灭;串口4向PC发送04BB;
*********************************************************************/
#include <stdio.h>
#include <string.h>
#include <STC15W4KXX.H>
#include <intrins.h>	  //用nop函数延时的头文件
#include"Init_IO.h" 
#define uchar unsigned char//宏定义无符号字符型
#define uint unsigned int  //宏定义无符号整型
unsigned char T0RH = 0;  //T0重载值的高字节
unsigned char T0RL = 0;  //T0重载值的低字节
void ConfigTimer0(unsigned int ms);	   //配置并启动T0，ms-T0定时时间 



extern void UartDriver1();				//串口驱动函数，监测数据帧的接收，调度功能函数，需在主循环中调用
extern void ConfigUART1(unsigned int baud1);	//串口配置函数，baud-通信波特率 
extern void UartRxMonitor1(unsigned char ms1); //串口接收监控，由空闲时间判定帧结束，需在定时中断中调用，ms-定时间隔
extern void UartWrite1(unsigned char *buf1, unsigned char len1);	//串口数据写入，即串口发送函数，buf-待发送数据的指针，len-指定的发送长度

extern void UartDriver2();				//串口驱动函数，监测数据帧的接收，调度功能函数，需在主循环中调用
extern void ConfigUART2(unsigned int baud2);	//串口配置函数，baud-通信波特率 
extern void UartRxMonitor2(unsigned char ms2); //串口接收监控，由空闲时间判定帧结束，需在定时中断中调用，ms-定时间隔
extern void UartWrite2(unsigned char *buf2, unsigned char len2);	//串口数据写入，即串口发送函数，buf-待发送数据的指针，len-指定的发送长度

extern void UartDriver3();				//串口驱动函数，监测数据帧的接收，调度功能函数，需在主循环中调用
extern void ConfigUART3(unsigned int baud3);	//串口配置函数，baud-通信波特率 
extern void UartRxMonitor3(unsigned char ms3); //串口接收监控，由空闲时间判定帧结束，需在定时中断中调用，ms-定时间隔
extern void UartWrite3(unsigned char *buf3, unsigned char len3);	//串口数据写入，即串口发送函数，buf-待发送数据的指针，len-指定的发送长度

extern void UartDriver4();				//串口驱动函数，监测数据帧的接收，调度功能函数，需在主循环中调用
extern void ConfigUART4(unsigned int baud4);	//串口配置函数，baud-通信波特率 
extern void UartRxMonitor4(unsigned char ms4); //串口接收监控，由空闲时间判定帧结束，需在定时中断中调用，ms-定时间隔
extern void UartWrite4(unsigned char *buf4, unsigned char len4);	//串口数据写入，即串口发送函数，buf-待发送数据的指针，len-指定的发送长度

uchar sdat;
int sendData[8] = {10,20,30,40,50,60,70,80};
uchar int2str1[5];
uchar int2str2[5];
uchar int2str3[5];
uchar int2str4[5];
uchar int2str5[5];
uchar int2str6[5];
uchar int2str7[5];
uchar int2str8[5];
uchar strSend[50];


void Delay300ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	i = 20;
	j = 67;
	k = 183;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void my_itoa(int n,char s[]);

void ConfigP0P1P2P3P4P5P6P7(void)
{
   P0M1=0x00;		   
   P0M0=0x00; 
   P1M1=0x00;		   
   P1M0=0x00;		   
   P2M1=0x00;
   P2M0=0x00;		   
   P3M1=0x00; 
   P3M0=0x00;		   
   P4M1=0x00; 
   P4M0=0x00;		   
   P5M1=0x00;
   P5M0=0x00;		   
   P6M1=0x00; 
   P6M0=0x00;		   
   P7M1=0x00; 
   P7M0=0x00;		     

}
void main()
{
     
    EA = 1;            //开总中断
    ConfigP0P1P2P3P4P5P6P7();
    ConfigTimer0(1);   //配置T0定时1ms
    ConfigUART1(57600);  //配置波特率为9600。115200似乎有问题
    ConfigUART2(57600);  //配置波特率为9600。 115200似乎有问题
    ConfigUART3(57600);  //配置波特率为9600。 115200似乎有问题    
    ConfigUART4(57600);  //配置波特率为9600。 115200似乎有问题
    
	
    while (1)
    {	
	    Delay300ms();
		Delay300ms();
		//Delay300ms();
        sendData[7]=51;

		//my_itoa输入数据最大9999
		my_itoa(sendData[0],int2str1);
		my_itoa(sendData[1],int2str2);
		my_itoa(sendData[2],int2str3);
		my_itoa(sendData[3],int2str4);
		my_itoa(sendData[4],int2str5);
		my_itoa(sendData[5],int2str6);
		my_itoa(sendData[6],int2str7);
		my_itoa(sendData[7],int2str8);

		sprintf(strSend,"%s %s %s %s %s %s %s %s %s\n",int2str1,int2str2,int2str3,int2str4,int2str5,int2str6,int2str7,int2str8);

	   	//UartWrite4("1 2 3 4 5 6 7 1\n",16+1);
	//	UartWrite3("1 2 3 4 5 6 7 1\n",16+1);
		UartWrite4(strSend,strlen(strSend));
		//UartWrite3(strSend,strlen(strSend)+1);
		
		//sprintf(strSend,"%d %d %d %d %d %d %d %d %d",1,2,3,4,5,6,7,8); 51单片机不支持sprintf
		//UartWrite4(strSend,strlen(strSend)+1)	;


		UartDriver1();  //调用串口驱动
        UartDriver2();  //调用串口驱动
        UartDriver3();  //调用串口驱动 
        UartDriver4();  //调用串口驱动


    }
}

/* 串口动作函数，根据接收到的命令帧执行响应的动作
   buf-接收到的命令帧指针，len-命令帧长度 */
void UartAction(unsigned char *buf1, unsigned char len1,unsigned char *buf2, unsigned char len2,unsigned char *buf3, unsigned char len3,unsigned char *buf4, unsigned char len4)
{
		/*
        if((buf1[0] ==0x01)&&(buf1[1] ==0x00)&&(len1 ==2))
            {OUT1=OUT2=OUT3=OUT4=1;UartWrite1(a1, 2);}     
        if((buf1[0] ==0X01)&&(buf1[1] ==0xFF)&&(len1 ==2))
            {OUT1=OUT2=OUT3=OUT4=0;UartWrite1(a2, 2);} 
     
        if((buf2[0] ==0x02)&&(buf2[1] ==0x00)&&(len2 ==2))
            {OUT1=OUT2=OUT3=OUT4=1;UartWrite2(b1, 2);}     
        if((buf2[0] ==0X02)&&(buf2[1] ==0xFF)&&(len2 ==2))
            {OUT1=OUT2=OUT3=OUT4=0;UartWrite2(b2, 2);}

        if((buf3[0] ==0x03)&&(buf3[1] ==0x00)&&(len3 ==2))
            {OUT1=OUT2=OUT3=OUT4=1;UartWrite3(c1, 2);}     
        if((buf3[0] ==0X03)&&(buf3[1] ==0xFF)&&(len3 ==2))
            {OUT1=OUT2=OUT3=OUT4=0;UartWrite3(c2, 2);}

        if((buf4[0] ==0x04)&&(buf4[1] ==0x00)&&(len4 ==2))
            {OUT1=OUT2=OUT3=OUT4=1;UartWrite4(d1, 2);}     
        if((buf4[0] ==0X04)&&(buf4[1] ==0xFF)&&(len4 ==2))
            {OUT1=OUT2=OUT3=OUT4=0;UartWrite4(d2, 2);}	  */
			
		 UartWrite3(buf4,len4);
			

}
/* 配置并启动T0，ms-T0定时时间 */
void ConfigTimer0(unsigned int ms)
{
    unsigned long tmp;  //临时变量
    
    tmp = 11059200 / 12;      //定时器计数频率
    tmp = (tmp * ms) / 1000;  //计算所需的计数值
    tmp = 65536 - tmp;        //计算定时器重载值
    tmp = tmp + 33;           //补偿中断响应延时造成的误差
    T0RH = (unsigned char)(tmp>>8);  //定时器重载值拆分为高低字节
    T0RL = (unsigned char)tmp;
    TMOD &= 0xF0;   //清零T0的控制位
    TMOD |= 0x01;   //配置T0为模式1
    TH0 = T0RH;     //加载T0重载值
    TL0 = T0RL;
    ET0 = 1;        //使能T0中断
    TR0 = 1;        //启动T0
}
/* T0中断服务函数，执行串口接收监控 */
void InterruptTimer0() interrupt 1
{

    TH0 = T0RH;  //重新加载重载值
    TL0 = T0RL;
    UartRxMonitor1(1);  //串口接收监控
    UartRxMonitor2(1);  //串口接收监控 
    UartRxMonitor3(1);  //串口接收监控  
    UartRxMonitor4(1);  //串口接收监控    


}




 

/*

 * 功能：整数转换为字符串

 * char s[] 的作用是存储整数的每一位

 */
	//n最大9999
void my_itoa(int n,char s[])

{

    int i = 0,isNegative = 0;
	//reverse
	 char temp;

    char *p = s;    //p指向s的头部

    char *q = s;    //q指向s的尾部



     memset(s,'0',sizeof(s));

    if((isNegative = n) < 0) //如果是负数，先转为正数

    {

        n = -n;

    }

    do      //从各位开始变为字符，直到最高位，最后应该反转

    {

        s[i++] = n%10 + '0';

        n = n/10;

    }while(n > 0);

    

    if(isNegative < 0)   //如果是负数，补上负号

    {

        s[i++] = '-';

    }

    s[i] = '\0';    //最后加上字符串结束符

    

    while(*q)

        ++q;

    q--;

    

    //交换移动指针，直到p和q交叉

    while(q > p)

    {

        temp = *p;

        *p++ = *q;

        *q-- = temp;

    }

   

}