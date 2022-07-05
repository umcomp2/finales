import asyncio
import os
from functools import reduce
from hashlib import sha1
from math import ceil
from bitfield import BitField


class FileManager:

    def __init__(self, info: dict):
        pass

    def write_piece(self, piece: tuple):
        pass

    def get_piece(self, piece_id: int):
        pass

    def calculate_bitfield(self):
        pass

    def get_torrent_length(self):
        pass


class SingleFileManager(FileManager):

    def __init__(self, info: dict):
        self.name = str(info["name"], "UTF-8")
        self.length = info["length"]
        self.piece_len = info["piece length"]
        self.pieces = info["pieces"]

        try:
            self.file = open("./" + self.name, "x+b")
        except FileExistsError:
            self.file = open("./" + self.name, "r+b")

        self.file.seek(0)

    def write_piece(self, piece):
        offset = piece[0] * self.piece_len
        self.file.seek(offset)
        self.file.write(piece[1])

    def get_piece(self, piece_id: int) -> list:
        self.file.seek(piece_id * self.piece_len)
        piece = self.file.read(self.piece_len)
        return [piece_id, piece]

    def calculate_bitfield(self):
        piece_id = 0
        bitfield = BitField(ceil(self.length / self.piece_len / 8))

        while (piece := self.file.read(self.piece_len)) != b"":
            phash = sha1(piece).digest()
            if phash == self.pieces[piece_id]:
                bitfield.add(piece_id)
            piece_id += 1

        return bitfield

    def get_torrent_length(self):
        return self.length


class MultiFileManager(FileManager):

    def __init__(self, info: dict):
        self.piece_len = info["piece length"]
        self.dir_name = info["name"].decode()
        self.files = []
        self.pieces = info["pieces"]
        for file_info in info["files"]:
            file_info[b"path"] = [node.decode("utf-8") for node in file_info[b"path"]]
            file_path = "/".join(file_info[b"path"][0:-1]) + file_info[b"path"][-1]
            if len(file_info[b"path"]) > 1:
                pass  # Create dirs
            try:
                f = (open(self.dir_name + "/" + file_path, "x+b"), file_info[b"length"])
            except FileExistsError:
                f = (open(self.dir_name + "/" + file_path, "r+b"), file_info[b"length"])
            self.files.append(f)

    def write_piece(self, piece):
        piece_offset = piece[0] * self.piece_len
        file_index, file_offset = self._find_file(piece_offset)
        len_written = 0
        while len_written < self.piece_len and file_index < len(self.files):
            relative_offset = piece_offset - file_offset + len_written
            file_remain = self.files[file_index][1] - relative_offset  # How much data can be written to the file

            self.files[file_index][0].seek(relative_offset)
            n = self.files[file_index][0].write(piece[1][len_written:len_written+file_remain])
            len_written += n

            # Changes the file to the next one
            file_offset += self.files[file_index][1]
            file_index += 1

    def get_piece(self, piece_id: int):
        piece_offset = piece_id * self.piece_len
        file_index, file_offset = self._find_file(piece_offset)
        piece = b""
        while len(piece) < self.piece_len and file_index < len(self.files):
            n = self.piece_len - len(piece)
            relative_offset = piece_offset - file_offset + len(piece)
            self.files[file_index][0].seek(relative_offset)
            piece += self.files[file_index][0].read(n)
            file_offset += self.files[file_index][1]
            file_index += 1

        return [piece_id, piece]

    def calculate_bitfield(self):
        bitfield = BitField(ceil(self.get_torrent_length() / self.piece_len / 8))
        files = [file for file in self.files]
        file = files.pop(0)
        for piece_id in range(len(self.pieces)):
            len_read = 0
            piece = b""
            while len_read < self.piece_len:
                piece_aux = file[0].read(self.piece_len - len_read)               
                if len(piece_aux) == 0:
                    if len(files) > 0:
                        file = files.pop(0)
                    else:
                        break
                else:
                    len_read += len(piece_aux)
                    piece += piece_aux
            phash = sha1(piece).digest()
            if phash == self.pieces[piece_id]:
                bitfield.add(piece_id)

        return bitfield

    def get_torrent_length(self):
        tlen = reduce(lambda a, b: (0, a[1] + b[1]), self.files)
        return tlen[-1]

    def _find_file(self, piece_offset):
        file_offset = 0
        for i in range(len(self.files)):
            next_file_offset = file_offset + self.files[i][1]
            if file_offset <= piece_offset < next_file_offset:
                return i, file_offset
            file_offset = next_file_offset
