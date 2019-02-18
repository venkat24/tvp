/**
 * @file gpu_interface.h
 * Declares the interface for a GPU
 */

#include "cpu/register/register_interface.h"
#include "cpu/utils.h"

#include <cstdint>

#pragma once

namespace gpu {

class GPUInterface {
  public:
	/**
	 * Performs GPU operations corresponding to the given number of CPU cycles
	 * This is necessary since the CPU and GPU operate in parallel. The GPU must
	 * only perform as much work as the CPU has (i.e. in the same number of
	 * cycles) when we emulate them serially
	 */
	virtual void tick(cpu::ClockCycles cycles) = 0;

	/// Getters for the 12 GPU registers
	virtual cpu::IReg *get_lcdc() = 0;
	virtual cpu::IReg *get_stat() = 0;
	virtual cpu::IReg *get_scy() = 0;
	virtual cpu::IReg *get_scx() = 0;
	virtual cpu::IReg *get_ly() = 0;
	virtual cpu::IReg *get_lyc() = 0;
	virtual cpu::IReg *get_wy() = 0;
	virtual cpu::IReg *get_wx() = 0;
	virtual cpu::IReg *get_bgp() = 0;
	virtual cpu::IReg *get_obp0() = 0;
	virtual cpu::IReg *get_obp1() = 0;
	virtual cpu::IReg *get_dma() = 0;
};

} // namespace gpu
