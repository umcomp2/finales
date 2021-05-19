import argparse
import os

def importe():

    global control
    fd = os.open("server.config", os.O_RDONLY)
    body = os.read(fd, 100).decode()

    if len(body) == 0:
        return ("Vacio")
    else:
        return(body.split()[0], body.split()[1], body.split()[2])

def argumentos():
    parser = argparse.ArgumentParser(usage="\nproyecto.py [-h] -d DIR -p PUERTO -s SIZE")

    parser.add_argument('-s', '--size', metavar='SIZE', type=str, default="20000",
                        help="Bloque de escritura maxima para los archivos")

    parser.add_argument('-p', '--port', metavar='PORT', type=str,
                        help="Puerto en donde espera conexiones nuevas",
                        default="5000")

    parser.add_argument('-d', '--documentroot', metavar='DIR', type=str,
                        help="Directorio donde estan los documentos web")

    args = parser.parse_args()
    try:
        if not args.port:
            raise NameError
        if int(args.port) < 1024 or int(args.port) > 65535:
            raise NameError
    except NameError:
        print("\nDebe ingresar un puerto valido.\n")
        exit()

    try:
        if not args.documentroot or os.path.isdir(args.documentroot) is False:
            raise NameError
    except NameError:
        print("\nFaltan argumentos\n")
        exit()

    try:
        if (int(args.size) < 10000):
            raise ValueError
    except ValueError:
        print("\nDebe ingresar un size mayor a 10kB.\n")
        exit()            
    return(args)
