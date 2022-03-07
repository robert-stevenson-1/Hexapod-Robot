import socket

HEADER = 128  # Message length from communication
SERVER = socket.gethostbyname(socket.gethostname())
PORT = 5050  # set the port to connect on
FORMAT = 'utf-8'
DISCONNECT_MESSAGE = "CLIENT_DISCONNECT"
SERVER = "192.168.56.1"
ADDRESS = (SERVER, PORT)

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect(ADDRESS)

def send(msg):
    message = msg.encode(FORMAT)
    msgLength = len(message)
    sendLen = str(msgLength).encode(FORMAT)
    sendLen += b' ' * (HEADER - len(sendLen))
    client.send(sendLen)
    client.send(message)
    print(client.recv(2048).decode(FORMAT))

def start():
    connected = True
    while connected:
        msg = input("Client:> ")
        if 'exit' in msg:
            connected = False
            return
        send(msg)


if __name__ == '__main__':
    start()