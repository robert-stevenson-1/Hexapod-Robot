import socket
import threading

class Client:
    def __init__(self):
        self.name = "Client"
        # create the server socket
        self.socket = socket.socket()
        # set the port of the server
        self.port = 40674
        self.address = '127.0.0.1'

    def startClient(self):
        # connect to the server on local computer
        self.socket.connect((self.address, self.port))

        # receive data from the server
        print(self.socket.recv(1024))

        # close the connection
        self.socket.close()

if __name__ == '__main__':
    client = Client()
    client.startClient()
