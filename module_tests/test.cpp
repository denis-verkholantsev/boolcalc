#include "pch.h"
#include"C:\Users\dverh\source\repos\boolcalc\boolexpr.h"
#include"C:\Users\dverh\source\repos\boolcalc\boolexpr.cpp"
#include"C:\Users\dverh\source\repos\boolcalc\avltree.h"
#include"C:\Users\dverh\source\repos\boolcalc\global.cpp"
#include"C:\Users\dverh\source\repos\boolcalc\boolvec.h"

TEST(DnfTest, correctness) {
	BooleanExpression a("x1^x2"), b("x1|x2"),c("~x1"), d("x1>x2"), e("1+x3vx2");
	EXPECT_EQ(a.dnf().operator std::string(), (std::string)"(~(x1)) & (~(x2))");
	EXPECT_EQ((std::string)b.dnf(), (std::string)"((x1) & (~(x2))) v (((~(x1)) & (x2)) v ((~(x1)) & (~(x2))))");
	EXPECT_EQ((std::string)c.dnf(), (std::string)"~(x1)");
	EXPECT_EQ((std::string)d.dnf(), (std::string)"((x1) & (x2)) v (((~(x1)) & (x2)) v ((~(x1)) & (~(x2))))");
	EXPECT_EQ((std::string)e.dnf(), (std::string)"((x2) & (x3)) v (((x2) & (~(x3))) v ((~(x2)) & (~(x3))))");
}

TEST(CnfTest, correctness) {
	BooleanExpression a("x1^x2"), b("x1|x2"), c("~x1"), d("x1>x2"), e("1+x3vx2");
	EXPECT_EQ((std::string)a.cnf(), (std::string)"((~(x1)) v (~(x2))) & (((~(x1)) v (x2)) & ((x1) v (~(x2))))");
	EXPECT_EQ((std::string)b.cnf(), (std::string)"(~(x1)) v (~(x2))");
	EXPECT_EQ((std::string)c.cnf(), (std::string)"~(x1)");
	EXPECT_EQ((std::string)d.cnf(), (std::string)"(~(x1)) v (x2)");
	EXPECT_EQ((std::string)e.cnf(), (std::string)"(x2) v (~(x3))");
}

TEST(Zhigalkintest, correctness) {
	BooleanExpression a("x1^x2"), b("x1|x2"), c("~x1"), d("x1>x2"), e("1+x3vx2");
	EXPECT_EQ((std::string)a.zhegalkin(), (std::string)"(((1) + (x2)) + (x1)) + ((x1) & (x2))");
	EXPECT_EQ((std::string)b.zhegalkin(), (std::string)"(1) + ((x1) & (x2))");
	EXPECT_EQ((std::string)c.zhegalkin(), (std::string)"(1) + (x1)");
	EXPECT_EQ((std::string)d.zhegalkin(), (std::string)"((1) + (x1)) + ((x1) & (x2))");
	EXPECT_EQ((std::string)e.zhegalkin(), (std::string)"((1) + (x3)) + ((x2) & (x3))");
}

TEST(CalcAndBoolvec, correctness) {
	BooleanExpression a("x2+x3+x1");
	a.push_node();
	BoolVec vec(global::Workspace.size());
	auto iter = global::Workspace.begin();
	for (size_t i = 0; i < global::Workspace.size(); ++i,++iter) {
		(*iter).set_value(vec.bits[i]);
	}
	EXPECT_EQ(a.calc(), false);
	++vec;
	iter = global::Workspace.begin();
	for (size_t i = 0; i < global::Workspace.size(); ++i, ++iter) {
		(*iter).set_value(vec.bits[i]);
	}
	EXPECT_EQ(a.calc(), true);
	global::Workspace.clear();
}

TEST(Calc, correctness) {
	bool values[] = { true,false,true,true,true,true,true,false };
	BooleanExpression a("x1 + (~x2) | x3");
	a.push_node();
	BoolVec vec(global::Workspace.size());
	for (size_t j = 0; j < 8; ++j) {
		auto iter = global::Workspace.begin();
		for (size_t i = 0; i < global::Workspace.size(); ++i, ++iter) {
			(*iter).set_value(vec.bits[i]);
		}
		EXPECT_EQ(a.calc(),values[j]);
		++vec;
	}
	global::Workspace.clear();
}

TEST(ErrorsInExpressions, incorrectness) {
	EXPECT_ANY_THROW(BooleanExpression("x1u"));
	EXPECT_ANY_THROW(BooleanExpression("~3"));
	EXPECT_ANY_THROW(BooleanExpression("x1++x2+x7"));
	EXPECT_ANY_THROW(BooleanExpression("x1+x2v"));
	EXPECT_ANY_THROW(BooleanExpression("x1w+x2"));
	EXPECT_ANY_THROW(BooleanExpression("x1~x2"));
}

TEST(ConstantsCalc, correctness) {
	BooleanExpression a("0");
	BooleanExpression b("1");
	BooleanExpression c("~0");
	EXPECT_EQ(a.calc(), false);
	EXPECT_EQ(b.calc(), true);
	EXPECT_EQ(c.calc(), true);
}