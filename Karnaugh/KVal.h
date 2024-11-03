#pragma once

#include <functional>

enum KVal_Val {
	X = 0,
	T,
	F
};

class KVal {
	KVal_Val value;
public:
	KVal(KVal_Val value = X);

	KVal& operator=(KVal const& b);
	KVal& operator&=(KVal const& b);
	KVal& operator|=(KVal const& b);
	KVal& operator^=(KVal const& b);

	KVal operator~() const;
	KVal operator&(KVal const& b) const;
	KVal operator|(KVal const& b) const;
	KVal operator^(KVal const& b) const;

	bool operator==(KVal const& b) const;
	bool operator!=(KVal const& b) const;

	KVal exact() const;
	KVal maybe() const;

	size_t hash() const;
};

template<>
struct std::hash<KVal> {
	size_t operator()(const KVal& value) const noexcept {
		return value.hash();
	}
};
