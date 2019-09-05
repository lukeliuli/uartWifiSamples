
#include <STC15W4KXX.H>
#include <intrins.h>
#include"Init_IO.h"
bit flagFrame2 = 0;  //֡������ɱ�־�������յ�һ֡������
bit flagTxd2 = 0;    //���ֽڷ�����ɱ�־���������TXD�жϱ�־λ
unsigned char cntRxd2 = 0;   //�����ֽڼ�����
unsigned char pdata bufRxd2[32];  //�����ֽڻ�����
#define ES2 0x01
#define S2RI 0x01
#define S2TI 0x02
extern void UartAction(unsigned char *buf1, unsigned char len1,unsigned char *buf2, unsigned char len2,unsigned char *buf3, unsigned char len3,unsigned char *buf4, unsigned char len4);

void ConfigUART2(unsigned int baud2)
{
//     RS485_DIR2 = 0; //RS485����Ϊ���շ���
	S2CON = 0x50;			 //���ڷ�ʽ1��8λ���ݣ��ɱ䲨����
    AUXR |= 0x04;		//��ʱ��2ʱ��ΪFosc,��1T
	T2L =(65536-(11059200/4/baud2));		//�趨��ʱ��ֵ
	T2H =(65536-(11059200/4/baud2))>>8;		//�趨��ʱ��ֵ
	AUXR |= 0x10;		//������ʱ��2
	IE2|=ES2;				 //������2�ж�
}


/* �����ʱ��������ʱʱ��(t*10)us */
void DelayX10us2(unsigned char t2)
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
        
        
    } while (--t2);
}
/* ��������д�룬�����ڷ��ͺ�����buf-���������ݵ�ָ�룬len-ָ���ķ��ͳ��� */
void UartWrite2(unsigned char *buf2, unsigned char len2)
{
//     RS485_DIR2 = 1;  //RS485����Ϊ����
    while (len2--)   //ѭ�����������ֽ�
    {
        flagTxd2 = 0;      //���㷢�ͱ�־
        S2BUF = *buf2++;    //����һ���ֽ�����
        while (!flagTxd2); //�ȴ����ֽڷ������
    }
    DelayX10us2(5);  //�ȴ�����ֹͣλ��ɣ���ʱʱ���ɲ����ʾ���
//     RS485_DIR2 = 0;  //RS485����Ϊ����
}



/* �������ݶ�ȡ������buf2-����ָ�룬len2-ָ���Ķ�ȡ���ȣ�����ֵ-ʵ�ʶ����ĳ��� */
unsigned char UartRead2(unsigned char *buf2, unsigned char len2)
{
    unsigned char i;
    
    if (len2 > cntRxd2)  //ָ����ȡ���ȴ���ʵ�ʽ��յ������ݳ���ʱ��
    {                  //��ȡ��������Ϊʵ�ʽ��յ������ݳ���
        len2 = cntRxd2;
    }
    for (i=0; i<len2; i++)  //�������յ������ݵ�����ָ����
    {
        *buf2++ = bufRxd2[i];
    }
    cntRxd2 = 0;  //���ռ���������
    
    return len2;  //����ʵ�ʶ�ȡ����
}
/* ���ڽ��ռ�أ��ɿ���ʱ���ж�֡���������ڶ�ʱ�ж��е��ã�ms-��ʱ��� */
void UartRxMonitor2(unsigned char ms2)
{
    static unsigned char cntbkp2 = 0;
    static unsigned char idletmr2 = 0;

    if (cntRxd2 > 0)  //���ռ�����������ʱ��������߿���ʱ��
    {
        if (cntbkp2 != cntRxd2)  //���ռ������ı䣬���ս��յ�����ʱ��������м�ʱ
        {
            cntbkp2 = cntRxd2;
            idletmr2 = 0;
        }
        else                   //���ռ�����δ�ı䣬�����߿���ʱ���ۻ�����ʱ��
        {
            if (idletmr2 < 30)  //���м�ʱС��30msʱ�������ۼ�
            {
                idletmr2 += ms2;
                if (idletmr2 >= 30)  //����ʱ��ﵽ30msʱ�����ж�Ϊһ֡�������	
                {
                    flagFrame2 = 1;  //����֡������ɱ�־
                }
            }
        }
    }
    else
    {
        cntbkp2 = 0;
    }
}
/* ���������������������֡�Ľ��գ����ȹ��ܺ�����������ѭ���е��� */
void UartDriver2()
{
    unsigned char len1;
    unsigned char pdata buf1[40];    
    unsigned char len2;
    unsigned char pdata buf2[40];
    unsigned char len3;
    unsigned char pdata buf3[40];    
    unsigned char len4;
    unsigned char pdata buf4[40];
    
    if (flagFrame2) //�������ʱ����ȡ���������
    {
        flagFrame2 = 0;
        len2 = UartRead2(buf2, sizeof(buf2)-2); //�����յ��������ȡ����������	 
        UartAction(buf1, len1,buf2, len2,buf3, len3, buf4, len4);  //��������֡�����ö���ִ�к���
    }
}
/* �����жϷ����� */
void InterruptUART2() interrupt 8
{
    if (S2CON&S2RI)  //���յ����ֽ�	   ���ֽڷ��͵�����λ��һ���ʱ��S2RI���ձ�־��Ϊ1  ����ܿ��ж�
    {
        S2CON&=~S2RI;  //��������жϱ�־λ
        if (cntRxd2 < sizeof(bufRxd2)) //���ջ�������δ����ʱ��
        {                            //��������ֽڣ�������������
            bufRxd2[cntRxd2++] = S2BUF;	
        }
    }
    if (S2CON&S2TI)  //�ֽڷ������
    {
        S2CON&=~S2TI;   //���㷢���жϱ�־λ
        flagTxd2 = 1;  //�����ֽڷ�����ɱ�־
    }
}
