import os, asyncio, array, socket, queue, threading
from os import remove
from pedido import argumentos, importe
from convertidor_doc import pdf_to_word , word_to_pdf
from convertidor_imag import imagenes
from convertidor_audios import audio
#args = argumentos()

args = importe()
if args == "Vacio":
    print("\nEl archivo de configuracion se encuentra Vacio:")
    argu = argumentos()
    args = (argu.size, argu.port, argu.documentroot)
adr = ""


async def handle_echo(reader, writer):

    dic = {"txt": " text/plain", "pdf":"application/pdf", "jpg": " image/jpeg", "TIFF": " image/TIFF", "gif": " image/gif", "png": " image/png", "BMP": " image/BMP", "EPS": " image/EPS", "jpeg": " image/jpeg", "ppm": " image/x-portable-pixmap", "html": " text/html", "docx": "application/docx", "ico": "image/x-icon", "mp3":"audio/mp3", "wav":"audio/wav", "aif":"audio/aif", "flac":"audio/flac", "ogg":"audio/ogg"}
    data = b''
    fin = True
    data = await reader.read(100)
    error = 0

    if len(data) == 0:
        encabezado = "GET /index.html"  # Mantener viva la conexion
    else:
        encabezado = data.decode().splitlines()[0] # GET /imagen.jpg

    if encabezado.split()[0] == "GET":  # Para paginas html.
        archivo = args[2] + encabezado.split()[1]
    
    if encabezado.split()[0] == "POST":

        if (b"User-Agent: curl") in data:
            data = await reader.readuntil(separator=b'--\r\n')
            datos = data.split(b'\r\n\r\n')[2]
            extension_out = data.split(b'output=')[1].split(b';')[0].decode()
        else:
            data = await reader.readuntil(separator=b'--\r\n')
            datos = data.split(b'\r\n\r\n')[3]
            extension_out = data.split(b"\r\n\r\n")[2].split(b"\r\n")[0].decode()

        entrada = data.split(b" filename=")[1].split(b'\r\n')[0].split(b'"')[1].decode()
        extension_in = entrada.split(".")[1]
        print("\n-Archivo recibido:", entrada, "Extension de entrada:",extension_in, "Extension de salida:", str(extension_out))

        with open(entrada, 'wb') as f:
            f.write(bytearray(datos))

        # Conversor Documentos:
        q = queue.Queue()

        if extension_out == "docx":
            funcion = pdf_to_word
        
        elif extension_out == "pdf":
            funcion = word_to_pdf

        # Conversor de Imagenes:
        if extension_out == "jpg" or extension_out == "png" or extension_out == "ppm" or extension_out == "jpeg" or extension_out == "BMP" or extension_out == "gif" or extension_out == "TIFF" or extension_out == "EPS":
            funcion = imagenes

        # Conversor de Audio:
        if extension_out == "ogg" or extension_out == "mp3" or extension_out == "wav" or extension_out == "flac" or extension_out == "aif":
            archivo = audio(entrada, extension_out)
        
        if extension_out != "html" and extension_out != "ico" and extension_out != "ogg" and extension_out != "mp3" and extension_out != "wav" and extension_out != "flac" :
            hilo = threading.Thread(target=funcion, args=(entrada, q, adr, extension_out,))
            hilo.start()
            archivo = q.get()
            hilo.join()


    if archivo == (args[2] + "/"):
        archivo = args[2] + '/index.html'

    if archivo == "Error":
        archivo = args[2] + '/500error.html'
        codigo = "HTTP/1.1 500 Internal Server Error"
        extension_out = "html"

    if os.path.isfile(archivo) is False:
        archivo = args[2] + '/400error.html'
        codigo = "HTTP/1.1 400 File Not Found"
        extension_out = "html"
    
    else:
        extension_out = archivo.split('.')[1]
        codigo = "HTTP/1.1 200 OK"

    header = bytearray(codigo + "\r\nContent-type:" +
                       dic[extension_out] + "\r\nContent-length:"+str((os.path.getsize(archivo)))+"\r\n\r\n", 'utf8')

    
    writer.write(header)
    fd = os.open(archivo, os.O_RDONLY)
    fin = True
    while fin is True:
        body = os.read(fd, int(args[0]))
        writer.write(body)
        if (len(body) != int(args[0])):
            os.close(fd)
            try:
                await writer.drain()
            except ConnectionResetError:
                pass
            fin = False
    writer.close()

    if archivo.split(".")[1] != "html" and archivo.split(".")[1] != "py":
        remove(archivo)
        if extension_in != extension_out:
            remove(entrada)


async def main():

    global adr

    ip = "127.0.0.1"
    #ip = socket.gethostbyname(socket.gethostname())
    server = await asyncio.start_server(
        handle_echo, host=[str(ip)], port=int(args[1]), loop=None, limit=50000000) 

    addr = server.sockets[0].getsockname()
    adr= "\nServidor en:"+str(addr)
    print(adr)

    async with server:
        await server.serve_forever()

asyncio.run(main())