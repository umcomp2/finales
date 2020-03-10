import socketserver
import queue as queue
import threading
import file_funcs as ff
from startup import *
import webpage as wp

class TcpThreads(socketserver.ThreadingMixIn, socketserver.TCPServer):
    socketserver.TCPServer.allow_reuse_address = True #reuse address when the server is restarted
    pass

class ServerHandler(socketserver.BaseRequestHandler):
    def setup(self):
        pass
        #print("New connection from: ",self.client_address[0])
    def handle(self):
        self.data = self.request.recv(1024)
        #detects whether a sensor is connecting or a browser
        if 'User' in str(self.data):
            #if user is present in the request then is a browser, generate and send the html
            #if there is an id in the request plot it and return it, else return the form
            if (str(self.data).find('id=')) != -1 and 'favicon.ico' not in str(self.data):
                sensor_id = str(self.data).split('/')[1].strip('index?id= http HTTP')
                sensor_id = int(sensor_id)-1
                try:
                    wp.showplot(self,sensor_id,events[sensor_id],configuration[10]) 
                except:
                    print(f'Error plotting a sensor')
            else:
                try:
                    wp.generate_interface(self)
                except:
                    print('Error showing interface')
        else:
            #prepare the string to add to the queue
            try:
                ff.prepare(self.data,q)
            except:
                splited = str(self.data).split('/')
                splited = [i.strip('THID=') for i in splited]
                print('Error handling the lecture from sensor {}, it may be corrupted contact support'.format(splited[1]))

if __name__ == "__main__":
    
    read_conf()
    print('Configuration:',configuration)
    q = multiprocessing.Queue() 
    qa = multiprocessing.Queue() #queue for the alert process
    ff.initialize(int(configuration[9]))
    
    events = []

    for i in range(int(configuration[9])):
        events.append(multiprocessing.Event())
        events[i].set()

    startup(q,qa)
    address = (configuration[0],int(configuration[1]))
    servidor = TcpThreads(address,ServerHandler) #uses the TcpThread class then handler class
    run(servidor)

