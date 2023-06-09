#include<stdio.h>
#include<stdlib.h>
#include"semantic.h"

extern FILE* yyin;
extern int yylineno;
extern int yyparse();
extern int yyrestart(FILE*);
extern struct Node* head;
extern int is_error;
int yylex();



int main(int argc,char** argv){
    if(argc<=1)//没有命令行参数
        return 1;
    FILE* f=fopen(argv[1],"r");
    if(!f)
    {
        perror(argv[1]);
        return 1;
    }
    yylineno=1;//记录行号
    yyrestart(f);//将语法分析器指向新打开的文件
    yyparse();//运行语法分析器
    //printf("head: %u\n",head);
    if(is_error==0){
        //print_tree(head);
        Program(head);
        }
    return 0;
}

