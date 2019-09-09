clc;
clear all;
close all;
%测试ESP8266的串口数据，并发送到thingspeak
newobjs = instrfind
if length(newobjs)>0
fclose(newobjs)
end
a1=88;
a2= 88;
a3 = 12;
a4 =88;
a5 =88;
a6=88;
a7 =88
a8 =88;
sdat= sprintf('%d %d %d %d %d %d %d %d %d\n',a1,a2,a3,a4,a5,a6,a7,a8);
disp(sdat)
s2 = serial('COM4','BaudRate',57600);


stop =0;
fopen(s2)
count = 0;
fprintf(s2,sdat)
while(stop==0)
    
    pause(1);
    
    idn = fscanf(s2);
    disp(idn);
    
    A= sscanf(idn,"readThingspeakData is:%d %d %d %d %d %d %d %d\n");
    disp(A)
    count=count+1;
    if count >100
        break;
    end
        
end

fclose(s2)