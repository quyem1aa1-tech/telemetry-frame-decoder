#pragma once
#include <decoder/frame.hpp>

namespace decoder
{

struct TelemetryFrame;

auto verifyChecksumxor(const decoder::TelemetryFrame& frame) -> bool;
auto printTelemetryData(const decoder::TelemetryFrame& frame) -> void;

}  