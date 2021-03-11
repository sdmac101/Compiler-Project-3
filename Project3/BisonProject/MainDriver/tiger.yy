
%debug
%verbose	/*generate file: tiger.output to check grammar*/
%locations

%{
#include <iostream>
#include <string>
#include "ErrorMsg.h"
#include <FlexLexer.h>

int yylex(void);		/* function prototype */
void yyerror(char *s);	//called by the parser whenever an eror occurs

%}

%union {
	int		ival;	//integer value of INT token
	std::string* sval;	//pointer to name of IDENTIFIER or value of STRING	
					//I have to use pointers since C++ does not support 
					//string object as the union member
}

/* TOKENs and their associated data type */
%token <sval> ID STRING
%token <ival> INT

%token 
  COMMA COLON SEMICOLON LPAREN RPAREN LBRACK RBRACK 
  LBRACE RBRACE DOT 
  ARRAY IF THEN ELSE WHILE FOR TO DO LET IN END OF 
  BREAK NIL
  FUNCTION VAR TYPE 

/* add your own predence level of operators here */
%left ASSIGN
%left AND OR
%left EQ NEQ GT LT GE LE 
%left PLUS MINUS
%left TIMES DIVIDE
%left UMINUS

%start program

%%

/* This is a skeleton grammar file, meant to illustrate what kind of
 * declarations are necessary above the %% mark.  Students are expected
 *  to replace the two dummy productions below with an actual grammar. 
 */

program	:	exp
		;

exp		:	STRING		
		|	INT		
		|	NIL			
		|	lval
		|	MINUS exp %prec UMINUS
		|	exp PLUS exp
		|	exp MINUS exp
		|	exp DIVIDE exp
		|	exp TIMES exp
		|	exp EQ exp
		|	exp NEQ exp
		|	exp GT exp
		|	exp LT exp
		|	exp GE exp
		|	exp LE exp
		|	exp AND exp
		|	exp OR exp
		|	exp ASSIGN exp
		|	lval ASSIGN exp
		|	ID LPAREN exp-list RPAREN
		|	LPAREN exp-seq RPAREN
		|	ID LBRACK exp RBRACK OF exp
		|	IF exp THEN exp
		|	IF exp THEN exp ELSE exp
		|	WHILE exp DO exp
		|	FOR ID ASSIGN exp TO exp DO exp
		|	BREAK
		|	LET declaration-list IN exp-seq END
		;

exp-seq	:	exp
		|	exp-seq SEMICOLON exp
		;

exp-list	:	exp
			|	exp-list COMMA exp
			;

lval	:	ID
		|	lval LBRACK exp RBRACK
		;

declaration-list	:	declaration
					|	declaration-list declaration
					;

declaration	:	type-declaration
			|	variable-declaration
			;

type-declaration	:	TYPE ID EQ type
					;

type	:	ID
		|	ARRAY OF ID
		;

variable-declaration	:	VAR ID ASSIGN exp
						|	VAR ID COLON ID ASSIGN exp

%%
extern yyFlexLexer	lexer;
int yylex(void)
{
	return lexer.yylex();
}

void yyerror(char *s)
{
	extern int	linenum;			//line no of current matched token
	extern int	colnum;
	extern void error(int, int, std::string);

	error(linenum, colnum, s);
}

