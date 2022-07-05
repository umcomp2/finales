from hashlib import sha1
from math import ceil
from random import randint
from socketserver import BaseRequestHandler
from bitfield import BitField
from filemanager import SingleFileManager, MultiFileManager
from torrentfile import TorrentMetaData
from trackers import TrackerError
import client_data
import logging
import socket
import asyncio

CHOKE = b"\x00\x00\x00\x01\x00"
UNCHOKE = b"\x00\x00\x00\x01\x01"
INTERESTED = b"\x00\x00\x00\x01\x02"
NOT_INTERESTED = b"\x00\x00\x00\x01\x03"
BLOCK_SIZE = 16384

class PeersNotFound(Exception):
    def __init__(self, message):
        self.message = message

class PeerNotAvailable(Exception):
    def __init__(self, message):
        self.message = message

class Peer():

    def __init__(self, peer_data: dict, torrent):
        self.torrent = torrent
        self.ip = peer_data[b"ip"]
        self.port = peer_data[b"port"]
        self.reader = None
        self.writer = None
        self.am_choking = True
        self.am_interested = False
        self.peer_choking = True
        self.peer_interested = False
        self.bitfield = None
        self.current_piece = [-1, bytearray(0)]  # [1] piece id, [2] data

    @classmethod
    async def from_connection(cls, streams, torrent):
        peer_data = {b"ip": 0, b"port": 0}
        peer = cls(peer_data, torrent)
        peer.reader = streams[0]
        peer.writer = streams[1]
        print("Un par se conecto")
        asyncio.create_task(peer.share(), name="Peer share")

    async def start(self):
        handshake = (19).to_bytes(1, "big") + bytes("BitTorrent protocol", "utf-8") + b"\0\0\0\0\0\0\0\0" + self.torrent.torrent_meta_data.info_hash + bytes(client_data.client_id, "utf-8")
        try:
            r, w = await asyncio.open_connection(host=self.ip, port=self.port)
            self.reader = r
            self.writer = w
            self.writer.write(handshake)
            handshake_response = await self.reader.read(68)
            await self.writer.drain()
        except socket.timeout as err:
            raise PeerNotAvailable(err.with_traceback(None))
        except ConnectionRefusedError as err:
            raise PeerNotAvailable(err.with_traceback(None))
        except ConnectionResetError as err:
            raise PeerNotAvailable(err.with_traceback(None))
        except OSError as err: # No route to host is one possible error
            raise PeerNotAvailable(err.with_traceback(None))

        if b"\x13BitTorrent protocol" not in handshake_response or len(handshake) != 68:
            raise PeerNotAvailable("Peer answered with a corrupt handshake")
        
        asyncio.create_task(self.share(), name="Peer share")
    
    async def share(self):
        self.writer.write(INTERESTED)
        await self.writer.drain()
        self.am_interested = True
        self.writer.write(UNCHOKE)
        await self.writer.drain()
        self.am_choking = False
        
        while True:
            try:
                await self.__check_input()
            except socket.timeout:
                pass
            except PeerNotAvailable as err:
                print(err.message)
                return

            piece_id = randint(0, self.torrent.number_of_pieces - 1)
            if (self.bitfield and not self.peer_choking and self.am_interested
                and self.torrent.bitfield[piece_id] == 0 and self.current_piece[0] == -1):
                self.current_piece[0] = piece_id 
                for i in range(self.torrent.number_of_blocks):
                    await self.writer.drain()
                    self.request_piece({"index": self.current_piece[0], "begin": i*BLOCK_SIZE, "length": BLOCK_SIZE}) 
    
    def request_piece(self, request_data: dict):
        request = bytearray()
        request += (13).to_bytes(4, "big")
        request += (6).to_bytes(1, "big")
        request += request_data["index"].to_bytes(4, "big")
        request += request_data["begin"].to_bytes(4, "big")
        request += request_data["length"].to_bytes(4, "big")
        self.writer.write(request)
        # self.pending.append(request_data["index"])

    async def __check_msg(self, msg: bytes):
        if len(msg) < 4:
            return
        if msg == b"\0\0\0\0":
            # keep alive
            return
        if msg[4] == 0:
            self.peer_choking = True
            return
        if msg[4] == 1:
            self.peer_choking = False
            return
        if msg[4] == 2:
            self.peer_interested = True
            return
        if msg[4] == 3:
            self.peer_interested = False
            return
        if msg[4] == 4 and len(msg) == 9:
            piece_index = int.from_bytes(msg[5:9], "big")
            self.bitfield.add(piece_index)
            return
        if msg[4] == 5:
            bitfield_length = int.from_bytes(msg[0:4], "big") - 1
            if bitfield_length != ceil(self.torrent.number_of_pieces/8) or len(msg[5:]) != ceil(self.torrent.number_of_pieces/8): 
                # raise PeerNotAvailable("Peer sent corrupt bitfield")
                pass
            self.bitfield = BitField.from_bytes(msg[5:])
            return
        if msg[4] == 6:
            self.__answer_block_request(msg)
            await self.writer.drain()
            return
        if msg[4] == 7:
            offset = int.from_bytes(msg[9:13], "big")
            length = int.from_bytes(msg[0:4], "big") - 9
            self.current_piece[1][offset:offset+length] = msg[13:]
            if sha1(self.current_piece[1]).digest() == self.torrent.torrent_meta_data.info["pieces"][self.current_piece[0]]:
                print(f"Se recibio una pieza de ID = {str(self.current_piece[0])} de {self.ip}:{self.port}")
                await asyncio.to_thread(self.torrent.add_piece, self.current_piece)
                # self.torrent.add_piece(self.current_piece)
                self.current_piece[0] = -1
                self.current_piece[1] = bytearray(0)
            return
        if msg[4] == 8:
            print("Se cancelo una solicitud de bloque")
            return
        if msg[4] == 9:
            print("Puerto DHT")
            return
        print("LLego un mensaje corrupto")
        
    async def __check_input(self):
        data = b""
        try:
            data = await asyncio.wait_for(self.reader.read(4), 0.5)
        except:
            pass
        if len(data) == 4:
            msg = await self.reader.read(int.from_bytes(data, "big"))
            await self.__check_msg(data + msg)

    def __answer_block_request(self, request_msg):
        piece_id = int.from_bytes(request_msg[5:9], "big")
        if self.torrent.bitfield[piece_id] == True:
            block_offset = int.from_bytes(request_msg[9:13], "big")
            length = int.from_bytes(request_msg[13:17], "big")
            piece = self.torrent.get_piece(piece_id)
            piece[1] = piece[1][block_offset:block_offset+length]
            piece_msg = (9 + len(piece[1])).to_bytes(4, "big") + (7).to_bytes(1, "big") + request_msg[5:9] + request_msg[9:13] + piece[1]
            self.writer.write(piece_msg)

