#pragma once
#include"boolexpr.h"
class VectorBE {
	BooleanExpression* vec;
	unsigned int size;
public:
	VectorBE() : vec(nullptr), size(0) {};
	void push_back(BooleanExpression& X) {
		BooleanExpression* tmp = new BooleanExpression[size + 1];
		for (size_t i = 0; i < size; ++i) {
			tmp[i] = vec[i];
		}
		tmp[size++] = X;
		delete[] vec;
		vec = tmp;
		tmp = nullptr;
		delete[] tmp;
	};
	unsigned int size() const {
		return size;
	}
	BooleanExpression& operator[](unsigned int i) const {
		return vec[i];
	}
};