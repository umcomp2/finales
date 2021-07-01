# Python program to implement client side of chat room.
import socket
import select
import sys


class Cliente: #El cliente no necesita crear ningun hilo, el cliente se conecta al servidor a travez del socket.

    def __init__(self, host='localhost', port=4000):
        self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        IP_address = input('Ingresar ip del servidor (default: localhost): ') or host #Si estoy conectado en la misma red pero en distintas computadoras, deberia ingresar la IP del servidor.
        Port = 4000
        self.server.connect((IP_address, Port))
        self.name=input('Ingresá tu email o tu nick: ')
        self.enviar_login()
        self.empezar_chat()

    def enviar_login(self):
        self.send_msg(self.name + "&login")

    def enviar_logoff(self):
        self.send_msg(self.name + "&logoff")

    def send_msg(self, mensaje):
        self.server.send(mensaje.encode('utf-8'))

    def empezar_chat(self):
        while True:
            sockets_list = [sys.stdin, self.server]
            #Estar escuchando todo lo que ingresas por teclado o escuchar el servidor, a travez del Select.
            read_sockets,write_socket, error_socket = select.select(sockets_list,[],[]) #Este select es el que se usa para saber que hago (Interrupcion por teclado o mensaje del servidor).

            for socks in read_sockets:
                if socks == self.server: #Socket del server
                    message = socks.recv(2048)
                    print(message.decode('utf-8')) #Imprimo en pantalla lo que me enviaron
                    sys.stdout.write("-> ")
                    sys.stdout.flush()

                else:
                    message = sys.stdin.readline() #Socket del teclado
                    self.procesar_mensaje(message) #(logoff/exit) o envia mensaje al servidor.
                    sys.stdout.write("-> ")
                    sys.stdout.flush()
        self.server.close()

    def procesar_mensaje(self, mensaje):
        if mensaje == 'exit\n':
            self.enviar_logoff()
            self.server.close()
            sys.exit()
        else:
            self.send_msg(mensaje)

if __name__ == "__main__":
    print("************INFORMACION*****************")
    print("'match': Trae el alumno asignado.")
    print("'list': Lista de los alumnos conectados.")
    print("'exit': Sale de la aplicacion.")
    print("****************************************")
    Cliente() #Se instancia al cliente.