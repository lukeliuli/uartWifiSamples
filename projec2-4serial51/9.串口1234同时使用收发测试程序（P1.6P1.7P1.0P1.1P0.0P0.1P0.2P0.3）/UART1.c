
#include <STC15W4KXX.H>
#include <intrins.h>
#include"Init_IO.h" 
bit flagFrame1 = 0;  //帧接收完成标志，即接收到一帧新数据
bit flagTxd1 = 0;    //单字节发送完成标志，用来替代TXD中断标志位
unsigned char cntRxd1 = 0;   //接收字节计数器
unsigned char pdata bufRxd1[32];  //接收字节缓冲区
#define S1_S0 0x40
#define S1_S1 0x80

extern void UartAction(unsigned char *buf1, unsigned char len1,unsigned char *buf2, unsigned char len2,unsigned char *buf3, unsigned char len3,unsigned char *buf4, unsigned char len4);
/* 串口配置函数，baud-通信波特率 */

void ConfigUART1(unsigned long baud1)
{

    P_SW1&=~(S1_S0);  //将串口一切换至P1.6和P1.7
	P_SW1|=(S1_S1);	  //将串口一切换至P1.6和P1.7


	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设定定时器1为16位自动重装方式
    TL1 = (65536-(11059200/4/baud1));  //计算T1重载值
    TH1 = (65536-(11059200/4/baud1))>>8;     //初值等于重载值
    ET1 = 0;       //禁止T1中断
    TR1 = 1;       //启动T1
    ES  = 1;       //使能串口中断
    
}
/* 软件延时函数，延时时间(t*10)us */
void DelayX10us1(unsigned char t1)
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
        
        
    } while (--t1);
}
/* 串口数据写入，即串口发送函数，buf-待发送数据的指针，len-指定的发送长度 */
void UartWrite1(unsigned char *buf1, unsigned char len1)
{

    while (len1--)   //循环发送所有字节
    {
        flagTxd1 = 0;      //清零发送标志
        SBUF = *buf1++;    //发送一个字节数据
        while (!flagTxd1); //等待该字节发送完成
    }
    DelayX10us1(5);  //等待最后的停止位完成，延时时间由波特率决定

}

/* 串口数据读取函数，buf-接收指针，len-指定的读取长度，返回值-实际读到的长度 */
unsigned char UartRead1(unsigned char *buf1, unsigned char len1)
{
    unsigned char i;
    
    if (len1 > cntRxd1)  //指定读取长度大于实际接收到的数据长度时，
    {                  //读取长度设置为实际接收到的数据长度
        len1 = cntRxd1;
    }
    for (i=0; i<len1; i++)  //拷贝接收到的数据到接收指针上
    {
        *buf1++ = bufRxd1[i];
    }
    cntRxd1 = 0;  //接收计数器清零
    
    return len1;  //返回实际读取长度
}
/* 串口接收监控，由空闲时间判定帧结束，需在定时中断中调用，ms-定时间隔 */
void UartRxMonitor1(unsigned char ms1)
{
    static unsigned char cntbkp1 = 0;
    static unsigned char idletmr1 = 0;

    if (cntRxd1 > 0)  //接收计数器大于零时，监控总线空闲时间
    {
        if (cntbkp1 != cntRxd1)  //接收计数器改变，即刚接收到数据时，清零空闲计时
        {
            cntbkp1 = cntRxd1;
            idletmr1 = 0;
        }
        else                   //接收计数器未改变，即总线空闲时，累积空闲时间
        {
            if (idletmr1 < 30)  //空闲计时小于30ms时，持续累加
            {
                idletmr1 += ms1;
                if (idletmr1 >= 30)  //空闲时间达到30ms时，即判定为一帧接收完毕	
                {
                    flagFrame1 = 1;  //设置帧接收完成标志
                }
            }
        }
    }
    else
    {
        cntbkp1 = 0;
    }
}
/* 串口驱动函数，监测数据帧的接收，调度功能函数，需在主循环中调用 */
void UartDriver1()
{
    unsigned char len1;
    unsigned char pdata buf1[40];
    unsigned char len2;
    unsigned char pdata buf2[40];    
    unsigned char len3;
    unsigned char pdata buf3[40];
    unsigned char len4;
    unsigned char pdata buf4[40];
    
    if (flagFrame1) //有命令到达时，读取处理该命令
    {
        flagFrame1 = 0;
        len1 = UartRead1(buf1, sizeof(buf1)-2); //将接收到的命令读取到缓冲区中	 
        UartAction(buf1, len1,buf2, len2,buf3, len3, buf4, len4);  //传递数据帧，调用动作执行函数
    }
}
/* 串口中断服务函数 */
void InterruptUART1() interrupt 4
{
    if (RI)  //接收到新字节	   当字节发送到结束位的一半的时候RI接收标志变为1  进入窜口中断
    {
        RI = 0;  //清零接收中断标志位
        if (cntRxd1< sizeof(bufRxd1)) //接收缓冲区尚未用完时，
        {                            //保存接收字节，并递增计数器
            bufRxd1[cntRxd1++] = SBUF;

        }
    }
    if (TI)  //字节发送完毕
    {
        TI = 0;   //清零发送中断标志位
        flagTxd1 = 1;  //设置字节发送完成标志
    }
}
