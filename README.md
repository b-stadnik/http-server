# Http server project

The project provides http server as an abstraction for embedded device connected via serial port.
It consist of:

1. UART connection - wrapped in C++ code
2. Database handling - using Sqlite3
3. REST API - wrapped in Python FastAPI

The Python and C++ code use named pipes for communication.

## Getting started

### Prerequisites

1. git
2. cmake
3. boost
4. [FastAPI](https://github.com/tiangolo/fastapi)

## Building the project

### Setup

1. Initialize the working directory

```bash
git submodule update --init
```

### Building
<a name="building"></a>

1. Enter the workspace folder:

```bash
cd ./workspace
```
2. Run CMake:

```bash
cmake -S . -B ./build
```

3. Enter the build folder and use make:

```bash
cd ./build
make
```

### Using Docker

You can use Docker to prepare build and run this project

#### Building image

```bash
docker build -t custom_image ./tools/Docker/
```

#### Running container

Using previously build image, create container:

```bash
# Assuming you are in project root folder

docker run --rm --net=host -v ./:/workspace -it custom_image:latest
```

#### Building

While inside container, follow the same instruction as in previous [Project building](#building) section

## Running application

Program will create and configure database, you can also create your own database and provide path:

```bash
touch database.db
```

To run application, simply run prepared bash command:

```bash
./run.sh
```
Override default arguments as neceserry, like path to database etc.

By default the device mock will be running and acting like an actual device.

## Quering to server

Right now there is no fronted provided, you can directly enter your webbrowser and test different http methods:

```bash
http://0.0.0.0:7100/messages?limit=5
http://0.0.0.0:7100/start
http://0.0.0.0:7100/stop

```

For PUT method, use CURL from command line:

```bash
curl -X PUT -H "Content-Type: application/json" -d '{"frequency": 1, "debug": 1}' http://localhost:7100/configure
{"success":true}

```

## TODO
1. Use yaml files for config storage
2. Improve logging
3. Improve pipe creation - currently user might need to call application twice if pipes dont exist before run
