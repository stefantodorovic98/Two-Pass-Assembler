#include "RegularExpressions.h"

regex RegularExpressions::LABEL("^[a-zA-Z_][a-zA-Z0-9_]*:.*$");
regex RegularExpressions::SECTION("^\\.section\\s+[a-zA-Z_][a-zA-Z0-9_]*$");
regex RegularExpressions::GLOBAL("^\\.global\\s+[a-zA-Z_][a-zA-Z0-9_]*(\\s*,\\s*[a-zA-Z_][a-zA-Z0-9_]*)*$");
regex RegularExpressions::EXTERN("^\\.extern\\s+[a-zA-Z_][a-zA-Z0-9_]*(\\s*,\\s*[a-zA-Z_][a-zA-Z0-9_]*)*$");
regex RegularExpressions::WORD("^\\.word\\s+(([a-zA-Z_][a-zA-Z0-9_]*)|([1-9][0-9]*)|(0)|(0[xX][0-9a-fA-F]+))(\\s*,\\s*(([a-zA-Z_][a-zA-Z0-9_]*)|([1-9][0-9]*)|(0)|(0[xX][0-9a-fA-F]+)))*$");
regex RegularExpressions::SKIP("^\\.skip\\s+(([1-9][0-9]*)|(0)|(0[xX][0-9a-fA-F]+))$");
regex RegularExpressions::EQU("^\\.equ\\s+[a-zA-Z_][a-zA-Z0-9_]*\\s*,\\s*(([1-9][0-9]*)|(0)|(0[xX][0-9a-fA-F]+))$");
regex RegularExpressions::END("^\\.end$");

regex RegularExpressions::HALT("^halt$");
regex RegularExpressions::INT("^int\\s+((r[0-7])|sp|pc|psw)$");
regex RegularExpressions::IRET("^iret$");
regex RegularExpressions::RET("^ret$");
regex RegularExpressions::PUSH("^push\\s+((r[0-7])|sp|pc|psw)$");
regex RegularExpressions::POP("^pop\\s+((r[0-7])|sp|pc|psw)$");
regex RegularExpressions::XCHG("^xchg\\s+((r[0-7])|sp|pc|psw)\\s*,\\s*((r[0-7])|sp|pc|psw)$");

regex RegularExpressions::ADD("^add\\s+((r[0-7])|sp|pc|psw)\\s*,\\s*((r[0-7])|sp|pc|psw)$");
regex RegularExpressions::SUB("^sub\\s+((r[0-7])|sp|pc|psw)\\s*,\\s*((r[0-7])|sp|pc|psw)$");
regex RegularExpressions::MUL("^mul\\s+((r[0-7])|sp|pc|psw)\\s*,\\s*((r[0-7])|sp|pc|psw)$");
regex RegularExpressions::DIV("^div\\s+((r[0-7])|sp|pc|psw)\\s*,\\s*((r[0-7])|sp|pc|psw)$");
regex RegularExpressions::CMP("^cmp\\s+((r[0-7])|sp|pc|psw)\\s*,\\s*((r[0-7])|sp|pc|psw)$");

regex RegularExpressions::NOT("^not\\s+((r[0-7])|sp|pc|psw)$");
regex RegularExpressions::AND("^and\\s+((r[0-7])|sp|pc|psw)\\s*,\\s*((r[0-7])|sp|pc|psw)$");
regex RegularExpressions::OR("^or\\s+((r[0-7])|sp|pc|psw)\\s*,\\s*((r[0-7])|sp|pc|psw)$");
regex RegularExpressions::XOR("^xor\\s+((r[0-7])|sp|pc|psw)\\s*,\\s*((r[0-7])|sp|pc|psw)$");
regex RegularExpressions::TEST("^test\\s+((r[0-7])|sp|pc|psw)\\s*,\\s*((r[0-7])|sp|pc|psw)$");

regex RegularExpressions::SHL("^shl\\s+((r[0-7])|sp|pc|psw)\\s*,\\s*((r[0-7])|sp|pc|psw)$");
regex RegularExpressions::SHR("^shr\\s+((r[0-7])|sp|pc|psw)\\s*,\\s*((r[0-7])|sp|pc|psw)$");

