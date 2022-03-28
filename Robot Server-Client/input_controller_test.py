import math
import time
import inputs
from inputs import devices, GamePad, get_gamepad

MAX_JOY = math.pow(2, 15)

gp_events = [
    "ABS_X",
    "ABS_Y",
    "ABS_RX",
    "ABS_RY"
]


def controller_test():
    # get the first gamepad found by the pc
    gp: GamePad = devices.gamepads[0]
    print("Name: " + str(gp.name))
    print("Device Type: " + str(gp.device_type))
    print("leds: " + str(gp.leds))

    while True:
        events = get_gamepad()
        for event in events:
            if event.code == 'ABS_X':
                print(f'Left joystick x: {round(event.state/MAX_JOY, 2)}')
            elif event.code == 'ABS_Y':
                print(f'Left joystick y: {round(event.state/MAX_JOY, 2)}')
            elif event.code == 'ABS_RX':
                print(f'Right joystick x: {round(event.state/MAX_JOY, 2)}')
            elif event.code == 'ABS_RY':
                print(f'Right joystick y: {round(event.state/MAX_JOY, 2)}')
            elif event.code == 'ABS_HAT0X':
                print(f'D-Pad x: {event.state}')
            elif event.code == 'ABS_HAT0Y':
                print(f'D-Pad y: {event.state}')
            elif event.code != 'SYN_REPORT':
                print('{0}, {1}'.format(event.code, event.state))


# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    controller_test()
