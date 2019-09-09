
#include <STC15W4KXX.H>
#include <intrins.h>
#include"Init_IO.h"
#include<string.h>
bit flagFrame4 = 0;  //帧接收完成标志，即接收到一帧新数据
bit flagTxd4 = 0;    //单字节发送完成标志，用来替代TXD中断标志位
unsigned char cntRxd4 = 0;   //接收字节计数器
unsigned char  bufRxd4[128];  //接收字节缓冲区	 //原来只有32字节，我改为128字节
unsigned char len4;
unsigned char buf4[128];	   //原来只有40字节，我改为128字节
#define ES4 0x10
#define S4RI 0x01
#define S4TI 0x02
extern void UartAction4(unsigned char *buf4, unsigned char len4);
extern timer4count;
void ConfigUART4(unsigned int baud4)
{
//     RS485_DIR4 = 0; //RS485设置为接收方向
	S4CON = 0x10;		//8位数据,可变波特率
	S4CON |= 0x40;		//串口3选择定时器3为波特率发生器
	T4T3M |= 0x20;		//定时器3时钟为Fosc,即1T
	T4L =(65536-(11059200/4/baud4));		//设定定时初值
	T4H =(65536-(11059200/4/baud4))>>8;		//设定定时初值
	T4T3M |= 0x80;		//启动定时器3
	IE2|=ES4;				 //开串口2中断
}


/* 软件延时函数，延时时间(t*10)us */
void DelayX10us4(unsigned char t4)
{
    do {
        _nop_();
        _nop_();	  
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
		_nop_();
        _nop_();
        
        
    } while (--t4);
}
/* 串口数据写入，即串口发送函数，buf-待发送数据的指针，len-指定的发送长度 */
void UartWrite4(unsigned char *buf4, unsigned char len4)
{
//     RS485_DIR4 = 1;  //RS485设置为发送
    while (len4--)   //循环发送所有字节
    {
        flagTxd4 = 0;      //清零发送标志
        S4BUF = *buf4++;    //发送一个字节数据
        while (!flagTxd4); //等待该字节发送完成
    }
    DelayX10us4(5);  //等待最后的停止位完成，延时时间由波特率决定
//     RS485_DIR4 = 0;  //RS485设置为接收
}



/* 串口数据读取函数，buf4-接收指针，len4-指定的读取长度，返回值-实际读到的长度 */
unsigned char UartRead4(unsigned char *buf4, unsigned char len4)
{
    unsigned char i;
		memset(buf4,0,sizeof(buf4));	  //接受寄存器清零
    
    if (len4 > cntRxd4)  //指定读取长度大于实际接收到的数据长度时，
    {                  //读取长度设置为实际接收到的数据长度
        len4 = cntRxd4;
    }
    for (i=0; i<len4; i++)  //拷贝接收到的数据到接收指针上
    {
        *buf4++ = bufRxd4[i];
    }
    cntRxd4 = 0;  //接收计数器清零
	memset(bufRxd4,0,sizeof(bufRxd4));	  //接受寄存器清零
    
    return len4;  //返回实际读取长度
}
/* 串口接收监控，由空闲时间判定帧结束，需在定时中断中调用，ms-定时间隔 */

void UartRxMonitor4(unsigned char ms4)
{
    static unsigned char cntbkp4 = 0;
    static unsigned char idletmr4 = 0;

    if (cntRxd4 > 0)  //接收计数器大于零时，监控总线空闲时间
    {
        if (cntbkp4 != cntRxd4)  //接收计数器改变，即刚接收到数据时，清零空闲计时
        {
            cntbkp4 = cntRxd4;
            idletmr4 = 0;
        }
        else                   //接收计数器未改变，即总线空闲时，累积空闲时间
        {
            if (idletmr4 < 30)  //空闲计时小于30ms时，持续累加
            {
                idletmr4 += ms4;
                if (idletmr4 >= 30)  //空闲时间达到30ms时，即判定为一帧接收完毕	
                {
                    flagFrame4 = 1;  //设置帧接收完成标志
                }

			
            }
        }
    }
    else
    {
        cntbkp4 = 0;
    }
	
 timer4count++;



}
/* 串口驱动函数，监测数据帧的接收，调度功能函数，需在主循环中调用 */
void UartDriver4()
{
   // unsigned char len1;
  //  unsigned char pdata buf1[40];      //原来只有40字节，我改为128字节
  ////  unsigned char len2;
   // unsigned char pdata buf2[40];	 	//原来只有40字节，我改为128字节
  //  unsigned char len3;
  //  unsigned char pdata buf3[40];	   //原来只有40字节，我改为128字节

    
    if (flagFrame4) //有命令到达时，读取处理该命令
    {
        flagFrame4 = 0;
        len4 = UartRead4(buf4, sizeof(buf4)); //将接收到的命令读取到缓冲区中	 
        UartAction4(buf4, len4);  //传递数据帧，调用动作执行函数
    }
}
/* 串口中断服务函数 */
void InterruptUART4() interrupt 18 
{
   
	if (S4CON&S4RI)  //接收到新字节	   当字节发送到结束位的一半的时候S2RI接收标志变为1  进入窜口中断
    {
        S4CON&=~S4RI;  //清零接收中断标志位
        if (cntRxd4 < sizeof(bufRxd4)) //接收缓冲区尚未用完时，
        {                            //保存接收字节，并递增计数器
            
			bufRxd4[cntRxd4++] = S4BUF;	

			if(S4BUF == '\0' || S4BUF == '\n' || S4BUF == '\r')
			flagFrame4 = 1;

        }
		
    }
    if (S4CON&S4TI)  //字节发送完毕
    {
        S4CON&=~S4TI;   //清零发送中断标志位
        flagTxd4 = 1;  //设置字节发送完成标志
    }

	 timer4count++;
}
