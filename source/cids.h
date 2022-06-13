//------------------------------------------------------------------------
// Copyright(c) 2022 .
//------------------------------------------------------------------------

#pragma once

#include <pluginterfaces/base/funknown.h>
#include <pluginterfaces/vst/vsttypes.h>

namespace ecpj {
//------------------------------------------------------------------------
static const Steinberg::FUID kVestyProcessorUID (0xBE45AF84, 0xF5275DDB, 0x94799F0E, 0x7D3E3BC0);
static const Steinberg::FUID kVestyControllerUID (0x37E3AA75, 0xDCAB5B6C, 0x9BD6B107, 0x7A6D57C1);

#define VestyVST3Category "Fx"

//------------------------------------------------------------------------
} // namespace ecpj
