
#include <STC15W4KXX.H>
#include <intrins.h>
#include"Init_IO.h"
#include<string.h>
bit flagFrame4 = 0;  //֡������ɱ�־�������յ�һ֡������
bit flagTxd4 = 0;    //���ֽڷ�����ɱ�־���������TXD�жϱ�־λ
unsigned char cntRxd4 = 0;   //�����ֽڼ�����
unsigned char  bufRxd4[128];  //�����ֽڻ�����	 //ԭ��ֻ��32�ֽڣ��Ҹ�Ϊ128�ֽ�
unsigned char len4;
unsigned char buf4[128];	   //ԭ��ֻ��40�ֽڣ��Ҹ�Ϊ128�ֽ�
#define ES4 0x10
#define S4RI 0x01
#define S4TI 0x02
extern void UartAction4(unsigned char *buf4, unsigned char len4);
extern timer4count;
void ConfigUART4(unsigned int baud4)
{
//     RS485_DIR4 = 0; //RS485����Ϊ���շ���
	S4CON = 0x10;		//8λ����,�ɱ䲨����
	S4CON |= 0x40;		//����3ѡ��ʱ��3Ϊ�����ʷ�����
	T4T3M |= 0x20;		//��ʱ��3ʱ��ΪFosc,��1T
	T4L =(65536-(11059200/4/baud4));		//�趨��ʱ��ֵ
	T4H =(65536-(11059200/4/baud4))>>8;		//�趨��ʱ��ֵ
	T4T3M |= 0x80;		//������ʱ��3
	IE2|=ES4;				 //������2�ж�
}


/* �����ʱ��������ʱʱ��(t*10)us */
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
/* ��������д�룬�����ڷ��ͺ�����buf-���������ݵ�ָ�룬len-ָ���ķ��ͳ��� */
void UartWrite4(unsigned char *buf4, unsigned char len4)
{
//     RS485_DIR4 = 1;  //RS485����Ϊ����
    while (len4--)   //ѭ�����������ֽ�
    {
        flagTxd4 = 0;      //���㷢�ͱ�־
        S4BUF = *buf4++;    //����һ���ֽ�����
        while (!flagTxd4); //�ȴ����ֽڷ������
    }
    DelayX10us4(5);  //�ȴ�����ֹͣλ��ɣ���ʱʱ���ɲ����ʾ���
//     RS485_DIR4 = 0;  //RS485����Ϊ����
}



/* �������ݶ�ȡ������buf4-����ָ�룬len4-ָ���Ķ�ȡ���ȣ�����ֵ-ʵ�ʶ����ĳ��� */
unsigned char UartRead4(unsigned char *buf4, unsigned char len4)
{
    unsigned char i;
		memset(buf4,0,sizeof(buf4));	  //���ܼĴ�������
    
    if (len4 > cntRxd4)  //ָ����ȡ���ȴ���ʵ�ʽ��յ������ݳ���ʱ��
    {                  //��ȡ��������Ϊʵ�ʽ��յ������ݳ���
        len4 = cntRxd4;
    }
    for (i=0; i<len4; i++)  //�������յ������ݵ�����ָ����
    {
        *buf4++ = bufRxd4[i];
    }
    cntRxd4 = 0;  //���ռ���������
	memset(bufRxd4,0,sizeof(bufRxd4));	  //���ܼĴ�������
    
    return len4;  //����ʵ�ʶ�ȡ����
}
/* ���ڽ��ռ�أ��ɿ���ʱ���ж�֡���������ڶ�ʱ�ж��е��ã�ms-��ʱ��� */

void UartRxMonitor4(unsigned char ms4)
{
    static unsigned char cntbkp4 = 0;
    static unsigned char idletmr4 = 0;

    if (cntRxd4 > 0)  //���ռ�����������ʱ��������߿���ʱ��
    {
        if (cntbkp4 != cntRxd4)  //���ռ������ı䣬���ս��յ�����ʱ��������м�ʱ
        {
            cntbkp4 = cntRxd4;
            idletmr4 = 0;
        }
        else                   //���ռ�����δ�ı䣬�����߿���ʱ���ۻ�����ʱ��
        {
            if (idletmr4 < 30)  //���м�ʱС��30msʱ�������ۼ�
            {
                idletmr4 += ms4;
                if (idletmr4 >= 30)  //����ʱ��ﵽ30msʱ�����ж�Ϊһ֡�������	
                {
                    flagFrame4 = 1;  //����֡������ɱ�־
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
/* ���������������������֡�Ľ��գ����ȹ��ܺ�����������ѭ���е��� */
void UartDriver4()
{
   // unsigned char len1;
  //  unsigned char pdata buf1[40];      //ԭ��ֻ��40�ֽڣ��Ҹ�Ϊ128�ֽ�
  ////  unsigned char len2;
   // unsigned char pdata buf2[40];	 	//ԭ��ֻ��40�ֽڣ��Ҹ�Ϊ128�ֽ�
  //  unsigned char len3;
  //  unsigned char pdata buf3[40];	   //ԭ��ֻ��40�ֽڣ��Ҹ�Ϊ128�ֽ�

    
    if (flagFrame4) //�������ʱ����ȡ���������
    {
        flagFrame4 = 0;
        len4 = UartRead4(buf4, sizeof(buf4)); //�����յ��������ȡ����������	 
        UartAction4(buf4, len4);  //��������֡�����ö���ִ�к���
    }
}
/* �����жϷ����� */
void InterruptUART4() interrupt 18 
{
   
	if (S4CON&S4RI)  //���յ����ֽ�	   ���ֽڷ��͵�����λ��һ���ʱ��S2RI���ձ�־��Ϊ1  ����ܿ��ж�
    {
        S4CON&=~S4RI;  //��������жϱ�־λ
        if (cntRxd4 < sizeof(bufRxd4)) //���ջ�������δ����ʱ��
        {                            //��������ֽڣ�������������
            
			bufRxd4[cntRxd4++] = S4BUF;	

			if(S4BUF == '\0' || S4BUF == '\n' || S4BUF == '\r')
			flagFrame4 = 1;

        }
		
    }
    if (S4CON&S4TI)  //�ֽڷ������
    {
        S4CON&=~S4TI;   //���㷢���жϱ�־λ
        flagTxd4 = 1;  //�����ֽڷ�����ɱ�־
    }

	 timer4count++;
}
