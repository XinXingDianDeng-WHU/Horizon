#pragma once
#include "AST.h"
#include "Enums_Structs.h"
using namespace zx;
class TypeAST :
	public AST
{
public:
	Type type;
	TypeAST(Type type)
	{
		this->type = type;
	}
};

