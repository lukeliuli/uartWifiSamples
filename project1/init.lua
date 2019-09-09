

print("\n nodemcu-esp82660-liuli")
--uart using 19200  for 51 
uart.setup(0, 9600, 8, uart.PARITY_NONE, uart.STOPBITS_1, 0)
--dofile("test3.lua")
dofile("test3For51.lua")