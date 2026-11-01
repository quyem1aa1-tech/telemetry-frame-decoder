#include <fmt/color.h>
#include <fmt/core.h>


#include <decoder/decoder.hpp>
#include <fstream>
#include <iostream>

auto main(int argc, char* argv[]) -> int
{
    // 1. Validate command line arguments
    if (argc < 2)
    {
        fmt::print(stderr, fg(fmt::color::red), "[ERROR]: Missing FIFO path argument!\n");
        fmt::print(stderr, "Usage: {} <fifo_path>\n", argv[0]);
        return 1;
    }

    const std::string pipe_path = argv[1];
    std::ifstream fifo(pipe_path, std::ios::binary);

    // 2. Check if the FIFO pipe is successfully opened
    if (!fifo.is_open())
    {
        fmt::print(stderr, fg(fmt::color::red), "[ERROR]: Failed to open FIFO at: {}\n", pipe_path);
        fmt::print(stderr, "[INFO]: Did you run the setup script to create the pipe?\n");
        return 1;
    }

    fmt::print(fg(fmt::color::cyan), "[INFO]: Successfully connected to telemetry stream.\n");
    fmt::print("[INFO]: Waiting for incoming binary frames...\n");

    decoder::TelemetryFrame frame{};

    // 3. Infinite loop to continuously process incoming data
    while (true)
    {
        // Attempt to read exactly 7 bytes into the frame struct
        if (fifo.read(reinterpret_cast<char*>(&frame), sizeof(frame)))
        {
            // Validate payload integrity using XOR checksum
            if (!decoder::verifyChecksumxor(frame))
            {
                fmt::print(
                    stderr, fg(fmt::color::yellow),
                    "[WARN]: Corrupted frame detected (checksum mismatch)! Dropping data.\n");
                continue;  // Skip the rest of the loop and wait for the next frame
            }

            // Data is valid, forward frame to business logic
            decoder::printTelemetryData(frame);
        }
        else
        {
            // Detect End-Of-File (EOF) when the Python writer finishes and closes the pipe
            if (fifo.eof())
            {
                fmt::print(fg(fmt::color::light_blue),
                           "[INFO]: Writer disconnected (EOF reached). Exiting loop cleanly.\n");
                break;  // Stop loop safely instead of blocking indefinitely
            }

            // Handle other unexpected I/O stream errors
            fmt::print(stderr, "[ERROR]: Stream read failure occurred.\n");
            break;
        }
    }

    return 0;
}