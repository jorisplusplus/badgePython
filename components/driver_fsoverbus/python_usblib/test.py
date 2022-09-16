from fsob import fsob, fsob_message
from time import sleep

def callbk(arg):
    print(arg.response)

dev = fsob()
dev.start()

dev.sendMessage(fsob_message(1, None, callbk, callbk))
dev.sendMessage(fsob_message(1, None, callbk, callbk))
dev.sendMessage(fsob_message(4096, "/flash".encode("ascii")+b"\00", callbk, callbk))
input()
dev.stop()