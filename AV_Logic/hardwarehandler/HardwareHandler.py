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
controlBuffer = ""
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

def ThreadedTCPControlServer(host, port, icon_in, icom_out):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    t0 = 0;
    s.bind((host, port))
    conn, addr = s.accept()
    conn.setBlocking(0)
    print("HardwareHandler::ThreadedTCPControlServer() Connected")
    if conn:
        while True:
            # Data from local host
            try:
                thread_data = icom_in.get(False)
                if thread_data:
                    conn.sendall(thread_data)
            except:
                thread_data = None

            # Data from control
            try:
                data = conn.received_data
                if data:
                    print("HardwareHandler::ThreadedTCPControlServer() Data from Control: " + data)
                    if "#" in data and start_byte is False:
                        controlBuffer = ""
                        start_byte = True
                    if start_byte:
                        print("HardwareHandler::ThreadedTCPControlServer() Data from control: " + data)
                        controlBuffer += received_data
                        if "#" in ble_buffer and "*" in ble_buffer:
                            print("HardwareHandler::ThreadedTCPControlServer() Sending forward: " + controlBuffer)
                            icom_out.put("TCP"+ controlBuffer)
                            start_byte = False
                            controlBuffer = ""
                 except:
                     data = none;
# Define threads
serverThread = threading.Thread(target = ThreadedServer, args = ('localhost', 10000, icomRx, icomTx))
bluetoothThread = threading.Thread(target = BluetoothServer, args = ("/dev/ttyAMA0", 9600, icomTx, icomRx))
tcpControlThread = threading.Thread(target= ThreadedTCPControlServer, args = ('localhost', 100001), icomTx, icomRx)
# Launch threads
serverThread.start()
bluetoothThread.start()
tcpControlThread.start()
# Connect queue
icomTx.join()
icomRx.join()

