
--noting chinaese commet is not allowed in this version lua
uart.setup(0, 9600, 8, uart.PARITY_NONE, uart.STOPBITS_1, 0)
--noting : UART为57600
revFieldVal = {-1,-1,-1,-1,-1,-1,-1,-1};
revUartVal = {-1,-1,-1,-1,-1,-1,-1,-1};
uartDataRcv = 0;
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


--thingspeak,read and write key and channelID
CHANNEL_READ_ID = 846323;
CHANNEL_READ_API = "49IF4JCEHOREDFIY";
writekey ="ZK3IYTYF96JA7S7G"


         
tmr.create():alarm(300000, tmr.ALARM_AUTO, function()

  if wifi.sta.getip()== nil then
     --print("IP unavaiable, Waiting...")
  else
    -- print("Uptime (probably):"..tmr.time())
     --print("this wifi mod IP is "..wifi.sta.getip()) 
     --print("this wifi mod hostname is "..wifi.sta.gethostname())
  end
end)



function writeThingspeak(writekey,val1,val2,val3,val4,val5,val6,val7,val8)

    --print("\n Sending data to thingspeak.com")
    conn=net.createConnection(net.TCP, 0) 
    --conn:on("receive", function(conn, payload) 
   -- print(payload) 
    --end)
   
    
   str1 = "&field1="..val1.."&field2="..val2
   str2 = "&field3="..val3.."&field4="..val4
   str3 = "&field5="..val5.."&field6="..val6
   str4 = "&field7="..val7.."&field8="..val8


    conn:connect(80,"184.106.153.149") 


    
    conn:send("GET /update?key="..writekey)


    
    conn:send(str1..str2)
    conn:send(str3..str4)
    conn:send(" HTTP/1.1\r\n") 
    conn:send("Host: api.thingspeak.com\r\n") 
    conn:send("Accept: */*\r\n") 
    conn:send("User-Agent: Mozilla/4.0 (compatible; esp8266 Lua; Windows NT 5.1)\r\n")
    conn:send("\r\n")
    
    conn:on("sent",function(conn)
                      --print("Closing connection")
                      conn:close()
                  end)
    conn:on("disconnection", function(conn)
          --print("Got disconnection...")
  end)
end

-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
function readThingSpeak(CHANNEL_READ_ID,CHANNEL_READ_API)
    
    sockTS = nil
    sockTS = net.createConnection(net.TCP, 0)
    
    sockTS:on("receive", function(sock, payload)
       
        --print("Receive data:\n"..payload.."\n");
        
        if (string.find(payload, "Status: 200 OK") == nil) then
            --print("RCV: HTTP No OK") 
            return 
        end
        -- Everything is OK
        lastline = ""
        for line in payload:gmatch("[^\r\n]+") do
            lastline = line
        end
        --print("Last line on received data:\n"..lastline)

        numbers = {}
        cnt = 1
        for line in payload:gmatch("field%d+\":\"[+-]?%d+\"-") do
            
            tmp = string.gsub(line,"field%d+\":\"","")
            
            numbers[cnt] = tonumber(tmp)
            --print(numbers[cnt])
            cnt = cnt+1
        end
        --print("Numbers extracted: ")
        --print(numbers)
        --for key,value in pairs(numbers) do print("  "..key.." = "..value) end

        -- Do something with the numbers extracted
        if cnt < 8 then
            --print("\n Reading ThingSpeak ERROR! No numbers in payload!"..cnt)
            return 
        else
          --print("RCV : HTTP OK. data: ")
          --print(numbers[1])
         
          revFieldVal = numbers;  

        --some 51mcu,30ms is uart receving interval time
        --uart.write(0,"TS1:"..revFieldVal[1]..'\n');
        
        --tmr.delay(1000*50)
        --uart.write(0,"TS2:"..revFieldVal[2]..'\n');
        --tmr.delay(1000*50) 
        --uart.write(0,"TS3:"..revFieldVal[3]..'\n');
        --tmr.delay(1000*50)
        --uart.write(0,"TS4:"..revFieldVal[4]..'\n');
        --tmr.delay(1000*50)
        --uart.write(0,"TS5:"..revFieldVal[5]..'\n');
        --tmr.delay(1000*50)
        --uart.write(0,"TS6:"..revFieldVal[6]..'\n');
       -- tmr.delay(1000*50)
        --uart.write(0,"TS7:"..revFieldVal[7]..'\n');
       -- tmr.delay(1000*50)
        --uart.write(0,"TS8:"..revFieldVal[8]..'\n');
           
       
      
         
            --chengkeyue,
          uart.write(0,"TS:"..revFieldVal[1].." "
          ..revFieldVal[2].." "
          ..revFieldVal[3].." "
          ..revFieldVal[4].." "
          ..revFieldVal[5].." "
          ..revFieldVal[6].." "
          ..revFieldVal[7].." "
          ..revFieldVal[8].."\n"
          
          )
           
       
          
        
        end
    end)
 
    -- Callback at successful connection 
    sockTS:on("connection", function(sock, payloadout)
 
        sendstring = "GET /channels/"..CHANNEL_READ_ID.."/feeds/last?api_key="..CHANNEL_READ_API
        .. " HTTP/1.1\r\n"
        .. "Host: api.thingspeak.com\r\n"
        .. "Connection: close\r\n"
        .. "Accept: */*\r\n"
        .. "User-Agent: Mozilla/4.0 (compatible; esp8266 Lua; Windows NT 5.1)\r\n"
        .. "\r\n"

        --print("Requesting last field entries in ThingSpeak channel "..CHANNEL_READ_ID)
        sock:send(sendstring)
    end)
 
   -- sockTS:connect(80,'api.thingspeak.com')
    sockTS:connect(80,'184.106.153.149')
