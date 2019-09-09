
#include <STC15W4KXX.H>
#include <intrins.h>
#include"Init_IO.h"
bit flagFrame3 = 0;  //帧接收完成标志，即接收到一帧新数据
bit flagTxd3 = 0;    //单字节发送完成标志，用来替代TXD中断标志位
unsigned char cntRxd3 = 0;   //接收字节计数器
unsigned char xdata bufRxd3[32];  //接收字节缓冲区	 //原来只有32字节，我改为128字节
#define ES3 0x08
#define S3RI 0x01
#define S3TI 0x02
extern void UartAction(unsigned char *buf1, unsigned char len1,unsigned char *buf2, unsigned char len2,unsigned char *buf3, unsigned char len3,unsigned char *buf4, unsigned char len4);

void ConfigUART3(unsigned int baud3)
{
//     RS485_DIR3 = 0; //RS485设置为接收方向
	S3CON = 0x10;		//8位数据,可变波特率
	S3CON |= 0x40;		//串口3选择定时器3为波特率发生器
	T4T3M |= 0x02;		//定时器3时钟为Fosc,即1T
	T3L =(65536-(11059200/4/baud3));		//设定定时初值
	T3H =(65536-(11059200/4/baud3))>>8;		//设定定时初值
	T4T3M |= 0x08;		//启动定时器3
	IE2|=ES3;				 //开串口2中断
}


/* 软件延时函数，延时时间(t*10)us */
void DelayX10us3(unsigned char t3)
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
        
        
    } while (--t3);
}
/* 串口数据写入，即串口发送函数，buf-待发送数据的指针，len-指定的发送长度 */
void UartWrite3(unsigned char *buf3, unsigned char len3)
{
//     RS485_DIR3 = 1;  //RS485设置为发送
    while (len3--)   //循环发送所有字节
    {
        flagTxd3 = 0;      //清零发送标志
        S3BUF = *buf3++;    //发送一个字节数据
        while (!flagTxd3); //等待该字节发送完成
    }
    DelayX10us3(5);  //等待最后的停止位完成，延时时间由波特率决定
//     RS485_DIR3 = 0;  //RS485设置为接收
}



/* 串口数据读取函数，buf2-接收指针，len2-指定的读取长度，返回值-实际读到的长度 */
unsigned char UartRead3(unsigned char *buf3, unsigned char len3)
{
    unsigned char i;
    
    if (len3 > cntRxd3)  //指定读取长度大于实际接收到的数据长度时，
    {                  //读取长度设置为实际接收到的数据长度
        len3 = cntRxd3;
    }
    for (i=0; i<len3; i++)  //拷贝接收到的数据到接收指针上
    {
        *buf3++ = bufRxd3[i];
    }
    cntRxd3 = 0;  //接收计数器清零
    
    return len3;  //返回实际读取长度
}
/* 串口接收监控，由空闲时间判定帧结束，需在定时中断中调用，ms-定时间隔 */
void UartRxMonitor3(unsigned char ms3)
{
    static unsigned char cntbkp3 = 0;
    static unsigned char idletmr3 = 0;

    if (cntRxd3 > 0)  //接收计数器大于零时，监控总线空闲时间
    {
        if (cntbkp3 != cntRxd3)  //接收计数器改变，即刚接收到数据时，清零空闲计时
        {
            cntbkp3 = cntRxd3;
            idletmr3 = 0;
        }
        else                   //接收计数器未改变，即总线空闲时，累积空闲时间
        {
            if (idletmr3 < 30)  //空闲计时小于30ms时，持续累加
            {
                idletmr3 += ms3;
                if (idletmr3 >= 30)  //空闲时间达到30ms时，即判定为一帧接收完毕	
                {
                    flagFrame3 = 1;  //设置帧接收完成标志
                }
            }
        }
    }
    else
    {
        cntbkp3 = 0;
    }
}
/* 串口驱动函数，监测数据帧的接收，调度功能函数，需在主循环中调用 */
void UartDriver3()
{
    unsigned char len1;
    unsigned char pdata buf1[40];    //原来只有40字节，我改为128字节
    unsigned char len2;
    unsigned char pdata buf2[40];	 	 //原来只有40字节，我改为128字节
    unsigned char len3;
    unsigned char pdata buf3[40];		//原来只有40字节，我改为128字节
    unsigned char len4;
    unsigned char pdata buf4[40];	   //原来只有40字节，我改为128字节
    
    if (flagFrame3) //有命令到达时，读取处理该命令
    {
        flagFrame3 = 0;
        len3 = UartRead3(buf3, sizeof(buf3)-2); //将接收到的命令读取到缓冲区中	 
        UartAction(buf1, len1,buf2, len2,buf3, len3, buf4, len4);  //传递数据帧，调用动作执行函数
    }
}
/* 串口中断服务函数 */
void InterruptUART3() interrupt 17 
{
    if (S3CON&S3RI)  //接收到新字节	   当字节发送到结束位的一半的时候S2RI接收标志变为1  进入窜口中断
    {
        S3CON&=~S3RI;  //清零接收中断标志位
        if (cntRxd3 < sizeof(bufRxd3)) //接收缓冲区尚未用完时，
        {                            //保存接收字节，并递增计数器
            bufRxd3[cntRxd3++] = S3BUF;	
        }
    }
    if (S3CON&S3TI)  //字节发送完毕
    {
        S3CON&=~S3TI;   //清零发送中断标志位
        flagTxd3 = 1;  //设置字节发送完成标志
    }
}
