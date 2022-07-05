import bencodepy
import socket
import logging
from hashlib import sha1
from pprint import pprint
from math import ceil
from trackers import UDPTracker, HTTPTracker

class TorrentMetaData():

    def __init__(self, file_path: str):
        with open(file_path, "rb") as file:
            """ Class atributes """
            self.trackers = []
            self.info_hash: bytes = None
            self.info = {}
            
            """ Read and decode metainfo file """
            buffer = file.read()
            meta_data: dict = bencodepy.decode(buffer)
            
            """ Set variables """
            info_index = buffer.find(b"4:infod")
            self.info_hash = sha1(buffer[info_index+6:-1]).digest()
                        
            """ Set trackers """
            trackers_url = []
            trackers_url.append(meta_data.pop(b"announce"))
            try:
                announce_list = meta_data.pop(b"announce-list")
                for url in announce_list:
                    trackers_url.append(url.pop())
            except Exception:
                print("No hay announce-list")
                
            for url in trackers_url:
                url = str(url, "utf-8")
                if "udp" in url:
                    self.trackers.append(UDPTracker(url))
                else:
                    self.trackers.append(HTTPTracker(url))

            """ Set info dict """            
            info_dict = meta_data[b"info"]
            for key in info_dict:
                self.info[str(key, "utf-8")] = info_dict[key]

            """ Make the 'pieces' key in the info dict a list indexed by piece id """
            pieces_hashes = self.info["pieces"]
            self.info["pieces"] = []
            total_pieces = int(len(pieces_hashes) / 20)
            for i in range(0, total_pieces):
                self.info["pieces"].append(pieces_hashes[i*20:i*20+20])

if __name__ == "__main__":
    t = TorrentMetaData("./The Complete Chess Course - From Beginning to Winning Chess - 21st Century Edition (2016).epub Gooner-[rarbg.to].torrent")
    print(t.info["pieces"][2])