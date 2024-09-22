import nvs, binascii, ure, rsa, valuestore

if all([x == -1 for x in valuestore.stat(keyname='flags')]) is None:
    valuestore.save(keyname='flags', value={})

# Privkey is (171935606338639264366893831733042339201, 104341409208544193681324948524324376945)
# print("WARNING: create_flag is still present in firmware!")
# def create_flag(challenge, points):
#     n, e = (
#         171935606338639264366893831733042339201,
#         104341409208544193681324948524324376945,
#     )
#
#     return "BRUCON{%s}" % binascii.hexlify(rsa.encrypt(n, e, "%s,%d" % (challenge, points))).decode()

def parse_flag(flag):
    match = ure.match("BRUCON\{(.*)\}", flag)
    try:
        contents = match.group(1)
    except:
        return None

    n, d = (171935606338639264366893831733042339201, 65537)
    try:
        decrypted = rsa.decrypt(n, d, binascii.unhexlify(contents)).decode()
    except:
        return None
    try:
        match = ure.match("([a-z]),(\d\d\d)", decrypted)
        challenge, points = match.group(1), int(match.group(2))
    except:
        return None

    return challenge, points


def get_found_flags():
    flag_dict = valuestore.load(keyname="flags")
    found_flags = {}

    for flag_object in flag_dict.values():
        try:
            challenge, points = parse_flag(flag_object["flag"])
            found_flags[challenge] = points
        except Exception:
            pass

    return found_flags


def submit_flag(flag):
    result = parse_flag(flag)
    if result is None:
        print("This is not a correct flag!")
        return
    try:
        challenge, points = result
    except:
        print("This is not a correct flag!")
        return

    flag_dict = valuestore.load(keyname="flags")
    flag_dict[challenge] = {"points": points, "flag": flag}
    valuestore.save(keyname="flags", value=flag_dict)
    print("Successfully submitted your flag!")

    total_points = 0
    for flag_object in flag_dict.values():
        try:
            chall, pts = parse_flag(flag_object["flag"])
            total_points += pts
        except:
            pass

    if not nvs.get_int("system", "ctf_done"):
        if total_points == 1300:
            nvs.set_int("system", "ctf_done", 1)
            print(("Congratulations, you have finished all CTF challenges! Complimentary new apps have been added to your homescreen."))
            import uinterface, time, system

            time.sleep(1)
            uinterface.skippabletext("u R winrar! New apps added.")
            time.sleep(10)
            system.home()