# uartWifiSamples
using esp8266 (nodemcu and lua) for transfering msg between thingspeak and mcu(k60 and 51)  

一、ESP8266方面
1.ESP8266方面 running test3.lua. test3.lua是当前主运行文件,其余是测试程序.Init.lua文件用于设定串口波特率，运行test3.lua。注意部分如下  
(1) ESP8266如果接51单片机，请设定波特率为115200以下。本项目默认57600  
****
二、51单片机方面  
1.main.c是51单片机从串口发信息给ESP8266的程序（用于4串口51单片机，也可以稍微修改一下用于51单片机）。关键程序和地方是  
(1)my_itoa用于把整数转换为字符串  

(2)用sprintf，把八个整数转换后的字符串，进行整合成一个字符串（类似100 200 300 400 500 600 700 800）并从串口发送给ESP8266  

(3)注意因为使用大量的全局字符串，会出现“*** ERROR L107: ADDRESS SPACE OVERFLOW”，需要做对keil做一下修改
“右键单击Target选择“Options for Target'Target1'”就会出来一个框，在memory model中选择large选项 就可以了”  

(4)delay300ms很重要，不能随意删除。  
****
三.testSerialbyMatlab是用matlab测试ESP8266的程序  