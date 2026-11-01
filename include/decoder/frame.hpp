#pragma once

#include <cstdint>
#include <decoder/sensortype.hpp>

namespace decoder
{
// Blueprint for the 7-byte incoming data from Python
// Using GCC/Clang attribute to prevent memory padding
struct TelemetryFrame
{
    uint8_t sync_1;     // First sync byte (0xAA)
    uint8_t sync_2;     // Second sync byte (0x55)
    uint8_t sensor_id;  // ID of the device (0x01)
    uint8_t length;     // Payload length (0x02)
    uint8_t data_msb;   // High byte of temperature
    uint8_t data_lsb;   // Low byte of temperature
    uint8_t checksum;   // XOR of the first 6 bytes
} __attribute__((packed));

static_assert(sizeof(TelemetryFrame) == 7, "Frame size must be exactly 7 bytes!");
}  // namespace decoder