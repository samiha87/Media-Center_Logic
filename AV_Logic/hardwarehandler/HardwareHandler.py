import serial
import socket
import sys
import asyncore
from TCPServer import ThreadedServer
import select, socket, sys
from Queue import Queue
import time
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
    s.bind((host, port))
    tcp_start_byte = False
    t0 = 0;

    while True:
        s.listen(1)
        conn, addr = s.accept()
        conn.setblocking(0)
        t0 = time.time()
        print("HardwareHandler::ThreadedTCPControlServer() Connected")
        if conn:
            # Connection test
            # Add connection test, if connection lost close current connection and go to listening mode
            if t0 < time.time() -  5 :	# Every 5 seconds check connection
                t0 = time.time()
                try:
                    conn.send("test")
                except socket.error, e:
                    print("TCPServer, ThreadedTCPControlServer() Connection lost")
                    conn.close() # Close socket and exit while loop
                    # Wait until new connection is established
                    break

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
                    data = conn.recv(1024)
                    if data:
                        print("HardwareHandler::ThreadedTCPControlServer() Data from Control: " + data)
                        if "#" in data and tcp_start_byte is False:
                            controlBuffer = ""
                            tcp_start_byte = True
                        if tcp_start_byte:
                            print("HardwareHandler::ThreadedTCPControlServer() Data from control: " + data)
                            controlBuffer += data
                            if "#" in controlBuffer and "*" in controlBuffer:
                                print("HardwareHandler::ThreadedTCPControlServer() Sending forward: " + controlBuffer)
                                icom_out.put("TCP" + controlBuffer)
                                tcp_start_byte = False
                                controlBuffer = ""
                except:
                    data = None

# Define threads
serverThread = threading.Thread(target = ThreadedServer, args = ('localhost', 10000, icomRx, icomTx))
bluetoothThread = threading.Thread(target = BluetoothServer, args = ("/dev/ttyAMA0", 9600, icomTx, icomRx))
tcpControlThread = threading.Thread(target = ThreadedTCPControlServer, args = ("10.42.0.2", 10001, icomTx, icomRx))

# Launch threads
serverThread.start()
bluetoothThread.start()
tcpControlThread.start()
# Connect queue
icomTx.join()
icomRx.join()

