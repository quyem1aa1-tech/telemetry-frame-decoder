#!/bin/bash

set -e

ROOT_PROJECT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT_PROJECT"

FIFO_DIR="$ROOT_PROJECT/tests/tmp"
TELEMETRY_FIFO="$FIFO_DIR/telemetry_fifo.bin"

mkdir -p "$FIFO_DIR"

if [ ! -p "$TELEMETRY_FIFO" ]; then
    echo "[INFO]: Creating FIFO at $TELEMETRY_FIFO"
    rm -f "$TELEMETRY_FIFO"
    mkfifo "$TELEMETRY_FIFO"
else
    echo "[INFO]: FIFO already exists at $TELEMETRY_FIFO, skipping creation"
fi
