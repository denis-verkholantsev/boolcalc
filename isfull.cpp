#include"boolexpr.h"

bool is_monotonic(const VecValuesFunc& vec) {
	unsigned long long shift = vec.size;
	while ((shift /= 2) > 0) {
		unsigned long long i = 0;
		while (i < vec.size) {
			for (unsigned long long j = shift; j--; ++i)
				if (vec.val_func[i] > vec.val_func[i + shift]) {
					return false;
				}
		}
		i += shift;
	}
	return true;
}

bool isFullSystem(std::vector<BooleanExpression> functions) {
	AVLTree<ParamValue>::iterator iter;
	bool truth = true;
	//T0
	for (BooleanExpression function : functions) {
		function.push_node();
		iter = global::Workspace.begin();
		for (size_t i = 0; i < global::Workspace.size(); ++i, ++iter) {
			(*iter).set_value(false);
		}
		if (function.calc()) {
			truth = false;
			break;
		}
		global::Workspace.clear();
	}
	if (truth) {
		return false;
	}
	truth = true;
	//T1
	for (BooleanExpression function : functions) {
		function.push_node();
		iter = global::Workspace.begin();
		for (size_t i = 0; i < global::Workspace.size(); ++i, ++iter) {
			(*iter).set_value(true);
		}
		if (!function.calc()) {
			truth = false;
			break;
		}
		global::Workspace.clear();
	}
	if (truth) {
		return false;
	}
	truth = true;
	// S
	for (BooleanExpression function : functions) {
		size_t counter = 0;
		function.push_node();
		BoolVec vec(global::Workspace.size());
		VecValuesFunc func(global::Workspace.size());
		size_t j = 0;
		bool flag = false;
		do {
			if (!flag) {
				flag = true;
			}
			else {
				++vec;
			}
			iter = global::Workspace.begin();
			for (size_t i = 0; i < global::Workspace.size(); ++i, ++iter) {
				(*iter).set_value(vec.bits[i]);
			}
			func.val_func[j++] = function.calc();
		} while (!vec.max_value());
		global::Workspace.clear();
		for (size_t i = 0; i < func.size / 2; ++i) {
			if (func.val_func[i] == func.val_func[func.size - i - 1]) {
				truth = false;
				break;
			}
		}
		if (!truth) {
			break;
		}
	}
	if (truth) {
		return false;
	}
	truth = true;
	// M
	for (BooleanExpression function : functions) {
		function.push_node();
		BoolVec vec(global::Workspace.size());
		VecValuesFunc func(global::Workspace.size());
		size_t j = 0;
		bool flag = false;
		do {
			if (!flag) {
				flag = true;
			}
			else {
				++vec;
			}
			iter = global::Workspace.begin();
			for (size_t i = 0; i < global::Workspace.size(); ++i, ++iter) {
				(*iter).set_value(vec.bits[i]);
			}
			func.val_func[j++] = function.calc();
		} while (!vec.max_value());
		global::Workspace.clear();
		if (!is_monotonic(func)) {
			truth = false;
			break;
		}
		if (!truth) {
			break;
		}
	}
	if (truth) {
		return false;
	}
	truth = true;
	//zhegalkin
	for (BooleanExpression& function : functions) {
		function.push_node();
		BooleanExpression tmp = function.zhegalkin();
		if (!function.zhegalkin().get_lineal()) {
			truth = false;
			break;
		}
		global::Workspace.clear();
	}
	if (truth) {
		return false;
	}
	return true;
}