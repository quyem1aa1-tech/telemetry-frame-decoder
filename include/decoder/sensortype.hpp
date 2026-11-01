#pragma once

#include <cstdint>


namespace decoder
{
enum class SensorType : uint8_t
{
    LivingRoomTemp = 0x01,
    KitchenTemp = 0x02,
    OutdoorTemp = 0x03
};
}