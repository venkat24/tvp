#pragma once

#include "cpu/register/register_interface.h"

#include <cstdint>
#include <gmock/gmock.h>

using namespace std;
using namespace cpu;

class RegisterMock : public RegisterInterface {
  public:
	MOCK_METHOD1(set, void(uint8_t));
	MOCK_CONST_METHOD0(get, uint8_t());
	MOCK_METHOD2(set_bit, void(uint8_t, bool));
	MOCK_CONST_METHOD1(get_bit, bool(uint8_t));
	MOCK_METHOD0(operator_increment, void());
	MOCK_METHOD0(operator_decrement, void());
	virtual void operator++(__attribute__((unused)) int i) {
		operator_increment();
	}
	virtual void operator++() { operator_increment(); }
	virtual void operator--(__attribute__((unused)) int i) {
		operator_decrement();
	}
	virtual void operator--() { operator_decrement(); }
};

class DoubleRegisterMock : public DoubleRegisterInterface {
  public:
	MOCK_METHOD1(set, void(uint16_t));
	MOCK_CONST_METHOD0(get, uint16_t());
	MOCK_CONST_METHOD0(get_high, uint8_t());
	MOCK_CONST_METHOD0(get_low, uint8_t());
	MOCK_METHOD2(set_bit, void(uint8_t, bool));
	MOCK_CONST_METHOD1(get_bit, bool(uint8_t));
	MOCK_METHOD0(operator_increment, void());
	MOCK_METHOD0(operator_decrement, void());
	virtual void operator++(__attribute__((unused)) int i) {
		operator_increment();
	}
	virtual void operator++() { operator_increment(); }
	virtual void operator--(__attribute__((unused)) int i) {
		operator_decrement();
	}
	virtual void operator--() { operator_decrement(); }
};
