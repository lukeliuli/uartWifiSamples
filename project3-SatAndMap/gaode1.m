

%刚才上传的是使用matlab的m语言程序调用高德地图API查询天气地址导航的程序（输入GPS地址或者当前真正IP地址）

%1.自己百度高德地图API，查询使用高德地图的WEB服务API,获取各种信息和导航
%例如：https://lbs.amap.com/api/webservice/guide/api/direction
%2.你们正好在学matlab，玩一下，弄懂其中的原理。
%3.相关程序会用lua以及C重写，使用在ESP8266以及K60上面


method="GET";
IP = "110.53.215.199";
KEY ="f469ef4b32f89b0cae6ebbc8711d2d47";

%IP变地址
%fullURL="https://restapi.amap.com/v3/ip?ip=110.53.215.199&output=xml&key=f469ef4b32f89b0cae6ebbc8711d2d47"
fullURL="https://restapi.amap.com/v3/ip?ip="+IP+"&output=JSON"+"&key="+KEY';
ret1 = webread(fullURL,'Timeout',5);
ret1.city


%输入地址变经纬度
address = "湖南省长沙市长沙理工大学云塘校区工科一楼";
myCity = "长沙";
%https://restapi.amap.com/v3/geocode/geo?address=湖南省长沙市长沙理工大学云塘校区工科一楼&city=长沙9&output=JSON&key=f469ef4b32f89b0cae6ebbc8711d2d47
fullURL="https://restapi.amap.com/v3/geocode/geo?address="+address+"&city="+myCity+"&output=JSON"+"&key="+KEY;
ret2 = webread(fullURL);
ret2.geocodes.location
A= sscanf(ret2.geocodes.location,"%f,%f");
lon = A(1);
lat = A(2);
 
 
 %输入经纬度变地址
 % https://restapi.amap.com/v3/geocode/regeo?output=xml&location=116.310003,39.991957&key=<用户的key>&radius=1000&extensions=all
 fullURL=" https://restapi.amap.com/v3/geocode/regeo?output=JSON&location="+lon+","+lat+"&key="+KEY+"&radius=10&extensions=all";
 ret2 = webread(fullURL);
 ret2.regeocode.formatted_address
 
 %查询天气
cityCode = 430105;
fullURL="https://restapi.amap.com/v3/weather/weatherInfo?city="+ cityCode+"&key="+KEY;
ret2 = webread(fullURL);
ret2.lives
ret2.lives.temperature
 

  %导航
 % https://restapi.amap.com/v3/geocode/regeo?output=xml&location=116.310003,39.991957&key=<用户的key>&radius=1000&extensions=all
way = "walking";
%way = "driving";
startPoint = "112.977322,28.155963";%长沙理工大学工科1楼
endPoint = "112.977406,28.154944";%长沙理工大学云塘校区(西门)
 fullURL="https://restapi.amap.com/v3/direction/"+way+"?origin="+startPoint+"&destination="+endPoint+"&output=JSON"+"&key="+KEY;
  fullURL
 ret2 = webread(fullURL);
ret2.route.paths.distance
ret2.route.paths.duration

%给出地图
A = thingSpeakRead(846323,'Location',true)
lat = A(9)
lon = A(10)
startPoint = "112.977322,28.155963";%长沙理工大学工科1楼

fullURL="https://restapi.amap.com/v3/staticmap?location="+startPoint+"&zoom=10&size=750*300"+"&key="+KEY;
fullURL="https://restapi.amap.com/v3/staticmap?location="+lat+","+lon+"&zoom=19&size=750*300"+"&key="+KEY;
fullURL
ret2 = webread(fullURL);
web(fullURL)


%没有实现
%地理围栏服务适用于需要针对特定区域，监控用户位置与区域关系的场景中。包括但不限于：

%签到打卡类场景，在用户打卡操作前，判断用户是否已经在对应的地理围栏区域内；
%共享单车类场景，当用户骑车离开合法使用区域时，第一时间获知；或当用户关锁支付时，判断用户是否停在了合法区域内；
%线下门店促销场景，当获取到用户定位在促销门店区域的附近时，向用户发送引导进入店铺的促销信息。
 %https://lbs.amap.com/api/webservice/guide/api/geofence_service
 
 