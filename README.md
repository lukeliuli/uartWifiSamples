# uartWifiSamples
using esp8266 (nodemcu and lua) for transfering msg between thingspeak and mcu(k60 and 51)  

一、ESP8266方面  
1.ESP8266方面 running test3.lua. test3.lua是当前主运行文件,其余是测试程序.Init.lua文件用于设定串口波特率，运行test3.lua。注意部分如下  
(1) ESP8266如果接51单片机，请设定波特率为115200以下。本项目默认57600  
(2) ESP8266读取thingspeak的数据，并通过串口传递给单片机，串口输出给单片机数据格式类似  
  "TS:1 2 3 4 5 6 7 8\n"   
(3)test3For51.lua,代码与test3.lua一样，但是除了输出数据以外，其余的调试信息都屏蔽了，专门用于51单片机的信息传递


****
二、51单片机方面  
1.main51.c是51单片机从串口发信息给ESP8266的程序（用于4串口51单片机，也可以稍微修改一下用于51单片机）。关键程序和地方是  
(1)my_itoa用于把整数转换为字符串  

(2)用sprintf，把八个整数转换后的字符串，进行整合成一个字符串（类似100 200 300 400 500 600 700 800）并从串口发送给ESP8266  

(3)注意因为使用大量的全局字符串，会出现“*** ERROR L107: ADDRESS SPACE OVERFLOW”，需要做对keil做一下修改
“右键单击Target选择“Options for Target'Target1'”就会出来一个框，在memory model中选择large选项 就可以了”  

(4)delay300ms很重要，不能随意删除。

(5) 51单片机不能用sprintf来将整数转换为字符串，会出现乱码。  

(6) 51单片机的从串口接收ESP8266的信息比较复杂，因为我用的是字符串形式从8266到51单片机，而51单片机的接收中断程序，需要大量修改（有些51单片机以30ms空闲作为接受字符串结束标志，这与很多单片机和PC很不一样）。自己慢慢调试吧。  

(7) 51单片机UART接收中断和接收字符串大小，有限制。自己慢慢调试吧。  

(8) 从51单片机接收字符串，直接使用sscanf函数或者字符串中各个field的整数值（代码中还有一些预处理）  
  sscanf(buf4,"TS:%d %d %d %d %d %d %d %d\n",&TSval1,&TSval2,&TSval3,&TSval4,&TSval5,&TSval6,&TSval7,&TSval8);

****
三、K60方面,代码在mainK60.c中  
1.IAR+K60可以用sprintf将整数转换为字符串  
2 IAR+K60可以用sprintf不可以将浮点数转换为字符串，会出现错误
3 IAR+K60接收ESP8266，直接使用sscanf函数或者字符串中各个field的整数值（代码中还有一些预处理）  
char buf4[100]="TS:1 2 3 4 5 6 7 8";    
sscanf(buf4,"TS:%d %d %d %d %d %d %d %d\n",&TSval1,&TSval2,&TSval3,&TSval4,&TSval5,&TSval6,&TSval7,&TSval8);

****
四.testSerialbyMatlab是用matlab测试ESP8266的程序    

****
PS:  
1 同时提取整数浮点数的正则式是"[+-]?%d+%p?%d*"，但是因为luna的tonumber出现错误，导致无法将浮点字符串转换为浮点数,所以暂时只支持整数.  