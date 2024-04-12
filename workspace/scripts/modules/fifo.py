#!/usr/bin/env python3

import os


class BiDirectionalFIFO:
    def __init__(self, read_path, write_path):
        self._read_path = read_path
        self._write_path = write_path

        # Create the FIFO files if they don't exist
        if not os.path.exists(self._read_path):
            os.mkfifo(self._read_path)
        if not os.path.exists(self._write_path):
            os.mkfifo(self._write_path)

        self._read_fifo = open(self._read_path, "r")
        self._write_fifo = open(self._write_path, "w")

    def write(self, data):
        self._write_fifo.write(self.format_msg(data))
        self._write_fifo.flush()

    def read(self):
        return self._read_fifo.readline().rstrip("\n")

    def format_msg(self, data):
        return f"${data}\n"

    def __del__(self):
        self._read_fifo.close()
        self._write_fifo.close()
