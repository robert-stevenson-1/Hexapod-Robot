import datetime
import pickle
import socket
import struct
import threading

import cv2
from inputs import get_gamepad

HEADER = 128  # Message length from communication
PORT = 5050  # set the port to connect on
PORT_CAM = 9999

FORMAT = 'utf-8'  # message format used for sending and receiving data via the socket connection
DISCONNECT_MESSAGE = "CLIENT_DISCONNECT"
# Sever address on the network
# SERVER = 'HEXAPOD'
SERVER = '192.168.1.202'  # 'HEXAPOD'
# SERVER = "192.168.56.1"  # 'DESKTOP TEST SERVER'

ADDRESS = (SERVER, PORT)
ADDRESS_CAM = ('192.168.1.90', PORT_CAM)  # TODO: !!!!! <----  get the IP address of the client system and use it here
print(ADDRESS_CAM)

# setup the socket
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
cam_client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
cam_client.bind(ADDRESS_CAM)

# control var for telling if the client is still connected
connected = True
can_send = True

msg_size = struct.calcsize("L")

# short handle commands dictionary
commands = {
    "ABS_HAT0Y-1": "<9 0 0 40 0 0 0>",  # Move Forwards
    "ABS_HAT0Y1": "<9 0 0 -40 0 0 0>",  # Move Backwards
    "ABS_HAT0X1": "<9 40 0 0 0 0 0>",  # Move Right
    "ABS_HAT0X-1": "<9 -40 0 0 0 0 0>",  # Move Left
    "ABS_RZ255": "<9 0 0 0 -15 0 0>",  # Rotate on the spot
    "ABS_Z255": "<9 0 0 0 15 0 0>",  # Rotate on the spot
    "BTN_NORTH1": "<9 0 0 0 0 1 0>",  # head up
    # "BTN_NORTH0": "0 0",  #
    "BTN_SOUTH1": "<9 0 0 0 0 -1 0>",  # head right
    # "BTN_SOUTH0": "0 0",  #
    "BTN_WEST1": "<9 0 0 0 0 0 1>",  # head left
    # "BTN_WEST0": "0 0",  #
    "BTN_EAST1": "<9 0 0 0 0 0 -1>",  # head right
    # "BTN_EAST0": "0 0",  #
    "BTN_SELECT1": DISCONNECT_MESSAGE
}


def client_exit():
    global connected
    connected = False
    send(DISCONNECT_MESSAGE)
    print('Exiting...')
    print('Sent: ' + str(DISCONNECT_MESSAGE))
    return -1


def cam_start():
    vid_data = b''
    cam_client.listen()
    conn, addr = cam_client.accept()
    global connected, msg_size
    print("Camera receive stream started")
    while connected:
        start_time = datetime.datetime.now()
        # get the message size
        while len(vid_data) < msg_size:
            vid_data += conn.recv(4096)
        packed_msg_size = vid_data[:msg_size]
        vid_data = vid_data[msg_size:]
        unpacked_msg_size = struct.unpack("L", packed_msg_size)[0]
        # retrieve the video data
        while len(vid_data) < unpacked_msg_size:
            vid_data += conn.recv(8196)
        frame_data = vid_data[:unpacked_msg_size]
        vid_data = vid_data[unpacked_msg_size:]
        # extract the video frame
        frame = pickle.loads(frame_data)

        # calc fps
        end_time = datetime.datetime.now()
        fps = 1 / ((end_time - start_time).total_seconds() + 0.00000000000000000001)  # prevent div by zero error
        print("Fps: ", round(fps, 2))

        # show the data
        cv2.imshow("frame", frame)
        cv2.waitKey(1)


def send(msg):
    # global can_send
    # check if we are allowed to send
    # if can_send:
    can_send = False
    message = msg.encode(FORMAT)
    msgLength = len(message)
    sendLen = str(msgLength).encode(FORMAT)
    sendLen += b' ' * (HEADER - len(sendLen))
    client.send(sendLen)
    client.send(message)
    received = client.recv(2048).decode(FORMAT)
    if len(received) > 0:
        # can_send = True
        print(received)


def start():
    try:
        client.connect(ADDRESS)  # Connect to the server
        global connected  # access the global var value
        cam_thread = threading.Thread(target=cam_start)
        cam_thread.start()
        cmd = ''  # initialise the command
        # loop while connected
        while connected:
            # get the controller events
            events = get_gamepad()
            for event in events:
                # ignore system report events
                if event.code != 'SYN_REPORT':
                    print("Event: {0} val: {1}".format(str(event.code), str(event.state)))
                    # check if we are at default/event reset
                    if event.state != 0:
                        cmd_key = str(event.code) + str(event.state)
                        # get the full command from the commands dictionary
                        if cmd_key in commands:
                            cmd = commands[cmd_key]
                            print("Command: " + str(cmd))
                            # check if the command entered was the 'exit' cmd
                            if cmd == DISCONNECT_MESSAGE:
                                client_exit()
                                # exit the loop early
                                return
                            send(cmd)
                            print('Sent: ' + cmd)
                    else:
                        print('Sent reset!!')
                        # send a reset to the robot (acts as a stop command)
                        send("<8 0 0 0 0 0 0 5>")
    except socket.error as e:  # try triggered when connection timed out
        print("ERROR: Failed to Connect to Server...")
        print('-----> Exception: ' + str(e))


if __name__ == '__main__':
    controller_thread = threading.Thread(target=start)
    controller_thread.start()
