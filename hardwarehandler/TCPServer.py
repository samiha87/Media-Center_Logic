import select, socket, sys
from Queue import Queue
from HardwareVolume import processMessage
import time

def ThreadedServer(host, port, icom_in, icom_out):
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	t0 = 0	
	s.bind((host, port))
	
	while True:
		s.listen(1)
		t0 = time.time()	# Return time in seconds
		conn, addr = s.accept()
		conn.setblocking(0)
		print("TCPServer, ThreadedServer() We have a connection")
		if conn:
			while True:	
				# Add connection test, if connection lost close current connection and go to listening mode
				if t0 < time.time() -  20 :	# Every 20 seconds check connection				
					t0 = time.time()					
					try:
						conn.send("test")
					except socket.error, e:
						
					    print("TCPServer, ThreadedServer() Connection lost")
					    conn.close() # Close socket and exit while loop
					    # Wait until new connection is established
					    break							
					

				try:
					data = conn.recv(1024)
					if data:
						# Send tp queue
						# Data from Qt app
						print("Data from Qt: ")
						print(data)
						if("#HW,Vol" in data):
							print("Processing volume")
							hw_data = processMessage(data)
							if hw_data:
								print(hw_data)
						icom_out.put(data)
						print(data)
				except:
					data = None
				try:
				# Set non blocking
					thread_data = icom_in.get(False)
					if thread_data:
						print("ThreadedServer__Data from queue")
						print(thread_data)
						conn.sendall(thread_data)
				except:
					thread_data = None	
