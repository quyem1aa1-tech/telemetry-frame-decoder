import random
from pathlib import Path

FILE_PATH = Path(__file__).resolve().parent.parent / "tmp" / "telemetry_fifo.bin"


def calculate_xor_checksum(data: list) -> int:
    """Calculate the XOR checksum for a given list of bytes."""
    checksum = 0
    for byte in data:
        checksum ^= byte

    return checksum


def generate_telemetry_frame():
    """Generate random telemetry data and construct a 7-byte binary frame."""

    temp = random.randint(100, 400)
    sensor_id = random.choice([0x01, 0x02, 0x03, 0x04])

    msb = (temp >> 8) & 0xFF
    lsb = temp & 0xFF

    frame = [0xAA, 0x55, sensor_id, 0x02, msb, lsb]

    checksum = calculate_xor_checksum(frame)

    if random.random() < 0.1:
        checksum ^= 0xFF

    frame.append(checksum)

    return temp, bytearray(frame)


if __name__ == "__main__":
    TOTAL_ROWS = 100
    print(f"Generating {TOTAL_ROWS} telemetry frames...")

    with open(FILE_PATH, "wb") as pipe:
        for _ in range(TOTAL_ROWS):
            # Call our function to get the temperature and packed frame
            temp, binary_frame = generate_telemetry_frame()

            # Write the 7-byte frame followed by a newline byte
            pipe.write(binary_frame)

        pipe.flush()

    print("Completed successfully!")
