#pragma once
#include "AST.h"
class RealAST :
	public AST
{
public:
	double real_number;
	RealAST(double real_number)
	{
		this->real_number = real_number;
	};
};

