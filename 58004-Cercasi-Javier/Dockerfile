FROM python:3           

RUN git clone https://github.com/javicercasi/Final_computacion2.git

WORKDIR /Final_computacion2/58004-Cercasi-Javier

RUN apt-get update && apt-get install -y ffmpeg

RUN apt-get install -y abiword

RUN pip3 install -r requirements.txt

CMD [ "sh","-c", "python3 proyecto.py -p ${PORT} -s ${SIZE} -d /Final_computacion2/58004-Cercasi-Javier"]
