# uartWifiSamples
using esp8266 (nodemcu and lua) for transfering msg between thingspeak and mcu(k60 and 51)


1.running test3.lua. test3.lua是当前主运行文件,其余是测试程序
2.51mcu_main是51单片机从串口发信息给ESP8266的程序。关键程序和地方是
(1)my_itoa用于把整数转换为字符串
（2）用sprintf，把八个整数转换后的字符串，进行整合成一个字符串（类似100 200 300 400 500 600 700 800）并从串口发送给ESP8266
（3）注意因为使用大量的全局字符串，会出现“*** ERROR L107: ADDRESS SPACE OVERFLOW”，需要做对keil做一下修改
“右键单击Target选择“Options for Target'Target1'”就会出来一个框，在memory model中选择compact:variables in pdata 就可以了”

3.testSerialbyMatlab是用matlab测试ESP8266的程序