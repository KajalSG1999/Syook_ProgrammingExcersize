# File: Makefile
CC = gcc
CFLAGS = -Wall
LIBS = -L/usr/lib/x86_64-linux-gnu -lbluetooth
all:	BLE_Accelerometer_ReadData
BLE_Accelerometer_ReadData:	BLE_Accelerometer_ReadData.c
	$(CC)	$(CFLAGS)	-o	$@ $< $(LIBS)
clean:
	rm	-f	BLE_Accelerometer_ReadData

