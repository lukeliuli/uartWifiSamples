print("\n nodemcu-esp82660-liuli")
uart.setup(0, 57600, 8, uart.PARITY_NONE, uart.STOPBITS_1, 0)
--51单片机序列用1152000波特率有问题
dofile("test3.lua")
