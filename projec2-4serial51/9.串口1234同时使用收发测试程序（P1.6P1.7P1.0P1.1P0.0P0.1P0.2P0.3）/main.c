/********************************************************************
                           ΰ����ӹ�����

ʵ�ֹ���:����STC12C5A60S2��Ƭ��ʵ��˫���ڻ���˫485�԰˸�led�Ŀ���
ʹ��оƬ��STC12C5A60S2ϵ��
����11.0592MHZ
�����ʣ�9600(����У��λ���ޣ�����λ��8��ֹͣλ��1��)
���ݸ�ʽ��01/02/03/04+ff/00
���뻷����Keil
���ߣ�fenglongmeng
qq 490022104 CALL18003710439
��վ��wddz.taobao.com
���������˳��������ѧϰ��ο���������ע����Ȩ��������Ϣ��     

*********************************************************************/
/********************************************************************
                           ����˵��������1234ͨѶ��

PC����01FF����1����ȫ��;����1��PC����01AA;
PC����0100����1����ȫ��;����1��PC����01BB;
PC����02FF����2����ȫ��;����2��PC����02AA;
PC����0200����2����ȫ��;����2��PC����02BB;
PC����03FF����3����ȫ��;����3��PC����03AA;
PC����0300����3����ȫ��;����3��PC����03BB;
PC����04FF����4����ȫ��;����4��PC����04AA;
PC����0400����4����ȫ��;����4��PC����04BB;
*********************************************************************/
#include <stdio.h>
#include <string.h>
#include <STC15W4KXX.H>
#include <intrins.h>	  //��nop������ʱ��ͷ�ļ�
#include"Init_IO.h" 
#define uchar unsigned char//�궨���޷����ַ���
#define uint unsigned int  //�궨���޷�������
unsigned char T0RH = 0;  //T0����ֵ�ĸ��ֽ�
unsigned char T0RL = 0;  //T0����ֵ�ĵ��ֽ�
void ConfigTimer0(unsigned int ms);	   //���ò�����T0��ms-T0��ʱʱ�� 



extern void UartDriver1();				//���������������������֡�Ľ��գ����ȹ��ܺ�����������ѭ���е���
extern void ConfigUART1(unsigned int baud1);	//�������ú�����baud-ͨ�Ų����� 
extern void UartRxMonitor1(unsigned char ms1); //���ڽ��ռ�أ��ɿ���ʱ���ж�֡���������ڶ�ʱ�ж��е��ã�ms-��ʱ���
extern void UartWrite1(unsigned char *buf1, unsigned char len1);	//��������д�룬�����ڷ��ͺ�����buf-���������ݵ�ָ�룬len-ָ���ķ��ͳ���

extern void UartDriver2();				//���������������������֡�Ľ��գ����ȹ��ܺ�����������ѭ���е���
extern void ConfigUART2(unsigned int baud2);	//�������ú�����baud-ͨ�Ų����� 
extern void UartRxMonitor2(unsigned char ms2); //���ڽ��ռ�أ��ɿ���ʱ���ж�֡���������ڶ�ʱ�ж��е��ã�ms-��ʱ���
extern void UartWrite2(unsigned char *buf2, unsigned char len2);	//��������д�룬�����ڷ��ͺ�����buf-���������ݵ�ָ�룬len-ָ���ķ��ͳ���

extern void UartDriver3();				//���������������������֡�Ľ��գ����ȹ��ܺ�����������ѭ���е���
extern void ConfigUART3(unsigned int baud3);	//�������ú�����baud-ͨ�Ų����� 
extern void UartRxMonitor3(unsigned char ms3); //���ڽ��ռ�أ��ɿ���ʱ���ж�֡���������ڶ�ʱ�ж��е��ã�ms-��ʱ���
extern void UartWrite3(unsigned char *buf3, unsigned char len3);	//��������д�룬�����ڷ��ͺ�����buf-���������ݵ�ָ�룬len-ָ���ķ��ͳ���

