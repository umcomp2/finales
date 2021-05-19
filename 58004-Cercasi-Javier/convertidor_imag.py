from PIL import Image


def imagenes(entrada, q, adr, output):

    try:
        salida = entrada.split(".")[0]+'.'+output
        im = Image.open(entrada)
        rgb_im = im.convert('RGB')
        rgb_im.save(salida, quality=95)
        q.put(salida)
    except:
        q.put("Error")