end

--readThingSpeak every 3 seconds
tmr.create():alarm(1000, tmr.ALARM_AUTO, function()
   
      if wifi.sta.getip()== nil then
         --print("\n ReadThingspeakData:NO IP")
      else
        --print("\n ReadThingspeakData:Reading \n")
     
         readThingspeakOk=readThingSpeak(CHANNEL_READ_ID,CHANNEL_READ_API)
    
        
      end

end)




uart.on("data","\n",function (data)
     uartDataRcv = 0;
     if wifi.sta.getip()== nil then
     --print("UART:NO IP\n")
     return
     end       

 --uart.write(0,"\n uart rev data:"..data.." type is:"..type(data))
 --uart.write(0,"\n uart rev data length:"..#data.."\n")
 
if #data<15 then
return
end


TMP = {-1,-1,-1,-1,-1,-1,-1,-1};

cnt = 1
--the extracting float number patterm mode is "[+-]?%d+%p?%d*"，
-- BUT luna tonumber can not convert float string to float numbers
-- so the intege number is only available
for word in string.gmatch(data, "[+-]?%d+") 
do 
TMP[cnt]=tonumber(word)

--uart.write(0,'-') 
--uart.write(0,word) 
--uart.write(0,'~') 
--print(tonumber(word))
cnt=cnt+1
end

if cnt<8 then
return
end

revUartVal = TMP;
 uartDataRcv =1;
    --     print("rev data:"..data[3])
     
    -- uartData1=tonumber(data[1]);
    -- uartData2=tonumber(data[2]);
   --  uartData3=tonumber(data[3]);
    -- uartData4=tonumber(data[4]);
    -- uartData5=tonumber(data[5]);
   --  uartData6=tonumber(data[6]);
   --  uartData7=tonumber(data[7]);
   --  uartData8=tonumber(data[8]);

   
end)

--writeThingSpeak every 100 ms 
tmr.create():alarm(100, tmr.ALARM_AUTO, function()

 if wifi.sta.getip()== nil then
    -- print("writeThingspeak:NO IP")
  else


if uartDataRcv == 1 then

val1 = revUartVal[1]
val2 = revUartVal[2]
val3 = revUartVal[3]
val4 = revUartVal[4]
val5 = revUartVal[5]
val6 = revUartVal[6]
val7 = revUartVal[7]
val8 = revUartVal[8]


 uartDataRcv =0;
 writeThingspeak(writekey,val1,val2,val3,val4,val5,val6,val7,val8)

  end
end

end)