extern void UartDriver4();				//���������������������֡�Ľ��գ����ȹ��ܺ�����������ѭ���е���
extern void ConfigUART4(unsigned int baud4);	//�������ú�����baud-ͨ�Ų����� 
extern void UartRxMonitor4(unsigned char ms4); //���ڽ��ռ�أ��ɿ���ʱ���ж�֡���������ڶ�ʱ�ж��е��ã�ms-��ʱ���
extern void UartWrite4(unsigned char *buf4, unsigned char len4);	//��������д�룬�����ڷ��ͺ�����buf-���������ݵ�ָ�룬len-ָ���ķ��ͳ���

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
     
    EA = 1;            //�����ж�
    ConfigP0P1P2P3P4P5P6P7();
    ConfigTimer0(1);   //����T0��ʱ1ms
    ConfigUART1(57600);  //���ò�����Ϊ9600��115200�ƺ�������
    ConfigUART2(57600);  //���ò�����Ϊ9600�� 115200�ƺ�������
    ConfigUART3(57600);  //���ò�����Ϊ9600�� 115200�ƺ�������    
    ConfigUART4(57600);  //���ò�����Ϊ9600�� 115200�ƺ�������
    
	
    while (1)
    {	
	    Delay300ms();
		Delay300ms();
		//Delay300ms();
        sendData[7]=51;

		//my_itoa�����������9999
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
		
		//sprintf(strSend,"%d %d %d %d %d %d %d %d %d",1,2,3,4,5,6,7,8); 51��Ƭ����֧��sprintf
		//UartWrite4(strSend,strlen(strSend)+1)	;


		UartDriver1();  //���ô�������
        UartDriver2();  //���ô�������
        UartDriver3();  //���ô������� 
        UartDriver4();  //���ô�������


    }
}

/* ���ڶ������������ݽ��յ�������ִ֡����Ӧ�Ķ���
   buf-���յ�������ָ֡�룬len-����֡���� */
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
/* ���ò�����T0��ms-T0��ʱʱ�� */
void ConfigTimer0(unsigned int ms)
{
    unsigned long tmp;  //��ʱ����
    
    tmp = 11059200 / 12;      //��ʱ������Ƶ��
    tmp = (tmp * ms) / 1000;  //��������ļ���ֵ
    tmp = 65536 - tmp;        //���㶨ʱ������ֵ
    tmp = tmp + 33;           //�����ж���Ӧ��ʱ��ɵ����
    T0RH = (unsigned char)(tmp>>8);  //��ʱ������ֵ���Ϊ�ߵ��ֽ�
    T0RL = (unsigned char)tmp;
    TMOD &= 0xF0;   //����T0�Ŀ���λ
    TMOD |= 0x01;   //����T0Ϊģʽ1
    TH0 = T0RH;     //����T0����ֵ
    TL0 = T0RL;
    ET0 = 1;        //ʹ��T0�ж�
    TR0 = 1;        //����T0
}
/* T0�жϷ�������ִ�д��ڽ��ռ�� */
void InterruptTimer0() interrupt 1
{

    TH0 = T0RH;  //���¼�������ֵ
    TL0 = T0RL;
    UartRxMonitor1(1);  //���ڽ��ռ��
    UartRxMonitor2(1);  //���ڽ��ռ�� 
    UartRxMonitor3(1);  //���ڽ��ռ��  
    UartRxMonitor4(1);  //���ڽ��ռ��    


}




 

/*

 * ���ܣ�����ת��Ϊ�ַ���

 * char s[] �������Ǵ洢������ÿһλ

 */
	//n���9999
void my_itoa(int n,char s[])

{

    int i = 0,isNegative = 0;
	//reverse
	 char temp;

    char *p = s;    //pָ��s��ͷ��

    char *q = s;    //qָ��s��β��



     memset(s,'0',sizeof(s));

    if((isNegative = n) < 0) //����Ǹ�������תΪ����

    {

        n = -n;

    }

    do      //�Ӹ�λ��ʼ��Ϊ�ַ���ֱ�����λ�����Ӧ�÷�ת

    {

        s[i++] = n%10 + '0';

        n = n/10;

    }while(n > 0);

    

    if(isNegative < 0)   //����Ǹ��������ϸ���

    {

        s[i++] = '-';

    }

    s[i] = '\0';    //�������ַ���������

    

    while(*q)

        ++q;

    q--;

    

    //�����ƶ�ָ�룬ֱ��p��q����

    while(q > p)

    {

        temp = *p;

        *p++ = *q;

        *q-- = temp;

    }

   

}