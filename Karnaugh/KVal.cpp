#include "KVal.h"

KVal::KVal(KVal_Val value) : value{ value } {}

KVal& KVal::operator=(KVal const& b) {
	value = b.value;
	return *this;
}

KVal& KVal::operator&=(KVal const& b) {
	if (value == T && b == T) {
		value = T;
	}
	else if (value == F || b == F) {
		value = F;
	}
	else {
		value = X;
	}
	return *this;
}

KVal& KVal::operator|=(KVal const& b) {
	if (value == T || b == T) {
		value = T;
	}
	else if (value == F && b == F) {
		value = F;
	}
	else {
		value = X;
	}
	return *this;
}

KVal& KVal::operator^=(KVal const& b) {
	if (value == X || b == X) {
		value = X;
	}
	else {
		value = (value == b.value) ? F : T;
	}
	return *this;
}

KVal KVal::operator~() const {
	if (value == T) {
		return F;
	}
	else if (value == F) {
		return T;
	}
	else {
		return X;
	}
}

KVal KVal::operator&(KVal const& b) const {
	if (value == T && b == T) {
		return T;
	}
	else if (value == F || b == F) {
		return F;
	}
	else {
		return X;
	}
}

KVal KVal::operator|(KVal const& b) const {
	if (value == T || b == T) {
		return T;
	}
	else if (value == F && b == F) {
		return F;
	}
	else {
		return X;
	}
}

KVal KVal::operator^(KVal const& b) const {
	if (value == X || b == X) {
		return X;
	}
	else {
		return (value == b.value) ? F : T;
	}
}

bool KVal::operator==(KVal const& b) const {
	return value == b.value;
}

bool KVal::operator!=(KVal const& b) const {
	return value != b.value;
}

KVal KVal::exact() const {
	return (value == X) ? F : value;
}

KVal KVal::maybe() const {
	return (value == X) ? T : value;
}

size_t KVal::hash() const {
	return std::hash<size_t>()(value);
}
