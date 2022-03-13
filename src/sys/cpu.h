#pragma once

#include <stdint.h>

// returns 0 on failure (no support)
uint8_t cpu_checkTSCSupport();

// returns 0 on failure (not invariant)
uint8_t cpu_checkTSCInvariant();