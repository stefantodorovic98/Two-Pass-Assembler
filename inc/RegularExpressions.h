#ifndef REGULAR_EXPRESSIONS_H
#define REGULAR_EXPRESSIONS_H

#include <iostream>
#include <regex>
using namespace std;

class RegularExpressions
{
	public:
	static regex LABEL;
	static regex SECTION;
	static regex GLOBAL;
	static regex EXTERN;
	static regex WORD;
	static regex SKIP;
	static regex EQU;
	static regex END;

	static regex HALT;
	static regex INT;
	static regex IRET;
	//Call
	static regex CALL_3_B;
	static regex CALL_5_B;
	static regex RET;
	//Jmps
	static regex JMP_3_B;
	static regex JMP_5_B;
	static regex JEQ_3_B;
	static regex JEQ_5_B;
	static regex JNE_3_B;
	static regex JNE_5_B;
	static regex JGT_3_B;
	static regex JGT_5_B;

	static regex PUSH;
	static regex POP;
	static regex XCHG;

	static regex ADD;
	static regex SUB;
	static regex MUL;
	static regex DIV;
	static regex CMP;

	static regex NOT;
	static regex AND;
	static regex OR;
	static regex XOR;
	static regex TEST;

	static regex SHL;
	static regex SHR;

	static regex LDR_3_B;
	static regex LDR_5_B;
	static regex STR_3_B;
	static regex STR_5_B;

	static regex DEC_NUM;
	static regex HEX_NUM;
	static regex BLANK;
	static regex COMMENT;

	//SECOND PASS

	static regex REGISTER_0_7;

	//LDR AND STR

	static regex REG_DIR;
	static regex REG_IND;

	static regex REG_IND_LITERAL;
	static regex REG_IND_SYMBOL;

	static regex IMM_LITERAL;
	static regex IMM_SYMBOL;
	static regex MEM_LITERAL;
	static regex MEM_SYMBOL;
	static regex PC_MEM_SYMBOL;

	//CALL AND JMP

	static regex REG_DIR_JMP;
	static regex REG_IND_JMP;

	static regex REG_IND_LITERAL_JMP;
	static regex REG_IND_SYMBOL_JMP;

	static regex IMM_LITERAL_JMP;
	static regex IMM_SYMBOL_JMP;
	static regex PC_IMM_SYMBOL_JMP;
	static regex MEM_LITERAL_JMP;
	static regex MEM_SYMBOL_JMP;
};

#endif

