import fluidsim, valuestore, appconfig, rsa, binascii, time, machine

rsa_n, rsa_d = (3181043203283898343167976280123643027884448147992670247681, 65537)

accepted_list = []
config = appconfig.get('pouringgame', {
    'colour': 0xffbf00,
    'droplets_encrypted': [
        b'0b0fe297a6613f0fb057892b41d5d8e6f9ac48b01c79a800',
        b'61febc6d65a2c8d075168e7f885564290b18fbe85acf9e32',
        b'6bf86b8e0db5ac957cef9c2d420a38bf18057c9e138bc788',
        b'51ff45f0dc5c8a57d9d7ef0295a17ddc402dee99843a5c08',
        b'472d93e91b63f601efb26bc2efa3d38fed0292cbd3be9a73',
        b'6ae3419b65e47142cbd3690f4d6adda6206ae91bd5b97f25',
        b'8174844b3bb959177040d469a89f32e571aba3e541d5e3f5',
        b'436f27d848a0f51db55c892ebfd1775a6d258d368ac302da',
        b'3adc64fd5362c37cb217ce99a7ca49e68f366d56802ee758',
        b'80c9a1ec64478ec1d84e13b8a377e384b8f95d9065b1caaf',
        b'05715217706498f9d90aeebc11dbac9af877229f64033a0e',
        b'4a1ea9da29b04f53198f63516bfa9e52d23c7603c7589a9f',
        b'0e6b6bf698392dd88075b302069bc2977c8bec016673734c',
        b'759b4a9d86f9caaf2e644c08c573fe1d7a7558dbb1faf979',
        b'17b6b8dc1370352054a71a90ad96c7a6d117ff927de608e7',
        b'3a767358ac355faf8909bd3b9d79dcf636e52aa97cacbaaf',
        b'5cb6e1627e258fd2d8a867101303b90cc75d907e50c451db',
        b'124df818572fc0ea7bad9d9c7222368458bb9160e8dce213',
        b'4a05e048fc11944706b2b60dc6ecf1d892983e1317ae4a14',
        b'4fcdda3e4890990eefc1c3f335f782e2ce88598efbcbb82e',
        b'422fb121b4be63fe1dbb412cac892ed09a45a672ab5a2514',
        b'3267f1b35777087ffdd6157eb5eba02a2589fda0375c0600',
        b'4c1999830b5b6748ff422f6f7ac5e839e5c8a9e46c95486f',
        b'3c90a4e4d482d41a89eb1684ba511201918bb69d06319285',
        b'7e4266f763df1750f6323bfa9ebfe1c2a2ecb209e0e59055',
        b'43e8b8e85a1465cc3fac82c461e9f2bbd869bed56931bdba',
        b'5e22173fc2b29b8ffcde9524f4d98b1e00018a2b2f93031a',
        b'1498f950de51bfe073bcac4ffd0e3c53d56830bbf5eb4816',
        b'208c52a41c15ce1bb46ddab9b590002137eef7e308a7b6e0',
        b'1b9004c1b3d1cf4d585405c39a83bbe5b6ba8d32f088c77b',
        b'00b723052861daa3e0dbb688b324ae08e7c7500cd722e911',
        b'761810f28a737d65cf0b58f2b402c5592c368121ac97dead',
        b'737a1c1653c782ab55c4f7403f222ea019ea8219b2e39043',
        b'5ef2f0f79f9b538c430f559e09ab527548f675648da094c9',
        b'0dcef6f5bb866761c352eadf2cffecd59686f75317782f51',
        b'097b827786a2deed0e658ca08c5e78bfd49d3ea1e501d793',
        b'33f7e06f3c7e698798ba648aaaaf221b0abc8a303f6d7652',
        b'24bf9e2f4d8a4f5f0b2149e45ccaf3aff5fd569647433fd9',
        b'283f64483ce5484843fbc5d984550f4bde29dfb33a174351',
        b'26cba95f3413ae57836143ba3342b128d9e20c2698d86aeb',
        b'3b8c3701e68ed572e20565f6b0c67eaf1364f9c9c3dca0ad',
        b'63083543b3463867c3051a5951dc095184eb8f86ee3f0f1a',
        b'6a80c641e1d5276b06396a32f5270a8753cb6d8e7a052fea',
        b'3e07f23128b7f0e4f0af9cf01b452a4d87717c7e97aaec47',
        b'1a80bf6b5c81ce37caeeec87a1ad448e6358b83e472d7fd7',
        b'523e4eff32ac591cbb7d96ce7da4f0a3ada365258a1af0bd',
        b'18ab0822765b2ac076f69eff514435c7c46f078037ad2558',
        b'34078b235ebb423eba9ce81ba717b547c273fe1ab8214cb1',
        b'64412bb5357a8b74f46d64cdea073fe76319e755001f0c04',
        b'179502461e7a32738c950279e90e2d84b324a11ddea5ebda',
        b'17269139e1b8ac6996e28fdf560759446763fb776e2eb614',
        b'5e8315a47eb5baf6753e3b1cf185562fb1bb0ab3370d36bf',
        b'5eef79c01e544d8d74713bc3b77a7f7d3c806051b35187bf',
        b'4af155ca3909907d4baf2927a5366cfa66ce42a927713051',
        b'0066e786de8911c76b7c99947fa9f5a1690ddac695f5939a',
        b'144b29b93bc317308da46f56ca231f5113c7bce118b1f1ea',
        b'3a200fd0e2d48c7a573435196cf1f34bddbd9d23dca8ee7c',
        b'10dba203707152980e488fa11833e5b3cf714f87b2499699',
        b'3d93dc4397a0df3ed0d48b3d35fe777ffc09156a78043d5a',
        b'63ed412a26ec4172d75619d33d21ef6d5c7a9fdf430f17ba',
        b'271f699e29b9a8c8b7b0aa8e6a4c2d6b5442f2f789d2064c',
        b'7420ecf26590945963438d30f20d342b207c56c15883652f',
        b'4dcfdb08b206c0f8183d97ac8227afd84d181022c9ea916e',
        b'0d7a6c27ea337d4ab64a764dceeb496de17724cb4c8eeb44',
        b'582607b650c2f27ddab94795baaf66404551cb0f7e75c385',
        b'1eeef4647fd5f10183eb861248f2fc68d1a3653d02d43a36',
        b'23f018a591dd7efb610318cdf464bde8dc45a887cb482371',
        b'250b7d82f9102a92b2e89a4a87c5cc6b467d098f8d3b6251',
        b'349abb380c9d98875f01aceb4f2fe538158c527f90974b12',
        b'6d2e7af35ec16a20e25921d7bed935c0f4ec4b67499314cb',
        b'269c541d4d261bc3b49f4c2bc3725f2a2482505ea4e6d6f6',
        b'05e6ac329a3eeab0a119b2628a09d48235a5483c51655130',
        b'75841b182b0e6a63533958f24c917013f4ede6a43f271976',
        b'59af2c572ef900a73054153b585ea7a485688c79874d332b',
        b'6d5378c03b1153fa529807b37fc0def63dddf1328cb3f569',
        b'1f53162b951649c76b47eb77a059c7e25dc12a9d877df2f6',
        b'3061d847f44c648cadb72549235a81f147b09b339ef034d4',
        b'5079a024a572f49c0826efdbdc76225a679baa088a34c3d8',
        b'232dfe1d1119dc869f00fdf494c28cb058dec550f35075af',
        b'5927aedfbb61b01df7433bf85775bfbc19c2a1e2a35859f3',
        b'70a287e57de1ff142f71a30265cd66e21c3a1030974d1eba',
        b'016258a1dc0b9935511d6b2d8de3075272ab679a4549d682',
        b'1898bfb613998701c899f7cd2298d03e8c7c3bb2f7cfe643',
        b'7eb666835fc33b320945c727f9227334ad87d15a0efc27f1',
        b'140a0d7a4c555f1aa83a54e365d3ce2d14c80871aee987dd',
        b'4cec2df8dbb6d954c718ad4beeb0cf39bf5d68df2fa85c32',
        b'513ad8f7ca5a5d6a8945c1c1f4771a93ef7865ac464ac4e3',
        b'788404c7e12adff4e141ab2339fe66d3203de8a1e5b68325',
        b'4d1d8226ba869b7c095e77cb10633a08a2df071e780dd591',
        b'27f6dd8033bc9f74f73fff6adb0095c166e20e5c9d14638d',
        b'1b0fadd9762e9188148a13fbc8fce3c8d473b901fd0028a0',
        b'1cb9c62fee1bcfa100c5e2bb5654b0c4344edad966eab5af',
        b'1691737172e134ca797f2df3e57c607eac30fc015f170d37',
        b'68853f7dff8c1bdcb3c4bfed0b483fd5e29108cf274455ef',
        b'16781e2df05e6e6d3b1842682e1798e699fe0791da8382ec',
        b'14b12533cd4df3216b3d202390b16453ca69e466f91e09aa',
        b'0045ff2f50a921763cde471b431df1afab4202854d6dbe15',
        b'25c0e6e59f3c29e7fec1105d075770e52dd740a39820e45f',
        b'6bbe22f1bc5be0ae9c0ebe461c52e65a6fc6a2e7092cf280',
        b'5dda266707a371a0633d8fcb38ee84c0a29790de4f27a01b',
        b'4c137d61b175ecfe2191837fa19edb36de4792baeee65425',
        b'7b2141377204fff0d009f7460d90dd4f0d058a31723ca7e2',
        b'4923fffadbcefa8f2323372e8bd5c7f33bb124750bed8ed8',
        b'1d4b2eebbec8694435f525a5ced245e7813b8e8ac8879a21',
        b'41c14177d1894dc15b9135071ea3a574f902cc546ab951e1',
        b'73673bcec5a746ddfcd41a87201e01b175c572fbdec4c091',
        b'234f56bafc76107124131d871f149523c2b78c945d388615',
        b'5648cf089866a11713d0f7dba4c2afe5ff8639f4f8e36101',
        b'6717be22fc1f4275285c58ae1982f860facf4c5481820c24',
        b'3380884db7b11daa008ca31e7b75466a78dd8e32f0b20477',
        b'0e97f1fadb7eee86c48734df20074b57e7a2dcd934fc7ea3',
        b'5e100bac2be41b03af80997e34779802b4496fb0d39664ca',
        b'68b01adde5695e62a2af735f4272d57eb06243cce50c7c47',
        b'21df80acee01cd0ad4c39fbb4a3debddb0f93fff27b4d6ad',
        b'5d92b83d7cde7eba160f5496bfb03ad124c92ac471e955ab',
        b'78c6a7ba1ba1baa9a5d1c1d1b3de183e9b05214fc603a8bb',
        b'333dbf476c34858192125caae765f45793ed153191658ad0',
        b'28c01ea8f874e55ae65b6ca8233a00e3bce4a3250e7f16d2',
        b'4787d12903d3edf82a8bf7bfc9c6541a8978c7ebdc7eb09f',
        b'148415df1708df6c7f853b8aecef051b8d46fc6dddfdf35b',
        b'3803cfa7eb35605515a5a26df79be0f382440d7fd65e62af',
        b'4dd35cd242e142a83910e834f5a2553753dd54aab88e3a8b',
        b'10c9b5a8c4e4f0a1cebede17a44f9b7f23d04ccd66b395f8',
        b'3bda09420621ac154cc96682bd1e73ee0e57bdace5aa3432',
        b'1bb9e9a5275d0b4107c7e397a535339f511bc2ed167bc9ae',
        b'12b42c395a6b55e68cfa755e208da63c041e1dd95ff95c0a',
        b'742d1a54f746a9206409ddd745fe7895ba48198b088f77ce',
        b'57905da766a627759da49f9e509f449d795d4915817f3d28',
        b'5961297b9bbfbe61dcf318b57e0ce8b4ba06d2b70dfd5328',
        b'640cb113efb0219cebf86326e8e9bf5b188fd568821ff9fb',
        b'6c96368ee12316437f4cedfb9905b8518f0ac923817db998',
        b'659d1d354fbfe2916d238ab374801bdb92d3769bf184ce59',
        b'0d9742df86d2087605b77175e6824aa87143d1a27df6809f',
        b'28975ca58c275cde16baf879dbe0986a2e050460a1deed2d',
        b'13d84dd592b6dd6c747eb7e2bf707463530c116292d22658',
        b'193d74d09e1f19f26ae527b8e499814eb9b62746062f6eab',
        b'26a444636335c8f9c9789b093e45400a091d36d04b9adcca',
        b'20f64f1db1d30383af4776c6b2d522b3939fa69219a28262',
        b'2e3c316e3f8016d55c53ab29a73a960c83e4682d4078c257',
        b'7df9162d81c5d9e884b64c8af5c691b8a2909677be349bab',
        b'57e67d98d36f831f11a25c265073809d8770c692df72440f',
        b'4bb8a4ca4f2fa0eb22e1890b7ba9d09ad169e7a99add4dd9',
        b'7385c86ead2a08e8a90c3bfde6e5c50d1c1cf08ecbbd51b8',
        b'1f58518ad74d3a841f18be7e9194eb63334300758a5b57b3',
        b'0fccaa47f34e0d971f82b57c618978e14f3cc880168e65c6',
        b'346db65f5134063bd9a7f738362d7b1f35c8c4bc2af644a9',
        b'7c3422cbbe0e79c71a383412dfc1b7700213affd33a80294',
        b'5271648bc65bcfa394b262f4eff4cffe84c9092c54366b05',
        b'6ce40d49f0abb37f4ea67a3489a53968e119bd6db015bcf6',
        b'57f6293a424b78c546b44fa3ce57caca063afac6e9645777'
    ]
})


