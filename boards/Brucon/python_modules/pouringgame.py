import fluidsim, valuestore, appconfig, rsa, binascii, time, machine

rsa_n, rsa_d = (5761068715642095319418692770738575717788571182868274601949, 65537)

accepted_list = []
config = appconfig.get('pouringgame', {
    'colour': 0xffbf00,
    'droplets_encrypted': [b'22d0c2b2c2831ccc8eada2220a68a1d0cadb8f405257ed0b',
                           b'58f2a413187eb9a8d8f110a4805f227d9c8445a2e39fa49b',
                           b'ceda2fab2451a5131dd42d31e078311afca24cbd53a43bdb',
                           b'1ee81d451a439979a87107c67dee251d28f7f41fd99bd449',
                           b'9646fc30de72477a983671f5a7440a6057b72c8fbbedd612',
                           b'7f0bbe15c34d414c7640252144358a35f5af498bc50548ad',
                           b'a91c0378c129bfbd98598793383ffa4e742536c9d5876451',
                           b'92e1d85ee531cbfeb6cd24fa8e3f950f65d5dfdb442f68f6',
                           b'cf3a5fe45ec46876371629c272fb28cbb8566098317af63e',
                           b'd7ef21daa45fd1e2066127a004e2bbcbf2ec215fcec93510',
                           b'4cbeeec27b1936f62ffc5eea05fffd74f44f35f30ea4db5b',
                           b'cb90b678da84b8c9670223332382a953b87c97ace6a082df',
                           b'af8346b3e3100ee609a4863950ab9ede30c0aad745904d7b',
                           b'1bf979e2ec6c8c1afe33ca0fb5bb60423ae5cc1e781ab882',
                           b'a49e1cc4cf10d9f2af4d22fc8835ab0dd5b2681dff62a29d',
                           b'7c27dec8a1d8d70d3c1054b2e144e174c787affd9f0e8430',
                           b'9fdeea662834c0a8c22278ce60e4c2e0200c90a9d74f01bb',
                           b'1347acc652cfbd71ec068d78abff04e788d9b3cd357df7bb',
                           b'0b239c43ac210f2485491213ecec52c34d1a19b26e9e4be8',
                           b'd175eccbb9fb037660f54dbe35af651d5ff2332cdaf38e86',
                           b'4002e64db6e364c2afe634b00ece4e67b0b924fa5dbb174b',
                           b'5bfd0c847ed9c0e30c69a718fb280e4194ec697ad60a3171',
                           b'4fb1fc56a77ca92d43f53eadaf5a3e1a4976f00473a4a42d',
                           b'a4c68afb7608740dba9ef6d9e50dae88899c5b357c553983',
                           b'b4ac0aa40bf3c102b1c8f83c34c16978fe9a5dafb3e839b5',
                           b'8ad3fd2cba41a03193abeac4298945ebf0a44ef25167b657',
                           b'7f35967b35eb28bb0ca7262a7e2f78621327185cc64ba40e',
                           b'7de69c61dd0a16387f8e570751461c122a7fe2dc4d180008',
                           b'bf72be2a668546440b57fccffbecfe322d6f38d4e7975522',
                           b'a1921a103f291c9ac8911caccf172f0b0d55b2e09e605589',
                           b'ac50bb04d33f23c20de447ffd58da1bf4303a3c9712a389d',
                           b'ba496442b60779d6417e6d72aa3083f5be354563332122da',
                           b'2e1d90b49b28be6c365e884657c36603c06e1ff9a5e78b05',
                           b'0bab88771f57a99b1c05b832c4ad0828369616882259c7dc',
                           b'49b4d3fac8eb971f3ae36b1e492d76226974ab8644dd877d',
                           b'133a861cad16894ffef938759e3f203e03075eff62464726',
                           b'437999cbfdfb4b4d5324f0a568fad5ee5923b070f94d8ab1',
                           b'71d8feda219c04ff923e573a6ae4b6d65465d0aa459d1cd2',
                           b'7f4ba75dec7501a5c2df52349f7fb7c412d1ce074efc50dc',
                           b'2e0bcbdcd5818b6bb8239f4ccf830e4a6416ba0106edf0bb',
                           b'a0fa7ca2cb988e5fe8605721324b4d3245db0b43114855a6',
                           b'e07ade5bd6fe3c9fc1870715374ae2c9aebd15277adab07f',
                           b'ddd604862ed0d9510b7be1e7525f47ad7c076cbee3ada982',
                           b'5c6d25f4e4d63ae008a7abb4a1b26694e6d26cd78b487605',
                           b'b1cbb016fe4edc3930ec3420053e4b05e369cfd435e5b57e',
                           b'1e674050254ef02fed95ba559668f701e99bd3243308f9d8',
                           b'ac181edbf4201ec6562d7f43b8c99ab3942c0976fecac235',
                           b'c0c1300334df2f2f3e09b78b1434e05e28397b6c273153fa',
                           b'729a0a88b88d48e8cad249420b1ccb7b3c38b5787d7bfe43',
                           b'16e8a114d46eeace06100b09fdbdfcdad77a0baa79994a03',
                           b'b2d48cfb4616f6ebb34b88b8c221a102bf2bac9874dc8548',
                           b'76d086bd75f94310ef73946f2031e23a07458bee0b43fa1f',
                           b'7660bb9ae00b32085b2d165aaf3073cb119fb9db16f9ae9c',
                           b'2d7c70a28422f6ad76e029561c57f2f0eb854a21b40490a3',
                           b'af09eeb0fa6adb8d41c13094a2da80ca550e6173674a6fca',
                           b'36faf13294f313e1f5771b07dede8a8cef0518d16b97d4b4',
                           b'34ce0aced9b6516f516dbdc9d95f770926293d29d17d2723',
                           b'ab8ac84df967a685a755b63a4fbea937ffdf6c897fa468bb',
                           b'29afe9ac551dd11b697d67b954d57194c1635af72699b9ca',
                           b'c70a5ee1fa8e1d276ef962c58d91610b3dd4b0edf7276917',
                           b'e37612e08388466636fcdad040cf47b7f5e83a487c359d45',
                           b'3b56e9d7992142ea126fc39c5d9272cf4f4ff08de224114b',
                           b'c122cc80d4db2776c0ab266c7356c6c827493fbbb7c6f543',
                           b'a6407f0779452192fa5eb301d8c87890d705076aae063307',
                           b'ce1f36635ba91c8e798351a6b7c27a9d6af7ade847b6773d',
                           b'81681f742d224fba10dc5dbcf323a5711f3fd6ce7035ea9c',
                           b'8a9846b0c8d0aad3590c150f22d7627fb92753bf8b284bf6',
                           b'2b54e0ac2e3cbe41123598fb096a5ac4409e54769a5eae7d',
                           b'1d8c5166b5f26c0107fdcbf8dad6ddc4bdbc958be535a290',
                           b'952cad8b942e31130ca2257d45d2e761c8da20776962eb10',
                           b'8a4110a82bcf8749978e0a93429632cebc6de6487b65d9e6',
                           b'92ad70c4bbe4f6942d85af4b7fbfc408861a7c2ee4c1a015',
                           b'b617b76a54ee930340d795c68ab233ab884cb1d8d5d03716',
                           b'2bad10d13515440b4af9e86dba0ceda0050233add9edddde',
                           b'35b6f6fd697fece35bf68c09b5d8352e80dbb086dc5a5fc5',
                           b'c6f308c7445b0247007750d08f3deec98dea7fd6cc8bd401',
                           b'a6e70d6d36f3716d2ea953cb4718fc539c1aa1927de5222c',
                           b'b48e63748f462565748dc0fbe93f57b4d619e366b987ae01',
                           b'8c26134706849e41e9399b40e56d37e873ec03c46c77a8ed',
                           b'57704199c6c0780f641b1dc1b4a67477556303c352ffd845',
                           b'd1c59df6cd99677a5c9fc21a0bfd92f3e296cdb60a1d6356',
                           b'311d015e56050133ed32c30e11d66d76429208175849422f',
                           b'39c4eb209c2d27f91b682076be064a90a32e73dfcf187bca',
                           b'19bd53b551322be6e100a3e865a133594a7e7a5dab1c9dab',
                           b'45948a7d424e6cae819aa2df9b1f8a226b7cb2c07cdf7410',
                           b'5e11cbd922593f867baa7c13f1f6f3366b33f09de9edab5d',
                           b'c5ed18c4a5dac1b72395f768cb60f9fb3ae96547082a1619',
                           b'507a81683b83b0a0208f50bf970bd00bc3a9c99a33da7111',
                           b'e32ece0d8447057f0792bbe9971c125cd6cbc9d669b8e1a5',
                           b'444f4803ad98247f2a13571e48cd098e35d7b54dfcf54550',
                           b'579908f4b1f19887d4c4ca8ffa0082ebaa6adeec1535d22f',
                           b'28f39d5ebdc0bd01d8c65b1365cc0779e7f4e64f43c211dc',
                           b'1e41f478790bf2df0dcdb89d4b1b0679bf47d8eea06ff3d6',
                           b'b8095cc4114eb73d5ad13e4093d81957943a90090d444398',
                           b'95419cf214f5091e00e8aea506425520ffb79375143a9f37',
                           b'6bdac0231c02ea4b21fa42082fbe0d2cbe2e1863534ceac0',
                           b'9d1ebbcddddf46269789befecf026abd49577706717d855a',
                           b'0fc289caa7c9e227e42c5f3e236fd400f1badfd5f64788c0',
                           b'02a35aa8884ef5a93059f8bdd2a543aeb93e4c5a35436216',
                           b'1f60159d89da03918a3fa02b9a13219561ab8ba87e0d84d4',
                           b'621b3cebde18b80842c90b1ac7c90aef9d0e60384815e074',
                           b'a096b932bc2caf38797a487fac2cbf93ff89583070930906',
                           b'c8875db7812cbfcf72d5951e250cb5cd145cb4d0a6a35ecb',
                           b'5692856772be19a5053d5d7b46796bae59da26e9b8289bcc',
                           b'3397f93cf3bfc961e8ed8962328f6a2b64c37a204b28f3b8',
                           b'9e097f7fa284849ce3d1ecb39363c1b66490c1f64d7a4f2a',
                           b'7b5670d0e9d3c3a29858321707aad942d67c1908b2b91e0d',
                           b'415742eac6026666c8db2caaf577c8c06b0daf1c0d6e3df7',
                           b'314630fc095f883f99249eeb23c1545ee366573d07e9ab5f',
                           b'44592435e32af14314e7b22fc0459fae1d65b05799b1adea',
                           b'0789c5de7b277b36d49926e25a5c8c822a9824f182c242f5',
                           b'dfcdc6f9f7be01549a642cdbff52f5b8d37d8da321be89d6',
                           b'235fc19d7e6aa12698de11c780afca06452f160f379805d5',
                           b'6daf7d5bfca7f5549a5bb7c97b809d5a1d21c67b3dc4589d',
                           b'1e9062ea5af373ae436b8f864fc132ef99129c258463c00f',
                           b'564b6154100b72c03be6be2766274b2983b5915d1a013cd4',
                           b'4656677ffe038576ea8bd893e8f83e04ec2bd242a9dc1f96',
                           b'71a169e56c67270464c91c54c30eea3a1abf72b56584b89b',
                           b'60c669bcfbf9d799b16470c7ac3da06d6c49b55f2d93cda9',
                           b'cf53ca2bb581a99d37818374f214585bad491d9598906251',
                           b'e69089f1f4a2f8e5a1246571470f2ac219dfb7d9a073d14d',
                           b'a92a95d4e618ef0521e6d194d15107b59f6cef4df3666136',
                           b'459e65cfb6bf444221279c2e637818318eaf903793385dff',
                           b'2a9d1937c385cea0f67bd4798d3e4151758a3befe667b7ae',
                           b'0c050a4193b2aa4e4ce5a427574e3bc3fdd2b26396039f2a',
                           b'd4433d63ab0ea6d532ba72cae380004a5c1283508f2c3547',
                           b'90b63f6ba4ec0f580004b860f0fa6ff799bf47be144c7469',
                           b'c92090d9938041d8012f058f297c52512ad7cc8e4ec26f15',
                           b'128e59bcec0ff18fffaa05a771fd7e34cbbe5c14c67cce10',
                           b'879fd5ad3009fafd0694f8aa5686e476a8f0b056bedbdc88',
                           b'3ec6513939cbcf74e5ca239b5576784a0429ee9de2ab9db5',
                           b'bca5c1496a6ee2e8c6dbba3fa89dd6826e2f94eaa4cd594a',
                           b'110f47789b635f9cb6ca38366b23f89af4c386fdf18fe7e7',
                           b'8af4af1817bf6256c7908dacb3cba7a96caab732b1f167d4',
                           b'e9f80fb0d78d6f09990b1fd949b08776fe3f3f6b11bf5d65',
                           b'616e810aba2f9e03f1be9b44799153162d89b91f945953bb',
                           b'5266d35d54c387e0907b9762bdca5ed8573c76d94f911f0f',
                           b'45c68007b132d527c986c7455d6ad7f05b7108897f086307',
                           b'9442bfaeda1a21187bdc003c14750014c2a4f5a44d790ab0',
                           b'ba114c247300a9cf01e7ee2fbcbb4fd9d2be2e46aeb9b328',
                           b'aa53e03d5f83501783bb77fd90773600e36b638c1fdaaef8',
                           b'046faf5cdce6f34fa90888b91435a8971d102e6ffcee6027',
                           b'c1d7478735b10a75e80f6d1f91b73646743349cd8c875675',
                           b'9c9bd9456892ab2d2fd662eeba744073f82f0bb2358f8f6b',
                           b'455e3315f091126513668300d659ade8e8492553e0fa5fcf',
                           b'7d6b2fbf6634460eace7c2841e175ea1fd4309b8082b3d33',
                           b'd078e5a1d262850581257563157a4ded7113ddb631f10222',
                           b'995c9b85eb9b3b72379b36895643cac17e7d594a0deaca00',
                           b'8c1be5f4f6c2334f3887d034ab3e73d4b7035c288a070146',
                           b'2cd3feb0afb0537106293f89d001ac73f41f5cdf96c489c6']
})


