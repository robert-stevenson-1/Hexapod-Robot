import base64
import pickle
import socket
import struct
import threading
import netifaces
import cv2
import cvzone
import numpy as np
from inputs import get_gamepad
from cvzone.HandTrackingModule import HandDetector
from cvzone.FaceDetectionModule import FaceDetector
from cvzone.FaceMeshModule import FaceMeshDetector

HEADER = 128  # Message length from communication
PORT = 5050  # set the port to connect on
PORT_CAM = 9999

FORMAT = 'utf-8'  # message format used for sending and receiving data via the socket connection
DISCONNECT_MESSAGE = "CLIENT_DISCONNECT"
# Sever address on the network
# SERVER = 'HEXAPOD'
# SERVER = '192.168.1.202'  # 'HEXAPOD'
SERVER = "192.168.56.1"  # 'DESKTOP TEST SERVER'

# get the client network interface
net_interface = netifaces.gateways()['default'][netifaces.AF_INET][1]
# get the ip of the client device
client_ip = netifaces.ifaddresses(net_interface)[netifaces.AF_INET][0]['addr']

ADDRESS = (SERVER, PORT)
ADDRESS_CAM = (client_ip, PORT_CAM)
print(ADDRESS_CAM)

# setup the socket
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
cam_client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
cam_client.bind(ADDRESS_CAM)

# control var for telling if the client is still connected
connected = True
can_send = True

msg_size = struct.calcsize("L")

# Frame buffer detecting hands in frame
hand_frames = []

# short handle commands dictionary
commands = {
    "ABS_HAT0Y-1": "<9 0 0 40 0 0 0>",  # Move Forwards
    "ABS_HAT0Y1": "<9 0 0 -40 0 0 0>",  # Move Backwards
    "ABS_HAT0X1": "<9 40 0 0 0 0 0>",  # Move Right
    "ABS_HAT0X-1": "<9 -40 0 0 0 0 0>",  # Move Left
    "ABS_RZ255": "<9 0 0 0 -15 0 0>",  # Rotate on the spot
    "ABS_Z255": "<9 0 0 0 15 0 0>",  # Rotate on the spot
    "BTN_NORTH1": "<9 0 0 0 0 1 0>",  # head up
    "BTN_SOUTH1": "<9 0 0 0 0 -1 0>",  # head down
    "BTN_WEST1": "<9 0 0 0 0 0 1>",  # head left
    "BTN_EAST1": "<9 0 0 0 0 0 -1>",  # head right
    "BTN_SELECT1": DISCONNECT_MESSAGE
}
# cmd generator control values
trans_val = 40  # translation amount used to translation cmd
rot_val = 15  # rotation amount used for rotation commands
head_opt = 0  # 0 to 5 (inclusive) indicates what head values are send in the command regardless fo the rest of the cmd
head_dir = 0

def client_exit():
    global connected
    connected = False
    send(DISCONNECT_MESSAGE)
    print('Exiting...')
    print('Sent: ' + str(DISCONNECT_MESSAGE))
    return -1


def cam_start():
    fpsReader = cvzone.FPS()
    # faceDetector = FaceDetector()
    # faceMeshDetector = FaceMeshDetector(maxFaces=1)
    avr_fps = []
    vid_data = b''
    cam_client.listen()
    conn, addr = cam_client.accept()
    global connected, msg_size
    print("Camera receive stream started")

    hands_thread = threading.Thread(target=processHands)
    hands_thread.start()

    while connected:
        # start_time = datetime.datetime.now()

        # get the message size
        while len(vid_data) < msg_size:
            vid_data += conn.recv(4096)
        packed_msg_size = vid_data[:msg_size]
        vid_data = vid_data[msg_size:]
        unpacked_msg_size = struct.unpack("L", packed_msg_size)[0]
        # retrieve the video data
        while len(vid_data) < unpacked_msg_size:
            vid_data += conn.recv(2**12)
        frame_data = vid_data[:unpacked_msg_size]
        vid_data = vid_data[unpacked_msg_size:]
        # extract the video frame

        #frame = pickle.loads(frame_data)

        #img = base64.b64decode(frame_data)
        #npimg = np.frombuffer(img, dtype=np.uint8)
        frame_data = np.frombuffer(frame_data, np.uint8)
        frame = cv2.imdecode(frame_data, cv2.IMREAD_COLOR)

        #hand detection
        #hands, frame = handDetector.findHands(frame)
        hand_frames.append(frame)

        #processHands(hands=hands, detector=handDetector)

        # face detection
        #frame, faces = faceDetector.findFaces(frame)

        # face mesh detection
        #frame, faces = faceMeshDetector.findFaceMesh(frame)

        # add fps counter to frame
        #fps, frame = fpsReader.update(img=frame, pos=(40, 80), color=(0, 255, 0), scale=2, thickness=1)
        fps = fpsReader.update()
        avr_fps.append(fps)

        # show the data
        cv2.imshow("frame", frame)
        cv2.waitKey(1)
    print("Avr FPS: " + str(sum(avr_fps)/len(avr_fps)))

