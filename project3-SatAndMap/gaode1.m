

%�ղ��ϴ�����ʹ��matlab��m���Գ�����øߵµ�ͼAPI��ѯ������ַ�����ĳ�������GPS��ַ���ߵ�ǰ����IP��ַ��

%1.�Լ��ٶȸߵµ�ͼAPI����ѯʹ�øߵµ�ͼ��WEB����API,��ȡ������Ϣ�͵���
%���磺https://lbs.amap.com/api/webservice/guide/api/direction
%2.����������ѧmatlab����һ�£�Ū�����е�ԭ��
%3.��س������lua�Լ�C��д��ʹ����ESP8266�Լ�K60����


method="GET";
IP = "110.53.215.199";
KEY ="f469ef4b32f89b0cae6ebbc8711d2d47";

%IP���ַ
%fullURL="https://restapi.amap.com/v3/ip?ip=110.53.215.199&output=xml&key=f469ef4b32f89b0cae6ebbc8711d2d47"
fullURL="https://restapi.amap.com/v3/ip?ip="+IP+"&output=JSON"+"&key="+KEY';
ret1 = webread(fullURL,'Timeout',5);
ret1.city


%�����ַ�侭γ��
address = "����ʡ��ɳ�г�ɳ����ѧ����У������һ¥";
myCity = "��ɳ";
%https://restapi.amap.com/v3/geocode/geo?address=����ʡ��ɳ�г�ɳ����ѧ����У������һ¥&city=��ɳ9&output=JSON&key=f469ef4b32f89b0cae6ebbc8711d2d47
fullURL="https://restapi.amap.com/v3/geocode/geo?address="+address+"&city="+myCity+"&output=JSON"+"&key="+KEY;
ret2 = webread(fullURL);
ret2.geocodes.location
A= sscanf(ret2.geocodes.location,"%f,%f");
lon = A(1);
lat = A(2);
 
 
 %���뾭γ�ȱ��ַ
 % https://restapi.amap.com/v3/geocode/regeo?output=xml&location=116.310003,39.991957&key=<�û���key>&radius=1000&extensions=all
 fullURL=" https://restapi.amap.com/v3/geocode/regeo?output=JSON&location="+lon+","+lat+"&key="+KEY+"&radius=10&extensions=all";
 ret2 = webread(fullURL);
 ret2.regeocode.formatted_address
 
 %��ѯ����
cityCode = 430105;
fullURL="https://restapi.amap.com/v3/weather/weatherInfo?city="+ cityCode+"&key="+KEY;
ret2 = webread(fullURL);
ret2.lives
ret2.lives.temperature
 

  %����
 % https://restapi.amap.com/v3/geocode/regeo?output=xml&location=116.310003,39.991957&key=<�û���key>&radius=1000&extensions=all
way = "walking";
%way = "driving";
startPoint = "112.977322,28.155963";%��ɳ����ѧ����1¥
endPoint = "112.977406,28.154944";%��ɳ����ѧ����У��(����)
 fullURL="https://restapi.amap.com/v3/direction/"+way+"?origin="+startPoint+"&destination="+endPoint+"&output=JSON"+"&key="+KEY;
  fullURL
 ret2 = webread(fullURL);
ret2.route.paths.distance
ret2.route.paths.duration

%������ͼ
A = thingSpeakRead(846323,'Location',true)
lat = A(9)
lon = A(10)
startPoint = "112.977322,28.155963";%��ɳ����ѧ����1¥

fullURL="https://restapi.amap.com/v3/staticmap?location="+startPoint+"&zoom=10&size=750*300"+"&key="+KEY;
fullURL="https://restapi.amap.com/v3/staticmap?location="+lat+","+lon+"&zoom=19&size=750*300"+"&key="+KEY;
fullURL
ret2 = webread(fullURL);
web(fullURL)


%û��ʵ��
%����Χ��������������Ҫ����ض����򣬼���û�λ���������ϵ�ĳ����С������������ڣ�

%ǩ�����ೡ�������û��򿨲���ǰ���ж��û��Ƿ��Ѿ��ڶ�Ӧ�ĵ���Χ�������ڣ�
%�������ೡ�������û��ﳵ�뿪�Ϸ�ʹ������ʱ����һʱ���֪�����û�����֧��ʱ���ж��û��Ƿ�ͣ���˺Ϸ������ڣ�
%�����ŵ��������������ȡ���û���λ�ڴ����ŵ�����ĸ���ʱ�����û���������������̵Ĵ�����Ϣ��
 %https://lbs.amap.com/api/webservice/guide/api/geofence_service
 
 