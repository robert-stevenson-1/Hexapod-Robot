import pickle
import socket
import struct
import threading
import cv2
import serial

HEADER = 128  # Message length from the client to the server

SERVER = socket.gethostbyname(socket.gethostname())
# set the port of the server
PORT = 5050
PORT_CAM = 4000
# get the host IP address
ADDRESS = (SERVER, PORT)
ADDRESS_CAM = (SERVER, PORT_CAM)
FORMAT = 'utf-8'
DISCONNECT_MESSAGE = "CLIENT_DISCONNECT"
# create the server
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# bind the socket the address
server.bind(ADDRESS)

# Serial Communication
controller = serial.Serial()
controller.port = 'COM3'
controller.baudrate = 115200

connected = True

def COMSend(msg):

    # Wait while there is data still to be sent
    while controller.out_waiting > 0:
        print('Server:> COM Waiting...')
        pass
    # write the data to the port
    controller.write(msg)
    print('Server:> Message sent to COM Port')


def handleClient(client, addr):
    global connected
    connected = True

    print("Server:> New connection from {0}".format(addr))
    print("Server:> Connected Stated: {0}".format(connected))

    cam_thread = threading.Thread(target=handle_camera)
    cam_thread.start()

    # while true receive data from the client
    while connected:
        msgLength = client.recv(HEADER).decode(FORMAT)  # Wait for a message from the client
        if msgLength:
            msgLength = int(msgLength)
            msg = client.recv(msgLength).decode(FORMAT)
            # check to see is a disconnect message has been sent
            if msg == DISCONNECT_MESSAGE:
                connected = False
            # send the received message to the COM port
            #COMSend(msg=msg.encode(FORMAT))
            # send a message to the client
            client.send(b"Message sent to COM port")
            # print client message to the server console
            print("Client<{0}>:> {1}".format(addr[0], msg))
    print("Server:> connection {0} closed".format(addr))
    client.close()


def handle_camera():
    global connected
    server_Cam = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    print("Cam Server:> Cam server started")
    print("Cam_Server:> THREAD STARTED")
    print("Cam Server:> Connected Stated: {0}".format(connected))

    server_Cam.connect(ADDRESS_CAM)
    video_cap = cv2.VideoCapture(0, cv2.CAP_DSHOW)
    while connected:
        img, frame = video_cap.read()
        video_data = pickle.dumps(frame)
        msg_size = struct.pack("L", len(video_data))
        server_Cam.sendall(msg_size + video_data)
    print("Cam_Server:> THREAD CLOSED")
    server_Cam.close()


def start():
    # put the server's socket into listening mode
    server.listen()
    print("Server:> Listening on: {0}:{1}) ".format(SERVER, PORT))

    # open the COM port to
    #controller.open()
    if controller.is_open:
        print("Server:>  Controller COM Port open!")

    while True:
        client, addr = server.accept()
        if client:
            print("Server:> Connection from {0}".format(addr))
            thread = threading.Thread(target=handleClient, args=(client, addr))
            thread.start()
        # print the number of active connections (Based on the number of client connections that are being processed)
        print("Server:> ACTIVE CONNECTIONS {0}".format(threading.activeCount() - 1))




if __name__ == '__main__':
    start()