string fixPartLdr = "^ldr\\s+((r[0-7])|sp|pc|psw)\\s*,\\s*";
string fixPartStr = "^str\\s+((r[0-7])|sp|pc|psw)\\s*,\\s*";

string immLiteral = "((\\$(([1-9][0-9]*)|(0)|(0[xX][0-9a-fA-F]+)))|";
string immSymbol = "(\\$[a-zA-Z_][a-zA-Z0-9_]*)|";
string memLiteral = "((([1-9][0-9]*)|(0)|(0[xX][0-9a-fA-F]+)))|";
string memSymbol = "([a-zA-Z_][a-zA-Z0-9_]*)|";
string pcMemSymbol = "(%[a-zA-Z_][a-zA-Z0-9_]*)|";

string regDir = "(((r[0-7])|sp|pc|psw)|";
string regInd = "(\\[\\s*((r[0-7])|sp|pc|psw)\\s*\\]))$";

string regIndLit = "(\\[\\s*((r[0-7])|sp|pc|psw)\\s*\\+\\s*(([1-9][0-9]*)|(0)|(0[xX][0-9a-fA-F]+))\\s*\\])|";
string regIndSym = "(\\[\\s*((r[0-7])|sp|pc|psw)\\s*\\+\\s*([a-zA-Z_][a-zA-Z0-9_]*)\\s*\\]))$";

regex RegularExpressions::LDR_3_B(fixPartLdr + regDir + regInd);
regex RegularExpressions::LDR_5_B(fixPartLdr + immLiteral + immSymbol + memLiteral + memSymbol + pcMemSymbol + regIndLit + regIndSym);
regex RegularExpressions::STR_3_B(fixPartStr + regDir + regInd);
regex RegularExpressions::STR_5_B(fixPartStr + immLiteral + immSymbol + memLiteral + memSymbol + pcMemSymbol + regIndLit + regIndSym);

string fixPartCall = "^call\\s+";
string fixPartJmp = "^jmp\\s+";
string fixPartJeq = "^jeq\\s+";
string fixPartJne = "^jne\\s+";
string fixPartJgt = "^jgt\\s+";

string immLiteralJmp = "((([1-9][0-9]*)|(0)|(0[xX][0-9a-fA-F]+))|";
string immSymbolJmp = "([a-zA-Z_][a-zA-Z0-9_]*)|";
string pcImmSymbolJmp = "(%[a-zA-Z_][a-zA-Z0-9_]*)|";
string memLiteralJmp = "(\\*(([1-9][0-9]*)|(0)|(0[xX][0-9a-fA-F]+)))|";
string memSymbolJmp = "(\\*[a-zA-Z_][a-zA-Z0-9_]*)|";

string regDirJmp = "(\\*((r[0-7])|sp|pc|psw)|";
string regIndJmp = "(\\*\\[\\s*((r[0-7])|sp|pc|psw)\\s*\\]))$";

string regIndLitJmp = "(\\*\\[\\s*((r[0-7])|sp|pc|psw)\\s*\\+\\s*(([1-9][0-9]*)|(0)|(0[xX][0-9a-fA-F]+))\\s*\\])|";
string regIndSymJmp = "(\\*\\[\\s*((r[0-7])|sp|pc|psw)\\s*\\+\\s*([a-zA-Z_][a-zA-Z0-9_]*)\\s*\\]))$";

regex RegularExpressions::CALL_3_B(fixPartCall + regDirJmp + regIndJmp);
regex RegularExpressions::CALL_5_B(fixPartCall + immLiteralJmp + immSymbolJmp + pcImmSymbolJmp + memLiteralJmp + memSymbolJmp + 
									regIndLitJmp + regIndSymJmp);

regex RegularExpressions::JMP_3_B(fixPartJmp + regDirJmp + regIndJmp);
regex RegularExpressions::JMP_5_B(fixPartJmp + immLiteralJmp + immSymbolJmp + pcImmSymbolJmp + memLiteralJmp + memSymbolJmp +
	regIndLitJmp + regIndSymJmp);

regex RegularExpressions::JEQ_3_B(fixPartJeq + regDirJmp + regIndJmp);
regex RegularExpressions::JEQ_5_B(fixPartJeq + immLiteralJmp + immSymbolJmp + pcImmSymbolJmp + memLiteralJmp + memSymbolJmp +
	regIndLitJmp + regIndSymJmp);

