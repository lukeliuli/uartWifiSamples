--noting chinaese commet is not allowed in this version lua
--GET http://184.106.153.149/channels/846323/feeds.json?results=1&location=true
--POS http://184.106.153.149/update.json...
--put https://thingspeak.com/channels/846323/maps/channel_show

revFieldVal = {-1,-1,-1,-1,-1,-1,-1,-1};
revUartVal2 ={}
TMP2 = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
cnt2 = 1;
wifi.setmode(wifi.STATION)
wifi.sta.sethostname("LL-ESP8266")
print(wifi.sta.gethostname())
station_cfg={}
station_cfg.ssid="liuli"
station_cfg.pwd=""
station_cfg.ssid="912-903"
station_cfg.pwd="19820812"
station_cfg.save=false
wifi.sta.config(station_cfg)
wifi.sta.autoconnect(1)
--api.thingspeak.com=184.106.153.149

--thingspeak,read and write key and channelID
CHANNEL_READ_ID = 846323;
CHANNEL_READ_API = "49IF4JCEHOREDFIY";
writekey ="ZK3IYTYF96JA7S7G"
UserAPIKey="46SSUIKZSON0BPM0"

----------------------------------------------------------------------------------
----------------------------------------------------------------------------------
tmr.create():alarm(300000, tmr.ALARM_AUTO, function()

  if wifi.sta.getip()== nil then
     --print("IP unavaiable, Waiting...")
  else
    -- print("Uptime (probably):"..tmr.time())
     --print("this wifi mod IP is "..wifi.sta.getip()) 
     --print("this wifi mod hostname is "..wifi.sta.gethostname())
  end
end)

----------------------------------------------------------------------------------
----------------------------------------------------------------------------------

function writeThingspeak(writekey,val1,val2,val3,val4,val5,val6,val7,val8,val9,val10)


        str1 =  "\"field1\":"..val1..",";
        str2 =  "\"field2\":"..val2..",";
        str3 =  "\"field3\":"..val3..",";
        str4 =  "\"field4\":"..val4..",";
        str5 =  "\"field5\":"..val5..",";
        str6 =  "\"field6\":"..val6..",";
        str7 =  "\"field7\":"..val7..",";
        str8 =  "\"field8\":"..val8..",";
        str9 =  "\"latitude\":"..val9..",";
        str10 =  "\"longitude\":"..val10;
        
       

        strA = str1..str2..str3..str4..str5..str6..str7..str8..str9..str10;
        strB = "\"api_key\":".."\""..writekey.."\"";
   strC = "{"..strB..","..strA.."}";
   --print(strC)
   http.post('http://184.106.153.149/update.json',
  'Content-Type: application/json\r\n',
  strC,
  function(code, data)
    if (code < 0) then
      --print("HTTP request failed")
    else
      --print(code, data)
    end
  end)


  
--  print("HTTP put")

--strA = "api_key="..UserAPIKey;
--strB = "&latitude="..val9;
--strC = "&longitude="..val10;
--strD = strA..strB..strC..'\r\n'
--strD = 'api_key=46SSUIKZSON0BPM0&latitude=2&longitude=3'
--print(strD)
--http.put('http://184.106.153.149/channels/846323.json',
-- 'application/x-www-form-urlencoded\r\n',
--strD,
-- function(code, data)
-- if (code < 0) then
--   print("HTTP request failed")
-- else
--   print("HTTP CODE>0")
--   print(code, data)
--  end
--end)


   

end


