import pandas as pd
from multiprocessing import Process
import numpy as np

#DataFrame TUTORES
urlTutor=open('tutor.txt','r')
url0=urlTutor.read()
print(url0)
tutores=pd.read_csv(url0)
df0=pd.DataFrame(tutores)
print(df0)


#DataFrame EXTRANJEROS
urlExtranjero=open('extranjero.txt','r')
url1=urlExtranjero
informacion=pd.read_csv(url1)
df=pd.DataFrame(informacion)

def academico():
    #Definimos las dimensiones que necesitamos segun el TEST de Autoconcepto.
    dfAcademico=pd.DataFrame(informacion,columns=[
        '1) Hago bien los trabajos escolares (profesionales).',
        '6) Mis profesores me consideran un buen trabajador.',
        '11) Trabajo mucho en clase.',
        '16) Mis profesores me estiman.',
        '21)  Soy un buen estudiante.',
        '26) Mis profesores me consideran inteligente y trabajador.'
        ])
    return dfAcademico.sum(axis=1)/60

def social():
    dfSocial=pd.DataFrame(informacion,columns=[
        '2)  Hago fácilmente amigos.', 
        '7) Soy una persona amigable.',
        '12) Es difícil para mí hacer amigos.',
        '17) Soy una persona alegre.',
        '22) Me cuesta hablar con desconocidos.',
        '27) Tengo muchos amigos.'
        ])
    return dfSocial.sum(axis=1)/60

def emocional():
    dfEmocional=pd.DataFrame(informacion,columns=[
        '3) Tengo miedo de algunas cosas.',
        '8) Muchas cosas me ponen nervioso.',
        '13) Me asusto con facilidad.',
        '18) Cuando los mayores dicen algo me pongo muy nervioso.',
        '23) Me pongo nervioso cuando me pregunta el profesor.',
        '28) Me siento nervioso.'
        ])
    return (600-dfEmocional.sum(axis=1))/60

def match(user, printable=False):
    print('El usuario que realizo la peticion del Match es: '+ user)
    calculo = (academico() + social() + emocional())/3

    df1=df.assign(calculo = (calculo)).sort_values(by=['calculo'], ascending=True)
    #Agregué la columna de Ranking.
    df2=df1.assign(Ranking= (df.index+1))
    #Match entre Extranjero y Alumno de la UM.
    df3=pd.merge(df0,df2,on='Ranking').sort_values(by=['Ranking'], ascending=True)
    #Buscar por un usuario
    tutor = df3.loc[df3['ALUMNO UM'] == user]
    extranjero = df3.loc[df3['Correo'] == user]
    result = ''
    if tutor.empty or extranjero.empty:
        if tutor.empty and extranjero.empty:
            result = 'Este alumno, NO tiene Match!\n Contactate con el admin\n'
        else:
            if tutor.empty:
                result = f"Tu MATCH es: {extranjero[['ALUMNO UM']]}\n"

            else:
                result = f"Tu MATCH es: {tutor[['Correo']]}\n"
    if printable:
        print(result)
    return result

if __name__ == '__main__':

    user = input("Enter your mail: ")
    p = Process(target=match, args=(user, True))
    p.start()
    p.join()


#Bibliografia
#https://docs.python.org/2/library/multiprocessing.html#
#https://www.geeksforgeeks.org/multiprocessing-python-set-1/
