import rgb, valuestore, appconfig, rsa, binascii, time, machine

rsa_n, rsa_d = (3181043203283898343167976280123643027884448147992670247681, 65537)

accepted_list = []
config = appconfig.get('pouringdump', {
    'droplets_count': 0
})


def is_legit_droplet(ciphertext):
    try:
        plaintext = rsa.decrypt(rsa_n, rsa_d, binascii.unhexlify(ciphertext))
        return 'bru' in plaintext
    except:
        return False


def check_config():
    pass

serial = machine.UART(0, 115200, timeout=10, timeout_char=10, tx=43, rx=44)
txpin = machine.Pin(43, machine.Pin.IN, machine.Pin.PULL_UP)

serial.read()  # Clear entire receive buffer
last_written_config = time.time()
config_changed = False
def tx_mode():
    global serial
    serial.init(baudrate=115200, tx=43, rx=44)

def rx_mode():
    global serial
    txpin.init(txpin.IN, txpin.PULL_UP)


rgb.background((50,0,0))
while True:
    rgb.clear()
    rgb.text(f"{config['droplets_count']}")
    data = serial.read()
    if data is not None:
        if len(data) == 24:
            tx_mode()
            serial.write("Thanks!\n")
            serial.flush()
            rx_mode()
            serial.read()  # Clear entire receive buffer
            ciphertext = binascii.hexlify(data)
            if is_legit_droplet(ciphertext):
                config['droplets_count'] += 1
                config_changed = True
            else:
                print(f"err: {ciphertext}")
                serial.read()  # Clear entire receive buffer
        else:
            print("got: ", data)
            serial.read()  # Clear entire receive buffer

    if config_changed and (time.time() - last_written_config) >= 10:
        valuestore.save('app', 'pouringdump', config)
        last_written_config = time.time()
