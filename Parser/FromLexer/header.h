#pragma once
namespace zx {
	enum Token {
		// �ս��
		Plus,// +
		Minus,// -
		Star,// *
		Divide,// /
		Mode,// %
		PlusPlus,// ++
		MinusMinus,// --
		//Star,// *
		More,// >
		Less,// <
		MoreOrEq,// >=
		LessOrEq,// <=
		Eq,// ==
		Neq,// <>
		Or,//  ||
		And,// &&
		Void,// void
		Int,// int
		Real,// real
		Char,// char
		Lb,// {
		Rb,// }
		Ls,// [
		Rs,// ]
		Lp,// (
		Rp,// )
		Comma,// ,
		Semi,// ;
		Dot,// .
		Assign,// =
		Struct,// struct
		If,// if
		Else,// else
		While,// while
		Return,// return
		Continue,//continue
		Break,// break
		Tag,// ��ʶ��
		Const_int,// ��������1  1.1  a
		Const_real,
		Const_char,
		Const_string,
		Pointer_Sign,
		End,//$
		Not// !
	};
};