def is_legit_droplet(ciphertext):
    plaintext = rsa.decrypt(rsa_n, rsa_d, binascii.unhexlify(ciphertext))
    return 'bru' in plaintext


def check_config():
    global config
    accepted_list = []
    for droplet in config['droplets_encrypted']:
        plaintext = rsa.decrypt(rsa_n, rsa_d, binascii.unhexlify(droplet))
        if is_legit_droplet(ciphertext=droplet):
            accepted_list.append(droplet)
        else:
            print(f"Refused RSA-encrypted droplet '{droplet}' > droplet signature doesn't contain 'bru'")
    config['droplets_encrypted'] = accepted_list.copy()


fluidsim.begin(len(config['droplets_encrypted']), config['colour'])

serial = machine.UART(0, 115200, timeout=10, timeout_char=10)
last_written_config = time.time()
config_changed = False

while True:
    data = serial.read(24)
    if data is not None and len(data) == 24:
        ciphertext = binascii.hexlify(data)
        if is_legit_droplet(ciphertext):
            serial.writeline("Thanks!")
            print("You got a droplet from a friendly neighbour!")
            config['droplets_encrypted'].append(ciphertext)

    if fluidsim.has_particle(4, 0):
        ciphertext = config['droplets_encrypted'][-1]
        serial.write(binascii.unhexlify(ciphertext))
        time.sleep(0.2)
        reply = serial.readline()
        if reply is not None and 'Thanks' in reply:
            print("Sent droplet to a friendly neighbour!")
            config['droplets_encrypted'].pop()

    if config_changed and (time.time() - last_written_config) >= 10:
        valuestore.save('app', 'pouringgame', config)
