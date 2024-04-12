#!/usr/bin/env python3

import uvicorn
import argparse

from fastapi import FastAPI, Request
from fastapi.middleware.cors import CORSMiddleware
from modules.fifo import BiDirectionalFIFO
from modules.utils import MessageId, wait_for_condition
from modules.db_handler import DataBase


class Backend(FastAPI):

    def __init__(self, db_path):
        super().__init__()
        self._bi_fifo = BiDirectionalFIFO("/tmp/pipe2", "/tmp/pipe")
        self._db_handler = DataBase(db_path)

        self._init_urls()
        self._init_cors()

    def _init_cors(self):
        origins = ["*"]

        self.add_middleware(
            CORSMiddleware,
            allow_origins=origins,
            allow_credentials=True,
            allow_methods=["*"],
            allow_headers=["*"],
        )

    def _init_urls(self):
        mapping = [
            {'url': '/start', 'fun': self._start, 'http_method': 'get'},
            {'url': '/stop', 'fun': self._stop, 'http_method': 'get'},
            {'url': '/device', 'fun': self._device, 'http_method': 'get'},
            {'url': '/messages', 'fun': self._get_messages, 'http_method': 'get'},
            {'url': '/configure', 'fun': self._configure, 'http_method': 'put'},
        ]

        methods = {
            'get': self.get, 'post': self.post, 'put': self.put,
            'delete': self.delete, 'websocket': self.websocket
        }

        for m in mapping:
            fast_api_decorator = methods[m['http_method']]
            fast_api_decorator(m['url'])(m['fun'])

    async def _start(self):
        self._bi_fifo.write(MessageId.START)

        success = wait_for_condition(lambda: "ok" in self._bi_fifo.read().strip())

        return {"success": success}

    async def _stop(self):
        self._bi_fifo.write(MessageId.STOP)

        success = wait_for_condition(lambda: "ok" in self._bi_fifo.read().strip())

        return {"success": success}

    async def _get_messages(self, limit: int = 10):
        messages_data = self._db_handler.query(
            "SELECT pressure, temperature, velocity FROM messages ORDER BY id DESC LIMIT ?",
            (limit,)
        )
        messages = [{"pressure": row[0], "temperature": row[1], "velocity": row[2]} for row in messages_data]
        return {"messages": messages}

    async def _device(self):
        # Query data from the "configurations" table
        config_data = self._db_handler.query("SELECT * FROM configurations ORDER BY id DESC LIMIT 1")
        config = {}
        if config_data:
            config = {"frequency": config_data[0][1], "debug": bool(config_data[0][2])}

        # Query mean values from the "messages" table
        mean_data = self._db_handler.query("SELECT AVG(pressure), AVG(temperature), AVG(velocity) FROM messages")
        mean_last_10 = {"pressure": mean_data[0][0], "temperature": mean_data[0][1], "velocity": mean_data[0][2]}

        # Query latest values from the "messages" table
        latest_data = self._db_handler.query(
            "SELECT pressure, temperature, velocity FROM messages ORDER BY id DESC LIMIT 1"
        )
        latest = {}
        if latest_data:
            latest = {"pressure": latest_data[0][0], "temperature": latest_data[0][1], "velocity": latest_data[0][2]}

        return {
            "curr_config": config,
            "mean_last_10": mean_last_10,
            "latest": latest
        }

    async def _configure(self, request: Request):
        config = await request.json()
        msg = f"{MessageId.CONFIG},{config['frequency']},{config['debug']}"

        self._bi_fifo.write(msg)

        success = wait_for_condition(lambda: "ok" in self._bi_fifo.read().strip())

        return {"success": success}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="Run the backend server")
    parser.add_argument("--ip", default="0.0.0.0", help="IP address to bind to")
    parser.add_argument("--port", type=int, default=7100, help="Port number to listen on")
    parser.add_argument("--db-file", required=True, help="Path to the database file")
    args = parser.parse_args()

    app = Backend(args.db_file)
    uvicorn.run(app, host=args.ip, port=args.port, log_level="info")
