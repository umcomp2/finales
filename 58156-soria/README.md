# PMZ (PROJECT: MAZE SOLVER)

PMZ or Project maze solver is a web solution that allows users to upload mazes to the server (represented by a .txt file). Once the maze is uploaded, a multiprocess algorithm solves it and delivers the answer via a web page.


## Run Locally

Clone the project

```bash
  git clone https://github.com/LucaSor1a/pmz
```

Go to the project directory

```bash
  cd pmz
```

Install dependencies

```bash
  pip3 install -r requirements.txt
```

Build the Doker image

```bash
  sudo docker build -t pmz .
```

Start the server

```bash
  sudo docker-compose up -d
```

  
## Environment Variables

To run this project, you will need to add the following environment variables to your .env file

`IP`

`PORT`

  
## Features

- Asynchronous HTTP Server
- Multithreading log
- Multiprocessing maze solving solution
- Docker ready


## Tech / Documentation

**Server:** 
  - [aiohttp](https://docs.aiohttp.org/en/stable/)
  - [concurrent.futures](https://docs.python.org/3/library/concurrent.futures.html)
  - [multiprocessing](https://docs.python.org/3/library/multiprocessing.html)
  - [Docker](https://docs.docker.com/)
  - [Docker-compose](https://docs.docker.com/compose/)


**Client:**
  - Web Browser

  
## Authors

- [@LucaSor1a](https://github.com/LucaSor1a)

  
## License

[MIT](https://choosealicense.com/licenses/mit/)
