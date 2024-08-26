### Made originally for CampZone 2019 "i-Pane" by Sebastius
### Original app: https://hatchery.badge.team/files/37811
### Left/Right: Brightness
### Up/Down: Red or White (Red saves your eyesight at night)
### Todo: alert lights (blinking)

import rgb, buttons

# colors
colors = [
    # white
    (255, 255, 255),
    (255,   0,   0),
]

color = 0
direction = 0
brightness = rgb.getbrightness()
rgb.clear()

UP, DOWN, LEFT, RIGHT = buttons.BTN_UP, buttons.BTN_DOWN, buttons.BTN_LEFT, buttons.BTN_RIGHT
A, B = buttons.BTN_A, buttons.BTN_B

def input_up(pressed):
    global color
    if pressed:
        color = (color + 1) % (len(colors))
        rgb.background(colors[color])

def input_down(pressed):
    global color
    if pressed:
        color = (color - 1) % (len(colors))
        rgb.background(colors[color])

def input_left(pressed):
    global brightness
    if pressed and brightness > 3:
        brightness=brightness-1
        rgb.brightness(brightness)
        print(brightness)

def input_right(pressed):
    global brightness
    if pressed and brightness < 32:
        brightness=brightness+1
        rgb.brightness(brightness)
        print(brightness)

def input_A(pressed):
    pass

buttons.register(UP, input_up)
buttons.register(DOWN, input_down)
buttons.register(LEFT, input_left)
buttons.register(RIGHT, input_right)
buttons.register(A, input_A)

rgb.brightness(brightness)
rgb.background(colors[color])