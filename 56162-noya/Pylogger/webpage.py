#generates the html with the values
import file_funcs as ff
import matplotlib.pyplot as plt
import numpy as np
import os
import time
from datetime import datetime

def generate_interface(self):
    print('Generating interface for{}'.format(self.client_address))
    self.request.sendall(str.encode("HTTP/1.1 200 OK\n",'iso-8859-1'))
    self.request.sendall(str.encode('Content-Type: text/html\n', 'iso-8859-1'))
    self.request.send(str.encode('\r\n'))
    with open ('index.html','r') as index:
        for l in index:
            self.request.sendall(str.encode(""+l+"", 'iso-8859-1'))

def showplot(self,sensor_id,event_id,sampling_time):
    temp,hum = ff.realtime(sensor_id)

    name = 'plots/sensor'+str(sensor_id)+'.png'

    if event_id.is_set():
        event_id.clear()
        if get_date(name,sampling_time):
            plot(temp,hum,sensor_id)
        event_id.set()
    event_id.wait()


    with open(name, 'rb') as f1:
        data = f1.read()
        HTTP_RESPONSE = b'\r\n'.join([
        b"HTTP/1.1 200 OK",
        b"Connection: close",
        b"Content-Type: image/jpg",
        bytes("Content-Length: %s" % len(data) ,'utf-8'),
        b'', data 
        ] )
        self.request.sendall(HTTP_RESPONSE)

def plot(temp,hum,i):

    print(f'Ploteando: id:{i} temp:{temp} hum:{hum}')

    name = 'plots/sensor'+str(i)+'.png'
    if os.path.exists(name): #check if the file exists and delete it  
         os.remove(name)
    fig, ax = plt.subplots()
    x_var = ['Temperature','Humidity']
    plt.xlabel('Variable')
    plt.ylabel('C° , hum%')
    height = [temp,hum]
    y_pos = [0,1] 
    plt.bar(y_pos, height, width=0.4)
    plt.xticks(y_pos, x_var)
    plt.savefig(name)
    plt.cla()
    plt.close(fig)

def get_date(name,sampling_time):
    time_now = datetime.now().strftime("%H:%M:%S").split(':')
    file_time = time.ctime(os.path.getctime(name)).split()[3].split(':')
    delta_time = ((int(time_now[0])*60)+int(time_now[1])+(int(time_now[2])/60))-((int(file_time[0])*60)+int(file_time[1])+(int(file_time[2])/60))
    if delta_time >= int(sampling_time) or delta_time<0:
        return True
    return False
