#include "register.h"

namespace cpu {

// 8-bit Register
Register::Register() : _value(static_cast<uint8_t>(0)) {}
Register::Register(uint8_t value) : _value(value) {}

void Register::set(uint8_t value) { _value = value; }

void Register::set_bit(uint8_t bit, bool value) {
	if (value) {
		_value = _value | (1 << bit);
	} else {
		_value = _value & ~(1 << bit);
	}
}

uint8_t Register::get() const { return _value; }

bool Register::get_bit(uint8_t bit) const {
	return static_cast<bool>(_value & (1 << bit));
}

void Register::operator++() { _value++; }

void Register::operator++(int) { _value++; }

void Register::operator--() { _value--; }

void Register::operator--(int) { _value--; }

// 16-bit Register
DoubleRegister::DoubleRegister() : _value(static_cast<uint16_t>(0)) {}
DoubleRegister::DoubleRegister(uint16_t value) : _value(value) {}

void DoubleRegister::set(uint16_t value) { _value = value; }

void DoubleRegister::set_bit(uint8_t bit, bool value) {
	if (value) {
		_value = _value | (1 << bit);
	} else {
		_value = _value & ~(1 << bit);
	}
}

uint16_t DoubleRegister::get() const { return _value; }

bool DoubleRegister::get_bit(uint8_t bit) const {
	return static_cast<bool>(_value & (1 << bit));
}

void DoubleRegister::operator++() { _value++; }

void DoubleRegister::operator++(int) { _value++; }

void DoubleRegister::operator--() { _value--; }

void DoubleRegister::operator--(int) { _value--; }

// PairRegister
PairRegister::PairRegister(RegisterInterface *first, RegisterInterface *second)
    : _first(first), _second(second) {}

void PairRegister::set(uint16_t value) {
	auto higher = static_cast<uint8_t>(value >> 8);
	auto lower = static_cast<uint8_t>(value & 0x00FF);

	_first->set(higher);
	_second->set(lower);
}

void PairRegister::set_bit(uint8_t bit, bool value) {
	if (value) {
		if (bit <= 7) {
			auto value = _second->get();
			value = value | (1 << bit);
			_second->set(value);
		} else {
			auto value = _first->get();
			value = value | (1 << (bit - 8));
			_first->set(value);
		}
	} else {
		if (bit <= 7) {
			auto value = _second->get();
			value = value & ~(1 << bit);
			_second->set(value);
		} else {
			auto value = _first->get();
			value = value & ~(1 << (bit - 8));
			_first->set(value);
		}
	}
}

uint16_t PairRegister::get() const {
	return (_first->get() << 8) + (_second->get());
}

bool PairRegister::get_bit(uint8_t bit) const {
	if (bit <= 7) {
		return static_cast<bool>(_second->get() & (1 << bit));
	} else {
		return static_cast<bool>(_first->get() & (1 << (bit - 8)));
	}
}

void PairRegister::operator++() { this->set(this->get() + 1); }

void PairRegister::operator++(int) { this->set(this->get() + 1); }

void PairRegister::operator--() { this->set(this->get() - 1); }

void PairRegister::operator--(int) { this->set(this->get() - 1); }

} // namespace cpu
