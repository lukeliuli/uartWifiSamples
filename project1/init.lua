
wifi.setmode(wifi.STATION)
wifi.sta.sethostname("Node-MCU")
print(wifi.sta.gethostname())

station_cfg={}
station_cfg.ssid="912-903"
station_cfg.pwd="19820812"
station_cfg.save=true
wifi.sta.config(station_cfg)
wifi.sta.autoconnect(1)
wifi.sta.connect()
print(wifi.sta.getip())