class Torrent():

    def __init__(self, file_path: str):
        self.torrent_meta_data = TorrentMetaData(file_path)
        self.peers = []
        self.file_manager = None
        self.number_of_pieces = None
        self.number_of_blocks = None
        self.bitfield = None

        if "files" in self.torrent_meta_data.info.keys():
            self.file_manager = MultiFileManager(self.torrent_meta_data.info)
        else:
            self.file_manager = SingleFileManager(self.torrent_meta_data.info)

        self.number_of_pieces = ceil(self.file_manager.get_torrent_length()/self.torrent_meta_data.info["piece length"])
        self.number_of_blocks = int(self.torrent_meta_data.info["piece length"] / BLOCK_SIZE)
        self.bitfield = self.file_manager.calculate_bitfield()
        
        self.request_peers()

    async def share(self):
        """
        peer_data = {b"ip": "127.0.0.2", b"port": 56056}
        peer = Peer(peer_data, self)
        asyncio.create_task(peer.start(), name="Peer start")
        peer_data = {b"ip": "127.0.0.3", b"port": 56056}
        peer = Peer(peer_data, self)
        asyncio.create_task(peer.start(), name="Peer start")
        """
        for peer in self.peers:
            try:
                asyncio.create_task(peer.start())
            except PeerNotAvailable as err:
                logging.info(err.with_traceback(None))

    def request_peers(self):
        peers = []
        request_data = {
            "info_hash": self.torrent_meta_data.info_hash,
            "peer_id": client_data.client_id,
            "downloaded": 0,
            "left": 0,
            "uploaded": 0,
            "compact": 1,
            "event": 1,
            "ip": 0,
            "key": 0,
            "numwant": -1,
            "port": client_data.port}
        print("Me conecto al tracker")
        trackers = self.torrent_meta_data.trackers
        for tracker in trackers:
            try:
                peers += tracker.request_peers(request_data)
                self.peers += [Peer(peer, self) for peer in peers]
            except TrackerError as err:
                logging.info(err.message)
        print(f"{self.peers[0].ip}:{self.peers[0].port}")
    
    def add_piece(self, piece: tuple):
        self.bitfield.locks[piece[0]].acquire()
        if self.bitfield[piece[0]] == 0:
            self.file_manager.write_piece(piece)
            self.bitfield.add(piece[0])
        self.bitfield.locks[piece[0]].release()

    def get_piece(self, piece_id):
        return self.file_manager.get_piece(piece_id)

if __name__ == "__main__":
    logging.basicConfig(level = "INFO")
    torrent = Torrent("./texto.txt.torrent")
    torrent.share()
