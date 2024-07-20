import uinterface, system, nvs, time
import rgb, buttons

nickname = nvs.get_str("system", 'nickname')
should_set = False

def set_nickname():
    rgb.clear()
    uinterface.skippabletext('Set nickname:')
    new_name = uinterface.text_input()
    if new_name is not None and new_name != '':
        nvs.set_int("system", 'nickname', new_name)
    system.start('nickname')


def render():
    rgb.clear()
    rgb.scrolltext(nickname)

def reset_nickname(_):
    global should_set
    should_set = True

if nickname is None or nickname == '':
    should_set = True

buttons.register(buttons.BTN_A, reset_nickname)

if not should_set:
    render()

# Because we can't use uinterface from within interrupt handlers
while True:
    if should_set:
        set_nickname()
        render()
        should_set = False
    time.sleep(0.1)