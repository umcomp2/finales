#aathis submodule contains the reader/writter/prepare functions for main.py (server)
from datetime import datetime
import multiprocessing
import re
from startup import *
from tasks import *

def initialize(nof):
    manager = multiprocessing.Manager() 
    global temperature 
    temperature = manager.list() #manager is for global shared list (or other objects) between processes
    temperature = [0]*nof
    global count
    count = manager.list()
    count = [0]*nof
    global humidity
    humidity = manager.list() 
    humidity = [0]*nof

def writer(q,qa):
    buff = [] #this would be a buffer to save a certain amounts of lectures until you open the file and write, it's a performance test
    read = []
    limit = int(configuration[3]) #this limit should be taken from the conf file
    while True:
        if not q.empty():
            read = q.get()

            temperature[int(read[0])-1] = temperature[int(read[0])-1] + float(read[1])
            humidity[int(read[0])-1] = humidity[int(read[0])-1] + float(read[2])
            count[int(int(read[0])-1)] = int(count[int(int(read[0])-1)]) 

            if float(read[1]) >= limit: #alarm
                print('--Sending Alert--')
                send_email.delay(configuration[6],configuration[7],configuration[8],read)

            buff.append(read)
            if len(buff) >= int(configuration[4]): #block and wrtie the file
                with open (configuration[2], 'a') as lectures:
                    print('-.-.-writing in file-.-.-')
                    for el in buff:
                       lectures.write(str(el)+'\n')
                buff = []

def realtime(sensor_id):
    avg_temperature = 0
    avg_humidity = 0

    avg_temperature = round(temperature[int(sensor_id)-1]/count[int(sensor_id)]-1,2)
    avg_humidity = round(humidity[int(sensor_id)-1]/count[int(sensor_id)]-1,2)

    return avg_temperature,avg_humidity;

def prepare(data,q):
    timestamp = datetime.now().strftime("%d-%m-%Y %H:%M:%S")
    splited = str(data).split('/')
    splited = [i.strip('THID=') for i in splited]
    splited = splited[1:4]
    splited.append(timestamp)
    q.put(splited)
   
def average():
    avg_temperature = []
    avg_humidity = []
    for i in range(len(temperature)):
        avg_temperature.append(round(temperature[i]/(count[i]),2))
        avg_humidity.append(round(humidity[i]/(count[i]),2))

    print('--------------------------------------')
    print('Average Temp:{}\nAverage Hum:{}\n--------------------------------------'.format(avg_temperature,avg_humidity))
    return avg_temperature,avg_humidity;

def reader_full():
    process_me = []

    with open (configuration[2], 'r') as lectures:
       for line in lectures:
           process_me.append(line)
    
    pool = multiprocessing.Pool(multiprocessing.cpu_count()*2)
    for element in process_me:
        pool.apply_async(calculate, args=(element,), callback=finish)

    pool.close()
    pool.join()
    
    print('--------------------------------------')
    print('Temperatures:',temperature)
    print('Humidities:',humidity)
    print('Lectures:',count)

def calculate(string):
    data_per_line = []
    string = string.split(',')
    index = int(string[0].strip("'['"))
    local_temperature = float(re.sub(r'[\']', '',string[1]).strip("'[']'"))#replace non alfanumeric values with spaces then strip them
    local_humidity = float(re.sub(r'[\']', '',string[2]).strip("'[']'"))
    data_per_line.append(index)
    data_per_line.append(local_temperature)
    data_per_line.append(local_humidity)
    return data_per_line

def finish(processed):
    temperature[processed[0]-1] = temperature[processed[0]-1] + processed[1]
    humidity[processed[0]-1] = humidity[processed[0]-1] + processed[2]
    count[processed[0]-1] += 1
    

