import network
import json
from time import sleep

class wifi():
    def __init__(self):
        self.wlan = network.WLAN(network.STA_IF)
        self.wlan.active(True)
        self.load()

    def save(self):
        with open("ssids.json", "w") as j:
            json.dump(sefl.ssids, j)

    def load(self):
        try:
            with open("ssids.json", "r") as j:
                self.ssids = json.load(j)
        except:
            self.ssids = dict()

    def autoconnect(self):
        res = self.wlan.scan()
        for item in res:
            ssid = item[0].decode()
            if ssid in self.ssids:
                self.wlan.connect(ssid, self.ssids[ssid])
    
    def connect(self, ssid, password):
        self.wlan.disconnect()
        self.wlan.connect(ssid, password)
        for i in range(0, 10):
            if self.wlan.isconnected():
                self.ssids[ssid] = password
                self.save()
                return
            sleep(1)
    
    def scan(self):
        return [ap[0].decode() for ap in self.wlan.scan()]


WIFI = wifi()