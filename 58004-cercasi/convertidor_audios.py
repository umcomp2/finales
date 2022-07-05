from pydub import AudioSegment
from pydub.playback import play
from concurrent import futures
duracion = 0
tema = b""

def hilo(cota):
    t = 1000
    intervalo = round(duracion / 5) #36
    anterior = int(cota) #0 36 72
    superior = int(cota + intervalo) #36 72 108
    
    if superior > duracion or superior == duracion - 1:
        superior = duracion
    parte = tema[anterior*t:superior*t]
    return (parte)

def audio(direccion, salida):

    mix = 0
    global duracion, tema
    try:
        tema = AudioSegment.from_mp3(r"{}".format(direccion))
    except:
        return("Error")
    duracion = int(tema.duration_seconds) + 1   
    hilos = futures.ProcessPoolExecutor(max_workers=5)
    resultado_a_futuro = hilos.map(hilo ,range(0,duracion,round(duracion/5)))   #180s, #36s
    for elemento in list(resultado_a_futuro):
        mix += elemento
    mix.export("Mix."+salida, format=salida)
    return("Mix."+salida)
