import logging
import threading


class BitField():

    def __init__(self, size: int):
        self.size = size
        self.bits = bytearray(self.size)
        self.locks = [threading.Lock()] * self.size * 8

    @classmethod
    def from_bytes(cls, data: bytes):
        bitfield = cls(len(data))
        bitfield.bits = bytearray(data)
        return bitfield

    def add(self, piece_index):
        byte_index = int(piece_index/8)
        byte_value = self.bits[byte_index]
        bit_index_in_byte = 7 - (piece_index - byte_index * 8)
        byte_value = byte_value | (2**bit_index_in_byte)
        try:
            self.bits[byte_index] = byte_value
        except IndexError:
            logging.info("Index out of bounds")

    def __getitem__(self, bit_index: int) -> bool:
        byte_index = int(bit_index/8)
        byte_value = self.bits[byte_index]
        bit_index_in_byte = 7 - (bit_index - byte_index * 8)
        byte_value = byte_value >> bit_index_in_byte
        byte_value = byte_value & 0b00000001
        return byte_value
