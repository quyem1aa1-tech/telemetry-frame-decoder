# Telemetry Frame Decoder

A simple C++17 program that reads and decodes 7-byte binary telemetry frames from a Linux Named Pipe (FIFO). It checks frame checksums and prints sensor readings to the console.

# Overview

The main objective of this project is to apply Effective Modern C++ practices to embedded/systems software development:

- **Stack-based parsing:** Reads 7-byte binary frames directly into a struct without dynamic heap allocations.
- **FIFO stream handling:** Reads data from a Linux Named Pipe and exits cleanly on End-Of-File (EOF).
- **Data integrity:** Validates frames using XOR checksums and handles corrupted packets or unknown sensor IDs safely.

# Technical Stack

- **Language Standard:** C++17

- **Compiler:** g++ (GCC)

- **Build System:** CMake (>= 3.20) and GNU Make

- **External Dependencies:** {fmt} (Modern formatting library)

- **Testing & Simulation:** Python 3, Bash shell scripts

- **Target Environment**: Linux / POSIX

# Telemetry Protocol Specification

The decoder processes a fixed 7-byte binary telemetry frame:

| Byte Offset | Field | Value / Type | Description |
| :--- | :--- | :--- | :--- |
| `0x00` | Header 1 | `0xAA` | Frame synchronization marker 1 |
| `0x01` | Header 2 | `0x55` | Frame synchronization marker 2 |
| `0x02` | Sensor ID | `uint8_t` | Device identifier (e.g., `0x01`, `0x02`, `0x03`) |
| `0x03` | Length | `0x02` | Payload size in bytes |
| `0x04` | Temp MSB | `uint8_t` | Temperature data (High byte, Big-Endian) |
| `0x05` | Temp LSB | `uint8_t` | Temperature data (Low byte, Big-Endian) |
| `0x06` | Checksum | `uint8_t` | XOR checksum of bytes 0 through 5 |

# Getting Started

## 1. Prerequisites
On Ubuntu/Debian systems, install the necessary toolchain and {fmt} library:
```bash
sudo apt update
sudo apt install -y build-essential cmake g++ libfmt-dev python3
```

## 2. Build the Application

Configure and compile the project using CMake:
```bash
# Create build directory and compile
cmake -B build -S .
cmake --build build
```
Or run through the provided Makefile:
```bash
make build
```

## 3. Run End-to-End Simulation
You can run the integrated end-to-end test script, which sets up the FIFO pipe, runs the Python simulator in the background, and starts the C++ decoder:

```bash
chmod +x scripts/run_test.sh
./scripts/run_test.sh
```

Or using Make:
```bash
make test
```

# Error Handling & Edge Cases

- **Corrupted Checksum:** If an XOR checksum mismatch occurs, a warning is printed to stderr and the corrupted frame is dropped without breaking the stream.

- **Unknown Sensor ID:** If a sensor ID is not recognized (e.g., 0x04), the decoder logs an error message but continues processing subsequent frames safely.

- **Clean Disconnection:** When the sender stops producing data, the decoder detects the EOF state and shuts down gracefully without lingering zombie processes.