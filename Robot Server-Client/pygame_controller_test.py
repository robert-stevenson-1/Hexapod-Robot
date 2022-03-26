import time

import pygame as pg
import pygame.joystick


def controller_test():
    pg.init()
    # initialize the controller module
    pg.joystick.init()

    # get a the first controller connected reference
    controller = pygame.joystick.Joystick(0)
    controller.init()  # init the controller
    if controller.get_init():
        print("controller init")

        print("Controller information:")
        print("name: " + controller.get_name())

        cont_axes = controller.get_numaxes()
        print("num. of axis: " + str(cont_axes))

        while True:
            pg.event.pump()  # pump the event to get the joystick functions to work
            print("--------------")
            for i in range(cont_axes):
                print("Axis " + str(i) + " value:" + str(controller.get_axis(i)))
            # print("X-axis: " + str(axes.get_axis(0)) + " Y-axis: " + str(controller.get_axis(1)))
            print("--------------")
            time.sleep(0.1)
    pass


# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    controller_test()