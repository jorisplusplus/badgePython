import buttons, rgb, nvs, system

currentState = nvs.get_int("system", 'factory_checked') or 0
if currentState >= 2:
    nvs.set_int("system", 'factory_checked', 3)
    system.home()

RED     = (255, 0, 0)
GREEN   = (0, 255, 0)
BLUE    = (0, 0, 255)
WHITE   = (255, 255, 255)
BLACK   = (0, 0, 0)

CYAN   = (0, 255, 200)
YELLOW   = (255, 255, 0)

checklist = [
    (RED,   WHITE, 10, "UP",      buttons.BTN_UP),
    (GREEN, WHITE, 4,  "DOWN",    buttons.BTN_DOWN),
    (BLUE,  WHITE, 4,  "LEFT",    buttons.BTN_LEFT),
    (BLACK, WHITE, 2,  "RIGHT",   buttons.BTN_RIGHT),
    (BLACK, WHITE, 13, "A",       buttons.BTN_A),
    (BLACK, WHITE, 13, "B",       buttons.BTN_B),
]

def next_check():
    if(len(checklist) == 0):
        rgb.clear()
        nvs.set_int("system", 'factory_checked', 2)
        import partsim
        return

    background, textcolor, x_pos, text, gpio = checklist.pop(0)
    rgb.clear()
    rgb.background(background)
    rgb.text(text, textcolor, (x_pos, 1))

    buttons.register(gpio, lambda pressed, gpio=gpio: (buttons.unassign(gpio), next_check()) if not pressed else None)

# Dim the screen so the factory checks can be done from USB only
rgb.brightness(5)
next_check()
