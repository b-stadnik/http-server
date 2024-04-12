#!/usr/bin/env python3

import os


class BiDirectionalFIFO:
    def __init__(self, read_path, write_path):
        # Create the FIFO files if they don't exist
        if not os.path.exists(read_path):
            os.mkfifo(read_path)
        if not os.path.exists(write_path):
            os.mkfifo(write_path)

        self._write_fifo = open(write_path, "w")
        self._read_fifo = open(read_path, "r")
        print("FIFOS connected")

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