def processFace(faces, detector):
    pass

def processHands():
    handDetector = HandDetector(detectionCon=0.8, maxHands=2)
    global hand_frames
    while connected:
        if len(hand_frames) > 0:
            hands = handDetector.findHands(hand_frames, draw=False)
            if hands:
                # Hand 1
                hand1 = hands[0]
                lmList1 = hand1["lmList"]  # List of 21 Landmark points
                bbox1 = hand1["bbox"]  # Bounding box info x,y,w,h
                centerPoint1 = hand1['center']  # center of the hand cx,cy
                handType1 = hand1["type"]  # Handtype Left or Right

                fingers1 = handDetector.fingersUp(hand1)

                print("Hands fingers: " + str(fingers1))

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
                    cmd = generateCMD(event.code, event.state)
                    cmd = cmd.format()
                    print("Generated Cmd: " + cmd)
                    # # check if we are at default/event reset
                    # if event.state != 0:
                    #     cmd_key = str(event.code) + str(event.state)
                    #     # get the full command from the commands dictionary
                    #     if cmd_key in commands:
                    #         cmd = commands[cmd_key]

                    print("Command:       " + str(cmd))
                    # check if the command entered was the 'exit' cmd
                    if cmd == DISCONNECT_MESSAGE:
                        client_exit()
                        # exit the loop early
                        return
                    send(cmd)
                    print('Sent: ' + cmd)
                    # else:
                    #     # send a reset to the robot (acts as a stop command)
                    #     send(str("<8 0 0 0 0 0 0 5>").format())
                    #     print("===============\n")
    except socket.error as e:  # try triggered when connection timed out
        print("ERROR: Failed to Connect to Server...")
        print('-----> Exception: ' + str(e))

# val: the value from event (key) used to decide direction in the returned cmd value
def generateCMD(key, val):
    cmd_body = "<9 0 0 0 0 {0}>"  # contains the main body of the command without the head information
    # cmd_head = ""  # contain the head part of the cmd that is added to the cmd body last

    global head_opt, head_dir

    if key == 'ABS_HAT0Y':
        cmd_body = "<9 0 0 {0} 0 {1}>".format(str(-1*trans_val*val), "{0}")
    elif key == 'ABS_HAT0X':
        cmd_body = "<9 {0} 0 0 0 {1}>".format(str(trans_val*val), "{0}")
    elif key == 'ABS_Z':
        cmd_body = "<9 0 0 0 {0} {1}>".format(str(rot_val), "{0}")
    elif key == 'ABS_RZ':
        cmd_body = "<9 0 0 0 {0} {1}>".format(str(-1*rot_val), "{0}")
    elif key == 'BTN_NORTH':
        head_dir = val
        if head_dir:
            head_opt = 1
        else:
            head_opt = 0
    elif key == 'BTN_SOUTH':
        head_dir = val
        if head_dir:
            head_opt = 2
        else:
            head_opt = 0
    elif key == 'BTN_WEST':
        head_dir = val
        if head_dir:
            head_opt = 3
        else:
            head_opt = 0
    elif key == 'BTN_EAST':
        head_dir = val
        if head_dir:
            head_opt = 4
        else:
            head_opt = 0
    elif key == 'BTN_SELECT':
        return DISCONNECT_MESSAGE

    # get the head cmd part
    if head_opt == 1:
        cmd_head = "{0} 0".format(str(1*head_dir))
    elif head_opt == 2:
        cmd_head = "{0} 0".format(str(-1*head_dir))
    elif head_opt == 3:
        cmd_head = "0 {0}".format(str(1*head_dir))
    elif head_opt == 4:
        cmd_head = "0 {0}".format(str(-1*head_dir))
    else:
        cmd_head = "0 0"

    return cmd_body.format(cmd_head)

if __name__ == '__main__':
    controller_thread = threading.Thread(target=start)
    controller_thread.start()
