#!/bin/bash

IP="0.0.0.0"
PORT=7100
BAUD_RATE=115200
DB_FILE="workspace/build/database.db"
UART_DEVICE="/tty/USB0"
MOCK=true


while [[ $# -gt 0 ]]; do
    key="$1"
    case $key in
        --ip)
            IP="$2"
            shift 2
            ;;
        --port)
            PORT="$2"
            shift 2
            ;;
        --db-file)
            DB_FILE="$2"
            shift 2
            ;;
        --uart)
            UART_DEVICE="$2"
            shift 2
            ;;
        --mock)
            MOCK=true
            shift
            ;;
        *)
            echo "Unknown option: $1"
            exit 1
            ;;
    esac
done

if [ "$MOCK" = true ]; then
    python3 workspace/scripts/mock_device.py --device-port "/tmp/serial_mock1" &
    UART_DEVICE="/tmp/serial_mock2"
fi

./workspace/build/uart_handler "$BAUD_RATE" "$UART_DEVICE" "$DB_FILE" &

python3 workspace/scripts/http_server.py --ip "$IP" --port "$PORT" --db-file "$DB_FILE"

