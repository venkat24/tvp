/**
 * @file memory_interface.h
 * Declares the interface for accessing main memory
 */

#include "cpu/cpu_interface.h"
#include "gpu/gpu_interface.h"
#include "memory/utils.h"
#include <cstdint>

#pragma once

namespace memory {

class MemoryInterface {
  public:
	/**
	 * Virtual Destructor
	 */
	virtual ~MemoryInterface(){};

	/**
	 * Read a byte from the specified location
	 *
	 * @param address Location to read byte from
	 * @return Value of the byte
	 */
	virtual uint8_t read(Address address) const = 0;

	/**
	 * Write a byte to the specified location
	 *
	 * @param address Location to write byte to
	 * @param data Value of the byte
	 */
	virtual void write(Address address, uint8_t data) = 0;

	/**
	 * Set the CPU Object pointer for this class
	 */
	virtual void set_cpu(cpu::CPUInterface *cpu) = 0;

	/**
	 * Set the GPU Object pointer for this class
	 */
	virtual void set_gpu(gpu::GPUInterface *gpu) = 0;
};

} // namespace memory
