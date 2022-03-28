import socket
from inputs import get_gamepad

HEADER = 128  # Message length from communication
PORT = 5050  # set the port to connect on
FORMAT = 'utf-8'  # message format used for sending and receiving data via the socket connection
DISCONNECT_MESSAGE = "CLIENT_DISCONNECT"
# Sever address on the network
SERVER = "192.168.1.202"  # 'HEXAPOD'
ADDRESS = (SERVER, PORT)

# setup the socket
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# control var for telling if the client is still connected
connected = True

# short handle commands dictionary
commands = {
    "ABS_HAT0Y-1": "<9 0 0 40 0 0 0>",  # Move Forwards
    "ABS_HAT0Y1": "<9 0 0 -40 0 0 0>",  # Move Backwards
    "ABS_HAT0X1": "<9 40 0 0 0 0 0>",  # Move Right
    "ABS_HAT0X-1": "<9 -40 0 0 0 0 0>",  # Move Left
    "ABS_RZ255": "<9 0 0 0 0 -15 0>",  # Rotate on the spot
    "ABS_Z255": "<9 0 0 0 0 15 0>",  # Rotate on the spot
    "BTN_SELECT1": -1
}

def client_exit():
    global connected
    connected = False
    send(DISCONNECT_MESSAGE)
    print('Exiting...')
    print('Sent: ' + str(DISCONNECT_MESSAGE))
    return -1


def send(msg):
    message = msg.encode(FORMAT)
    msgLength = len(message)
    sendLen = str(msgLength).encode(FORMAT)
    sendLen += b' ' * (HEADER - len(sendLen))
    client.send(sendLen)
    client.send(message)
    print(client.recv(2048).decode(FORMAT))


def start():
    try:
        client.connect(ADDRESS)  # Connect to the server
        global connected  # access the global var value

        # loop while connected
        while connected:
            cmd = ""
            # get the controller events
            events = get_gamepad()
            for event in events:
                # if event.code == 'ABS_HAT0X':
                #     print(f'D-Pad x: {event.state}')
                # elif event.code == 'ABS_HAT0Y':
                #     print(f'D-Pad y: {event.state}')
                # elif event.code != 'SYN_REPORT':
                #     print('{0}, {1}'.format(event.code, event.state))
                cmd = str(event.code) + str(event.state)
            print("Command: " + cmd)
            # check if a valid command was entered
            if cmd in commands:
                # get the full command from the commands dictionary
                to_send = commands[cmd]
                # check if the command entered was the 'exit' cmd
                if to_send == -1:
                    client_exit()
                    # exit the loop early
                    return
                send(to_send)
                print('Sent: ' + to_send)
            else:
                print("WARNING: Command not found")
    except socket.error as e:  # try triggered when connection timed out
        print("ERROR: Failed to Connect to Server...")
        print('-----> Exception: ' + str(e))


if __name__ == '__main__':
    start()
