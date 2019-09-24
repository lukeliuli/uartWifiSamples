%     MyBody = matlab.net.http.MessageBody('api_key=46SSUIKZSON0BPM0&latitude=1&longitude=2');
%     Request = matlab.net.http.RequestMessage;
%     Request.Method = matlab.net.http.RequestMethod.PUT;
%     Request.Header = matlab.net.http.HeaderField('Content-Type','application/x-www-form-urlencoded');
%     Request.Body = MyBody;
%     uri = matlab.net.URI('http://184.106.153.149/channels/846323.json');
%     
%      options = matlab.net.http.HTTPOptions('ConnectTimeout',20);
%       Response = Request.send(uri, options);
%        Response 
    
       %113.010490,28.066321
       %经度度是大的
KEY ="f469ef4b32f89b0cae6ebbc8711d2d47";
userkey = '46SSUIKZSON0BPM0'
A = thingSpeakRead(846323,'Location',true);
lat = A(9)
lon = A(10)
str= sprintf('api_key=%s&latitude=%f&longitude=%f',userkey,lat,lon)
h1 = webmap;

    centerLatitude = lat;
    centerLongitude = lon;
    zoomLevel = 16;
    wmcenter(h1, centerLatitude, centerLongitude, zoomLevel)
    
 
MyBody = matlab.net.http.MessageBody(str);
Request = matlab.net.http.RequestMessage;
Request.Method = matlab.net.http.RequestMethod.PUT;
Request.Header = matlab.net.http.HeaderField('Content-Type','application/x-www-form-urlencoded');
Request.Body = MyBody;
uri = matlab.net.URI('http://184.106.153.149/channels/846323.json');

options = matlab.net.http.HTTPOptions('ConnectTimeout',20);
Response = Request.send(uri, options);


%https://thingspeak.com/channels/846323/maps/channel_show
 