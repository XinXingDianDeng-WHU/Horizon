#pragma once
#include "TypeSpecifyAST.h"
#include "IdentifierAST.h"
#include "TypeAST.h"

class DirectTypeSpecifyAST :
	public TypeSpecifyAST
{
public:
	TypeAST* type = nullptr;
	IdentifierAST* structNameIdentifier = nullptr;//����ǽṹ�壬��Ϊ�ṹ������

	DirectTypeSpecifyAST(TypeAST* type);
	DirectTypeSpecifyAST(IdentifierAST* structNameIdentifier);

	virtual ~DirectTypeSpecifyAST();
};

