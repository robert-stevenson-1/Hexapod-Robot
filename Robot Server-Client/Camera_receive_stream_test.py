import pickle
import socket
import struct

import cv2

PORT = 4000  # set the port to connect on
DISCONNECT_MESSAGE = "CLIENT_DISCONNECT"
# Sever address on the network
# SERVER = "192.168.1.202"  # 'HEXAPOD'
SERVER = "192.168.56.1"  # 'DESKTOP TEST SERVER'
ADDRESS = (SERVER, PORT)

# setup the socket
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.bind(ADDRESS)

connected = True
msg_size = struct.calcsize("L")

def cam_start():
    vid_data = b''
    client.listen()
    conn, addr = client.accept()
    global connected, msg_size
    while connected:
        # get the message size
        while len(vid_data) < msg_size:
            vid_data += conn.recv(4096)
        packed_msg_size = vid_data[:msg_size]
        vid_data = vid_data[msg_size:]
        unpacked_msg_size = struct.unpack("L", packed_msg_size)[0]
        # retrieve the video data
        while len(vid_data) < unpacked_msg_size:
            vid_data += conn.recv(4096)
        frame_data = vid_data[:unpacked_msg_size]
        vid_data = vid_data[unpacked_msg_size:]
        # extract the video frame
        frame = pickle.loads(frame_data)
        # show the data
        cv2.imshow("frame", frame)
        cv2.waitKey(1)


if __name__ == '__main__':
    cam_start()
