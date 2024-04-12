#!/usr/bin/env python3

import time
import random
import subprocess
import argparse
import os

from modules.utils import MessageId

class SerialMock:
    def __init__(self, virtual_serial_interface, msg_frequency=0.5):
        self.virtual_serial_interface = virtual_serial_interface
        self.msg_frequency = msg_frequency
        self.debug_flag = True

    def generate_mock_data(self):
        pressure = round(random.uniform(0.0, 1000.0), 2)
        temperature = round(random.uniform(0.0, 1000.0), 2)
        velocity = round(random.uniform(0.0, 1000.0), 2)
        return f"${pressure},{temperature},{velocity}\n"

    def send_message(self, message: str):
        with open(self.virtual_serial_interface, "wb") as serial_port:
            serial_port.write(message.encode())
            serial_port.flush()

    def read_input(self):
        with os.fdopen(os.open(self.virtual_serial_interface, os.O_NONBLOCK | os.O_RDONLY)) as serial_port:
            return serial_port.readline()

    def process_input(self, input_data: str):
        input_data = input_data.strip()  # Remove leading and trailing whitespaces
        response = "invalid format\n"
        if input_data[0] == "$":
            input_data = input_data[1:]  # Strip first and last characters ('$' and '\n')
            command, *args = input_data.split(',')
            if int(command) == MessageId.CONFIG:
                self.msg_frequency = int(args[0])
                debug_flag = bool(args[1])
                response = f"${MessageId.CONFIG},{self.msg_frequency},{debug_flag},ok\n"
            if int(command) == MessageId.START:
                response = f"${MessageId.START},ok\n"
            if int(command) == MessageId.STOP:
                response = f"${MessageId.STOP},ok\n"
        else:
            response = "invalid format\n"

        self.send_message(response)

    def main_loop(self):
        # Start socat to create the virtual serial interface
        socat_process = subprocess.Popen(["socat", "-d", "-d", f"pty,raw,echo=0,link={self.virtual_serial_interface}",
                                          f"pty,raw,echo=0,link=/tmp/serial_mock2"], stdout=subprocess.PIPE)

        time.sleep(3)
        # Main loop to generate and send mock data
        while True:
            mock_data = self.generate_mock_data()
            print("Sending mock data:", mock_data.strip())
            self.send_message(mock_data)
            input_data = self.read_input()
            if input_data:
                print("Received input:", input_data)
                self.process_input(input_data)

            time.sleep(1 / self.msg_frequency)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Run the mocked uart device")
    parser.add_argument("--device-port", default="/tmp/serial_mock1", help="Mocked device port")
    args = parser.parse_args()

    serial_mock = SerialMock(args.device_port)
    serial_mock.main_loop()
