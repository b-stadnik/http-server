#!/usr/bin/env python3

import time
from enum import IntEnum, unique


def wait_for_condition(condition_func, timeout=5):
    start_time = time.time()
    condition_result = condition_func()
    while not condition_result and time.time() - start_time < timeout:
        time.sleep(0.1)
        condition_result = condition_func()

    return condition_result


@unique
class MessageId(IntEnum):
    START = 0
    STOP = 1
    CONFIG = 2
