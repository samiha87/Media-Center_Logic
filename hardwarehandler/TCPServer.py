import select, socket, sys
from Queue import Queue
from HardwareVolume import processMessage

def ThreadedServer(host, port, icom_in, icom_out):
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	
	s.bind((host, port))
	s.listen(1)
	#s.setblocking(0)
	while True:
		conn, addr = s.accept()
		conn.setblocking(0)
		if conn:
			while True:	
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
