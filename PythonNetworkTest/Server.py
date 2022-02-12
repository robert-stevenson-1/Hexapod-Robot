import socket
import threading

HEADER = 128  # Message length from the client to the server

SERVER = socket.gethostbyname(socket.gethostname())
# set the port of the server
PORT = 5050
# get the host IP address
ADDRESS = (SERVER, PORT)
FORMAT = 'utf-8'
DISCONNECT_MESSAGE = "CLIENT_DISCONNECT"
# create the server
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# bind the socket the address
server.bind(ADDRESS)


def handleClient(client, addr):
    print("Server:> New connection from {0}".format(addr))

    connected = True
    # while true receive data from the client
    while connected:
        msgLength = client.recv(HEADER).decode(FORMAT)  # Wait for a message from the client
        if msgLength:
            msgLength = int(msgLength)
            msg = client.recv(msgLength).decode(FORMAT)
            if msg == DISCONNECT_MESSAGE:
                connected = False
            print("Client<{0}>:> {1}".format(addr[0], msg))
            client.send("Message Received".encode(FORMAT))
    client.close()


def start():
    # put the server's socket into listening mode
    server.listen()
    print("Server:> Listening on: {0}:{1}) ".format(SERVER, PORT))
    while True:
        client, addr = server.accept()
        thread = threading.Thread(target=handleClient, args=(client, addr))
        thread.start()
        # print the number of active connections (Based on the number of client connections that are being processed)
        print("Server:> ACTIVE CONNECTIONS {0}".format(threading.activeCount() - 1))
        # send a message to the client
        client.send(b"Hello from the server")



if __name__ == '__main__':
    start()
