#pragma once
#include "AST.h"
class DecExpAST:public AST
{
public:
	int son = -1;//0 ��ʾ<var_dec>    1��ʾ  <struct_dec>
};