----------------------------------------------------------------------------------
----------------------------------------------------------------------------------
--https://www.cnblogs.com/sanghg/p/4114469.html
function readThingSpeak(CHANNEL_READ_ID,CHANNEL_READ_API)

   str1= "http://184.106.153.149/channels/"..CHANNEL_READ_ID.."/feeds.json?api_key="..CHANNEL_READ_API
   .."&results=1&location=true";

   --print(str1)
   http.get(str1,
  'Content-Type: application/json\r\n',
  function(code, data)
    if (code < 0) then
      --print("HTTP request failed")
    else
             --print(data)
             numbers = {}
             
            local decoder = sjson.decoder()
            decoder:write(data)
            ret= decoder:result()
            

            if(ret["feeds"][1]["field1"]==sjson.NULL) then
            print('ret["feeds"][1]["field1"]==null')
            return
            end
            
            
       
             numbers[1]=tonumber(ret["feeds"][1]["field1"])
             numbers[2]=tonumber(ret["feeds"][1]["field2"])
             numbers[3]=tonumber(ret["feeds"][1]["field3"])
             numbers[4]=tonumber(ret["feeds"][1]["field4"])
             numbers[5]=tonumber(ret["feeds"][1]["field5"])
             numbers[6]=tonumber(ret["feeds"][1]["field6"])
             numbers[7]=tonumber(ret["feeds"][1]["field7"])
             numbers[8]=tonumber(ret["feeds"][1]["field8"])

             
          

            if(ret["feeds"][1]["latitude"]==sjson.NULL) then
             lat = 0;
             lon = 0;
            else
             lat = tonumber(ret["feeds"][1]["latitude"]);
             lon = tonumber(ret["feeds"][1]["longitude"]);
            end
            
             revFieldVal =  numbers;  

            
          --chengkeyue,
          uart.write(0,"TS:"..revFieldVal[1].." "
          ..revFieldVal[2].." "
          ..revFieldVal[3].." "
          ..revFieldVal[4].." "
          ..revFieldVal[5].." "
          ..revFieldVal[6].." "
          ..revFieldVal[7].." "
          ..revFieldVal[8].." "
          ..lat.." "
          ..lon.."\n")
          

    end
  end)
   

end

----------------------------------------------------------------------------------
----------------------------------------------------------------------------------
uart.on("data","\n",function (data)
         uartDataRcv = 0;
         if wifi.sta.getip()== nil then
         print("UART:NO IP\n")
         return
         end       

 uart.write(0,"\n uart rev data:"..data.." type is:"..type(data))
 uart.write(0,"\n uart rev data length:"..#data.."\n")


        

       
        
  


--extracting field data and GPS data .all data must be pushed into uart which include 8 field and GPS(lat,lon)
--the extracting float number patterm mode is "[+-]?%d+%p?%d*"，
-- BUT luna tonumber can not convert float string to float numbers
-- so the intege number is only available


           -- 70=='F'
        if string.byte(data,1)==70 then
          cnt2=1
            for word in string.gmatch(data, "[+-]?%d+%p?%d*")  
            
            do 

            --uart.write(0,'-') 
            --uart.write(0,word) 
            --uart.write(0,'~') 
            -- print(tonumber(word))
             TMP2[cnt2]=tonumber(word)
                
                
               
                cnt2=cnt2+1
            end
        
            if cnt2<10 then
              return
            end
        
             revUartVal2 = TMP2;
             uartDataRcv =2;
        end
   
end)
----------------------------------------------------------------------------------
----------------------------------------------------------------------------------
--writeThingSpeak every 100 ms 
tmr.create():alarm(500, tmr.ALARM_AUTO, function()

 if wifi.sta.getip()== nil then
     print("writeThingspeak:NO IP")
  else

    
    if uartDataRcv == 2 then
    
    val1 = revUartVal2[1]
    val2 = revUartVal2[2]
    val3 = revUartVal2[3]
    val4 = revUartVal2[4]
    val5 = revUartVal2[5]
    val6 = revUartVal2[6]
    val7 = revUartVal2[7]
    val8 = revUartVal2[8]
    val9 = revUartVal2[9]
    val10 = revUartVal2[10]
    
    
     uartDataRcv =0;
     print(" writeThingspeak")
     writeThingspeak(writekey,val1,val2,val3,val4,val5,val6,val7,val8,val9,val10)
    
    --note:the bin version should be float
    
     end

 
end

end)

----------------------------------------------------------------------------------
----------------------------------------------------------------------------------

--readThingSpeak every 3 seconds
tmr.create():alarm(3000, tmr.ALARM_AUTO, function()
   
      if wifi.sta.getip()== nil then
         print("\n ReadThingspeakData:NO IP")
      else
        --print("\n ReadThingspeakData:Reading \n")
     
         readThingspeakOk=readThingSpeak(CHANNEL_READ_ID,CHANNEL_READ_API)
    
        
      end

end)



