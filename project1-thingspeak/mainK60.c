#include "include.h"
#include "stdlib.h"

/*
//把整数和浮点数转为字符串，并从串口发出送给ESP8266(要下载最新test3.lua)，仅仅适用于IAR+K60,其余KEA,KL也应该可以，但是没有测试

1.不支持把浮点数转为字符串，并从串口发出送给ESP8266,因为luna的tonumber出现错误，导致无法将浮点字符串转换为浮点数，所以暂时只支持整数

*/
char fl2str1[10];
char fl2str2[10];
char fl2str3[10];
char fl2str4[10];
char fl2str5[10];
char fl2str6[10];
char fl2str7[10];
char fl2str8[10];
char strSend[100];

float sendData[8] = { 10.11,20.22,30.33,40.44,50.55,66.66,77.77,80.88 };



char buf4[100] = "TS: 1 2 3 4 5 6 7 8";
int TSval1;
int TSval2;
int TSval3;
int TSval4;
int TSval5;
int TSval6;
int TSval7;
int TSval8;



char* my_itoa(int value, char *str, int radix);
char *my_ftoa(double number, int ndigit, char *buf);
void ExtractThingspeakData(unsigned char *buf4, unsigned char len4);


//代码不能直接运行，只是做一个演示。
void main()
{


	
	uart_init(UART2, 57600);


	while (1)
	{
	

		delayms(1000);
		/*
		memset(fl2str1, 0, 10);
		memset(fl2str2, 0, 10);
		memset(fl2str3, 0, 10);
		memset(fl2str4, 0, 10);
		memset(fl2str5, 0, 10);
		memset(fl2str6, 0, 10);
		memset(fl2str7, 0, 10);
		memset(fl2str8, 0, 10);


		my_ftoa(sendData[0], 1, fl2str1);
		my_ftoa(sendData[1], 1, fl2str2);
		my_ftoa(sendData[2], 1, fl2str3);
		my_ftoa(sendData[3], 1, fl2str4);
		my_ftoa(sendData[4], 1, fl2str5);
		my_ftoa(sendData[5], 1, fl2str6);
		my_ftoa(sendData[6], 1, fl2str7);
		my_ftoa(sendData[7], 1, fl2str8);


		////把浮点数转为字符串，并从串口发出
		sprintf(strSend, "%s %s %s %s %s %s %s %s\n", fl2str1, fl2str2, fl2str3, fl2str4, fl2str5, fl2str6, fl2str7, fl2str8);
		uart_sendStr(UART2, strSend);*/

		///把整数数转为字符串，并从串口发出
		sprintf(strSend, "%d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 101);
		uart_sendStr(UART2, strSend);

		//char buf4[100] = "TS: 1 2 3 4 5 6 7 8";ESP8266读取的thingspeak数据，然后通过串口给51单片机
		ExtractThingspeakData(buf4, strlen(buf4))

	}
}







char* my_itoa(int value, char *str, int radix)
{
	int sign = 0;
	//char *s = str;
	char ps[32];
	memset(ps, 0, 32);
	int i = 0;
	if (value < 0)
	{
		sign = -1;
		value = -value;
	}
	do
	{
		if (value%radix > 9)
			ps[i] = value % radix + '0' + 7;
		else
			ps[i] = value % radix + '0';
		i++;
	} while ((value /= radix) > 0);
	if (sign < 0)
		ps[i] = '-';
	else
		i--;
	for (int j = i; j >= 0; j--)
	{
		str[i - j] = ps[j];
	}
	return str;
}

char *my_ftoa(double number, int ndigit, char *buf)
{
	long int_part;
	double float_part;
	char str_int[32];
	char str_float[32];
	memset(str_int, 0, 32);
	memset(str_float, 0, 32);
	int_part = (long)number;
	float_part = number - int_part;
	// 整数部分处理
	my_itoa(int_part, str_int, 10);
	// 小数部分处理
	if (ndigit > 0)
	{
		float_part = fabs(pow(10, ndigit)*float_part);
		my_itoa((long)float_part, str_float, 10);
	}
	int i = strlen(str_int);
	str_int[i] = '.';
	strcat(str_int, str_float);
	strcpy(buf, str_int);
	return buf;
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