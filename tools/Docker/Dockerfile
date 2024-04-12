FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y \
        build-essential \
        cmake \
        libboost-all-dev \
        sqlite3 \
        libsqlite3-dev \
        socat \
        python3-pip && \
    rm -rf /var/lib/apt/lists/*

RUN pip3 install \
    "uvicorn[standard]"==0.22.0 \
    anyio==3.7.1 \
    fastapi==0.85.0

WORKDIR /workspace

CMD ["bash"]
