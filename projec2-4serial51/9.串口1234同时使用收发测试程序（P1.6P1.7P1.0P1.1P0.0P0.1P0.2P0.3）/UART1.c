
#include <STC15W4KXX.H>
#include <intrins.h>
#include"Init_IO.h" 
bit flagFrame1 = 0;  //֡������ɱ�־�������յ�һ֡������
bit flagTxd1 = 0;    //���ֽڷ�����ɱ�־���������TXD�жϱ�־λ
unsigned char cntRxd1 = 0;   //�����ֽڼ�����
unsigned char pdata bufRxd1[32];  //�����ֽڻ�����
#define S1_S0 0x40
#define S1_S1 0x80

extern void UartAction(unsigned char *buf1, unsigned char len1,unsigned char *buf2, unsigned char len2,unsigned char *buf3, unsigned char len3,unsigned char *buf4, unsigned char len4);
/* �������ú�����baud-ͨ�Ų����� */

void ConfigUART1(unsigned long baud1)
{

    P_SW1&=~(S1_S0);  //������һ�л���P1.6��P1.7
	P_SW1|=(S1_S1);	  //������һ�л���P1.6��P1.7


	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x40;		//��ʱ��1ʱ��ΪFosc,��1T
	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;		//�趨��ʱ��1Ϊ16λ�Զ���װ��ʽ
    TL1 = (65536-(11059200/4/baud1));  //����T1����ֵ
    TH1 = (65536-(11059200/4/baud1))>>8;     //��ֵ��������ֵ
    ET1 = 0;       //��ֹT1�ж�
    TR1 = 1;       //����T1
    ES  = 1;       //ʹ�ܴ����ж�
    
}
/* �����ʱ��������ʱʱ��(t*10)us */
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
/* ��������д�룬�����ڷ��ͺ�����buf-���������ݵ�ָ�룬len-ָ���ķ��ͳ��� */
void UartWrite1(unsigned char *buf1, unsigned char len1)
{

    while (len1--)   //ѭ�����������ֽ�
    {
        flagTxd1 = 0;      //���㷢�ͱ�־
        SBUF = *buf1++;    //����һ���ֽ�����
        while (!flagTxd1); //�ȴ����ֽڷ������
    }
    DelayX10us1(5);  //�ȴ�����ֹͣλ��ɣ���ʱʱ���ɲ����ʾ���

}

/* �������ݶ�ȡ������buf-����ָ�룬len-ָ���Ķ�ȡ���ȣ�����ֵ-ʵ�ʶ����ĳ��� */
unsigned char UartRead1(unsigned char *buf1, unsigned char len1)
{
    unsigned char i;
    
    if (len1 > cntRxd1)  //ָ����ȡ���ȴ���ʵ�ʽ��յ������ݳ���ʱ��
    {                  //��ȡ��������Ϊʵ�ʽ��յ������ݳ���
        len1 = cntRxd1;
    }
    for (i=0; i<len1; i++)  //�������յ������ݵ�����ָ����
    {
        *buf1++ = bufRxd1[i];
    }
    cntRxd1 = 0;  //���ռ���������
    
    return len1;  //����ʵ�ʶ�ȡ����
}
/* ���ڽ��ռ�أ��ɿ���ʱ���ж�֡���������ڶ�ʱ�ж��е��ã�ms-��ʱ��� */
void UartRxMonitor1(unsigned char ms1)
{
    static unsigned char cntbkp1 = 0;
    static unsigned char idletmr1 = 0;

    if (cntRxd1 > 0)  //���ռ�����������ʱ��������߿���ʱ��
    {
        if (cntbkp1 != cntRxd1)  //���ռ������ı䣬���ս��յ�����ʱ��������м�ʱ
        {
            cntbkp1 = cntRxd1;
            idletmr1 = 0;
        }
        else                   //���ռ�����δ�ı䣬�����߿���ʱ���ۻ�����ʱ��
        {
            if (idletmr1 < 30)  //���м�ʱС��30msʱ�������ۼ�
            {
                idletmr1 += ms1;
                if (idletmr1 >= 30)  //����ʱ��ﵽ30msʱ�����ж�Ϊһ֡�������	
                {
                    flagFrame1 = 1;  //����֡������ɱ�־
                }
            }
        }
    }
    else
    {
        cntbkp1 = 0;
    }
}
/* ���������������������֡�Ľ��գ����ȹ��ܺ�����������ѭ���е��� */
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
    
    if (flagFrame1) //�������ʱ����ȡ���������
    {
        flagFrame1 = 0;
        len1 = UartRead1(buf1, sizeof(buf1)-2); //�����յ��������ȡ����������	 
        UartAction(buf1, len1,buf2, len2,buf3, len3, buf4, len4);  //��������֡�����ö���ִ�к���
    }
}
/* �����жϷ����� */
void InterruptUART1() interrupt 4
{
    if (RI)  //���յ����ֽ�	   ���ֽڷ��͵�����λ��һ���ʱ��RI���ձ�־��Ϊ1  ����ܿ��ж�
    {
        RI = 0;  //��������жϱ�־λ
        if (cntRxd1< sizeof(bufRxd1)) //���ջ�������δ����ʱ��
        {                            //��������ֽڣ�������������
            bufRxd1[cntRxd1++] = SBUF;

        }
    }
    if (TI)  //�ֽڷ������
    {
        TI = 0;   //���㷢���жϱ�־λ
        flagTxd1 = 1;  //�����ֽڷ�����ɱ�־
    }
}
