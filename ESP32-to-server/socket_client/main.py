# Import socket module
import socket

HOST, PORT = "172.21.28.77", 5022

def Main():
	with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
		sock.connect((HOST, PORT))
		sock.sendall(bytes.fromhex('1C'))
		response = sock.recv(1024)
		print("Response: ", response.hex())


# time.sleep(2)

if __name__ == '__main__':
	Main()
