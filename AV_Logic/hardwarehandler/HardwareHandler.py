import serial
import socket
import sys
import asyncore
from TCPServer import ThreadedServer
from Queue import Queue
import threading

#bluetooth = serial.Serial("/dev/ttyAMA0", baudrate = 9600, timeout = 3.0)
# Initialize server to communicate with Qt
# Wait until client connects
# Only one client is allowed
print("Hardware handler started")
print("Starting local server")
# Communication protocol between threads

icomTx = Queue()
icomRx = Queue()
ble_buffer = ""
start_byte = False

def BluetoothServer(device, baud, icom_in, icom_out):
	bluetooth = serial.Serial(device, baudrate = baud, timeout = 3.0)
	start_byte = False

	while True:
        	# Message from server thread
		try:
			thread_data = icom_in.get(False)
			if thread_data:
				bluetooth.write(thread_data)
		except:
			thread_data = None

		if bluetooth.inWaiting() > 0:
			data_left = bluetooth.inWaiting()
			# Read from bluetooth all bit
			received_data = bluetooth.read(data_left)
                        if "#" in received_data and start_byte is False:
				ble_buffer = ""
				start_byte = True
			if start_byte:
                                print("Data from bluetooth: " + received_data)
				ble_buffer += received_data
				if "#" in ble_buffer and "*" in ble_buffer:
                                        print("Data from bluetooth sending forward: " + ble_buffer)
                                        icom_out.put("BLE"+ ble_buffer)
					start_byte = False
					ble_buffer = ""	

serverThread = threading.Thread(target = ThreadedServer, args = ('localhost', 10000, icomRx, icomTx))
bluetoothThread = threading.Thread(target = BluetoothServer, args = ("/dev/ttyAMA0", 9600, icomTx, icomRx))

serverThread.start()
bluetoothThread.start()
icomTx.join()
icomRx.join()

