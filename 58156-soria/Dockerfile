FROM python:3.7

WORKDIR /home/lucas/Documents/compu\ 2/pmz

COPY requirements.txt ./
RUN pip3 install --no-cache-dir -r requirements.txt

COPY . .

CMD [ "python", "./Server.py" ]
