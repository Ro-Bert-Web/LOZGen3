#include "KBox.h"
#include <stdexcept>

KBox::KBox(int size) : size{ size } {
	state = std::vector<KVal>(size, X);
}

KBox::KBox(KState state) : size{ state.size } {
	this->state = std::vector<KVal>(size, X);
	for (int i = 0; i < size; i++)
		this->state[i] = state.get(i) ? T : F;
}

KBox& KBox::operator=(KBox const& b) {
	if (size != b.size)
		throw std::length_error("Sizes differ");
	state = b.state;
	return *this;
}

KVal KBox::get(int index) const {
	if (index >= size || index < 0)
		throw std::out_of_range("Index out of bounds");
	return state[index];
}

void KBox::set(int index, KVal value) {
	if (index >= size || index < 0)
		throw std::out_of_range("Index out of bounds");
	state[index] = value;
}

bool KBox::contains(KBox const& b) const {
	if (size != b.size)
		throw std::length_error("Sizes differ");

	for (int i = 0; i < size; i++) {
		if (b.get(i).maybe() == T && get(i).maybe() == F)
			return false;
		if ((~b.get(i)).maybe() == T && (~get(i)).maybe() == F)
			return false;
	}
	return true;
}

int KBox::specificity() const {
	int rslt = 0;
	for (int i = 0; i < size; i++)
		if (state[i] != X)
			rslt++;
	return rslt;
}

bool KBox::operator==(KBox const& b) const {
	if (size != b.size)
		return false;
	for (int i = 0; i < size; i++)
		if (get(i) != b.get(i))
			return false;
	return true;
}

bool KBox::operator!=(KBox const& b) const {
	return !operator==(b);
}

size_t KBox::hash() const {
	size_t hash = 0;

	std::hash<int> intHash;
	std::hash<KVal> kValHash;

	for (int i = 0; i < size; i++)
		hash ^= kValHash(get(i)) ^ intHash(i);

	return hash;
}