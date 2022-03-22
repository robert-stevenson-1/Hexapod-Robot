import socket

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
    "MF": "<9 0 0 {0} 0 0 0>",  # Move Forwards
    "MB": "<9 0 0 -{0} 0 0 0>",  # Move Backwards
    "MR": "<9 {0} 0 0 0 0 0>",  # Move Right
    "ML": "<9 -{0} 0 0 0 0 0>",  # Move Left
    "RR": "<9 0 0 0 0 -{0} 0>",  # Rotate on the spot
    "RL": "<9 0 0 0 0 {0} 0>",  # Rotate on the spot
    "Exit": -1
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
            # get the user input
            raw_msg = input("Client:> ")
            # tokenize the raw input
            msg_tokens = raw_msg.split()

            # get the command
            cmd = msg_tokens[0]
            # get the argument from the commands
            args = msg_tokens[1:]

            # check if a valid command was entered
            if cmd in commands:
                # Call the function in the commands dictionary and store any values that could be returned
                to_send = commands[cmd]
                # check if the command entered was the 'exit' cmd
                if to_send == -1:
                    client_exit()
                    # exit the loop early
                    return
                send(to_send.format(args[0]))
                print('Sent: ' + to_send.format(args[0]))
            else:
                print("WARNING: Command not found")
    except socket.error as e:  # try triggered when connection timed out
        print("ERROR: Failed to Connect to Server...")
        print('-----> Exception: ' + str(e))


if __name__ == '__main__':
    start()
