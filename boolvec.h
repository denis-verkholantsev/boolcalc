#pragma once
struct BoolVec {
	bool* bits;
	unsigned int size;
	BoolVec(unsigned int size_space): size(size_space) {
		bits = new bool[size];
		for (size_t i = 0; i < size; ++i) {
			bits[i] = false;
		}
	}
	~BoolVec() {
		delete[] bits;
	}
	BoolVec& operator++() {
		for (size_t i = size; i > 0; --i) {
			if ((bits[i-1] = bits[i-1] ^ 1) == true) 
				break;
		}
		return *this;
	}
	void to_null() {
		for (size_t i = 0; i < size; ++i) {
			bits[i] = false;
		}
	}
	bool max_value() {
		for (size_t i = 0; i < size; ++i) {
			if (bits[i] == false)
				return false;
		}
		return true;
	}
};

struct VecValuesFunc {
	bool* val_func;
	unsigned long long size;
	VecValuesFunc() : size(0), val_func(nullptr) {};
	VecValuesFunc(unsigned int number_of_variables) {
		size = 1;
		for (size_t i = 0; i < number_of_variables; ++i) {
			size *= 2;
		}
		val_func = new bool[size];
	};
	void xor_neighbors() {
		bool* tmp = new bool[size - 1];
		for (size_t i = 0; i < size-1; ++i) {
			tmp[i] = val_func[i] ^ val_func[i + 1];
		}
		delete[] val_func;
		val_func = tmp;
		tmp = nullptr;
		delete[] tmp;
	}
	~VecValuesFunc() {
		delete[] val_func;
	}
};