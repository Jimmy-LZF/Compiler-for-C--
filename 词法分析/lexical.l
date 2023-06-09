%{
    #include"node.h"
    #include "syntax.tab.h"

extern unsigned lexError;//词法错误标志
int lines = 0;
int yycolumn = 1;

#define YY_USER_ACTION \
    yylloc.first_line=yylloc.last_line=yylineno; \
    yylloc.first_column=yycolumn; \
    yylloc.last_column=yycolumn+yyleng-1; \
    yycolumn+=yyleng;
%}

%option yylineno

ws [ \t]
digit [0-9]
letter [_a-zA-Z]
ID {letter}{ld}*
INT 0|[1-9]+[0-9]*
FLOAT {digit}+"."{digit}+|{digit}*"."{digit}+[eE][+-]?{digit}+
|{digit}+"."{digit}*[eE][+-]?{digit}+
ld[0-9_a-zA-Z]
nonletter [^_a-zA-Z]
RELOP 	>|<|>=|<=|==|!=
PLUS "+"
MINUS "-"
STAR "*"
DIV "/"
AND "&&"
OR "||"
NOT "!"
DOT "."
SEMI ";"
COMMA ","
ASSIGNOP "="
IF if
ELSE else
WHILE while
TYPE int|float
STRUCT struct
RETURN return
LP "("
RP ")"
LB "["
RB "]"
LC "{"
RC "}"

%%
\n|\r    { yycolumn = 1; }
{ws}+    {;}
{IF}     { yylval.node = tokenNode(yylineno, TOKEN_OTHER, "IF", yytext); return IF; }
{ELSE}   { yylval.node = tokenNode(yylineno, TOKEN_OTHER, "ELSE", yytext); return ELSE; }
{WHILE}  { yylval.node = tokenNode(yylineno, TOKEN_OTHER, "WHILE", yytext); return WHILE; }
{TYPE}   { yylval.node = tokenNode(yylineno, TOKEN_TYPE, "TYPE", yytext); return TYPE; }
{STRUCT} { yylval.node = tokenNode(yylineno, TOKEN_OTHER, "STRUCT", yytext); return STRUCT; }
{RETURN} { yylval.node = tokenNode(yylineno, TOKEN_OTHER, "RETURN", yytext); return RETURN; }
{RELOP}  { yylval.node = tokenNode(yylineno, TOKEN_OTHER, "RELOP", yytext); return RELOP; }

{ID}     { yylval.node = tokenNode(yylineno, TOKEN_ID, "ID", yytext); return ID;}
{INT}    { yylval.node = tokenNode(yylineno, TOKEN_INT, "INT", yytext); return INT;}
{FLOAT}  { yylval.node = tokenNode(yylineno, TOKEN_FLOAT, "FLOAT", yytext); return FLOAT;}

{PLUS}   { yylval.node = tokenNode(yylineno, TOKEN_OTHER, "PLUS", yytext); return PLUS; }
{MINUS}  { yylval.node = tokenNode(yylineno, TOKEN_OTHER, "MINUS", yytext); return MINUS; }
{STAR}   { yylval.node = tokenNode(yylineno, TOKEN_OTHER, "STAR", yytext); return STAR; }
{DIV}    { yylval.node = tokenNode(yylineno, TOKEN_OTHER, "DIV", yytext); return DIV; }
{AND}    { yylval.node = tokenNode(yylineno, TOKEN_OTHER, "AND", yytext); return AND; }
{OR}     { yylval.node = tokenNode(yylineno, TOKEN_OTHER, "OR", yytext); return OR; }
{NOT}    { yylval.node = tokenNode(yylineno, TOKEN_OTHER, "NOT", yytext); return NOT; }

{DOT}       { yylval.node = tokenNode(yylineno, TOKEN_OTHER, "DOT", yytext); return DOT; }
{SEMI}      { yylval.node = tokenNode(yylineno, TOKEN_OTHER, "SEMI", yytext); return SEMI; }
{COMMA}     { yylval.node = tokenNode(yylineno, TOKEN_OTHER, "COMMA", yytext); return COMMA; }
{ASSIGNOP}  { yylval.node = tokenNode(yylineno, TOKEN_OTHER, "ASSIGNOP", yytext); return ASSIGNOP; }

{LP} { yylval.node = tokenNode(yylineno, TOKEN_OTHER, "LP", yytext); return LP; }
{RP} { yylval.node = tokenNode(yylineno, TOKEN_OTHER, "RP", yytext); return RP; }
{LB} { yylval.node = tokenNode(yylineno, TOKEN_OTHER, "LB", yytext); return LB; }
{RB} { yylval.node = tokenNode(yylineno, TOKEN_OTHER, "RB", yytext); return RB; }
{LC} { yylval.node = tokenNode(yylineno, TOKEN_OTHER, "LC", yytext); return LC; }
{RC} { yylval.node = tokenNode(yylineno, TOKEN_OTHER, "RC", yytext); return RC; }


{digit}+{ID} {lexError = TRUE; printf("Error type A at Line %d: Illegal ID \"%s\".\n", yylineno, yytext); }
"."{digit}+  { lexError = TRUE; printf("Error type A at Line %d: Illegal floating point number \"%s\".\n", yylineno, yytext); }
{digit}+"."  { lexError = TRUE; printf("Error type A at Line %d: Illegal floating point number \"%s\".\n", yylineno, yytext); }
{digit}*"."{digit}+[eE] { lexError = TRUE; printf("Error type A at Line %d: Illegal floating point number \"%s\".\n", yylineno, yytext); }
{digit}+"."{digit}*[eE] { lexError = TRUE; printf("Error type A at Line %d: Illegal floating point number \"%s\".\n", yylineno, yytext); }
{digit}+[eE][+-]?{digit}* { lexError = TRUE; printf("Error type A at Line %d: Illegal floating point number \"%s\".\n", yylineno, yytext); }
"."[eE][+-]?{digit}+ { lexError = TRUE; printf("Error type A at Line %d: Illegal floating point number \"%s\".\n", yylineno, yytext); }
. { lexError = TRUE; printf("Error type A at Line %d: Mysterious character \'%s\'.\n", yylineno, yytext); }
%%

