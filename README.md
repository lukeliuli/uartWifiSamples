# uartWifiSamples
using esp8266 (nodemcu and lua) for transfering msg between thingspeak and mcu(k60 and 51)


1.running test3.lua. test3.lua�ǵ�ǰ�������ļ�,�����ǲ��Գ���
2.51mcu_main��51��Ƭ���Ӵ��ڷ���Ϣ��ESP8266�ĳ��򡣹ؼ�����͵ط���
(1)my_itoa���ڰ�����ת��Ϊ�ַ���
��2����sprintf���Ѱ˸�����ת������ַ������������ϳ�һ���ַ���������100 200 300 400 500 600 700 800�����Ӵ��ڷ��͸�ESP8266
��3��ע����Ϊʹ�ô�����ȫ���ַ���������֡�*** ERROR L107: ADDRESS SPACE OVERFLOW������Ҫ����keil��һ���޸�
���Ҽ�����Targetѡ��Options for Target'Target1'���ͻ����һ������memory model��ѡ��compact:variables in pdata �Ϳ����ˡ�

3.testSerialbyMatlab����matlab����ESP8266�ĳ���