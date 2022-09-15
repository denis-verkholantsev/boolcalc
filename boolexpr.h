#pragma once
#include<iostream>
#include<string>
#include<stack>
#include<vector>
#include<cstring>
#include"BoolNode.h"
#include"boolexpr.h"
#include"boolvec.h"

class BooleanExpression {
	void InfixFilter_(const char*, char*);
	void Infix2Postfix(const char*, char*);
	BoolNode* Postfix2Tree(const char*);
	BoolNode* root;
	bool lineal;
public:
	BooleanExpression() :root(nullptr) {};
	BooleanExpression(const char*);
	BooleanExpression(const BooleanExpression&);
	BooleanExpression(BoolNode*);
	BooleanExpression(BoolNode*, bool);
	BooleanExpression& operator=(const BooleanExpression&);
	void set_lineal(bool);
	bool get_lineal() const;
	bool calc();
	bool TruthTable();
	BooleanExpression cnf();
	BooleanExpression dnf();
	BooleanExpression zhegalkin();
	operator std::string() const;
	void push_node();
};
bool isFullSystem(std::vector<BooleanExpression>);