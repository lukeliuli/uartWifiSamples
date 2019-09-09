



wifi.setmode(wifi.STATION)
wifi.sta.sethostname("Node-MCU-liuli")
print(wifi.sta.gethostname())
station_cfg={}
station_cfg.ssid="1206"
station_cfg.ssid="912-903"
station_cfg.pwd="19820812"
station_cfg.save=true
wifi.sta.config(station_cfg)
wifi.sta.autoconnect(1)


tmr.create():alarm(10000, tmr.ALARM_AUTO, function()

  if wifi.sta.getip()== nil then
     print("IP unavaiable, Waiting...")
  else
     print("Uptime (probably):"..tmr.time())
     print("this wifi mod IP is "..wifi.sta.getip()) 
     print("this wifi mod hostname is "..wifi.sta.gethostname())
     print("this wifi connected ap is  ".."912-903")
  end
end)

function postToThingspeak1()
   print("give and take info from Thingspeak")
end
function sendData(tmp)

    print("Sending data to thingspeak.com")
    conn=net.createConnection(net.TCP, 0) 
    conn:on("receive", function(conn, payload) 
    print(payload) 
    end)
--Write API Key = ZK3IYTYF96JA7S7G
--Read API Keys 49IF4JCEHOREDFIY
    conn:connect(80,'184.106.153.149') 
    conn:send("GET /update?key=ZK3IYTYF96JA7S7G&field1="..tmp.." HTTP/1.1\r\n") 
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

function sendDataAll(tmp)

    print("Sending data to thingspeak.com")
    conn=net.createConnection(net.TCP, 0) 
    conn:on("receive", function(conn, payload) 
    print(payload) 
    end)
--Write API Key = ZK3IYTYF96JA7S7G
--Read API Keys 49IF4JCEHOREDFIY
    conn:connect(80,'184.106.153.149') 
    conn:send("GET /update?key=ZK3IYTYF96JA7S7G&field1=0.1&field2=0.2&field3=0.3".." HTTP/1.1\r\n") 
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

function sendDataAll2(writekey,val1,val2,val3,val4,val5,val6,val7,val8)

   -- print("Sending data to thingspeak.com")
    conn=net.createConnection(net.TCP, 0) 
    --conn:on("receive", function(conn, payload) 
   -- print(payload) 
    --end)
   
    
   str1 = "&field1="..val1.."&field2="..val2
   str2 = "&field3="..val3.."&field4="..val4
   str3 = "&field5="..val5.."&field6="..val6
   str4 = "&field7="..val7.."&field8="..val8
    --str2 = "&field5="..val5.."&field6="..val6.."&field7="..val7.."&field8="..val8
--Write API Key = ZK3IYTYF96JA7S7G
--Read API Keys 49IF4JCEHOREDFIY
--str3 = str1..str2;
--print(str1..str2..str3..str4)

    conn:connect(80,'184.106.153.149') 
     --conn:send("GET /update?key="..writekey..str1..str2.." HTTP/1.1\r\n") 
    conn:send("GET /update?key=ZK3IYTYF96JA7S7G")
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


function sendData2(val,fieldnum,writekey)

   -- print("Sending data to thingspeak.com")
    conn=net.createConnection(net.TCP, 0) 
    conn:on("receive", function(conn, payload) 
    print(payload) 
    end)
--Write API Key = ZK3IYTYF96JA7S7G
--Read API Keys 49IF4JCEHOREDFIY
    conn:connect(80,'184.106.153.149') 
    conn:send("GET /update?key="..writekey.."&field"..fieldnum.."="..val.." HTTP/1.1\r\n") 
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
            print("RCV: HTTP No OK") 
            return
        end
        -- Everything is OK
        lastline = ""
        for line in payload:gmatch("[^\r\n]+") do
            lastline = line
        end
        print("Last line on received data:\n"..lastline)

        numbers = {}
        cnt = 1
        for line in payload:gmatch("field%d+\":\"[+-]?%d+\"-") do
            --print(line)
            tmp = string.gsub(line,"field%d+\":\"","")
            numbers[cnt] = tonumber(tmp)
            cnt = cnt+1
        end
        --print("Numbers extracted: ")
        --for key,value in pairs(numbers) do print("  "..key.." = "..value) end

        -- Do something with the numbers extracted
        if cnt < 9 then
            print("ERROR! No numbers in payload!"..cnt)
            return
        else
          print("RCV : HTTP OK. data: ")
          print(numbers)
         
        
        end
    end)
 
    -- Callback at successful connection 
    sockTS:on("connection", function(sock, payloadout)
 
        sendstring = "GET /channels/846323/feeds/last?api_key=CHANNEL_READ_API"
        .. " HTTP/1.1\r\n"
        .. "Host: api.thingspeak.com\r\n"
        .. "Connection: close\r\n"
        .. "Accept: */*\r\n"
        .. "User-Agent: Mozilla/4.0 (compatible; esp8266 Lua; Windows NT 5.1)\r\n"
        .. "\r\n"

        --print("Requesting last field entries in ThingSpeak channel "..CHANNEL_READ_ID)
        sock:send(sendstring)
    end)
 
    sockTS:connect(80,'api.thingspeak.com')
end


tmr.create():alarm(3000, tmr.ALARM_AUTO, function()

  if wifi.sta.getip()== nil then
     print("IP unavaiable, Waiting...")
  else
     val =100;
     fieldnum =1;
     writekey ="ZK3IYTYF96JA7S7G";
     --sendData2(val,fieldnum,writekey)
     
     val = 2;
     fieldnum =3;
     writekey ="ZK3IYTYF96JA7S7G";
     --sendData2(val,fieldnum,writekey)


     val = 3;
     fieldnum =2;
     writekey ="ZK3IYTYF96JA7S7G";
     --sendData2(val,fieldnum,writekey)

     
     --sendDataAll(fieldnum,writekey)
     val1=40;
     val2=0
     val3=0;
     val4=-1;
     val5=-1;
     val6=-1;
     val7=-1;
     val8=-1;
    sendDataAll2(writekey,val1,val2,val3,val4,val5,val6,val7,val8)
     
     CHANNEL_READ_ID = 846323;
     CHANNEL_READ_API = "49IF4JCEHOREDFIY";
     readThingSpeak(CHANNEL_READ_ID,CHANNEL_READ_API)
     
  end
end)



  
