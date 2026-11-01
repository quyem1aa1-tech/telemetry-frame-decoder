#!/usr/bin/env bash
set -e

# 1. Define clean, reusable paths
FIFO_PATH="${1:-tests/tmp/telemetry_fifo.bin}"
SIM_SCRIPT="tests/python/test_pipe.py"
APP_BIN="./build/src/app"

# 2. Ensure FIFO pipe exists before running
mkdir -p "$(dirname "$FIFO_PATH")"
if [ ! -p "$FIFO_PATH" ]; then
    rm -f "$FIFO_PATH"
    mkfifo "$FIFO_PATH"
fi

# 3. Start Python simulator in background
python3 "$SIM_SCRIPT" &
SIM_PID=$!

# Kill background Python process automatically on script exit (single quotes to fix ShellCheck)
trap 'kill -9 "$SIM_PID" 2>/dev/null || true' EXIT

# 4. Run the C++ decoder application
"$APP_BIN" "$FIFO_PATH"