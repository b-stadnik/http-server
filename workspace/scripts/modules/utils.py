#!/usr/bin/env python3

import time
from enum import IntEnum, unique


def wait_for_condition(condition_func, timeout=5):
    start_time = time.time()
    while True:
        if condition_func():
            return True
        if time.time() - start_time >= timeout:
            return False
        time.sleep(0.2)


@unique
class MessageId(IntEnum):
    START = 0
    STOP = 1
    CONFIG = 2
