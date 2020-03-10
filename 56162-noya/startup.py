#settings for the server when it starts
import multiprocessing
import file_funcs as ff
import alert as al
import os
import sys

m = multiprocessing.Manager()
configuration = m.list() #manager is for global shared list (or other objects) between processes

def startup(q,qa):
    if not os.path.exists(configuration[2]): #check if the file exists and create if it doesn't  
        os.mknod(configuration[2])
    try:
        ff.reader_full() #read the lectures and prepare for averages
        ff.average()
        w = multiprocessing.Process(target=ff.writer,  args=(q,qa,)) #cretes and start writter process
        w.start()
        print('Writter Process ... OK')
        if configuration[5] == '1':
            alert = multiprocessing.Process(target=al.sendmail, args=(qa, configuration,)) #creates and start the alert process
            alert.start()
            print('Alert Process ... OK')

    except:
        print('Writter Process ... Failed')
        print('-Warning, no records will be held from this point-')

def run(servidor):
    try:
        print('Server Process ... OK')
        servidor.serve_forever() #serve until ctrl+c (or other) is passed
    except KeyboardInterrupt:
        servidor.shutdown()
        servidor.socket.close()

def read_conf():
    try:
        with open ('config.txt', 'r') as conf_file:
            for line in conf_file:
                if line[0] != '-':
                    configuration.append(line.split('=')[1].strip('\n'))

    except:
        print('Unable to load the configuration file, shuting down the server')
        sys.exit() #terminate program with multiprocessing
        