regex RegularExpressions::JNE_3_B(fixPartJne + regDirJmp + regIndJmp);
regex RegularExpressions::JNE_5_B(fixPartJne + immLiteralJmp + immSymbolJmp + pcImmSymbolJmp + memLiteralJmp + memSymbolJmp +
	regIndLitJmp + regIndSymJmp);

regex RegularExpressions::JGT_3_B(fixPartJgt + regDirJmp + regIndJmp);
regex RegularExpressions::JGT_5_B(fixPartJgt + immLiteralJmp + immSymbolJmp + pcImmSymbolJmp + memLiteralJmp + memSymbolJmp +
	regIndLitJmp + regIndSymJmp);

regex RegularExpressions::DEC_NUM("^(([1-9][0-9]*)|(0))$");
regex RegularExpressions::HEX_NUM("^0[xX][0-9a-fA-F]+$");
regex RegularExpressions::BLANK("^\\s*$");
regex RegularExpressions::COMMENT("^#.*$");

//SECOND PASS

regex RegularExpressions::REGISTER_0_7("^r[0-7]$");

//LDR AND STR

regex RegularExpressions::REG_DIR("^(r[0-7])|sp|pc|psw$");
regex RegularExpressions::REG_IND("^\\[\\s*((r[0-7])|sp|pc|psw)\\s*\\]$");

regex RegularExpressions::REG_IND_LITERAL("^\\[\\s*((r[0-7])|sp|pc|psw)\\s*\\+\\s*(([1-9][0-9]*)|(0)|(0[xX][0-9a-fA-F]+))\\s*\\]$");
regex RegularExpressions::REG_IND_SYMBOL("^\\[\\s*((r[0-7])|sp|pc|psw)\\s*\\+\\s*([a-zA-Z_][a-zA-Z0-9_]*)\\s*\\]$");

regex RegularExpressions::IMM_LITERAL("^\\$(([1-9][0-9]*)|(0)|(0[xX][0-9a-fA-F]+))$");
regex RegularExpressions::IMM_SYMBOL("^\\$[a-zA-Z_][a-zA-Z0-9_]*$");
regex RegularExpressions::MEM_LITERAL("^(([1-9][0-9]*)|(0)|(0[xX][0-9a-fA-F]+))$");
regex RegularExpressions::MEM_SYMBOL("^[a-zA-Z_][a-zA-Z0-9_]*$");
regex RegularExpressions::PC_MEM_SYMBOL("^%[a-zA-Z_][a-zA-Z0-9_]*$");

//CALL AND JMP

regex RegularExpressions::REG_DIR_JMP("^\\*((r[0-7])|sp|pc|psw)$");
regex RegularExpressions::REG_IND_JMP("^\\*\\[\\s*((r[0-7])|sp|pc|psw)\\s*\\]$");

regex RegularExpressions::REG_IND_LITERAL_JMP("^\\*\\[\\s*((r[0-7])|sp|pc|psw)\\s*\\+\\s*(([1-9][0-9]*)|(0)|(0[xX][0-9a-fA-F]+))\\s*\\]$");
regex RegularExpressions::REG_IND_SYMBOL_JMP("^\\*\\[\\s*((r[0-7])|sp|pc|psw)\\s*\\+\\s*([a-zA-Z_][a-zA-Z0-9_]*)\\s*\\]$");

regex RegularExpressions::IMM_LITERAL_JMP("^(([1-9][0-9]*)|(0)|(0[xX][0-9a-fA-F]+))$");
regex RegularExpressions::IMM_SYMBOL_JMP("^[a-zA-Z_][a-zA-Z0-9_]*$");
regex RegularExpressions::PC_IMM_SYMBOL_JMP("^%[a-zA-Z_][a-zA-Z0-9_]*$");
regex RegularExpressions::MEM_LITERAL_JMP("^\\*(([1-9][0-9]*)|(0)|(0[xX][0-9a-fA-F]+))$");
regex RegularExpressions::MEM_SYMBOL_JMP("^\\*[a-zA-Z_][a-zA-Z0-9_]*$");