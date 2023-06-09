#ifndef INTERIM_H
#define INTERIM_H
#include<stdio.h>
#include<stdlib.h>
#include"semantic.h"
//优化
//1.t1=*a,t1=1 ==> *a=1
//2.0赋值多次

    //FROM_ARG用于传递给exp的arg，因为做为函数参数时的temp,temp[2]都会返回address
    //但实际上，temp[2]应返回basic
    //VARIABLE普通函数参数,char+int
      //先查询符号表中是否以及给该变量var值
    //TEMP临时变量,int
    //CONSTANT常数，int
    //ADDRESS结构体、数组做为函数参数char+int
    //WADDRESS写入地址，
    //FUNCTION函数，char
    //LABEL标号，
    //RELOP比较，见下,int
    //相等0，不等1，小于2，大于3，小于等于4，大于等于5



    
typedef struct Operand_* Operand;

typedef struct InterCode_* InterCode;



struct Operand_{

    enum{FROM_ARG,VARIABLE,TEMP,CONSTANT,ADDRESS,WADDRESS,FUNCTION,LABEL,RELOP}kind;
    int u_int;//t1t2
    char* u_char;//标记同一类型标签的序号
    Type type;//用于结构体和数组变量的Offset查询
};

//定义中间代码的操作类型
//PARAM打印param v+u_int
//ASSIGN打印：注意立即数
//ADDRASS2打印：t2=*t1
//ADDRADD3其实是在赋值语句中，如果左为address,右为其他的话
//ARG打印：如果address就&，不是则正常
struct InterCode_{
    enum{ILABEL,IFUNCTION,ASSIGN,
        ADD,SUB,MUL,DIV,
        ADDRASS1,ADDRASS2,ADDRASS3,
        GOTO,IF,RETURN,DEC,ARG,
        CALL,PARAM,READ,WRITE}kind;
    union{
        //LABEL,FUNCTION,GOTO,RETURN,ARG
        //PARAM,READ,WRITE
        struct{Operand op;}ulabel;
        //ASSIGN,CALL
        //ADDRASS1,ADDRASS2,ADDRASS3
        struct{Operand op1,op2;}uassign;
        //ADD,SUB,MUL,DIV
        struct{Operand result,op1,op2;}ubinop;
        //IF
        struct{Operand x,relop,y,z;}uif;
        //DEC
        struct{Operand op;int size;}udec;//内存空间申请
    }u;
    InterCode before;
    InterCode next;
};

void translate_print_test(InterCode temp);
void translate_print(FILE* f);
void translate_Program(struct Node* now,FILE* F);
void translate_ExtDef(struct Node* now);
void translate_FunDec(struct Node* now);
void translate_CompSt(struct Node* now);
void translate_DefList(struct Node* now);
void translate_StmtList(struct Node* now);
void translate_Def(struct Node* now);
void translate_Stmt(struct Node* now);
void translate_DecList(struct Node* now);
void translate_Exp(struct Node* now,Operand place);
void translate_CompSt(struct Node* now);
void translate_Cond(struct Node* now,Operand lt,Operand lf);
void translate_Dec(struct Node* now);
//void translate_VarList(struct Node* now);
void translate_VarDec(struct Node* now,Operand place);
void translate_Args(struct Node* now,InterCode here);
void translate_Specifier(struct Node* now);
void translate_StructSpecifier(struct Node* now);



void add_to_intercode(InterCode this);
Operand new_temp();
Operand new_label();
//t0=0表示从结构体中来，t0=1表示从数组中来
int get_offset(Type return_type,struct Node* after);
int get_size(Type type);



#endif