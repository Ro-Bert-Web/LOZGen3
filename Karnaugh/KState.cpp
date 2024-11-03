#include "KState.h"
#include <stdexcept>

KState::KState(int size, int state) : size{ size }, state{ state } {}

bool KState::get(int index) const {
	if (index >= size || index < 0)
		throw std::out_of_range("Index out of bounds");
	return (state >> index) & 1;
}

void KState::set(int index) {
	if (index >= size || index < 0)
		throw std::out_of_range("Index out of bounds");
	state |= 1 << index;
}

void KState::set(int index, bool value) {
	if (value)
		set(index);
	else
		clr(index);
}

void KState::clr(int index) {
	if (index >= size || index < 0)
		throw std::out_of_range("Index out of bounds");
	state &= ~(1 << index);
}

void KState::toggle(int index) {
	if (index >= size || index < 0)
		throw std::out_of_range("Index out of bounds");
	state ^= 1 << index;
}

KState& KState::operator=(KState const& b) {
	if (size != b.size)
		throw std::length_error("Sizes differ");
	state = b.state;
	return *this;
}

bool KState::operator==(KState const& b) const {
	return size == b.size && state == b.state;
}

bool KState::operator!=(KState const& b) const {
	return !operator==(b);
}

size_t KState::hash() const {
	return std::hash<int>()(state) ^ std::hash<int>()(size);
}
