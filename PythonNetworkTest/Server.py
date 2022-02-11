import socket

class Server:
    def __init__(self):
        self.name = "Server"
        # create the server socket
        self.soc = socket.socket()
        # set the port of the server
        self.port = 40674
        # bind the socket the the port (No IP set)
        self.soc.bind(('', self.port))

    def serverRun(self):
        # put the server's socket into listening mode
        self.soc.listen(5)
        print("Server Listening on Port: %s" % self.port)
        while True:
            client, addr = self.soc.accept()
            print("Got a connecting from", addr)

            # send a message to the client
            client.send(b"Hello from the server")

            # close the client connection
            client.close()


"""
    def bind(self, address):
        self.socket.bind(address=address)

    def listen(self, backlog):
        self.socket.listen(__backlog=backlog)

    def accept(self):
        self.socket.accept()

    def connect(self, address):
        self.socket.connect(address=address)

    def sendTCP(self, data, flags):
        self.socket.send(data=data, flags=flags)

    def sendUDP(self, data, flags):
        self.socket.sendto(data=data, flags=flags)

    def ReceiveTCP(self, bufsize, flags):
        self.socket.recv(bufsize=bufsize, flags=flags)

    def ReceiveUDP(self, bufsize, flags):
        self.socket.recvfrom(bufsize=bufsize, flags=flags)

    def close(self):
        self.socket.close()

    def getHostName(self):
        return gethostname()
"""
if __name__ == '__main__':
    server = Server()
    print(str(server.soc))
    server.serverRun()
