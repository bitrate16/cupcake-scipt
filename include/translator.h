#pragma once 

#include <vector>

#include "ast.h"

namespace ck_bytecodes {
	const int LINENO               = 9; // Marks the lineno change
	const int NOP                  = 10; // Does nothing.
	const int PUSH_CONST_INT       = 11; // Push Int
	const int PUSH_CONST_DOUBLE    = 12; // Push Double
	const int PUSH_CONST_BOOLEAN   = 13; // Push Bool
	const int PUSH_CONST_NULL      = 14; // Push Null
	const int PUSH_CONST_UNDEFINED = 15; // Push Undefined
	const int PUSH_CONST_STRING    = 16; // Push String
	const int LOAD_VAR             = 17; // scope.get_var(name)
	const int VSTACK_POP           = 18; // Pop the value from stack and do nothing with it
	const int PUSH_CONST_ARRAY     = 19; // Push Array
	const int PUSH_CONST_OBJECT    = 20; // Push Object
	const int DEFINE_VAR           = 21; // Define variable vith [top] value in current scope
	const int VSTACK_DUP           = 22; // Duplicate top of the stack
	const int LOAD_MEMBER          = 23; // top:[key, ref]
	const int LOAD_FIELD           = 24; // top:[ref]
	const int OPERATOR             = 25; // operate with b = stack.top and a = stack.next_to_top as a <opt> b
	const int STORE_VAR            = 26; // scope.store(name)
	const int STORE_FIELD          = 27; // top:[value, ref]
	const int STORE_MEMBER         = 28; // top:[value, key, ref]
	const int UNARY_OPERATOR       = 29; // Call of unary operator for [top]
	const int VSTACK_SWAP          = 30; // swap stack [top] and [top-1]
	const int VSTACK_SWAP1         = 31; // swap stack [top-1] and [top-2]
	const int VSTACK_SWAP2         = 32; // swap stack [top-2] and [top-3]
	const int VSTATE_PUSH_SCOPE    = 33; // Push scope to scope list
	const int VSTATE_POP_SCOPE     = 34; // Pop scope from scope list
	const int JMP_IF_ZERO          = 35; // Jump to [int] address if top of the stack is zero. stack.pop()
	const int JMP                  = 36; // Jump to [int] address.
	const int BCEND                = 37; // Halts the program
	const int THROW_NOARG          = 38; // raise;
	const int THROW                = 39; // raise <expression>;
	const int THROW_STRING         = 40; // raise from translator. ~Eof
	const int VSTATE_POP_SCOPES    = 41; // Pop N scopes from scope list
	const int RETURN_VALUE         = 42; // Return value
	const int PUSH_CONST_FUNCTION  = 43; // Push BytecodeFunction
	const int VSTATE_PUSH_TRY      = 44; // Push try frame
	const int VSTATE_POP_TRY       = 45; // Pop try frame
	const int PUSH_THIS            = 46; // Push this scope to stack
	const int JMP_IF_NOT_ZERO      = 47; // Jump to [int] address if top of the stack is not zero. stack.pop()
	
	const int CALL                 = 61; // (<expression>)()  top:[fun, args]
	const int CALL_NAME            = 62; // var_name()        top:[args]
	const int CALL_FIELD           = 63; // foo.var_name()    top:[ref, args]
	const int CALL_MEMBER          = 64; // foo["member"]()   top:[key, ref, args]
	
	const int OPT_ADD      = 1;
	const int OPT_SUB      = 2;
	const int OPT_MUL      = 3;
	const int OPT_DIV      = 4;
	const int OPT_BITRSH   = 5;
	const int OPT_BITLSH   = 6;
	const int OPT_BITURSH  = 7;
	const int OPT_DIR      = 9;
	const int OPT_PATH     = 10;
	const int OPT_MOD      = 11;
	const int OPT_BITOR    = 12;
	const int OPT_BITAND   = 13;
	const int OPT_BITXOR   = 14;
	const int OPT_HASH     = 15;	
	const int OPT_EQ       = 16;
	const int OPT_NEQ      = 17;
	const int OPT_OR       = 18;
	const int OPT_AND      = 19;
	const int OPT_GT       = 20;
	const int OPT_GE       = 21;
	const int OPT_LT       = 22;
	const int OPT_LE       = 23;
	const int OPT_PUSH     = 24;
	const int OPT_ARROW    = 25;
						   
	const int OPT_DOG      = 26;
	const int OPT_BITNOT   = 27;
	const int OPT_NOT      = 28;
	const int OPT_POS      = 29;
	const int OPT_NEG      = 30;
						   
	const int OPT_INC      = 31;	
	const int OPT_DEC      = 32;
						   
	const int OPT_TYPEOF   = 33;
	const int OPT_AS       = 34;
	const int OPT_ISTYPEOF = 35;
	
	const int OPT_BITULSH  = 36;
	const int OPT_LEQ      = 37;
	const int OPT_NLEQ     = 38;
	
	// Expects POP_TRY when finishing try block.
	// POP_TRY automatically executed on jumping to catch block.
	const int TRY_NO_CATCH = 1;
	const int TRY_NO_ARG   = 2;
	const int TRY_WITH_ARG = 3;
};

namespace ck_translator {
	
	// Wrapper for ck bytecodes
	struct ck_bytecode {
		std::vector<int> lineno_table;
		std::vector<unsigned char> bytemap;
	};
	
	void translate(ck_bytecode& bytecode, ck_ast::ASTNode* n);
	
	void translate(std::vector<unsigned char>& bytemap, std::vector<int>& lineno_table, ck_ast::ASTNode* n);
	
	void print(std::vector<unsigned char>& bytemap, int off = 0, int offset = -1, int limit = -1);
	
	void print_lineno_table(std::vector<int>& lineno_table);
};