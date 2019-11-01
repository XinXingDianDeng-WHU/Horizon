#pragma once
#include "StmtAST.h"
#include "StmtsAST.h"
#include "ExpAST.h"


class IfStmtAST ://������if-else�����
	public StmtAST
{
public:
	ExpAST* expAST;
	StmtsAST* thenStmts;
	StmtsAST* elseStmts = nullptr;

	IfStmtAST(ExpAST* expAST, StmtsAST* thenStmts);
	IfStmtAST(ExpAST* expAST, StmtsAST* thenStmts, StmtsAST* elseStmts);
	virtual ~IfStmtAST();

};

