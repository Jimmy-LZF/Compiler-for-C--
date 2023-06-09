%{
    #include<stdio.h>
    #include"node.h"
    #include"lex.yy.c"
    extern int synError;
    aNode root;
    #define YYERROR_VERBOSE 1
    int yyerror(char* msg);
%}

// types

%union{
    aNode node; 
}

// tokens

%token <node> INT
%token <node> FLOAT
%token <node> ID
%token <node> TYPE
%token <node> COMMA
%token <node> DOT
%token <node> SEMI
%token <node> RELOP
%token <node> ASSIGNOP
%token <node> PLUS MINUS STAR DIV
%token <node> AND OR NOT 
%token <node> LP RP LB RB LC RC
%token <node> IF
%token <node> ELSE
%token <node> WHILE
%token <node> STRUCT
%token <node> RETURN

// non-terminals

%type <node> Program ExtDefList ExtDef ExtDecList   //  High-level Definitions
%type <node> Specifier StructSpecifier OptTag Tag   //  Specifiers
%type <node> VarDec FunDec VarList ParamDec         //  Declarators
%type <node> CompSt StmtList Stmt                   //  Statements
%type <node> DefList Def Dec DecList                //  Local Definitions
%type <node> Exp Args                               //  Expressions

// precedence and associativity

%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT
%left DOT
%left LB RB
%left LP RP
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%

Program:            ExtDefList                              { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "Program", 1, $1); root = $$; }
    ; 
ExtDefList:         ExtDef ExtDefList                       { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "ExtDefList", 2, $1, $2); }
    |                                                       { $$ = NULL; } 
    ; 
ExtDef:             Specifier ExtDecList SEMI               { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "ExtDef", 3, $1, $2, $3); }
    |               Specifier SEMI                          { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "ExtDef", 2, $1, $2); }
    |               Specifier FunDec CompSt                 { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "ExtDef", 3, $1, $2, $3); }
    |               error SEMI                              { synError = TRUE; }
    ; 
ExtDecList:         VarDec                                  { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "ExtDecList", 1, $1); }
    |               VarDec COMMA ExtDecList                 { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "ExtDecList", 3, $1, $2, $3); }
    ; 


Specifier:          TYPE                                    { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "Specifier", 1, $1); }
    |               StructSpecifier                         { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "Specifier", 1, $1); }
    ; 
StructSpecifier:    STRUCT OptTag LC DefList RC             { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "StructSpecifier", 5, $1, $2, $3, $4, $5); }
    |               STRUCT Tag                              { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "StructSpecifier", 2, $1, $2); }
    ; 
OptTag:             ID                                      { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "OptTag", 1, $1); }
    |                                                       { $$ = NULL; }
    ; 
Tag:                ID                                      { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "Tag", 1, $1); }
    ; 


VarDec:             ID                                      { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "VarDec", 1, $1); }
    |               VarDec LB INT RB                        { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "VarDec", 4, $1, $2, $3, $4); }
    |               error RB                                { synError = TRUE; }
    ; 
FunDec:             ID LP VarList RP                        { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "FunDec", 4, $1, $2, $3, $4); }
    |               ID LP RP                                { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "FunDec", 3, $1, $2, $3); }
    |               error RP                                { synError = TRUE; }
    ; 
VarList:            ParamDec COMMA VarList                  { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "VarList", 3, $1, $2, $3); }
    |               ParamDec                                { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "VarList", 1, $1); }
    ; 
ParamDec:           Specifier VarDec                        { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "ParamDec", 2, $1, $2); }
    ; 

CompSt:             LC DefList StmtList RC                  { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "CompSt", 4, $1, $2, $3, $4); }
    |               error RC                                { synError = TRUE; }
    ; 
StmtList:           Stmt StmtList                           { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "StmtList", 2, $1, $2); }
    |                                                       { $$ = NULL; }
    ; 
Stmt:               Exp SEMI                                { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "Stmt", 2, $1, $2); }
    |               CompSt                                  { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "Stmt", 1, $1); }
    |               RETURN Exp SEMI                         { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "Stmt", 3, $1, $2, $3); }    
    |               IF LP Exp RP Stmt %prec LOWER_THAN_ELSE { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "Stmt", 5, $1, $2, $3, $4, $5); }
    |               IF LP Exp RP Stmt ELSE Stmt             { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "Stmt", 7, $1, $2, $3, $4, $5, $6, $7); }
    |               WHILE LP Exp RP Stmt                    { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "Stmt", 5, $1, $2, $3, $4, $5); }
    |               error SEMI                              { synError = TRUE; }
    ; 

DefList:            Def DefList                             { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "DefList", 2, $1, $2); }
    |                                                       { $$ = NULL; }
    ;     
Def:                Specifier DecList SEMI                  { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "Def", 3, $1, $2, $3); }
    ; 
DecList:            Dec                                     { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "DecList", 1, $1); }
    |               Dec COMMA DecList                       { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "DecList", 3, $1, $2, $3); }
    ; 
Dec:                VarDec                                  { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "Dec", 1, $1); }
    |               VarDec ASSIGNOP Exp                     { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "Dec", 3, $1, $2, $3); }
    ; 

Exp:                Exp ASSIGNOP Exp                        { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "Exp", 3, $1, $2, $3); }
    |               Exp AND Exp                             { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "Exp", 3, $1, $2, $3); }
    |               Exp OR Exp                              { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "Exp", 3, $1, $2, $3); }
    |               Exp RELOP Exp                           { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "Exp", 3, $1, $2, $3); }
    |               Exp PLUS Exp                            { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "Exp", 3, $1, $2, $3); }
    |               Exp MINUS Exp                           { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "Exp", 3, $1, $2, $3); }
    |               Exp STAR Exp                            { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "Exp", 3, $1, $2, $3); }
    |               Exp DIV Exp                             { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "Exp", 3, $1, $2, $3); }
    |               LP Exp RP                               { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "Exp", 3, $1, $2, $3); }
    |               MINUS Exp                               { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "Exp", 2, $1, $2); }
    |               NOT Exp                                 { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "Exp", 2, $1, $2); }
    |               ID LP Args RP                           { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "Exp", 4, $1, $2, $3, $4); }
    |               ID LP RP                                { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "Exp", 3, $1, $2, $3); }
    |               Exp LB Exp RB                           { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "Exp", 4, $1, $2, $3, $4); }
    |               Exp DOT ID                              { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "Exp", 3, $1, $2, $3); }
    |               ID                                      { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "Exp", 1, $1); }
    |               INT                                     { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "Exp", 1, $1); }
    |               FLOAT                                   { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "Exp", 1, $1); }
    ; 
Args :              Exp COMMA Args                          { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "Args", 3, $1, $2, $3); }
    |               Exp                                     { $$ = notTokenNode(@$.first_line, NOT_A_TOKEN, "Args", 1, $1); }
    ; 
%%

int yyerror(char* msg){
    printf("Error type B at line %d: %s.\n", yylineno, msg);
}