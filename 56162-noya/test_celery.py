import socket

if __name__ == "__main__":
    
    desc= socket.socket(family=socket.AF_INET,type=socket.SOCK_STREAM)
    desc.connect(("192.168.1.51",8080))
    string = "GET /ID=1/T=50.00/H=99.00/ HTTP/1.1\r\n"
    desc.sendall(str.encode(string))
    desc.close()