def is_legit_droplet(ciphertext):
    try:
        plaintext = rsa.decrypt(rsa_n, rsa_d, binascii.unhexlify(ciphertext))
        return 'bru' in plaintext
    except:
        return False


def check_config():
    global config
    accepted_list = []
    if 'droplets_encrypted' not in config or config['droplets_encrypted'] is None or len(config['droplets_encrypted']) < 1:
        config['droplets_encrypted'] = []
    for droplet in config['droplets_encrypted']:
        if is_legit_droplet(ciphertext=droplet):
            accepted_list.append(droplet)
        else:
            print(f"Refused RSA-encrypted droplet '{droplet}' > droplet signature doesn't contain 'bru'")
    config['droplets_encrypted'] = accepted_list.copy()


fluidsim.begin(len(config['droplets_encrypted']), config['colour'])

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


while True:
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
                print("You got a droplet from a friendly neighbour! ")
                config['droplets_encrypted'].append(ciphertext)
                fluidsim.push_particle()
                config_changed = True
            else:
                print(f"err: {ciphertext}")
                serial.read()  # Clear entire receive buffer
        else:
            print("got: ", data)
            serial.read()  # Clear entire receive buffer

    if fluidsim.has_particle(4, 0):
        ciphertext = config['droplets_encrypted'][-1]
        packet = binascii.unhexlify(ciphertext)
        tx_mode()
        serial.write(packet)
        serial.flush()
        rx_mode()

        tries = 3
        got_thanks = False
        while tries > 0 and not got_thanks:
            line = serial.read()
            if line is not None and 'Thanks!' in line:
                got_thanks = True
                print(f'Took {4-tries} tries')
            else:
                tries -= 1
                # time.sleep(0.1)

        if got_thanks:
            print("Sent droplet to a friendly neighbour!")
            config['droplets_encrypted'].pop()
            fluidsim.pop_particle()
            config_changed = True
        else:
            print('Didn\'t get thanks')

        serial.read()  # Clear entire receive buffer

    if config_changed and (time.time() - last_written_config) >= 10:
        valuestore.save('app', 'pouringgame', config)
        last_written_config = time.time()
