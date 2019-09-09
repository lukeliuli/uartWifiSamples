
#include <STC15W4KXX.H>
#include <intrins.h>
#include"Init_IO.h"
bit flagFrame3 = 0;  //֡������ɱ�־�������յ�һ֡������
bit flagTxd3 = 0;    //���ֽڷ�����ɱ�־���������TXD�жϱ�־λ
unsigned char cntRxd3 = 0;   //�����ֽڼ�����
unsigned char xdata bufRxd3[32];  //�����ֽڻ�����	 //ԭ��ֻ��32�ֽڣ��Ҹ�Ϊ128�ֽ�
#define ES3 0x08
#define S3RI 0x01
#define S3TI 0x02
extern void UartAction(unsigned char *buf1, unsigned char len1,unsigned char *buf2, unsigned char len2,unsigned char *buf3, unsigned char len3,unsigned char *buf4, unsigned char len4);

void ConfigUART3(unsigned int baud3)
{
//     RS485_DIR3 = 0; //RS485����Ϊ���շ���
	S3CON = 0x10;		//8λ����,�ɱ䲨����
	S3CON |= 0x40;		//����3ѡ��ʱ��3Ϊ�����ʷ�����
	T4T3M |= 0x02;		//��ʱ��3ʱ��ΪFosc,��1T
	T3L =(65536-(11059200/4/baud3));		//�趨��ʱ��ֵ
	T3H =(65536-(11059200/4/baud3))>>8;		//�趨��ʱ��ֵ
	T4T3M |= 0x08;		//������ʱ��3
	IE2|=ES3;				 //������2�ж�
}


/* �����ʱ��������ʱʱ��(t*10)us */
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
/* ��������д�룬�����ڷ��ͺ�����buf-���������ݵ�ָ�룬len-ָ���ķ��ͳ��� */
void UartWrite3(unsigned char *buf3, unsigned char len3)
{
//     RS485_DIR3 = 1;  //RS485����Ϊ����
    while (len3--)   //ѭ�����������ֽ�
    {
        flagTxd3 = 0;      //���㷢�ͱ�־
        S3BUF = *buf3++;    //����һ���ֽ�����
        while (!flagTxd3); //�ȴ����ֽڷ������
    }
    DelayX10us3(5);  //�ȴ�����ֹͣλ��ɣ���ʱʱ���ɲ����ʾ���
//     RS485_DIR3 = 0;  //RS485����Ϊ����
}



/* �������ݶ�ȡ������buf2-����ָ�룬len2-ָ���Ķ�ȡ���ȣ�����ֵ-ʵ�ʶ����ĳ��� */
unsigned char UartRead3(unsigned char *buf3, unsigned char len3)
{
    unsigned char i;
    
    if (len3 > cntRxd3)  //ָ����ȡ���ȴ���ʵ�ʽ��յ������ݳ���ʱ��
    {                  //��ȡ��������Ϊʵ�ʽ��յ������ݳ���
        len3 = cntRxd3;
    }
    for (i=0; i<len3; i++)  //�������յ������ݵ�����ָ����
    {
        *buf3++ = bufRxd3[i];
    }
    cntRxd3 = 0;  //���ռ���������
    
    return len3;  //����ʵ�ʶ�ȡ����
}
/* ���ڽ��ռ�أ��ɿ���ʱ���ж�֡���������ڶ�ʱ�ж��е��ã�ms-��ʱ��� */
void UartRxMonitor3(unsigned char ms3)
{
    static unsigned char cntbkp3 = 0;
    static unsigned char idletmr3 = 0;

    if (cntRxd3 > 0)  //���ռ�����������ʱ��������߿���ʱ��
    {
        if (cntbkp3 != cntRxd3)  //���ռ������ı䣬���ս��յ�����ʱ��������м�ʱ
        {
            cntbkp3 = cntRxd3;
            idletmr3 = 0;
        }
        else                   //���ռ�����δ�ı䣬�����߿���ʱ���ۻ�����ʱ��
        {
            if (idletmr3 < 30)  //���м�ʱС��30msʱ�������ۼ�
            {
                idletmr3 += ms3;
                if (idletmr3 >= 30)  //����ʱ��ﵽ30msʱ�����ж�Ϊһ֡�������	
                {
                    flagFrame3 = 1;  //����֡������ɱ�־
                }
            }
        }
    }
    else
    {
        cntbkp3 = 0;
    }
}
/* ���������������������֡�Ľ��գ����ȹ��ܺ�����������ѭ���е��� */
void UartDriver3()
{
    unsigned char len1;
    unsigned char pdata buf1[40];    //ԭ��ֻ��40�ֽڣ��Ҹ�Ϊ128�ֽ�
    unsigned char len2;
    unsigned char pdata buf2[40];	 	 //ԭ��ֻ��40�ֽڣ��Ҹ�Ϊ128�ֽ�
    unsigned char len3;
    unsigned char pdata buf3[40];		//ԭ��ֻ��40�ֽڣ��Ҹ�Ϊ128�ֽ�
    unsigned char len4;
    unsigned char pdata buf4[40];	   //ԭ��ֻ��40�ֽڣ��Ҹ�Ϊ128�ֽ�
    
    if (flagFrame3) //�������ʱ����ȡ���������
    {
        flagFrame3 = 0;
        len3 = UartRead3(buf3, sizeof(buf3)-2); //�����յ��������ȡ����������	 
        UartAction(buf1, len1,buf2, len2,buf3, len3, buf4, len4);  //��������֡�����ö���ִ�к���
    }
}
/* �����жϷ����� */
void InterruptUART3() interrupt 17 
{
    if (S3CON&S3RI)  //���յ����ֽ�	   ���ֽڷ��͵�����λ��һ���ʱ��S2RI���ձ�־��Ϊ1  ����ܿ��ж�
    {
        S3CON&=~S3RI;  //��������жϱ�־λ
        if (cntRxd3 < sizeof(bufRxd3)) //���ջ�������δ����ʱ��
        {                            //��������ֽڣ�������������
            bufRxd3[cntRxd3++] = S3BUF;	
        }
    }
    if (S3CON&S3TI)  //�ֽڷ������
    {
        S3CON&=~S3TI;   //���㷢���жϱ�־λ
        flagTxd3 = 1;  //�����ֽڷ�����ɱ�־
    }
}
