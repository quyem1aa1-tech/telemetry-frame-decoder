#include <fmt/color.h>
#include <fmt/core.h>

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <decoder/decoder.hpp>

namespace decoder
{
auto verifyChecksumxor(const decoder::TelemetryFrame& frame) -> bool
{
    const auto* bytes = reinterpret_cast<const uint8_t*>(&frame);

    constexpr size_t payload_len = sizeof(decoder::TelemetryFrame) - sizeof(frame.checksum);

    uint8_t sumxor = 0;
    for (size_t i = 0; i < payload_len; ++i)
    {
        sumxor ^= bytes[i];
    }


    return (sumxor == frame.checksum);
}

auto printTelemetryData(const decoder::TelemetryFrame& frame) -> void
{
    auto type_id = static_cast<decoder::SensorType>(frame.sensor_id);
    auto raw_temp =
        static_cast<uint16_t>((static_cast<uint16_t>(frame.data_msb) << 8) | frame.data_lsb);

    int16_t signed_val = 0;
    std::memcpy(&signed_val, &raw_temp, sizeof(signed_val));

    double temp = signed_val / 10.0;


    switch (type_id)
    {
        case decoder::SensorType::KitchenTemp:
            fmt::print(fg(fmt::color::green), "[INFO DEVICE]: {:<16} | Value: {:.1f} °C\n",
                       "Kitchen Temp", temp);
            break;

        case decoder::SensorType::LivingRoomTemp:
            fmt::print(fg(fmt::color::green), "[INFO DEVICE]: {:<16} | Value: {:.1f} °C\n",
                       "Living Room Temp", temp);
            break;

        case decoder::SensorType::OutdoorTemp:
            fmt::print(fg(fmt::color::green), "[INFO DEVICE]: {:<16} | Value: {:.1f} °C\n",
                       "Outdoor Temp", temp);
            break;

        default:
            fmt::print(fg(fmt::color::red) | fmt::emphasis::bold,
                       "[ERROR] Unknown Sensor ID: {:#04x}\n", frame.sensor_id);
            break;
    }
}
}  // namespace decoder