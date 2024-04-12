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

Make sure to create an empty database, program will configure its tables later on if they do not exist:

```bash
touch database.db
```

To run application, simply run prepared bash command:

```bash
./run.sh
```
Override default arguments as neceserry, like path to database etc.
