from urllib.parse import urlparse
from requests import get, Timeout, ConnectionError
from random import randint
import bencodepy
import socket

class TrackerError(Exception):
    def __init__(self, message):
        self.message = message

class Tracker:
    
    def __init__(self, url: str):
        url_parsed = urlparse(url)
        self.name = url_parsed.hostname
        self.port = url_parsed.port

    def request_peers(self, request_data: dict) -> list:
        pass

class UDPTracker(Tracker):
    
    def __init__(self, url: str):
        url_parsed = urlparse(url)
        self.name = url_parsed.hostname
        self.port = url_parsed.port
        self.connection: socket.socket = None
        self.connection_id: bytes = None

    def request_peers(self, request_data: dict) -> list:
        self.connection_id = self.send_connection_request()
        peers = self.send_announce(request_data)
        return peers

    def send_connection_request(self) -> int:
        try:
            tracker_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            tracker_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            tracker_socket.settimeout(0.5)
            tracker_socket.connect((self.name, self.port))
        except socket.gaierror as err:
            raise TrackerError(err.strerror)

        transaction_id = randint(0, 4294967295)
        connect_request = (0x41727101980).to_bytes(8, "big") + (0).to_bytes(4, "big") + (transaction_id).to_bytes(4, "big")
        tracker_socket.send(connect_request)
        try:
            connnect_response = tracker_socket.recv(150)
        except socket.timeout:
            raise TrackerError("Tracker timed out.")
        except ConnectionRefusedError as err:
            raise TrackerError(err.with_traceback(None))

        if len(connnect_response) < 16:
            raise TrackerError("Response smaller than 16 bytes.")
        if int.from_bytes(connnect_response[0:4], "big") != 0:
            raise TrackerError("Action field in response is not connect (0).")
        if int.from_bytes(connnect_response[4:8], "big") != transaction_id:
            raise TrackerError("The tracker answered with a different transaction id.")
        
        connection_id = int.from_bytes(connnect_response[8:], "big")
        tracker_socket.close()
        return connection_id

    def send_announce(self, request_data: dict) -> list:
        try:
            tracker_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            tracker_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            tracker_socket.settimeout(0.5)
            tracker_socket.connect((self.name, self.port))
        except socket.gaierror as err:
            raise TrackerError(err.strerror)

        peers = []
        transaction_id = randint(0, 4294967295)
        announce_request = self.make_announce_request(request_data)
        tracker_socket.send(announce_request)
        try:
            response = tracker_socket.recv(150)
        except socket.timeout:
            raise TrackerError("Socket timed out")

        if len(response) < 20:
            raise TrackerError("Response smaller than 20 bytes.")
        #if int.from_bytes(response[0:4], "big") != 1:
        #    raise TrackerError("Action field in response is not announce (1).")
        #if int.from_bytes(response[4:8], "big") != transaction_id:
        #    raise TrackerError("The tracker answered with a different transaction id.")
        print(response)
        for i in range(20, len(response), 6):
            ip = str(response[i]) + "." + str(response[i+1]) + "." + str(response[i+2]) + "." + str(response[i+3])
            port = int.from_bytes(response[i+4:i+6], "big")
            peers.append({b"ip": ip, b"port": port})
        
        tracker_socket.close()
        return peers

    def make_announce_request(self, request_data: dict) -> bytes:
        transaction_id = randint(0, 4294967295)
        announce_request = bytearray()

        announce_request += self.connection_id.to_bytes(8, "big")                  
        announce_request += (1).to_bytes(4, "big") # action field, 4 bytes
        announce_request += (transaction_id).to_bytes(4, "big") 
        announce_request += request_data["info_hash"]           
        announce_request += bytes(request_data["peer_id"], "utf-8") 
        announce_request += request_data["downloaded"].to_bytes(8, "big")
        announce_request += request_data["left"].to_bytes(8, "big")
        announce_request += request_data["uploaded"].to_bytes(8, "big")
        announce_request += request_data["event"].to_bytes(4, "big")
        announce_request += (0).to_bytes(4, "big")  # ip address field, 4 bytes, 0 default
        announce_request += (0).to_bytes(4, "big")  # key field, 4 bytes
        announce_request += (-1).to_bytes(4, "big", signed = True) # num_want field, 4 bytes, -1 default
        announce_request += (request_data["port"]).to_bytes(2, "big")

        return announce_request


    """ La comento porque ahora no la uso, quizas sea necesaria mas adelante.
    def udp_scrape(self, connection_id: int, tracker_socket: socket.socket):
        transaction_id = randint(0, 4294967295)
        udp_request = connection_id + \
            (2).to_bytes(4, "big") + \
            (transaction_id).to_bytes(4, "big") + self.info_hash
        tracker_socket.send(udp_request)
        answer = tracker_socket.recv(150)"""

    def get_connection(self, addr: tuple) -> socket.socket:
        if self.connection == None:
            self.connection = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            self.connection.settimeout(0.5)
            try:
                self.connection.connect((self.name, self.port))
            except socket.gaierror as err:
                raise TrackerError(err.strerror)

class HTTPTracker(Tracker):
    
    def __init__(self, url: str):
        self.url = url

    def request_peers(self, request_data: dict) -> list:
        try:
            peers = bencodepy.bdecode(get(self.url, request_data, timeout=0.5).content)
            return peers[b"peers"]
        except Timeout as err:
            raise TrackerError("HTTP GET request to tracker timed out.")
        except ConnectionError as err:
            raise TrackerError("Failed to establish a new connection when sending a HTTP GET request.")
