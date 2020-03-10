import socket
import time
import random
import multiprocessing

def bombit():
    while True:
        desc= socket.socket(family=socket.AF_INET,type=socket.SOCK_STREAM)
        desc.connect(("192.168.1.51",8080))
        string = "GET /ID="+str(random.randint(0,2))+"/T=" + str(random.uniform(10, 34)) + "/H=" + str(random.uniform(0, 50)) + "/ HTTP/1.1\r\n"
        desc.sendall(str.encode(string))
        time.sleep(random.uniform(0.005,0.008))
        desc.close()

p = []

for i in range(10):
    p.append( multiprocessing.Process(target=bombit))
    p[i].start()
