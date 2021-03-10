/* Sophie McIntyre */
/* tiger.ll */
/* 2/17/21 */
/* Dr. Zhijiang Dong */

/* input file for scanner */
/* recognizes reserved words, punctuation, identifiers, comments, int and string literals */

%option noyywrap
%option never-interactive
%option nounistd
%option c++

%{
#include <iostream>
#include <string>
#include <sstream>
#include "tiger.tab.hh"
#include "ErrorMsg.h"

using std::string;
using std::stringstream;

ErrorMsg			errormsg;	//error handler

int		comment_depth = 0;	// depth of the nested comment
string	value = "";			// the value of current string

int			beginLine=-1;	//beginning line no of a string or comment
int			beginCol=-1;	//beginning column no of a string or comment

int		linenum = 1;		//beginning line no of the current matched token
int		colnum = 1;			//beginning column no of the current matched token
int		tokenCol = 1;		//column no after the current matched token

//the following defines actions that will be taken automatically after 
//each token match. It is used to update colnum and tokenCol automatically.
#define YY_USER_ACTION {colnum = tokenCol; tokenCol=colnum+yyleng;}

int string2int(string);			//convert a string to integer value
void newline(void);				//trace the line #
void error(int, int, string);	//output the error message referring to the current token
%}

ALPHA		[A-Za-z]
DIGIT		[0-9]
INT			[0-9]+
IDENTIFIER	{ALPHA}(({ALPHA}|{DIGIT}|"_")*)

%x	COMMENT
%x  S_STRING
%%
" "				{}
\t				{}
\b				{}
\n				{newline(); }

"while"				{return WHILE; } /* reserved words */
"for"				{return FOR; }
"to"				{return TO; }
"break"				{return BREAK; }
"let"				{return LET; }
"in"				{return IN; }
"end"				{return END; }
"function"			{return FUNCTION; }
"var"				{return VAR; }
"type"				{return TYPE; }
"array"				{return ARRAY; }
"if"				{return IF; }
"then"				{return THEN; }
"else"				{return ELSE; }
"do"				{return DO; }
"of"				{return OF; }
"nil"				{return NIL; }


","				{return COMMA; } /* punctuation */
":"				{return COLON; }
";"				{return SEMICOLON; }
"("				{return LPAREN; }
")"				{return RPAREN; }
"["				{return LBRACK; }
"]"				{return RBRACK; }
"{"				{return LBRACE; }
"}"				{return RBRACE; }
"."				{return DOT; }
"+"				{return PLUS; }
"-"				{return MINUS; }
"*"				{return TIMES; }
"/"				{return DIVIDE; }
"="				{return EQ; }
"<>"			{return NEQ; }
"<" 			{ return LT; }
"<="			{return LE; }
">" 			{ return GT; }
">="			{return GE; }
"&"				{return AND; }
"|"				{return OR; }
":="			{return ASSIGN; }



{IDENTIFIER} 	{ value = YYText(); yylval.sval = new string(value); return ID; } /* identifier and int literal */
{INT}		 	{ yylval.ival = string2int(YYText()); return INT; }


\"				{   /* entering string */ 
							value = "";
							beginLine = linenum;
							beginCol = colnum;
							BEGIN(S_STRING);
						}
<S_STRING>\n		{	error(beginLine, beginCol, "unclosed string");
							newline();
							yylval.sval = new string(value);
							BEGIN(INITIAL);
							return STRING;
					}
<S_STRING>\\t		{value = value + "	";}
<S_STRING>\\n		{
						value = value + "\n";
					}
<S_STRING>\\\\		{value = value + "\\";}
<S_STRING>\\\"		{value = value + "\"";}
<S_STRING>\"		{	if(linenum == beginLine){
							yylval.sval = new string(value);
							BEGIN(INITIAL);
							return STRING;
						} else {
							error(beginLine, beginCol, "unclosed string");
							yylval.sval = new string(value);
							BEGIN(INITIAL);
							return STRING;
						
						}
					}
<S_STRING>\\.  {error(linenum, colnum, "illegal token");}
<S_STRING>.		{value = value + YYText();}


"/*"			{	/* entering comment */
					comment_depth ++;
					beginLine = linenum;
					beginCol = colnum;
					BEGIN(COMMENT);	
				}

<COMMENT>"/*"	{	/* nested comment */
					comment_depth ++;
				}

<COMMENT>[^*/\n]*	{	/*eat anything that's not a '*' */ }

<COMMENT>"/"+[^/*\n]*  {	/*eat anything that's not a '*' */ }

<COMMENT>"*"+[^*/\n]*	{	/* eat up '*'s not followed by '/'s */	}

<COMMENT>\n		{	newline();	}

<COMMENT>"*"+"/"	{	/* close of a comment */
						comment_depth --;
						if ( comment_depth == 0 )
						{
							BEGIN(INITIAL);	
							/* cout << "Comments at " << beginLine << ":" << beginCol << endl; */
						}
					}
<COMMENT><<EOF>>	{	/* unclosed comments */
						error(beginLine, beginCol, "unclosed comments");
						yyterminate();
					}


<<EOF>>			{	yyterminate(); } /* if end of file or a token thats not recognized */
.				{	error(linenum, colnum, string(YYText()) + " illegal token");}

%%

int string2int( string val )
{
	stringstream	ss(val);
	int				retval;

	ss >> retval;

	return retval;
}

void newline()
{
	linenum ++;
	colnum = 1;
	tokenCol = 1;
}

void error(int line, int col, string msg)
{
	errormsg.error(line, col, msg);
}
