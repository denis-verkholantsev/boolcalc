#include"boolexpr.h"
#include"boolvec.h"

const unsigned char ActionsTable[][7] = {
	 { 0, 2, 5, 1, 2, 2, 2},
	 { 3, 2, 3, 1, 3, 2, 3},
	 { 5, 2, 4, 1, 2, 2, 2},
	 { 3, 2, 3, 1, 3, 2, 3},
	 { 3, 2, 3, 1, 2, 2, 3} 
};

int ActionsRowNumber(char ch) {
	switch (ch)
	{
	case   0: return 0;
	case '&': return 1;
	case '(': return 2;
	case '~': return 3;
	}
	return 4;
}

bool CheckLogicSymbols(unsigned char ch, size_t num) {
	const char buf[9] = "^v|<>+=&";
	for (size_t i = 0; i < 8 - num; ++i) {
		if (buf[i] == ch) return true;
	}
	return false;
}

int ActionsColNumber(char ch) {
	switch (ch)
	{
	case 0:   return 0;
	case '(': return 1;
	case ')': return 2;
	case '&': return 4;
	case '~': return 5;
	}
	if (CheckLogicSymbols(ch,1)) {
		return 6;
	}
	return 3;
}
void BooleanExpression::InfixFilter_(const char* instr, char* outstr) {
	size_t i = 0, j = 0;
	size_t open_brackets = 0, close_brackets = 0;
	char ch;
	int index = 0;
	size_t buflen = 0;
	char buf[256];
	bool flag = 0;
	while ((ch = instr[i++]) != '\0') {
		if (ch == ' ' || ch == '\t'){
			continue;
		}
		if (ch == 'x' && instr[i]>'0' && instr[i]<='9' 
			&& !(instr[i+1] > '0' && instr[i+1] <= '9')) {
			outstr[j++] = ch;
			outstr[j++] = instr[i++];
			flag = true;
			continue;
		}
		else if (ch == 'x' && instr[i] > '0' && instr[i] <= '9'
			&& instr[i + 1] > '0' && instr[i + 1] <= '9') {
			throw "too big index";
		}
		else if (ch == 'x' && !(instr[i + 1] > '0' && instr[i + 1] <= '9')) {
			throw "Error in expression";
		}
		if (ch == '0' || ch == '1') {
			flag = true;
		}
		outstr[j++] = ch;
	}
	if (!flag) throw "No variables or constants";
	outstr[j] = '\0';
}

void BooleanExpression::Infix2Postfix(const char* instr, char* outstr) {
	size_t i = 0, j = 0;
	int row, col;
	unsigned char action;
	std::stack<char> stack;
	do {
		col = ActionsColNumber(instr[i]);
		row = stack.empty() ? 0 : ActionsRowNumber(stack.top());
		action = ActionsTable[row][col];
		switch (action) {
		case 0:
			outstr[j] = '\0';
			break;
		case 1:
			if (instr[i] == 'x') {
				outstr[j] = 'x';
				++j; ++i;
				outstr[j] = instr[i];
				++i; ++j;
			}
			else {
				if (instr[i] == '0') {
					outstr[j] = 'y';
				}
				else if (instr[i] == '1') {
					outstr[j] = 'z';
				}
				++j;
				++i;
			}
			break;
		case 2:
			stack.push(instr[i]);
			++i;
			break;
		case 3:
			outstr[j] = stack.top();
			++j;
			stack.pop();
			break;
		case 4:
			stack.pop();
			++i;
			break;
		case 5:
			throw "Error";
			break;
		}
	} while (action != 0);
}

BoolNode* BooleanExpression::Postfix2Tree(const char* str) {
	size_t i = 0;
	std::stack<BoolNode*> stack;
	char ch, ind;
	BoolNode* result, * left, * right, * next_node;
	result = left = right = next_node = nullptr;
	try {
		while ((ch = str[i]) != '\0') {
			left = right = next_node = nullptr;
			if (CheckLogicSymbols(ch, 0)) {
				if (stack.size() > 1) {
					right = stack.top();
					stack.pop();
					left = stack.top();
					stack.pop();
				}
				else {
					throw - 1;
				}
				switch (ch) {
				case '&':
					result = new AndNode(left, right);
					break;
				case 'v':
					result = new OrNode(left, right);
					break;
				case '>':
					result = new ImplicationNode(left, right);
					break;
				case '<':
					result = new ReverseImplicationNode(left, right);
					break;
				case '+':
					result = new XorNode(left, right);
					break;
				case '=':
					result = new EqualNode(left, right);
					break;
				case '|':
					result = new NandNode(left, right);
					break;
				case '^':
					result = new NorNode(left, right);
					break;
				}
			}
			else if (ch == '~') {
				if (stack.empty()) {
					throw - 1;
				}
				left = stack.top();
				stack.pop();
				result = new NotNode(left);
			}
			else {
				switch (ch) {
				case 'y':
					result = new ConstantNode(0);
					break;
				case 'z':
					result = new ConstantNode(1);
					break;
				case 'x':
					ind = str[++i];
					result = new ParamNode(ind - '0');
					break;
				default:
					throw 2;
				}
			}
			stack.push(result);
			++i;
		}
		if (stack.size() > 1)
		{
			left = right = nullptr;
			throw 2;
		}
		result = stack.top();
		return result;
	}
	catch (int e)
	{
		while (!stack.empty())
		{
			left = stack.top();
			stack.pop();
		}
		if (left) delete left;
		if (right) delete right;
		throw "Error in postfix expression";
	}
}

