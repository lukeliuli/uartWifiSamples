
/*��Ϊ51��Ƭ������Դ�Ƚ��٣������ٶ��������ݴ����������
51��Ƭ�������ø����������ϴ���ԭ����
1.51��Ƭ��Ҳ���Բ��ø����ͣ����������ʱ�ܴ����ĵ���ԴҲ�ࡣ
2.��Ϊluna��tonumber���ִ��󣬵����޷��������ַ���ת��Ϊ������

PS:��luna��tonumber�Ĵ����޸��������ø����������ϴ����أ���������main-k60��
*/

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


char buf4[100] = "TS: 1 2 3 4 5 6 7 8";
int TSval1;
int TSval2;
int TSval3;
int TSval4;
int TSval5;
int TSval6;
int TSval7;
int TSval8;

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

//���벻��ֱ�����У�ֻ����һ����ʾ��
void main()
{
    
    EA = 1;            //�����ж�
    
    ConfigUART1(9600);  //���ò�����Ϊ9600��115200�ƺ�������
    ConfigUART2(9600);  //���ò�����Ϊ9600�� 115200�ƺ�������
    ConfigUART3(9600);  //���ò�����Ϊ9600�� 115200�ƺ�������
    ConfigUART4(9600);  //���ò�����Ϊ9600�� 115200�ƺ�������
    
    
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
        UartWrite1(strSend,strlen(strSend));
        
        //sprintf(strSend,"%d %d %d %d %d %d %d %d %d\n",1,2,3,4,5,6,7,8); ��Щ51��Ƭ����֧��sprintf(%d).���ִ�������
        //UartWrite1(strSend, strlen(strSend));
        
		
		
		//char buf4[100] = "TS: 1 2 3 4 5 6 7 8";ESP8266��ȡ��thingspeak���ݣ�Ȼ��ͨ�����ڸ�51��Ƭ��
		ExtractThingspeakData(buf4, strlen(buf4))
    }
}

/*
 *
 * ���ܣ�����ת��Ϊ�ַ���
 *
 * char s[] �������Ǵ洢������ÿһλ
 *
 */
//n���9999
void my_itoa(int n, char s[])

{
    
    int i = 0, isNegative = 0;
    //reverse
    char temp;
    
    char *p = s;    //pָ��s��ͷ��
    
    char *q = s;    //qָ��s��β��
    
    
    
    memset(s, '0', sizeof(s));
    
    if ((isNegative = n) < 0) //����Ǹ�������תΪ����
        
    {
        
        n = -n;
        
    }
    
    do      //�Ӹ�λ��ʼ��Ϊ�ַ���ֱ�����λ�����Ӧ�÷�ת
        
    {
        
        s[i++] = n % 10 + '0';
        
        n = n / 10;
        
    } while (n > 0);
    
    
    
    if (isNegative < 0)   //����Ǹ��������ϸ���
        
    {
        
        s[i++] = '-';
        
    }
    
    s[i] = '\0';    //�������ַ���������
    
    
    
    while (*q)
        
        ++q;
    
    q--;
    
    
    
    //�����ƶ�ָ�룬ֱ��p��q����
    
    while (q > p)
        
    {
        
        temp = *p;
        
        *p++ = *q;
        
        *q-- = temp;
        
    }
    
    
    
}




void ExtractThingspeakData(unsigned char *buf4, unsigned char len4)
{




	uchar int2str1[5];
	uchar int2str2[5];
	uchar int2str3[5];
	uchar int2str4[5];
	uchar int2str5[5];
	uchar int2str6[5];
	uchar int2str7[5];
	uchar int2str8[5];



	//	UartWrite3(buf4,strlen(buf4));
	   //my_itoa(strlen(buf4),int2str1);
	   //UartWrite3(int2str1,strlen(int2str1));
	   //my_itoa(strlen(buf4),int2str1);
	   //UartWrite3("######",7)	;
	   //UartWrite3(int2str1,strlen(int2str1));


	if (len4 < 20)
		return;

	if (buf4[0] != 'T')
		return;

	if (buf4[1] != 'S')
		return;

	if (buf4[2] != ':')
		return;



	// UartWrite3("RVL is:\n",strlen("RVL is:\n")+1);

	sscanf(buf4, "TS:%d %d %d %d %d %d %d %d\n", &TSval1, &TSval2, &TSval3, &TSval4, &TSval5, &TSval6, &TSval7, &TSval8);


}