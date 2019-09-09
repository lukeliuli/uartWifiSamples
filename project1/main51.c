
/*因为51单片机的资源比较少，运算速度慢，数据传输采用整型
51单片机不采用浮点型数据上传的原因是
1.51单片机也可以采用浮点型，但是运算耗时很大，消耗的资源也多。
2.因为luna的tonumber出现错误，导致无法将浮点字符串转换为浮点数

PS:等luna的tonumber的错误修复，可以用浮点型数组上传下载，具体代码见main-k60）
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

//代码不能直接运行，只是做一个演示。
void main()
{
    
    EA = 1;            //开总中断
    
    ConfigUART1(9600);  //配置波特率为9600。115200似乎有问题
    ConfigUART2(9600);  //配置波特率为9600。 115200似乎有问题
    ConfigUART3(9600);  //配置波特率为9600。 115200似乎有问题
    ConfigUART4(9600);  //配置波特率为9600。 115200似乎有问题
    
    
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
        UartWrite1(strSend,strlen(strSend));
        
        //sprintf(strSend,"%d %d %d %d %d %d %d %d %d\n",1,2,3,4,5,6,7,8); 有些51单片机不支持sprintf(%d).出现大量乱码
        //UartWrite1(strSend, strlen(strSend));
        
		
		
		//char buf4[100] = "TS: 1 2 3 4 5 6 7 8";ESP8266读取的thingspeak数据，然后通过串口给51单片机
		ExtractThingspeakData(buf4, strlen(buf4))
    }
}

/*
 *
 * 功能：整数转换为字符串
 *
 * char s[] 的作用是存储整数的每一位
 *
 */
//n最大9999
void my_itoa(int n, char s[])

{
    
    int i = 0, isNegative = 0;
    //reverse
    char temp;
    
    char *p = s;    //p指向s的头部
    
    char *q = s;    //q指向s的尾部
    
    
    
    memset(s, '0', sizeof(s));
    
    if ((isNegative = n) < 0) //如果是负数，先转为正数
        
    {
        
        n = -n;
        
    }
    
    do      //从各位开始变为字符，直到最高位，最后应该反转
        
    {
        
        s[i++] = n % 10 + '0';
        
        n = n / 10;
        
    } while (n > 0);
    
    
    
    if (isNegative < 0)   //如果是负数，补上负号
        
    {
        
        s[i++] = '-';
        
    }
    
    s[i] = '\0';    //最后加上字符串结束符
    
    
    
    while (*q)
        
        ++q;
    
    q--;
    
    
    
    //交换移动指针，直到p和q交叉
    
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