BooleanExpression::BooleanExpression(const BooleanExpression& X) {
	root = X.root->clone();
	lineal = X.lineal;
}
BooleanExpression& BooleanExpression::operator=(const BooleanExpression& X) {
	delete root;
	root = nullptr;
	root = X.root->clone();
	lineal = X.lineal;
	return *this;
}

BooleanExpression::BooleanExpression(const char* str) {
	size_t Len = strlen(str);
	Len = (Len < 10) ? 20 : 2 * Len;
	char* str_infix = new char[Len];
	char* str_postfix = new char[Len];
	try {
		InfixFilter_(str, str_infix);
		Infix2Postfix(str_infix, str_postfix);
		root = Postfix2Tree(str_postfix);
	}
	catch (int e) {
		delete[] str_infix;
		str_infix = nullptr;
		delete[] str_postfix;
		root = nullptr;
		str_postfix = nullptr;
		throw "Invalid expression";
	}
	delete[] str_infix;
	delete[] str_postfix;
}
BooleanExpression::BooleanExpression(BoolNode* X){
	root = X;
}
BooleanExpression::BooleanExpression(BoolNode* X, bool lin) {
	root = X;
	lineal = lin;
}

void BooleanExpression::set_lineal(bool X) {
	lineal = X;
}
bool BooleanExpression::get_lineal() const {
	return lineal;
}

bool BooleanExpression::calc() {
	if (root)
		return root->calc();
	return false;
}

BooleanExpression::operator std::string() const {
	if (root) {
		return root->str();
	}
	return std::string("");
}

void BooleanExpression::push_node() {
	if (root)
		root->push_node();
}

bool BooleanExpression::TruthTable() {
	this->push_node();
	if (global::Workspace.size() == 0) {
		std::cout << operator std::string() << std::endl;
		std::cout << root->calc();
		return true;
	}
	BoolVec value_vec(global::Workspace.size());
	AVLTree<ParamValue>::iterator iter = global::Workspace.begin();
	for (size_t i = 0; i < global::Workspace.size(); ++i) {
		std::cout << (*iter).str() << "\t";
		++iter;
	}
	std::cout << operator std::string() << std::endl;
	while(true){
		iter = global::Workspace.begin();
		for (size_t i = 0; i < global::Workspace.size(); ++i) {
			std::cout << value_vec.bits[i] << "\t";
			(*iter).set_value(value_vec.bits[i]);
			++iter;
		}
		std::cout << this->calc() << std::endl;
		if (value_vec.max_value()) {
			break;
		}
		++value_vec;
	}
	std::cout << std::endl;
	return false;
}

BooleanExpression BooleanExpression::dnf() {
	this->push_node();
	BoolNode* left = nullptr, *right = nullptr;
	AVLTree<ParamValue>::iterator iter;
	BoolVec value_vec(global::Workspace.size());
	std::stack<BoolNode*> stack_and;
	std::stack<BoolNode*> stack_or;
	bool flag = false, flag_push = false;
	do {
		if (!flag) {
			flag = true;
		}
		else {
			++value_vec;
		}
		iter = global::Workspace.begin();
		for (size_t i = 0; i < global::Workspace.size(); ++i, ++iter) {
			(*iter).set_value(value_vec.bits[i]);
		}
		if (this->calc()) {
			if (stack_and.size() > 0)
				stack_and.pop();
		iter = global::Workspace.begin();
			for (size_t i = 0; i < global::Workspace.size(); ++i, ++iter) {
				if ((*iter).get_value()) {
					stack_and.push(new ParamNode((*iter).get_index()));
				}
				else {
					stack_and.push(new NotNode(new ParamNode((*iter).get_index())));
				}
				flag_push = true;
				if (stack_and.size() == 2) {
					right = stack_and.top();
					stack_and.pop();
					left = stack_and.top();
					stack_and.pop();
					stack_and.push(new AndNode(left, right));
				}
			}
			if (stack_and.size() == 0) {
				return *this;
			}
			stack_or.push(stack_and.top());
			if (stack_or.size() == 2) {
				left = stack_or.top();
				stack_or.pop();
				right = stack_or.top();
				stack_or.pop();
				stack_or.push(new OrNode(left, right));
			}
		}
	} while (!value_vec.max_value());
	if (!flag_push) {
		BooleanExpression res(new ConstantNode(false));
		return res;
	}
	else {
		global::Workspace.clear();
		BooleanExpression res(stack_or.top());
		return res;
	}
}

