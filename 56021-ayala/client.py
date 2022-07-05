from codecs import StreamReader, StreamWriter
from socket import create_server
from torrent import Torrent, Peer
from asyncio import start_server, create_task, run
import client_data
import argparse

class TorrentNotFound(Exception):
    pass

class PeerServer():

    def __init__(self, client, threads: int = None):
        self.client = client
        self.n_threads = threads
        self.addr = (client_data.ip, client_data.port)

    async def run(self):
        sock = create_server(self.addr)
        conn = ()
        while conn := sock.accept():
            create_task(self.__handle_new_connection(conn))

    async def __handle_new_connection(self, conn: tuple):
        handshake = conn[0].recv(68)
        if handshake[0:20] != (19).to_bytes(1, "big") + b"BitTorrent protocol":
            print("Peer sent corrupt handshake")
            conn[0].close()
            return
        info_hash = handshake[28:48]
        try:
            torrent = self.client.find_torrent_by_infohash(info_hash)
        except TorrentNotFound:
            conn[0].close()
            print("Torrent not found")
            return
        handshake = (19).to_bytes(1, "big") + bytes("BitTorrent protocol", "utf-8") + b"\0\0\0\0\0\0\0\0" + torrent.torrent_meta_data.info_hash + bytes(client_data.client_id, "utf-8")
        bitfield_msg = (1 + torrent.bitfield.size).to_bytes(4, "big") + (5).to_bytes(1, "big") + torrent.bitfield.bits
        conn[0].send(handshake)
        conn[0].send(bitfield_msg)
        Peer.from_connection(conn, torrent)

class Client():

    def __init__(self):
        self.torrents = []

    async def __handle_new_connection(self, reader: StreamReader, writer: StreamWriter):
        handshake = await reader.read(68)
        if handshake[0:20] != (19).to_bytes(1, "big") + b"BitTorrent protocol":
            print("Peer sent corrupt handshake")
            return
        info_hash = handshake[28:48]
        try:
            torrent = self.find_torrent_by_infohash(info_hash)
        except TorrentNotFound:
            print("Torrent not found")
            return
        handshake = (19).to_bytes(1, "big") + bytes("BitTorrent protocol", "utf-8") + b"\0\0\0\0\0\0\0\0" + torrent.torrent_meta_data.info_hash + bytes(client_data.client_id, "utf-8")
        bitfield_msg = (1 + torrent.bitfield.size).to_bytes(4, "big") + (5).to_bytes(1, "big") + torrent.bitfield.bits
        writer.write(handshake)
        writer.write(bitfield_msg)
        await writer.drain()
        create_task(Peer.from_connection((reader, writer), torrent))

    async def run(self):
        parser = argparse.ArgumentParser()
        parser.add_argument("-t", type=str)
        parser.add_argument("-p", type=str)
        args = parser.parse_args()

        server = await start_server(self.__handle_new_connection, client_data.ip, client_data.port)
        server_task = create_task(server.serve_forever(), name="Peer server")

        await self.__post_torrent(args.t)
        """ Modificaciones hechas durante el final """
        peer_file = open("./peers.txt", "r")
        for i in range(2):
            peer = peer_file.readline()
            peer = peer.split(":")
            self.torrents[-1].peers.append(Peer({b"ip": peer[0], b"port": peer[1]}, self.torrents[-1]))

        create_task(self.torrents[-1].share(), name="Torrent share")
        await server_task

    @classmethod
    async def main(cls):
        client = Client()
        await client.run()

    def find_torrent_by_infohash(self, infohash: bytes):
        for torrent in self.torrents:
            if torrent.torrent_meta_data.info_hash == infohash:
                return torrent
        raise TorrentNotFound

    async def __post_torrent(self, file_path):
        self.torrents.append(Torrent(file_path))
        #create_task(self.torrents[-1].share(), name="Torrent share")
    
if __name__ == "__main__":
   run(Client.main(), debug=True)
