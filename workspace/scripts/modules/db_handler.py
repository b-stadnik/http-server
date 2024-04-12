#!/usr/bin/env python3

import sqlite3


class DataBase:
    def __init__(self, db_file):
        self.conn = sqlite3.connect(db_file, uri=True, isolation_level=None)

    def query(self, query, params=None):
        cursor = self.conn.cursor()
        cursor.execute(query, params)
        return cursor.fetchall()

    def close(self):
        self.conn.close()