BooleanExpression BooleanExpression::cnf() {
	this->push_node();
	BoolNode* left = nullptr, * right = nullptr;
	AVLTree<ParamValue>::iterator iter;
	BoolVec value_vec(global::Workspace.size());
	std::stack<BoolNode*> stack_and;
	std::stack<BoolNode*> stack_or;
	bool flag = false, flag_push = false;
	do {
		if (!flag) {
			flag = true;
		}
		else {
			++value_vec;
		}
		iter = global::Workspace.begin();
		for (size_t i = 0; i < global::Workspace.size(); ++i, ++iter) {
			(*iter).set_value(value_vec.bits[i]);
		}
		if (!this->calc()) {
			if (stack_or.size() > 0)
				stack_or.pop();
			iter = global::Workspace.begin();
			for (size_t i = 0; i < global::Workspace.size(); ++i, ++iter) {
				if (!(*iter).get_value()) {
					stack_or.push(new ParamNode((*iter).get_index()));
				}
				else {
					stack_or.push(new NotNode(new ParamNode((*iter).get_index())));
				}
				flag_push = true;
				if (stack_or.size() == 2) {
					right = stack_or.top();
					stack_or.pop();
					left = stack_or.top();
					stack_or.pop();
					stack_or.push(new OrNode(left, right));
				}
			}
			if (stack_or.size()==0) {
				return *this;
			}
			stack_and.push(stack_or.top());
			if (stack_and.size() == 2) {
				left = stack_and.top();
				stack_and.pop();
				right = stack_and.top();
				stack_and.pop();
				stack_and.push(new AndNode(left, right));
			}
		}
	} while (!value_vec.max_value());
	if (!flag_push) {
		BooleanExpression res(new ConstantNode(true));
		return res;
	}
	else {
		global::Workspace.clear();
		BooleanExpression res(stack_and.top());
		return res;
	}
}

BooleanExpression BooleanExpression::zhegalkin() {
	AVLTree<ParamValue>::iterator iter;
	this->push_node();
	BoolVec vec(global::Workspace.size());
	VecValuesFunc func(global::Workspace.size());
	size_t j = 0;
	set_lineal(true);
	bool flag = false, flag_push = false;
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
		func.val_func[j++] = this->calc();
	} while (!vec.max_value());
	std::stack<BoolNode*> stack_and;
	std::stack<BoolNode*> stack_xor;
	BoolNode* left = nullptr, * right = nullptr;
	vec.to_null();
	flag = false;
	do {
		if (!flag) {
			flag = true;
		}
		else {
			++vec;
		}
		if (func.val_func[0]) {
			iter = global::Workspace.begin();
			for (size_t i = 0; i < global::Workspace.size(); ++i,++iter) {
				(*iter).set_value(vec.bits[i]);
			}
			if (stack_and.size())
				stack_and.pop();
			iter = global::Workspace.begin();
			for (size_t i = 0; i < global::Workspace.size(); ++i,++iter) {
				if ((*iter).get_value()) {
					stack_and.push(new ParamNode((*iter).get_index()));
					flag_push = true;
					if (stack_and.size() == 2) {
						right = stack_and.top();
						stack_and.pop();
						left = stack_and.top();
						stack_and.pop();
						stack_and.push(new AndNode(left, right));
						set_lineal(false);
					}
				}
			}
			if (stack_and.size()) {
				stack_xor.push(stack_and.top());
			}
			else {
				stack_xor.push(new ConstantNode(true));
			}
			if (stack_xor.size() == 2) {
				right = stack_xor.top();
				stack_xor.pop();
				left = stack_xor.top();
				stack_xor.pop();
				stack_xor.push(new XorNode(left, right));
			}
		}
		func.xor_neighbors();
	} while (!vec.max_value());
	if (!flag_push) {
		if (global::Workspace.size()==0) {
			BooleanExpression res(new ConstantNode(this->calc()), true);
			return res;
		}
	}
	global::Workspace.clear();
	BooleanExpression res(stack_xor.top(), lineal);
	return res;
}








