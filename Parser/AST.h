#pragma once
#include <vector>
#include <string>
#include "../Parser/FromLexer/MyLexer.h"
using namespace std;
class AST
{
public:
	int row = -1;
	string content;
	int level = -1;
	void setRow(int row);
	void setLevel(int lv) {
		this->level = lv;
	}
	virtual ~AST();
};

