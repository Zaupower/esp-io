import socket
import threading
import socketserver
import time


#Handle clients messages
class ThreadedTCPRequestHandler(socketserver.BaseRequestHandler):

    def handle(self):
        data = self.request.recv(1024)
        print("From client: ", self.client_address, "message: ", data, ", on thread: ", threading.current_thread().name)
        cur_thread = threading.current_thread()

        response = bytes.fromhex('01AF23')
        self.request.sendall(response)

class ThreadedTCPServer(socketserver.ThreadingMixIn, socketserver.TCPServer):
    pass


#client declaration
def client(ip, port, message):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.connect((ip, port))
        sock.sendall(bytes.fromhex('1C'))
        response = sock.recv(1024)
        print("Response: ",response.hex())
        #time.sleep(2)


def start_server():

    server = ThreadedTCPServer((HOST, PORT), ThreadedTCPRequestHandler)
    server2 = ThreadedTCPServer((HOST, 5555), ThreadedTCPRequestHandler)
    v = 5555
    with server,server2:
        ip, port = server.server_address
        ip, v = server.server_address
        # Start a thread with the server -- that thread will then start one
        # more thread for each request
        server_thread = threading.Thread(target=server.serve_forever)
        server_thread2 = threading.Thread(target=server2.serve_forever)
        # Exit the server thread when the main thread terminates
        server_thread.daemon = True
        server_thread.start()

        server_thread2.daemon = True
        server_thread2.start()
        print("Server loop running in thread:", server_thread.name)
        print("Server loop running in thread:", server_thread2.name)
        #client(ip, port, "1A")
        #client(ip, 5555, "1B")
        # client(ip, port, "3C")

        server.serve_forever()
        server2.serve_forever()

if __name__ == "__main__":
    # Port 0 means to select an arbitrary unused port
    HOST, PORT = "172.21.28.77", 1234

    start_server()
