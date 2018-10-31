#ifndef TEST_MEMORY_MOCKS_MEMORY_MOCK_H
#define TEST_MEMORY_MOCKS_MEMORY_MOCK_H

#include "memory/memory_interface.h"

#include <cstdint>
#include <gmock/gmock.h>

using namespace std;
using namespace memory;

class MemoryMock : public MemoryInterface {
	MOCK_CONST_METHOD1(read, uint8_t(Address));
	MOCK_METHOD2(write, void(Address, uint8_t));
};

#endif
