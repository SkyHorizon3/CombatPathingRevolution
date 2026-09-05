#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#ifdef DEBUG
#	undef DEBUG
#endif

// clib
#include "RE/Skyrim.h"
#include "REX/REX.h"
#include "SKSE/SKSE.h"

#include <xbyak/xbyak.h>

#include "Plugin.h"
#include <spdlog/sinks/basic_file_sink.h>
#include <xbyak/xbyak.h>

using namespace std::literals;
using namespace RE::literals;
using namespace REX::STR::literals;

#define DLLEXPORT extern "C" [[maybe_unused]] __declspec(dllexport)