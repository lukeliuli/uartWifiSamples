# uartWifiSamples
using esp8266 (nodemcu and lua) for transfering msg between thingspeak and mcu(k60 and 51)

һ��ESP8266����
1.ESP8266���� running test3.lua. test3.lua�ǵ�ǰ�������ļ�,�����ǲ��Գ���.Init.lua�ļ������趨���ڲ����ʣ�����test3.lua��ע�ⲿ������
(1) ESP8266�����51��Ƭ�������趨������Ϊ115200���¡�����ĿĬ��57600
(2)

����51��Ƭ������
1.main.c��51��Ƭ���Ӵ��ڷ���Ϣ��ESP8266�ĳ�������4����51��Ƭ����Ҳ������΢�޸�һ������51��Ƭ�������ؼ�����͵ط���
(1)my_itoa���ڰ�����ת��Ϊ�ַ���
(2)��sprintf���Ѱ˸�����ת������ַ������������ϳ�һ���ַ���������100 200 300 400 500 600 700 800�����Ӵ��ڷ��͸�ESP8266
(3)ע����Ϊʹ�ô�����ȫ���ַ���������֡�*** ERROR L107: ADDRESS SPACE OVERFLOW������Ҫ����keil��һ���޸�
���Ҽ�����Targetѡ��Options for Target'Target1'���ͻ����һ������memory model��ѡ��largeѡ�� �Ϳ����ˡ�
��4��delay300ms����Ҫ����������ɾ����

��.testSerialbyMatlab����matlab����ESP8266�ĳ���