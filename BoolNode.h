#pragma once
#include<string>
#include<iostream>
#include"avltree.h"
class BoolNode {
public:
    virtual bool calc() const = 0;
    virtual void push_node() = 0;
    virtual std::string str() const = 0;
    virtual BoolNode* clone() const = 0;
    virtual ~BoolNode() {}
};

class ParamValue {
    unsigned char index;
    bool value;
public:
    ParamValue(unsigned char symbol, bool input_value) : index(symbol), value(input_value) {}
    ParamValue(unsigned char symbol) : index(symbol), value(false) {}
    bool get_value() const {
        return value;
    }
    unsigned char get_index() const {
        return index;
    }
    void set_value(bool input_value) {
        value = input_value;
    }
    bool operator<(const ParamValue& X) const {
        return index < X.index;
    }
    bool operator==(const ParamValue& X) const {
        return index == X.index;
    }
    std::string str() const {
        return (std::string)("x" + std::to_string(index));
    }
};

namespace global {
    extern AVLTree<ParamValue> Workspace;
}

class ParamNode : public BoolNode {
    char index;
public:
    ParamNode(unsigned char symbol) : index(symbol) {}
    ParamNode(const char* str) {
        index = 0;
        unsigned char ch;
        size_t i = 0;
        if (str[i++] != 'x') {
            throw std::invalid_argument("Invalid arg");
        }
        while ((ch = str[i++]) != '\0') {
            if (ch < '0' || ch > '9') {
                throw std::invalid_argument("Invalid arg");
            }
            index = index * 10 + (ch - '0');
        }
    }
    bool calc() const {
        AVLTree<ParamValue>::iterator iter;
        if ((iter = global::Workspace.find(ParamValue(index, false))) != global::Workspace.end()) {
            return (*iter).get_value();
        }
        else {
            bool value_of_var;
            std::cout << "x" << (unsigned int)index << " = ";
            std::cin >> value_of_var;
            std::cin.ignore();
            global::Workspace.insert(ParamValue(index, value_of_var));
            return value_of_var;
        }
    }
    void push_node() {
        AVLTree<ParamValue>::iterator iter1, iter2;
        if ((iter1 = global::Workspace.find(ParamValue(index, false))) == global::Workspace.end() &&
            (iter2 = global::Workspace.find(ParamValue(index, false))) == global::Workspace.end())
        {
            global::Workspace.insert(ParamValue(index, false));
        }
    }
    std::string str() const {
        return (std::string)("x" + std::to_string(index));
    }
    BoolNode* clone() const {
        return new ParamNode(index);
    }
};

class ConstantNode : public BoolNode {
    bool value;
public:
    ConstantNode(bool X) : value(X) {}
    bool calc() const {
        return value;
    }
    void push_node() {
        return;
    }
    std::string str() const {
        return std::to_string(value);
    }
    BoolNode* clone() const {
        return new ConstantNode(value);
    }
};

class BinNode : public BoolNode {
    protected:
    BoolNode* left, * right;
public:
    BinNode(BoolNode* L, BoolNode* R) : left(L), right(R) {}
    ~BinNode() {
        if (left) {
            delete left;
        }
        if (right) {
            delete right;
        }
        left = right = nullptr;
    }
    void push_node() {
        left->push_node();
        right->push_node();
    }
};

class UnarNode : public BoolNode {
protected:
    BoolNode* centre;
public:
    UnarNode(BoolNode* for_centre) : centre(for_centre) {}
    ~UnarNode() {
        if (centre) {
            delete centre;
        }
        centre = nullptr;
    }
    void push_node() {
        centre->push_node();
    }
};

class NotNode : public UnarNode {
public:
    NotNode(BoolNode* R) : UnarNode(R) {}
    bool calc() const {
        return !(bool)(centre->calc());
    }
    std::string str() const {
        return (std::string)("~(" + centre->str() + ")" );
    }
    BoolNode* clone() const {
        return new NotNode(centre->clone());
    }
};

class AndNode : public BinNode {
public:
    AndNode(BoolNode* L, BoolNode* R) : BinNode(L, R) {}
    bool calc() const {
        return (bool)(left->calc() & right->calc());
    }
    std::string str() const {
        return (std::string)("(" + left->str() + ") & (" + right->str() + ")");
    }
    BoolNode* clone() const {
        return new AndNode(left->clone(), right->clone());
    }
};

class OrNode : public BinNode {
public:
    OrNode(BoolNode* L, BoolNode* R) : BinNode(L, R) {}
    bool calc() const {
        return (bool)(left->calc() + right->calc());
    }
    std::string str() const {
        return (std::string)("(" + left->str() + ") v (" + right->str() + ")");
    }
    BoolNode* clone() const {
        return new OrNode(left->clone(), right->clone());
    }
};

class ImplicationNode : public BinNode {
public:
    ImplicationNode(BoolNode* L, BoolNode* R) : BinNode(L, R) {}
    bool calc() const {
        bool a = left->calc(), b = right->calc();
        if (a && !b) {
            return false;
        }
        return true;
    }
    std::string str() const {
        return (std::string)("(" + left->str() + ") > (" + right->str() + ")");
    }
    BoolNode* clone() const {
        return new ImplicationNode(left->clone(), right->clone());
    }
};

class ReverseImplicationNode : public BinNode {
public:
    ReverseImplicationNode(BoolNode* L, BoolNode* R) : BinNode(L, R) {}
    bool calc() const {
        bool a = left->calc(), b = right->calc();
        if (!a && b) {
            return false;
        }
        return true;
    }
    std::string str() const {
        return (std::string)("(" + left->str() + ") < (" + right->str() + ")");
    }
    BoolNode* clone() const {
        return new ReverseImplicationNode(left->clone(), right->clone());
    }
};


class XorNode : public BinNode {
public:
    XorNode(BoolNode* L, BoolNode* R) : BinNode(L, R) {}
    bool calc() const {
        return (bool)(((unsigned int)left->calc() + (unsigned int)right->calc()) % 2);
    }
    std::string str() const {
        return (std::string)("(" + left->str() + ") + (" + right->str() + ")");
    }
    BoolNode* clone() const {
        return new XorNode(left->clone(), right->clone());
    }
};

class EqualNode : public BinNode {
public:
    EqualNode(BoolNode* L, BoolNode* R) : BinNode(L, R) {}
    bool calc() const {
        return (left->calc() == right->calc());
    }
    std::string str() const {
        return (std::string)("(" + left->str() + ") = (" + right->str() + ")");
    }
    BoolNode* clone() const {
        return new EqualNode(left->clone(), right->clone());
    }
};

class NorNode : public BinNode {
public:
    NorNode(BoolNode* L, BoolNode* R) : BinNode(L, R) {}
    bool calc() const {
        return !((bool)(left->calc() + right->calc()));
    }
    std::string str() const {
        return (std::string)("(" + left->str() + ") ^ (" + right->str() + ")");
    }
    BoolNode* clone() const {
        return new NorNode(left->clone(), right->clone());
    }
};

class NandNode : public BinNode {
public:
    NandNode(BoolNode* L, BoolNode* R) : BinNode(L, R) {}
    bool calc() const {
        return !((bool)((unsigned int)left->calc() * (unsigned int)right->calc()));
    }
    std::string str() const {
        return (std::string)("(" + left->str() + ") | (" + right->str() + ")");
    }
    BoolNode* clone() const {
        return new NandNode(left->clone(), right->clone());
    }
};

