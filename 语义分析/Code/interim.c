#include"interim.h"

int lab_num=1;//用于LABEL的命名
int interim_is_error=0;//程序无错=0，有错=1
InterCode in_head=NULL;//记录双向链表的头部
InterCode in_now=NULL;//记录现在的双向链表
extern int temp_num;
//对于符号表中的符号，在加入table时就赋予其数字
int b=0;
int c=0;

//生成中间代码
void translate_print_test(InterCode temp){
        if(temp->kind==ILABEL){
            //printf("a translate_print ILABEL %d\n",temp->u.ulabel.op->u_int);
            printf("LABEL label");
            printf("%d ",temp->u.ulabel.op->u_int);
            printf(":");
        }
        else if(temp->kind==IFUNCTION){
            //printf("a translate_print IFUNCTION\n");
            printf("FUNCTION ");
            printf("%s ",temp->u.ulabel.op->u_char);
            printf(":");
        }
        else if(temp->kind==ASSIGN){//赋值操作
            //printf("a translate_print ASSIGN\n");
            Operand t1=temp->u.uassign.op1;
            Operand t2=temp->u.uassign.op2;
            if(t1==NULL||t2==NULL){
                printf("NULL ASSIGN\n");
            }
            else if(t1->kind==ADDRESS&&t2->kind==ADDRESS){//两边都是地址
                //printf("a 1.1\n");
                printf("*t%d",t1->u_int);
                printf(" := ");
                printf("*t%d",t2->u_int);
            }
            else if(t2->kind==ADDRESS){//只有右侧是地址
                //printf("a 1.3\n");
                printf("t%d",t1->u_int);
                printf(" := ");
                printf("*t%d",t2->u_int);
            }
            else if(t1->kind==ADDRESS){//只有左侧是地址
                //printf("a 1.2\n");
                printf("&t%d",t1->u_int);
                printf(" := ");
                if(t2->kind==CONSTANT) printf("#%d",t2->u_int);
                else printf("t%d",t2->u_int);
            }
            else{//都不是地址
                //printf("a 1.4\n");
                printf("t%d",t1->u_int);
                printf(" := ");
                if(t2->kind==CONSTANT) printf("#%d",t2->u_int);
                else printf("t%d",t2->u_int);
            }
        }
        else if(temp->kind==ADD){//加
            //printf("a translate_print  ADD\n");
            Operand t1=temp->u.ubinop.result;//运算结果
            Operand t2=temp->u.ubinop.op1;//加数a
            Operand t3=temp->u.ubinop.op2;//加数b
            printf("t%d",t1->u_int);
            printf(" := ");
            if(t2->kind==CONSTANT) printf("#%d",t2->u_int);
            else printf("t%d",t2->u_int);
            printf(" + ");
            if(t3->kind==CONSTANT) printf("#%d",t3->u_int);
            else printf("t%d",t3->u_int);
        }
        else if(temp->kind==SUB){//减
            //printf("a translate_print SUB\n");
            Operand t1=temp->u.ubinop.result;
            Operand t2=temp->u.ubinop.op1;
            Operand t3=temp->u.ubinop.op2;
            printf("t%d",t1->u_int);
            printf(" := ");
            if(t2->kind==CONSTANT) printf("#%d",t2->u_int);
            else printf("t%d",t2->u_int);
            printf(" - ");
            if(t3->kind==CONSTANT) printf("#%d",t3->u_int);
            else printf("t%d",t3->u_int);
        }
        else if(temp->kind==MUL){//乘
            //printf("a translate_print MUL\n");
            Operand t1=temp->u.ubinop.result;
            Operand t2=temp->u.ubinop.op1;
            Operand t3=temp->u.ubinop.op2;
            printf("t%d",t1->u_int);
            printf(" := ");
            if(t2->kind==CONSTANT) printf("#%d",t2->u_int);
            else printf("t%d",t2->u_int);
            printf(" * ");
            if(t3->kind==CONSTANT) printf("#%d",t3->u_int);
            else printf("t%d",t3->u_int);
        }
        else if(temp->kind==DIV){//除
            //printf("a translate_print DIV\n");
            Operand t1=temp->u.ubinop.result;
            Operand t2=temp->u.ubinop.op1;
            Operand t3=temp->u.ubinop.op2;
            printf("t%d",t1->u_int);
            printf(" := ");
            if(t2->kind==CONSTANT) printf("#%d",t2->u_int);
            else printf("t%d",t2->u_int);
            printf(" / ");
            if(t3->kind==CONSTANT) printf("#%d",t3->u_int);
            else printf("t%d",t3->u_int);
        }
        else if(temp->kind==ADDRASS2){
            //printf("a translate_print ADDRASS2\n");
            Operand t1=temp->u.uassign.op1;
            Operand t2=temp->u.uassign.op2;
            printf("t%d",t1->u_int);
            printf(" := ");
            printf("*t%d",t2->u_int);
        }
        else if(temp->kind==ADDRASS3){
            //printf("a translate_print ADDRASS3\n");
            Operand t1=temp->u.uassign.op1;
            Operand t2=temp->u.uassign.op2;
            printf("*t%d",t1->u_int);
            printf(" := ");
            if(t2->kind==CONSTANT) printf("#%d",t2->u_int);
            else printf("t%d",t2->u_int);
        }
        else if(temp->kind==ADDRASS1){
            //printf("a translate_print ADDRASS2\n");
            Operand t1=temp->u.uassign.op1;
            Operand t2=temp->u.uassign.op2;
            printf("t%d",t1->u_int);
            printf(" := ");
            printf("&t%d",t2->u_int);
        }
        else if(temp->kind==GOTO){
            //printf("a translate_print GOTO\n");
            printf("GOTO label");
            printf("%d",temp->u.ulabel.op->u_int);
        }
        else if(temp->kind==IF){
            //printf("a translate_print IF\n");
            Operand t1=temp->u.uif.x;
            Operand re=temp->u.uif.relop;
            Operand t2=temp->u.uif.y;
            Operand t3=temp->u.uif.z;
            printf("IF ");
            if(t1->kind==CONSTANT) printf("#%d",t1->u_int);
            else if(t1->kind==ADDRESS) printf("*t%d",t1->u_int);
            else printf("t%d",t1->u_int);

            if(re->u_int==0) printf(" == ");
            else if(re->u_int==1) printf(" != ");
            else if(re->u_int==2) printf(" < ");
            else if(re->u_int==3) printf(" > ");
            else if(re->u_int==4) printf(" <= ");
            else if(re->u_int==5) printf(" >= ");

            if(t2->kind==CONSTANT) printf("#%d",t2->u_int);
            else if(t2->kind==ADDRESS) printf("*t%d",t2->u_int);
            else printf("t%d",t2->u_int);
            printf(" GOTO label");
            printf("%d",t3->u_int);
        }
        else if(temp->kind==RETURN){
            //printf("a translate_print RETURN\n");
            printf("RETURN ");
            if(temp->u.ulabel.op->kind==CONSTANT) printf("#%d",temp->u.ulabel.op->u_int);
            else printf("t%d",temp->u.ulabel.op->u_int);
        }
        else if(temp->kind==DEC){
            //printf("a translate_print DEC\n");
            printf("DEC ");
            printf("t%d ",temp->u.udec.op->u_int);
            printf("%d",temp->u.udec.size);
        }
        else if(temp->kind==ARG){
            //printf("a translate_print ARG\n");
            printf("ARG ");
            if(temp->u.ulabel.op->kind==CONSTANT) printf("#%d",temp->u.ulabel.op->u_int);
            else if(temp->u.ulabel.op->kind==ADDRESS) printf("&t%d",temp->u.ulabel.op->u_int);
            else if(temp->u.ulabel.op->kind==WADDRESS) printf("*t%d",temp->u.ulabel.op->u_int);
            else printf("t%d",temp->u.ulabel.op->u_int);
        }
        else if(temp->kind==CALL){
            //printf("a translate_print CALL\n");
            Operand t1=temp->u.uassign.op1;
            Operand t2=temp->u.uassign.op2;
            if(t1!=NULL) printf("t%d := ",t1->u_int);
            printf("CALL ");
            printf("%s",t2->u_char);
        }
        else if(temp->kind==PARAM){
            //printf("a translate_print PARAM\n");
            printf("PARAM ");
            if(temp->u.ulabel.op->kind==CONSTANT) printf("#%d",temp->u.ulabel.op->u_int);
            else printf("t%d",temp->u.ulabel.op->u_int);
        }
        else if(temp->kind==READ){
            //printf("a translate_print READ\n");
            printf("READ ");
            if(temp->u.ulabel.op->kind==CONSTANT) printf("#%d",temp->u.ulabel.op->u_int);
            else printf("t%d",temp->u.ulabel.op->u_int);
        }
        else if(temp->kind==WRITE){
            //printf("a translate_print WRITE\n");
            printf("WRITE ");
            if(temp->u.ulabel.op->kind==CONSTANT) printf("#%d",temp->u.ulabel.op->u_int);
            else printf("t%d",temp->u.ulabel.op->u_int);
        }
        printf("\n");
}

void translate_print(FILE* f){
    //printf("a translate_print\n");
    InterCode temp=in_head;
    // InterCode temp=in_now;
    // in_head->before=NULL;
    int judge_is_null=0;
    int a=0;
//add,sub,mul,div没考虑null

// while(temp!=NULL){
//     a++;
//     temp=temp->next;
// }
// //printf("a: %d\n",a);

    while(temp!=NULL){
        if(temp->kind==ILABEL){
            //printf("a translate_print ILABEL %d\n",temp->u.ulabel.op->u_int);
            fprintf(f,"LABEL label");
            fprintf(f,"%d ",temp->u.ulabel.op->u_int);
            fprintf(f,":");
        }
        else if(temp->kind==IFUNCTION){
            //printf("a translate_print IFUNCTION\n");
            fprintf(f,"FUNCTION ");
            fprintf(f,"%s ",temp->u.ulabel.op->u_char);
            fprintf(f,":");
        }
        else if(temp->kind==ASSIGN){
            //printf("a translate_print ASSIGN\n");
            Operand t1=temp->u.uassign.op1;
            Operand t2=temp->u.uassign.op2;
            if(t1==NULL||t2==NULL){
                judge_is_null=1;
                //printf("a 1\n");
            }
            else if(t1->kind==ADDRESS&&t2->kind==ADDRESS){
                //printf("a 1.1\n");
                fprintf(f,"*t%d",t1->u_int);
                fprintf(f," := ");
                fprintf(f,"*t%d",t2->u_int);
            }
            else if(t2->kind==ADDRESS){
                //printf("a 1.3\n");
                fprintf(f,"t%d",t1->u_int);
                fprintf(f," := ");
                fprintf(f,"*t%d",t2->u_int);
            }
            else if(t1->kind==ADDRESS){
                //printf("a 1.2\n");
                fprintf(f,"&t%d",t1->u_int);
                fprintf(f," := ");
                if(t2->kind==CONSTANT) fprintf(f,"#%d",t2->u_int);
                else fprintf(f,"t%d",t2->u_int);
            }
            else{
                //printf("a 1.4\n");
                fprintf(f,"t%d",t1->u_int);
                fprintf(f," := ");
                if(t2->kind==CONSTANT) fprintf(f,"#%d",t2->u_int);
                else fprintf(f,"t%d",t2->u_int);
            }
        }
        else if(temp->kind==ADD){
            //printf("a translate_print  ADD\n");
            Operand t1=temp->u.ubinop.result;
            Operand t2=temp->u.ubinop.op1;
            Operand t3=temp->u.ubinop.op2;
            if(t1==NULL);
            else{
            fprintf(f,"t%d",t1->u_int);
            fprintf(f," := ");
            if(t2->kind==CONSTANT) fprintf(f,"#%d",t2->u_int);
            else fprintf(f,"t%d",t2->u_int);
            fprintf(f," + ");
            if(t3->kind==CONSTANT) fprintf(f,"#%d",t3->u_int);
            else fprintf(f,"t%d",t3->u_int);}
        }
        else if(temp->kind==SUB){
            //printf("a translate_print SUB\n");
            Operand t1=temp->u.ubinop.result;
            Operand t2=temp->u.ubinop.op1;
            Operand t3=temp->u.ubinop.op2;
            if(t1==NULL);
            else{
            fprintf(f,"t%d",t1->u_int);
            fprintf(f," := ");
            if(t2->kind==CONSTANT) fprintf(f,"#%d",t2->u_int);
            else fprintf(f,"t%d",t2->u_int);
            fprintf(f," - ");
            if(t3->kind==CONSTANT) fprintf(f,"#%d",t3->u_int);
            else fprintf(f,"t%d",t3->u_int);}
        }
        else if(temp->kind==MUL){
            //printf("a translate_print MUL\n");
            Operand t1=temp->u.ubinop.result;
            Operand t2=temp->u.ubinop.op1;
            Operand t3=temp->u.ubinop.op2;
            if(t1==NULL);
            else{
            fprintf(f,"t%d",t1->u_int);
            fprintf(f," := ");
            if(t2->kind==CONSTANT) fprintf(f,"#%d",t2->u_int);
            else fprintf(f,"t%d",t2->u_int);
            fprintf(f," * ");
            if(t3->kind==CONSTANT) fprintf(f,"#%d",t3->u_int);
            else fprintf(f,"t%d",t3->u_int);}
        }
        else if(temp->kind==DIV){
            //printf("a translate_print DIV\n");
            Operand t1=temp->u.ubinop.result;
            Operand t2=temp->u.ubinop.op1;
            Operand t3=temp->u.ubinop.op2;
            if(t1==NULL);
            else{
            fprintf(f,"t%d",t1->u_int);
            fprintf(f," := ");
            if(t2->kind==CONSTANT) fprintf(f,"#%d",t2->u_int);
            else fprintf(f,"t%d",t2->u_int);
            fprintf(f," / ");
            if(t3->kind==CONSTANT) fprintf(f,"#%d",t3->u_int);
            else fprintf(f,"t%d",t3->u_int);}
        }
        else if(temp->kind==ADDRASS2){
            //printf("a translate_print ADDRASS2\n");
            Operand t1=temp->u.uassign.op1;
            Operand t2=temp->u.uassign.op2;
            fprintf(f,"t%d",t1->u_int);
            fprintf(f," := ");
            fprintf(f,"*t%d",t2->u_int);
        }
        else if(temp->kind==ADDRASS3){
            //printf("a translate_print ADDRASS3\n");
            Operand t1=temp->u.uassign.op1;
            Operand t2=temp->u.uassign.op2;
            fprintf(f,"*t%d",t1->u_int);
            fprintf(f," := ");
            if(t2->kind==CONSTANT) fprintf(f,"#%d",t2->u_int);
            else fprintf(f,"t%d",t2->u_int);
        }
        else if(temp->kind==ADDRASS1){
            //printf("a translate_print ADDRASS2\n");
            Operand t1=temp->u.uassign.op1;
            Operand t2=temp->u.uassign.op2;
            fprintf(f,"t%d",t1->u_int);
            fprintf(f," := ");
            fprintf(f,"&t%d",t2->u_int);
        }
        else if(temp->kind==GOTO){
            //printf("a translate_print GOTO\n");
            fprintf(f,"GOTO label");
            fprintf(f,"%d",temp->u.ulabel.op->u_int);
        }
        else if(temp->kind==IF){
            //printf("a translate_print IF\n");
            Operand t1=temp->u.uif.x;
            Operand re=temp->u.uif.relop;
            Operand t2=temp->u.uif.y;
            Operand t3=temp->u.uif.z;
            fprintf(f,"IF ");
            if(t1->kind==CONSTANT) fprintf(f,"#%d",t1->u_int);
            else if(t1->kind==ADDRESS) fprintf(f,"*t%d",t1->u_int);
            else fprintf(f,"t%d",t1->u_int);

            if(re->u_int==0) fprintf(f," == ");
            else if(re->u_int==1) fprintf(f," != ");
            else if(re->u_int==2) fprintf(f," < ");
            else if(re->u_int==3) fprintf(f," > ");
            else if(re->u_int==4) fprintf(f," <= ");
            else if(re->u_int==5) fprintf(f," >= ");

            if(t2->kind==CONSTANT) fprintf(f,"#%d",t2->u_int);
            else if(t2->kind==ADDRESS) fprintf(f,"*t%d",t2->u_int);
            else fprintf(f,"t%d",t2->u_int);
            fprintf(f," GOTO label");
            fprintf(f,"%d",t3->u_int);
        }
        else if(temp->kind==RETURN){
            //printf("a translate_print RETURN\n");
            fprintf(f,"RETURN ");
            if(temp->u.ulabel.op->kind==CONSTANT) fprintf(f,"#%d",temp->u.ulabel.op->u_int);
            else fprintf(f,"t%d",temp->u.ulabel.op->u_int);
        }
        else if(temp->kind==DEC){
            //printf("a translate_print DEC\n");
            fprintf(f,"DEC ");
            fprintf(f,"t%d ",temp->u.udec.op->u_int);
            fprintf(f,"%d",temp->u.udec.size);
        }
        else if(temp->kind==ARG){
            //printf("a translate_print ARG\n");
            fprintf(f,"ARG ");
            if(temp->u.ulabel.op->kind==CONSTANT) fprintf(f,"#%d",temp->u.ulabel.op->u_int);
            else if(temp->u.ulabel.op->kind==ADDRESS) fprintf(f,"&t%d",temp->u.ulabel.op->u_int);
            else if(temp->u.ulabel.op->kind==WADDRESS) fprintf(f,"*t%d",temp->u.ulabel.op->u_int);
            else fprintf(f,"t%d",temp->u.ulabel.op->u_int);
        }
        else if(temp->kind==CALL){
            //printf("a translate_print CALL\n");
            Operand t1=temp->u.uassign.op1;
            Operand t2=temp->u.uassign.op2;
            if(t1!=NULL) fprintf(f,"t%d := ",t1->u_int);
            else{
                Operand t0=new_temp();
                fprintf(f,"t%d := ",t0->u_int);
            }
            fprintf(f,"CALL ");
            ////printf("a %s\n",t2->kind);
            fprintf(f,"%s",t2->u_char);
        }
        else if(temp->kind==PARAM){
            //printf("a translate_print PARAM\n");
            fprintf(f,"PARAM ");
            if(temp->u.ulabel.op->kind==CONSTANT) fprintf(f,"#%d",temp->u.ulabel.op->u_int);
            else fprintf(f,"t%d",temp->u.ulabel.op->u_int);
        }
        else if(temp->kind==READ){
            //printf("a translate_print READ\n");
            fprintf(f,"READ ");
            if(temp->u.ulabel.op->kind==CONSTANT) fprintf(f,"#%d",temp->u.ulabel.op->u_int);
            else fprintf(f,"t%d",temp->u.ulabel.op->u_int);
        }
        else if(temp->kind==WRITE){
            //printf("a translate_print WRITE\n");
            fprintf(f,"WRITE ");
            if(temp->u.ulabel.op->kind==CONSTANT) fprintf(f,"#%d",temp->u.ulabel.op->u_int);
            else fprintf(f,"t%d",temp->u.ulabel.op->u_int);
        }
        if(judge_is_null==0) fprintf(f,"\n");
        else judge_is_null=0;
        // fprintf(f,"\n");
        ////printf("a 2\n");
        temp=temp->next;
        // temp=temp->before;
    }
    // //printf("a: %d\n",a);
    // printf("b: %d\n",b);
    // printf("c: %d\n",c);

}

void translate_Program(struct Node* now,FILE* F){//开头的program
    //printf("a translate_Program\n");
    if(interim_is_error==1) return;
    struct Node* extdeflist=now->child;
    while(extdeflist->child!=NULL){
       struct Node* extdef=extdeflist->child;
       translate_ExtDef(extdef);
       extdeflist=extdef->brother;
    }
    //最后没错，就进行打印
    //TODO:print
    // InterCode H=in_head;
    // while(H!=NULL){
    //     translate_print_test(H);
    //     H=H->next;
    // }
    if(interim_is_error==0){
        translate_print(F);
    }
}

void translate_ExtDef(struct Node* now){//全局定义
    /*
    ExtDef → Specifier ExtDecList SEMI
    | Specifier SEMI
    | Specifier FunDec CompSt
    */
   //printf("a translate_ExtDef\n");
    //func+specifier
    if(interim_is_error==1) return;

    struct Node* after_specifier=now->child->brother;
    if(strcmp(after_specifier->name,"FunDec\0")==0){//如果是函数
        translate_FunDec(after_specifier);//处理函数节点
        translate_CompSt(after_specifier->brother);//处理语句块（函数体）
    }
    else if(strcmp(now->child->name,"Specifier\0")==0){//针对结构体或者全局定义
        translate_Specifier(now->child);
    }
}

void translate_FunDec(struct Node* now){
    //FunDec → ID LP VarList RP| ID LP RP
    //printf("a translate_FunDec\n");
    if(interim_is_error==1) return; //（是否为函数定义）
    //翻译id，生成函数标签的中间代码
    Operand func_op=(Operand)malloc(sizeof(struct Operand_));
    func_op->kind=FUNCTION;
    func_op->u_char=now->child->char_name;
    InterCode func_in=(InterCode)malloc(sizeof(struct InterCode_));
    func_in->kind=IFUNCTION;
    func_in->u.ulabel.op=func_op;
    add_to_intercode(func_in);
    //翻译varlist，参数列表
    if(now->child->brother->brother->brother==NULL) return;
    struct FUNCTION* find_func=find_in_function(now->child->char_name);
    FieldList func_field=find_func->field;
    while(func_field!=NULL){
        //1.每一个参数生成一个对应的operand，根据结构体、非结构体设置kind
        Operand field_op=(Operand)malloc(sizeof(struct Operand_));
        if(func_field->type->kind==STRUCTURE||func_field->type->kind==ARRAY) field_op->kind=ADDRESS;
        else field_op->kind=VARIABLE;
        //记录用户给的名称，以及符号表中赋予的Num
        struct TABLE* find_it=find_struct_in_table(func_field->name);
        field_op->u_int=find_it->variable;
        field_op->u_char=func_field->name;
        //2.每个参数生成一条中间代码
        InterCode field_in=(InterCode)malloc(sizeof(struct InterCode_));
        field_in->kind=PARAM;
        field_in->u.ulabel.op=field_op;
        add_to_intercode(field_in);
        func_field=func_field->tail;
    }
}

void translate_CompSt(struct Node* now){
    //CompSt → LC DefList StmtList RC
    //printf("a translate_CompSt\n");
    if(interim_is_error==1) return;
    struct Node* deflist=now->child->brother;
    struct Node* stmtlist=deflist->brother;
    translate_DefList(deflist);//代码块中的定义语句
    translate_StmtList(stmtlist);//代码块中的其他语句
    return;
}

void translate_DefList(struct Node* now){
    //DefList → Def DefList| 
    //printf("a translate_DefList\n");
    if(interim_is_error==1) return;
    struct Node* newdef=now->child;
    while(newdef!=NULL){
        translate_Def(newdef);
        newdef=newdef->brother->child;
    }
}

void translate_StmtList(struct Node* now){
    //StmtList → Stmt StmtList| 
    //printf("a translate_StmtList\n");
    if(interim_is_error==1) return;
    struct Node* child=now->child;
    while(child!=NULL){
        translate_Stmt(child);
        child=child->brother->child;
    }
}

void translate_Def(struct Node* now){
    //Def → Specifier DecList SEMI
    //printf("a translate_Def\n");
    if(interim_is_error==1) return;
    struct Node* declist=now->child->brother;
    translate_DecList(declist);
}

void translate_Stmt(struct Node* now){
/*   
Stmt → Exp SEMI
| CompSt
| RETURN Exp SEMI
| IF LP Exp RP Stmt
| IF LP Exp RP Stmt ELSE Stmt
| WHILE LP Exp RP Stmt
*/
    //printf("a translate_Stmt\n");
    //printf("a %d\n",now);
    if(interim_is_error==1) return;
    //printf("a 1\n");
    struct Node* child=now->child;
    //printf("aw %s\n",now->name);
    //printf("a %s\n",child->name);
    if(strcmp(child->name,"Exp\0")==0){
        //Stmt → Exp SEMI
        //printf("a translate_Stmt Exp\n");
        translate_Exp(now->child,NULL);
    }
    else if(strcmp(child->name,"CompSt\0")==0){
        //Stmt →CompSt
        //printf("a translate_Stmt CompSt\n");
        translate_CompSt(now->child);
    }
    else if(strcmp(child->name,"RETURN\0")==0){
        //Stmt →RETURN Exp SEMI
        //printf("a translate_Stmt RETURN\n");
        Operand t1=new_temp();
        translate_Exp(now->child->brother,t1);
        Operand t0=NULL;
        if(t1->kind==ADDRESS){
            //printf("a there\n");
            t0=new_temp();
            InterCode code0_in=(InterCode)malloc(sizeof(struct InterCode_));
            code0_in->kind=ADDRASS2;
            code0_in->u.uassign.op1=t0;
            code0_in->u.uassign.op2=t1;
            add_to_intercode(code0_in);
        }

        InterCode return_in=(InterCode)malloc(sizeof(struct InterCode_));
        return_in->kind=RETURN;
        if(t0==NULL) return_in->u.ulabel.op=t1;
        else return_in->u.ulabel.op=t0;
        add_to_intercode(return_in);
    }
    else if(strcmp(child->name,"IF\0")==0){
        //Stmt →IF LP Exp RP Stmt|IF LP Exp RP Stmt ELSE Stmt
        //printf("a translate_Stmt IF\n");
        struct Node* else_node=child->brother->brother->brother->brother->brother;

        //对于第一种形式的 if 语句（即不带 else 的情况），我们需要先生成两个新的标签，分别称为 l1 和 l2，用来标识 if 条件表达式的真假两条路径。
        //我们再调用函数 translate_Cond 来对条件表达式进行翻译，翻译结果是一个转移三地址码，将其分别添加到中间代码列表（InterCodeList）中，
        //然后在这两个转移代码中间插入 l1 和 l2 标签的代码。接下来，我们调用函数 translate_Stmt 来翻译 if 语句的主体并将生成的中间代码添加到中间代码列表中。

        if(else_node==NULL){
            //Stmt →IF LP Exp RP Stmt
            Operand l1=new_label();
            Operand l2=new_label();
            translate_Cond(child->brother->brother,l1,l2);
            InterCode l1_in=(InterCode)malloc(sizeof(struct InterCode_));
            l1_in->kind=ILABEL;
            l1_in->u.ulabel.op=l1;
            add_to_intercode(l1_in);
            translate_Stmt(child->brother->brother->brother->brother);
            InterCode l2_in=(InterCode)malloc(sizeof(struct InterCode_));
            l2_in->kind=ILABEL;
            l2_in->u.ulabel.op=l2;
            add_to_intercode(l2_in);     
            return;      
        }

        /*对于第二种形式的 if 语句，我们需要生成三个新的标签，分别称为 l1、l2 和 l3，用来标识 if 条件表达式、if 分支和 else 分支的三条路径。
        我们同样需要调用函数 translate_Cond 来对条件表达式进行翻译，并将翻译结果的转移三地址码添加到中间代码列表中。接下来，我们插入 l1 标签的代码，
        然后再调用 translate_Stmt 来翻译 if 分支的主体，并将生成的中间代码添加到中间代码列表中。我们再插入一个 GOTO 语句，它的目标是 l3 标签。
        接下来，我们插入 l2 标签的代码，然后调用 translate_Stmt 来翻译 else 分支的主体，并将生成的中间代码添加到中间代码列表中。
        最后，我们插入 l3 标签的代码。这样，整个 if 语句的中间代码就生成完毕了。*/

        else{
            //IF LP Exp RP Stmt ELSE Stmt
            Operand l1=new_label();
            Operand l2=new_label();
            Operand l3=new_label();
            //code1
            translate_Cond(child->brother->brother,l1,l2);
            //LABEL L1
            InterCode l1_in=(InterCode)malloc(sizeof(struct InterCode_));
            l1_in->kind=ILABEL;
            l1_in->u.ulabel.op=l1;
            add_to_intercode(l1_in);
            //code2
            translate_Stmt(child->brother->brother->brother->brother);
            //GOTO label3
            InterCode l2_in=(InterCode)malloc(sizeof(struct InterCode_));
            l2_in->kind=GOTO;
            l2_in->u.ulabel.op=l3;
            add_to_intercode(l2_in); 
            //LABEL l2
            InterCode l3_in=(InterCode)malloc(sizeof(struct InterCode_));
            l3_in->kind=ILABEL;
            l3_in->u.ulabel.op=l2;
            add_to_intercode(l3_in); 
            //code3
            translate_Stmt(child->brother->brother->brother->brother->brother->brother);
            //LABEL l3
            InterCode l4_in=(InterCode)malloc(sizeof(struct InterCode_));
            l4_in->kind=ILABEL;
            l4_in->u.ulabel.op=l3;
            add_to_intercode(l4_in); 
            return;
        }
    }
    else if(strcmp(child->name,"WHILE\0")==0){
        //Stmt →WHILE LP Exp RP Stmt
        //printf("a translate_Stmt WHILE\n");
        //三个标签表示循环开始、循环体、循环结束
        Operand l1=new_label();
        Operand l2=new_label();
        Operand l3=new_label();
        //LABEL L1（循环开始）
        InterCode l1_in=(InterCode)malloc(sizeof(struct InterCode_));
        l1_in->kind=ILABEL;
        l1_in->u.ulabel.op=l1;
        add_to_intercode(l1_in);
        //code1（表达式为真）
        translate_Cond(child->brother->brother,l2,l3);
        //LABEL l2
        InterCode l3_in=(InterCode)malloc(sizeof(struct InterCode_));
        l3_in->kind=ILABEL;
        l3_in->u.ulabel.op=l2;
        add_to_intercode(l3_in); 
        //code2
        translate_Stmt(child->brother->brother->brother->brother);
        //GOTO label1
        InterCode l2_in=(InterCode)malloc(sizeof(struct InterCode_));
        l2_in->kind=GOTO;//（一遍结束返回开头继续）
        l2_in->u.ulabel.op=l1;
        add_to_intercode(l2_in); 
        //LABEL l3  循环结束，指向L3，继续执行后面的代码
        InterCode l4_in=(InterCode)malloc(sizeof(struct InterCode_));
        l4_in->kind=ILABEL;
        l4_in->u.ulabel.op=l3;
        add_to_intercode(l4_in); 
    }   
    //printf("a 3\n");
}

void translate_DecList(struct Node* now){
//DecList → Dec| Dec COMMA DecList
//printf("a translate_DecList\n");
    if(interim_is_error==1) return;
    struct Node* child=now->child;
    translate_Dec(child);
    struct Node* comma=child->brother;
    if(comma==NULL);
    else translate_DecList(comma->brother);
}

void translate_Exp(struct Node* now,Operand place){//翻译表达式
    //printf("a translate_Exp\n");
    if(interim_is_error==1) return;
    //printf("a, %s\n",now->name);

    struct Node* child=now->child;
    struct Node* fir_bro=child->brother;
    if(fir_bro==NULL){//兄弟节点为空
        if(strcmp(child->name,"INT\0")==0){
            //Exp → INT
            //printf("a translate_Exp INT\n");
            int value=child->int_number;
            if(place==NULL) return;
            // Operand func_ope=(Operand)malloc(sizeof(struct Operand_));
            // func_ope->kind=CONSTANT;
            // func_ope->u_int=value;
            // InterCode int_in=(InterCode)malloc(sizeof(struct InterCode_));
            // int_in->kind=ASSIGN;
            // int_in->u.uassign.op1=place;
            // int_in->u.uassign.op2=func_ope;
            // add_to_intercode(int_in);
            place->kind=CONSTANT;
            place->u_int=value;
            return;
        }
        //没有exp->float
        else if(strcmp(child->name,"ID\0")==0){
            //Exp → ID
            //这里也可能出现结构体以及数组，均返回address
            //如op.x=1因此要考虑
            //printf("a translate_Exp ID %s\n",child->char_name);
            struct TABLE* find_id=find_struct_in_table(child->char_name);
            char* name=child->char_name;
            if(place==NULL) return;
            // Operand func_ope=(Operand)malloc(sizeof(struct Operand_));
            if(find_id->is_from_func==0&&find_id->field->type->kind!=BASIC)
                 place->kind=ADDRESS;
            else place->kind=VARIABLE;
            place->u_char=name;
            place->u_int=find_id->variable;
            place->type=find_id->field->type;
            //printf("a out\n");
            // InterCode id_in=(InterCode)malloc(sizeof(struct InterCode_));
            // id_in->kind=ASSIGN;
            // id_in->u.uassign.op1=place;
            // id_in->u.uassign.op2=func_ope;
            // add_to_intercode(id_in);
            return;
        }
    }
    struct Node* sec_bro=fir_bro->brother;
    if(sec_bro==NULL){
        if(strcmp(child->name,"MINUS\0")==0){
            //Exp → MINUS Exp
            if(place==NULL) return;
            //printf("a translate_Exp MINUS\n");
            Operand t1=new_temp();
            translate_Exp(child->brother,t1);
            InterCode minus_in=(InterCode)malloc(sizeof(struct InterCode_));
            minus_in->kind=SUB;
            minus_in->u.ubinop.result=place;
            minus_in->u.ubinop.op2=t1;
            Operand t2=(Operand)malloc(sizeof(struct Operand_));
            t2->kind=CONSTANT;
            t2->u_int=0;
            minus_in->u.ubinop.op1=t2;
            if(place->kind==FROM_ARG) place->kind=VARIABLE;
            //printf("a ater_print\n");
            add_to_intercode(minus_in); 
            //printf("a MINUSEXPout\n");    
            return;      
        }
        else if(strcmp(child->name,"NOT\0")==0){
            //Exp →NOT Exp
            //printf("a translate_Exp NOT\n");
            Operand l1=new_label();
            Operand l2=new_label();
            //code0
            InterCode code0_in=(InterCode)malloc(sizeof(struct InterCode_));
            code0_in->kind=ASSIGN;
            code0_in->u.uassign.op1=place;
            Operand t1=(Operand)malloc(sizeof(struct Operand_));
            t1->kind=CONSTANT;
            t1->u_int=0;
            code0_in->u.uassign.op2=t1;
            add_to_intercode(code0_in); 
            //code1
            translate_Cond(now,l1,l2);
            //code2-1
            InterCode code2_in=(InterCode)malloc(sizeof(struct InterCode_));
            code2_in->kind=ILABEL;
            code2_in->u.ulabel.op=l1;
            add_to_intercode(code2_in);
            //code2-2
            InterCode code22_in=(InterCode)malloc(sizeof(struct InterCode_));
            code22_in->kind=ASSIGN;
            code22_in->u.uassign.op1=place;
            Operand t2=(Operand)malloc(sizeof(struct Operand_));
            t2->kind=CONSTANT;
            t2->u_int=1;
            code22_in->u.uassign.op2=t2;
            add_to_intercode(code22_in);
            //LABEL l2
            InterCode label_in=(InterCode)malloc(sizeof(struct InterCode_));
            label_in->kind=ILABEL;
            label_in->u.ulabel.op=l2;
            add_to_intercode(label_in);   
            return; 
        }
    }
    if(strcmp(fir_bro->name,"ASSIGNOP\0")==0){
        //Exp → Exp ASSIGNOP Exp
        //由于3.1，所以左侧的exp可能不是由ID生成
        //printf("a translate_Exp ASSIGNOP\n");
        if(strcmp(child->child->name,"ID\0")==0){
            //printf("a translate_Exp ASSIGNOP ID\n");
            Operand t1=new_temp();
            //code1
            translate_Exp(fir_bro->brother,t1);
            //code21
            InterCode code21_in=(InterCode)malloc(sizeof(struct InterCode_));
            code21_in->kind=ASSIGN;
            code21_in->u.uassign.op2=t1;
            Operand t2=(Operand)malloc(sizeof(struct Operand_));
            struct TABLE* find_it=find_struct_in_table(child->child->char_name);
            t2->kind=VARIABLE;
            t2->u_char=child->child->char_name;
            t2->u_int=find_it->variable;
            code21_in->u.uassign.op1=t2;
            add_to_intercode(code21_in);
            //code22
            //printf("a 2\n");
            InterCode code22_in=(InterCode)malloc(sizeof(struct InterCode_));
            code22_in->kind=ASSIGN;
            code22_in->u.uassign.op1=place;
            code22_in->u.uassign.op2=t2;
            add_to_intercode(code22_in);
            return;
        }
        else{
            Operand t1=new_temp();
            //code1
            translate_Exp(fir_bro->brother,t1);
            Operand t0=NULL;
            if(t1->kind==ADDRESS){
                //生成t0=*t1语句
                t0=new_temp();
                InterCode in_1=(InterCode)malloc(sizeof(struct InterCode_));
                in_1->kind=ADDRASS2;
                in_1->u.uassign.op1=t0;
                in_1->u.uassign.op2=t1;
                add_to_intercode(in_1);
            }
            //code21
            InterCode code21_in=(InterCode)malloc(sizeof(struct InterCode_));
            code21_in->kind=ADDRASS3;
            if(t0==NULL) code21_in->u.uassign.op2=t1;
            else code21_in->u.uassign.op2=t0;
            Operand t2=new_temp();
            translate_Exp(child,t2);
            code21_in->u.uassign.op1=t2;
            add_to_intercode(code21_in);
            //code22
            //printf("a 2\n");
            InterCode code22_in=(InterCode)malloc(sizeof(struct InterCode_));
    re
            return;
        }
    }
    if(strcmp(fir_bro->name,"AND\0")==0||strcmp(fir_bro->name,"OR\0")==0||strcmp(fir_bro->name,"RELOP\0")==0){
        //Exp → Exp AND\OR\RELOP Exp
            //printf("a translate_Exp AND\n");
            Operand l1=new_label();
            Operand l2=new_label();
            //code0
            InterCode code0_in=(InterCode)malloc(sizeof(struct InterCode_));
            code0_in->kind=ASSIGN;
            code0_in->u.uassign.op1=place;
            Operand t1=(Operand)malloc(sizeof(struct Operand_));
            t1->kind=CONSTANT;
            t1->u_int=0;
            code0_in->u.uassign.op2=t1;
            add_to_intercode(code0_in); 
            //code1
            translate_Cond(now,l1,l2);
            //code2-1
            InterCode code2_in=(InterCode)malloc(sizeof(struct InterCode_));
            code2_in->kind=ILABEL;
            code2_in->u.ulabel.op=l1;
            add_to_intercode(code2_in);
            //code2-2
            InterCode code22_in=(InterCode)malloc(sizeof(struct InterCode_));
            code22_in->kind=ASSIGN;
            code22_in->u.uassign.op1=place;
            Operand t2=(Operand)malloc(sizeof(struct Operand_));
            t2->kind=CONSTANT;
            t2->u_int=1;
            code22_in->u.uassign.op2=t2;
            add_to_intercode(code22_in);
            //LABEL l2
            InterCode label_in=(InterCode)malloc(sizeof(struct InterCode_));
            label_in->kind=ILABEL;
            label_in->u.ulabel.op=l2;
            add_to_intercode(label_in);   
            //printf("a ANDout\n");
            return;
    }
    if(strcmp(fir_bro->name,"PLUS\0")==0){
        //Exp → Exp PLUS Exp
        //printf("a translate_Exp PLUS\n");
        Operand t1=new_temp();
        Operand t2=new_temp();
        translate_Exp(child,t1);
        translate_Exp(fir_bro->brother,t2);
        Operand t3=NULL;
        Operand t4=NULL;
        if(t1->kind==CONSTANT&&t2->kind==CONSTANT) place->kind=VARIABLE;
        if(t1->kind==ADDRESS){
            t3=new_temp();
            InterCode in1=(InterCode)malloc(sizeof(struct InterCode_));
            in1->kind=ADDRASS2;
            in1->u.uassign.op1=t3;
            in1->u.uassign.op2=t1;
            add_to_intercode(in1);
        }
        if(t2->kind==ADDRESS){
            t4=new_temp();
            InterCode in1=(InterCode)malloc(sizeof(struct InterCode_));
            in1->kind=ADDRASS2;
            in1->u.uassign.op1=t4;
            in1->u.uassign.op2=t2;
            add_to_intercode(in1);
        }
        InterCode code0_in=(InterCode)malloc(sizeof(struct InterCode_));
        code0_in->kind=ADD;
        code0_in->u.ubinop.result=place;
        if(t3==NULL) code0_in->u.ubinop.op1=t1;
        else code0_in->u.ubinop.op1=t3;
        if(t4==NULL) code0_in->u.ubinop.op2=t2;
        else code0_in->u.ubinop.op2=t4;
        add_to_intercode(code0_in);
        //printf("a PLUSout\n");
        return; 
    }
    if(strcmp(fir_bro->name,"MINUS\0")==0){
        //Exp → Exp MINUS Exp
        //printf("a translate_Exp MINUS\n");
        Operand t1=new_temp();
        Operand t2=new_temp();
        translate_Exp(child,t1);
        translate_Exp(fir_bro->brother,t2);
        Operand t3=NULL;
        Operand t4=NULL;
        if(t1->kind==CONSTANT&&t2->kind==CONSTANT) place->kind=VARIABLE;
        if(t1->kind==ADDRESS){
            t3=new_temp();
            InterCode in1=(InterCode)malloc(sizeof(struct InterCode_));
            in1->kind=ADDRASS2;
            in1->u.uassign.op1=t3;
            in1->u.uassign.op2=t1;
            add_to_intercode(in1);
        }
        if(t2->kind==ADDRESS){
            t4=new_temp();
            InterCode in1=(InterCode)malloc(sizeof(struct InterCode_));
            in1->kind=ADDRASS2;
            in1->u.uassign.op1=t4;
            in1->u.uassign.op2=t2;
            add_to_intercode(in1);
        }
        InterCode code0_in=(InterCode)malloc(sizeof(struct InterCode_));
        code0_in->kind=SUB;
        code0_in->u.ubinop.result=place;
        if(t3==NULL) code0_in->u.ubinop.op1=t1;
        else code0_in->u.ubinop.op1=t3;
        if(t4==NULL) code0_in->u.ubinop.op2=t2;
        else code0_in->u.ubinop.op2=t4;
        add_to_intercode(code0_in);
        //printf("a MINUSout\n");
        return; 
    }
    if(strcmp(fir_bro->name,"STAR\0")==0){
        //Exp → Exp STAR Exp
        //printf("a translate_Exp STAR\n");
        Operand t1=new_temp();
        Operand t2=new_temp();
        translate_Exp(child,t1);
        translate_Exp(fir_bro->brother,t2);
        if(t1->kind==CONSTANT&&t2->kind==CONSTANT) place->kind=VARIABLE;
        Operand t3=NULL;
        Operand t4=NULL;
        if(t1->kind==ADDRESS){
            t3=new_temp();
            InterCode in1=(InterCode)malloc(sizeof(struct InterCode_));
            in1->kind=ADDRASS2;
            in1->u.uassign.op1=t3;
            in1->u.uassign.op2=t1;
            add_to_intercode(in1);
        }
        if(t2->kind==ADDRESS){
            t4=new_temp();
            InterCode in1=(InterCode)malloc(sizeof(struct InterCode_));
            in1->kind=ADDRASS2;
            in1->u.uassign.op1=t4;
            in1->u.uassign.op2=t2;
            add_to_intercode(in1);
        }
        InterCode code0_in=(InterCode)malloc(sizeof(struct InterCode_));
        code0_in->kind=MUL;
        code0_in->u.ubinop.result=place;
        if(t3==NULL) code0_in->u.ubinop.op1=t1;
        else code0_in->u.ubinop.op1=t3;
        if(t4==NULL) code0_in->u.ubinop.op2=t2;
        else code0_in->u.ubinop.op2=t4;
        add_to_intercode(code0_in);
        return; 
    }
    if(strcmp(fir_bro->name,"DIV\0")==0){
        //Exp → Exp DIV Exp
        //printf("a translate_Exp DIV\n");
        Operand t1=new_temp();
        Operand t2=new_temp();
        translate_Exp(child,t1);
        translate_Exp(fir_bro->brother,t2);
        //printf("a OUTDIV\n");
        Operand t3=NULL;
        Operand t4=NULL;
        if(t1->kind==CONSTANT&&t2->kind==CONSTANT) place->kind=VARIABLE;
        if(t1->kind==ADDRESS){
            t3=new_temp();
            InterCode in1=(InterCode)malloc(sizeof(struct InterCode_));
            in1->kind=ADDRASS2;
            in1->u.uassign.op1=t3;
            in1->u.uassign.op2=t1;
            add_to_intercode(in1);
        }
        //printf("a OUTDIV2\n");
        if(t2->kind==ADDRESS){
            t4=new_temp();
            InterCode in1=(InterCode)malloc(sizeof(struct InterCode_));
            in1->kind=ADDRASS2;
            in1->u.uassign.op1=t4;
            in1->u.uassign.op2=t2;
            add_to_intercode(in1);
        }
        //printf("a OUTDIV3\n");
        InterCode code0_in=(InterCode)malloc(sizeof(struct InterCode_));
        code0_in->kind=DIV;
        code0_in->u.ubinop.result=place;
        if(t3==NULL) code0_in->u.ubinop.op1=t1;
        else code0_in->u.ubinop.op1=t3;
        if(t4==NULL) code0_in->u.ubinop.op2=t2;
        else code0_in->u.ubinop.op2=t4;
        add_to_intercode(code0_in);
        return; 
    }
    if(strcmp(fir_bro->name,"DOT\0")==0){
        //Exp → Exp DOT ID,结构体
        //1.获取结构体的首地址 t2=t1+#4, t1\v1->address
        //printf("a translate_Exp DOT\n");
        Operand t1=new_temp();
        translate_Exp(child,t1);
        if(t1->kind==VARIABLE){
            //0.有address和variable的区别
            //需要增加一个&d的操作
            //printf("a Exp DOT ID-VARIABLE\n");
            Operand t0=new_temp();
            t0->kind=ADDRESS;
            InterCode in=(InterCode)malloc(sizeof(struct InterCode_));
            in->kind=ADDRASS1;
            in->u.uassign.op1=t0;
            in->u.uassign.op2=t1;
            add_to_intercode(in);
            //2.获取当前domain的偏移量
            int offset=get_offset(t1->type,fir_bro->brother);
            if(offset==-1) return;
            Operand t3=(Operand)malloc(sizeof(struct Operand_));
            t3->kind=CONSTANT;
            t3->u_int=offset;
            //3.计算获得现在的地址值，打印中间代码
            if(place==NULL) return;
            if(place->kind==FROM_ARG) place->kind=WADDRESS;
            else place->kind=ADDRESS;
            struct TABLE* F=find_struct_in_table(fir_bro->brother->char_name);
            place->type=F->field->type;
            // Operand t2=new_temp();
            // t2->kind=ADDRESS;
            InterCode code0_in=(InterCode)malloc(sizeof(struct InterCode_));
            code0_in->kind=ADD;
            code0_in->u.ubinop.result=place;
            code0_in->u.ubinop.op1=t0;
            code0_in->u.ubinop.op2=t3;
            add_to_intercode(code0_in);
            //PLACE=*t2
            // if(place==NULL){
            //     Operand t4=new_temp();
            //     t4->kind=VARIABLE;
            //     InterCode code1_in=(InterCode)malloc(sizeof(struct InterCode_));
            //     code1_in->kind=ASSIGN;
            //     code1_in->u.uassign.op1=t4;
            //     code1_in->u.uassign.op2=t2;
            //     add_to_intercode(code1_in);  
            //     }
            // else{
            //     place->kind=VARIABLE;
            //     InterCode code1_in=(InterCode)malloc(sizeof(struct InterCode_));
            //     code1_in->kind=ASSIGN;
            //     code1_in->u.uassign.op1=place;
            //     code1_in->u.uassign.op2=t2;
            //     add_to_intercode(code1_in);
            //     }
        }
        else{
            //2.获取当前domain的偏移量
            //printf("a Exp DOT ID-address\n");
            int offset=get_offset(t1->type,fir_bro->brother);
            if(offset==-1) return;
            Operand t3=(Operand)malloc(sizeof(struct Operand_));
            t3->kind=CONSTANT;
            t3->u_int=offset;
            //3.计算获得现在的地址值，打印中间代码
            // Operand t2=new_temp();
            // t2->kind=ADDRESS;
            if(place==NULL) return;
            if(place->kind==FROM_ARG) place->kind=WADDRESS;
            else place->kind=ADDRESS;
            //printf("a %s\n",fir_bro->brother->char_name);
            struct TABLE* F=find_struct_in_table(fir_bro->brother->char_name);
            place->type=F->field->type;

            InterCode code0_in=(InterCode)malloc(sizeof(struct InterCode_));
            code0_in->kind=ADD;
            code0_in->u.ubinop.result=place;
            code0_in->u.ubinop.op1=t1;
            code0_in->u.ubinop.op2=t3;
            add_to_intercode(code0_in);
            // if(place==NULL){
            //     Operand t4=new_temp();
            //     t4->kind=VARIABLE;
            //     InterCode code1_in=(InterCode)malloc(sizeof(struct InterCode_));
            //     code1_in->kind=ASSIGN;
            //     code1_in->u.uassign.op1=t4;
            //     code1_in->u.uassign.op2=t2;
            //     add_to_intercode(code1_in);  
            //     } 
            // else{
            //     place->kind=VARIABLE;
            //     InterCode code1_in=(InterCode)malloc(sizeof(struct InterCode_));
            //     code1_in->kind=ASSIGN;
            //     code1_in->u.uassign.op1=place;
            //     code1_in->u.uassign.op2=t2;
            //     add_to_intercode(code1_in);
            //     }
        }
        return;
    }
    if(strcmp(fir_bro->name,"LB\0")==0){
        //Exp → Exp LB Exp RB，数组
        //1.获取数组的首地址
        //printf("a translate_Exp LB\n");
        Operand t1=new_temp();
        translate_Exp(child,t1);
        if(t1->kind==VARIABLE){
            //0.有address和variable的区别
            //需要增加一个&d的操作
            //printf("a translate_Exp LB VARIABLE\n");
            Operand t0=new_temp();
            t0->kind=ADDRESS;
            InterCode in=(InterCode)malloc(sizeof(struct InterCode_));
            in->kind=ADDRASS1;
            in->u.uassign.op1=t0;
            in->u.uassign.op2=t1;
            add_to_intercode(in);
            //2.获取数组单个单位的长度offset
            int offset=get_offset(t1->type,NULL);
            //printf("a %s %d\n",child->child->char_name,offset);
            if(offset==-1) return;
            //printf("a offset:%d\n",offset);
            //3.生成t2=offset*i
            Operand i=new_temp();
            translate_Exp(fir_bro->brother,i);
            Operand t2=new_temp();
            Operand o_offset=(Operand)malloc(sizeof(struct Operand_));
            o_offset->kind=CONSTANT;
            o_offset->u_int=offset;
            InterCode in2=(InterCode)malloc(sizeof(struct InterCode_));
            in2->kind=MUL;
            in2->u.ubinop.result=t2;
            in2->u.ubinop.op1=i;
            in2->u.ubinop.op2=o_offset;
            add_to_intercode(in2);
            //4.生成中间代码t3=t1+t2
            // Operand t3=new_temp();
            // t3->kind=ADDRESS;
            if(place==NULL) return;
            if(place->kind==FROM_ARG) place->kind=WADDRESS;
            else place->kind=ADDRESS;
            place->type=t1->type->u.array.elem;
            InterCode code0_in=(InterCode)malloc(sizeof(struct InterCode_));
            code0_in->kind=ADD;
            code0_in->u.ubinop.result=place;
            code0_in->u.ubinop.op1=t0;
            code0_in->u.ubinop.op2=t2;
            add_to_intercode(code0_in);
            //printf("a here\n");
            //4.生成中间代码t4/PLACE=*t3
            // if(place==NULL){
            //     return;
            //     // Operand t4=new_temp();
            //     // t4->kind=VARIABLE;
            //     // InterCode code1_in=(InterCode)malloc(sizeof(struct InterCode_));
            //     // code1_in->kind=ASSIGN;
            //     // code1_in->u.uassign.op1=t4;
            //     // code1_in->u.uassign.op2=t3;
            //     // add_to_intercode(code1_in);  
            //     } 
            // else{
            //     place=
            //     // place->kind=VARIABLE;
            //     // InterCode code1_in=(InterCode)malloc(sizeof(struct InterCode_));
            //     // code1_in->kind=ASSIGN;
            //     // code1_in->u.uassign.op1=place;
            //     // code1_in->u.uassign.op2=t3;
            //     // add_to_intercode(code1_in);
            //     }
        }
        else{
            //2.获取数组单个单位的长度offset,生成t2=offset*i
            //printf("a translate_Exp LB NON-VARIABLE\n");
            int offset=get_offset(t1->type,NULL);
            if(offset==-1) return;
            Operand i=new_temp();
            translate_Exp(fir_bro->brother,i);
            Operand t2=new_temp();
            Operand o_offset=(Operand)malloc(sizeof(struct Operand_));
            o_offset->kind=CONSTANT;
            o_offset->u_int=offset;
            InterCode in2=(InterCode)malloc(sizeof(struct InterCode_));
            in2->kind=MUL;
            in2->u.ubinop.result=t2;
            in2->u.ubinop.op1=i;
            in2->u.ubinop.op2=o_offset;
            add_to_intercode(in2);
            //4.生成中间代码t3=t1+t2
            //Operand t3=new_temp();
            if(place==NULL) return;
            if(place->kind==FROM_ARG) place->kind=WADDRESS;
            else place->kind=ADDRESS;
            place->type=t1->type->u.array.elem;
            InterCode code0_in=(InterCode)malloc(sizeof(struct InterCode_));
            code0_in->kind=ADD;
            code0_in->u.ubinop.result=place;
            code0_in->u.ubinop.op1=t1;
            code0_in->u.ubinop.op2=t2;
            add_to_intercode(code0_in);
            //4.生成中间代码t4/PLACE=*t3
            // if(place==NULL){
            //     Operand t4=new_temp();
            //     t4->kind=VARIABLE;
            //     InterCode code1_in=(InterCode)malloc(sizeof(struct InterCode_));
            //     code1_in->kind=ADDRASS2;
            //     code1_in->u.uassign.op1=t4;
            //     code1_in->u.uassign.op2=t3;
            //     add_to_intercode(code1_in);  
            //     } 
            // else{
            //     place->kind=VARIABLE;
            //     InterCode code1_in=(InterCode)malloc(sizeof(struct InterCode_));
            //     code1_in->kind=ADDRASS2;
            //     code1_in->u.uassign.op1=place;
            //     code1_in->u.uassign.op2=t3;
            //     add_to_intercode(code1_in);
            //     }
        }
        //printf("a LBout\n");
        return;
    }
    if(strcmp(fir_bro->name,"Exp\0")==0){
        //Exp → LP Exp RP
        //printf("a translate_Exp Exp\n");
        translate_Exp(fir_bro,place);
        return;
    }
    if(strcmp(sec_bro->name,"Args\0")==0){
        //Exp → ID LP Args RP
        //函数调用，有参数
        //0.区分是否为read/write
        //printf("a translate_Exp Args\n");
        char* function_name=child->char_name;
        /*
        这段代码使用条件语句判断函数名是否为"write"，如果是，则创建一个临时变量t1，将其初始化为第二个子节点的值，然后将地址类的值取地址，并将其存储在t0中。

        接下来，创建一个表示写操作的中间代码code1_in并设置其操作数op为t0或t1（根据t0是否为空）。最后将code1_in添加到一个中间代码序列中，并返回。
        */
        if(strcmp(function_name,"write")==0){
            //printf("a translate_Exp Args Write\n");
            Operand t1=new_temp();
            translate_Exp(sec_bro->child,t1);
            //要对地址类的取地址处理
            Operand t0=NULL;
            //printf("a there\n");
            if(t1->kind==ADDRESS){
                t0=new_temp();
                InterCode code0_in=(InterCode)malloc(sizeof(struct InterCode_));
                code0_in->kind=ADDRASS2;
                code0_in->u.uassign.op1=t0;
                code0_in->u.uassign.op2=t1;
                add_to_intercode(code0_in);
            }
            //printf("a here\n");
            InterCode code1_in=(InterCode)malloc(sizeof(struct InterCode_));
            code1_in->kind=WRITE;
            if(t0==NULL) code1_in->u.ulabel.op=t1;
            else code1_in->u.ulabel.op=t0;
            add_to_intercode(code1_in);
            return;
        }
        //1.ARG传递参数，逆序
        translate_Args(sec_bro,NULL);
        //1.2创建函数operand
        Operand func_ope=(Operand)malloc(sizeof(struct Operand_));
        func_ope->kind=FUNCTION;
        func_ope->u_char=function_name;
        //printf("a %s\n",function_name);
        //2.place=CALL FUNCTION,保存返回值
        InterCode code1_in=(InterCode)malloc(sizeof(struct InterCode_));
        code1_in->kind=CALL;
        code1_in->u.uassign.op1=place;
        code1_in->u.uassign.op2=func_ope;
        add_to_intercode(code1_in);
        return;
    }
    if(strcmp(sec_bro->name,"RP\0")==0){
        //Exp → ID LP RP
        //没有参数的函数调用
        //0.区分是否为read/write
        //printf("a translate_Exp RP\n");
        char* function_name=child->char_name;
        if(strcmp(function_name,"read")==0){
            InterCode code1_in=(InterCode)malloc(sizeof(struct InterCode_));
            code1_in->kind=READ;
            code1_in->u.ulabel.op=place;
            add_to_intercode(code1_in);
            return;
        }
        //1.2创建函数operand
        Operand func_ope=(Operand)malloc(sizeof(struct Operand_));
        func_ope->kind=FUNCTION;
        func_ope->u_char=function_name;
        //printf("a %s\n",function_name);
        //2.place=CALL FUNCTION,保存返回值
        InterCode code1_in=(InterCode)malloc(sizeof(struct InterCode_));
        code1_in->kind=CALL;
        code1_in->u.uassign.op1=place;
        code1_in->u.uassign.op2=func_ope;
        add_to_intercode(code1_in);
        return;
    }
}

void translate_Cond(struct Node* now,Operand lt,Operand lf){
    //printf("a translate_Cond\n");
    if(interim_is_error==1) return;
    struct Node* child=now->child;

    if(child->brother!=NULL&&strcmp(child->brother->name,"RELOP\0")==0){
        //printf("a translate_Cond RELOP\n");
        Operand t1=new_temp();
        Operand t2=new_temp();
        //code1+code2
        translate_Exp(child,t1);
        translate_Exp(child->brother->brother,t2);
        Operand op=(Operand)malloc(sizeof(struct Operand_));
        op->kind=RELOP;
        op->u_int=child->brother->linenumber;
        //code3
        InterCode code3=(InterCode)malloc(sizeof(struct InterCode_));
        code3->kind=IF;
        code3->u.uif.x=t1;
        code3->u.uif.relop=op;
        code3->u.uif.y=t2;
        code3->u.uif.z=lt;
        add_to_intercode(code3);
        //goto labelfalse
        InterCode code4=(InterCode)malloc(sizeof(struct InterCode_));
        code4->kind=GOTO;
        code4->u.ulabel.op=lf;
        add_to_intercode(code4);
    }
    else if(strcmp(child->name,"NOT\0")==0){
        //printf("a translate_Cond NOT\n");
        translate_Cond(now->child->brother,lf,lt);
    }
    else if(child->brother!=NULL&&strcmp(child->brother->name,"AND\0")==0){
        //printf("a translate_Cond AND\n");
        Operand l1=new_label();
        //code1
        translate_Cond(child,l1,lf);
        //label1
        InterCode code1=(InterCode)malloc(sizeof(struct InterCode_));
        code1->kind=ILABEL;
        code1->u.ulabel.op=l1;
        add_to_intercode(code1);
        //code2
        translate_Cond(child->brother->brother,lt,lf);
    }
    else if(child->brother!=NULL&&strcmp(child->brother->name,"OR\0")==0){
        //printf("a translate_Cond OR\n");
        Operand l1=new_label();
        //code1
        translate_Cond(child,lt,l1);
        //label1
        InterCode code1=(InterCode)malloc(sizeof(struct InterCode_));
        code1->kind=ILABEL;
        code1->u.ulabel.op=l1;
        add_to_intercode(code1);
        //code2
        translate_Cond(child->brother->brother,lt,lf);
        //printf("a ORout\n");
    }
    else{
        //printf("a translate_Cond else\n");
        Operand t1=new_temp();
        //code1
        translate_Exp(now,t1);
        //code2
        Operand op=(Operand)malloc(sizeof(struct Operand_));
        op->kind=RELOP;
        op->u_int=1;
        Operand t2=(Operand)malloc(sizeof(struct Operand_));
        t2->kind=CONSTANT;
        t2->u_int=0;
        //printf("a out0\n");
        InterCode code2=(InterCode)malloc(sizeof(struct InterCode_));
        code2->kind=IF;
        code2->u.uif.x=t1;
        code2->u.uif.relop=op;
        code2->u.uif.y=t2;
        code2->u.uif.z=lt;
        add_to_intercode(code2);
        //goto
        InterCode code4=(InterCode)malloc(sizeof(struct InterCode_));
        code4->kind=GOTO;
        code4->u.ulabel.op=lf;
        add_to_intercode(code4);
        //printf("a out1\n");
    }
    return;
}

void translate_Dec(struct Node* now){
    //Dec → VarDec| VarDec ASSIGNOP Exp
    //printf("a translate_Dec\n");
    if(interim_is_error==1) return;
    struct Node* vardec=now->child;
    struct Node* fir_bro=vardec->brother;
    if(fir_bro==NULL){
        Operand t1=new_temp();
        translate_VarDec(vardec,t1);
    }
    else{
        Operand rem1=new_temp();
        Operand rem2=new_temp();
        translate_VarDec(vardec,rem1);
        translate_Exp(fir_bro->brother,rem2);
        
        InterCode func_in=(InterCode)malloc(sizeof(struct InterCode_));
        func_in->kind=ASSIGN;
        func_in->u.uassign.op1=rem1;
        func_in->u.uassign.op2=rem2;
        add_to_intercode(func_in);
    }
}

void translate_VarDec(struct Node* now,Operand place){
    //VarDec → ID| VarDec LB INT RB
    //会生成DEC语句
    //只会出现一维数组，所以在else语句中直接处理数组
    //printf("a translate_VarDec\n");
    if(interim_is_error==1) return;
    if(now->child->brother==NULL){
        //可能是普通变量，可能是structure
        struct TABLE* find_it=find_struct_in_table(now->child->char_name);
        //printf("a is_find?%d\n",find_it);
        FieldList find_field=find_it->field;
        if(find_field->type->kind==BASIC){
            //printf("a isBASIC\n");
            //printf("a %s\n",now->child->char_name);
            if(place==NULL) return;
            place->kind=VARIABLE;
            place->u_int=find_it->variable;
            place->u_char=now->child->char_name;
            return;
        }
        else if(find_field->type->kind==STRUCTURE){
            //printf("a isSTRUCTURE\n");
            //printf("a %s\n",now->child->char_name);
            if(place==NULL) return;
            place->kind=VARIABLE;
            place->u_int=find_it->variable;
            place->u_char=now->child->char_name;
            //生成dec语句
            //printf("a create_dec3\n");
            InterCode func_in=(InterCode)malloc(sizeof(struct InterCode_));
            func_in->kind=DEC;
            func_in->u.udec.op=place;
            func_in->u.udec.size=get_size(find_field->type);
            add_to_intercode(func_in);
            return;
        }
        //printf("a isERROR\n");
    }
    else{
        //只能是一维数组
        //1.出现多维数组，输出error
        if(strcmp(now->child->child->name,"ID\0")!=0){
            interim_is_error=1;
            printf("Cannot translate: Code contains variables of error array type.\n");
            return;
        }
        //printf("a isARRAY\n");
        if(place==NULL) return;
        place->kind=VARIABLE;
        place->u_char=now->child->char_name;
        struct TABLE* find_it=find_struct_in_table(now->child->child->char_name);
        place->u_int=find_it->variable;
        //生成DEC语句
        //printf("a create_dec2\n");
        InterCode func_in=(InterCode)malloc(sizeof(struct InterCode_));
        func_in->kind=DEC;
        func_in->u.udec.op=place;
        func_in->u.udec.size=get_size(find_it->field->type);
        add_to_intercode(func_in);
        return;
    }
}

void translate_Args(struct Node* now,InterCode here){
//传递的如果是数组、结构体，是一个address类型
//逆序
//Args → Exp COMMA Args| Exp
    //printf("a translate_Args\n");
    if(interim_is_error==1) return;

    struct Node* exp=now->child;
    //1.翻译exp信息
    Operand t1=new_temp();
    t1->kind=FROM_ARG;
    translate_Exp(exp,t1);
    //1.0修改kind类型
    if(t1->kind==CONSTANT||t1->kind==ADDRESS||t1->kind==WADDRESS);
    else{
        struct TABLE* find_id=find_struct_in_table(t1->u_char);
        if(find_id==NULL) t1->kind=VARIABLE;
        else if(find_id->field->type->kind!=BASIC) t1->kind=ADDRESS;
        else t1->kind=VARIABLE;
    }

    //2.生成ARG语句
    InterCode code1_in=(InterCode)malloc(sizeof(struct InterCode_));
    code1_in->kind=ARG;
    code1_in->u.ulabel.op=t1;
    //3.连接
    if(here==NULL) {
        code1_in->next=NULL;
        code1_in->before=NULL;
        }
    else{
        here->next=code1_in;
        code1_in->before=here;
        }
    exp=exp->brother;
    if(exp==NULL);
    else translate_Args(exp->brother,code1_in);
    if(here==NULL){
        InterCode temp=code1_in;
        while(temp->next!=NULL) temp=temp->next;
        while(temp!=NULL){
            InterCode rem=temp;
            temp=temp->before;
            add_to_intercode(rem);}
        }
}

void translate_Specifier(struct Node* now){
    //Specifier → TYPE| StructSpecifier
    //printf("a translate_Specifier\n");
    if(interim_is_error==1) return;
    if(strcmp(now->child->name,"StructSpecifier")==0){
        translate_StructSpecifier(now->child);
    }
}

void translate_StructSpecifier(struct Node* now){
    //StructSpecifier → STRUCT OptTag LC DefList RC| STRUCT Tag
    //printf("a translate_StructSpecifier\n");
    if(interim_is_error==1) return;
    struct Node* struct_s=now->child;
    struct Node* fir_bro=struct_s->brother;
    struct Node* sec_bro=fir_bro->brother;
    if(sec_bro==NULL){
        //Tag → ID,生成DEC语句
        Operand t1=(Operand)malloc(sizeof(struct Operand_));
        struct TABLE* find=find_struct_in_table(fir_bro->child->char_name);
        t1->kind=VARIABLE;
        t1->u_int=find->variable;
        t1->u_char=fir_bro->child->char_name;
        //printf("a create_dec1\n");
        InterCode func_in=(InterCode)malloc(sizeof(struct InterCode_));
        func_in->kind=DEC;
        func_in->u.udec.op=t1;
        func_in->u.udec.size=get_size(find->field->type);
        add_to_intercode(func_in);
        return;
    }
}



void add_to_intercode(InterCode this){
    // translate_print_test(this);
    // if(in_now!=NULL) translate_print_test(in_now);
    // printf("\n");
    this->before=NULL;
    this->next=NULL;
    if(in_head==NULL) {
        in_head=this;
        b++;
    }
    if(in_now==NULL) in_now=this;
    else{
        in_now->next=this;
        this->before=in_now;
        in_now=this;
    }
}

Operand new_temp(){
    Operand now=(Operand)malloc(sizeof(struct Operand_));
    now->kind=TEMP;
    now->u_int=temp_num;
    now->u_char="zhangyu\0";
    temp_num++;
    return now;
}

Operand new_label(){
    Operand now=(Operand)malloc(sizeof(struct Operand_));
    now->kind=LABEL;
    now->u_int=lab_num;
    lab_num++;
    return now;
}

//计算部分偏移
int get_offset(Type return_type,struct Node* after){
    //printf("a get_offset\n");
    if(return_type==NULL) return 0;//printf("a NULL\n");
    if(return_type->kind==BASIC) return 4;
    else if(return_type->kind==STRUCTURE){
        //printf("a get_offset STRUCTURE\n");
        //printf("a %s\n",after->char_name);
        int offset=0;
        FieldList count=return_type->u.structure;
        while(count!=NULL){
            if(strcmp(count->name,after->char_name)==0) break;
            offset=offset+get_size(count->type);
            count=count->tail;
        }
        return offset;
    }
    else if(return_type->kind==ARRAY){
        //只有一维数组，多出一维都报错
        //printf("a get_offset ARRAY\n");
        if(return_type->u.array.elem->kind==ARRAY){
            interim_is_error=1;
            printf("Cannot translate: Code contains variables of error array type.\n");
            return -1;
        }
        //返回该数组单个元素的长度
        int offset=0;
        Type elem=return_type->u.array.elem;
        offset=get_size(elem);
        return offset;
    }

}

//计算全部偏移，即type中的所有内存都要加上
int get_size(Type type){
    //printf("a get_size\n");
    if(type==NULL) return 0;
    if(type->kind==BASIC) return 4;
    //只有一维数组
    else if(type->kind==ARRAY){
        //printf("a get_size ARRAY\n");
        int temp=type->u.array.size*get_size(type->u.array.elem);
        return temp;
    }
    else if(type->kind==STRUCTURE){
        //printf("a get_size STRUCTURE\n");
        int offset=0;
        FieldList count=type->u.structure;
        while(count!=NULL){
            offset=offset+get_size(count->type);
            count=count->tail;
        }
        return offset;
    }
}






//void translate_ExtDecList(struct Node* now,Type type);
//FieldList translate_ParamDec(struct Node* now,int judge);

/*
Program → ExtDefList
ExtDefList → ExtDef ExtDefList
| 
ExtDef → Specifier ExtDecList SEMI
| Specifier SEMI
| Specifier FunDec CompSt
ExtDecList → VarDec
| VarDec COMMA ExtDecList

Specifier → TYPE
| StructSpecifier
StructSpecifier → STRUCT OptTag LC DefList RC
| STRUCT Tag
OptTag → ID
| 
Tag → ID

VarDec → ID
| VarDec LB INT RB
FunDec → ID LP VarList RP
| ID LP RP
VarList → ParamDec COMMA VarList
| ParamDec
ParamDec → Specifier VarDec

CompSt → LC DefList StmtList RC
StmtList → Stmt StmtList
| 
Stmt → Exp SEMI
| CompSt
| RETURN Exp SEMI
| IF LP Exp RP Stmt
| IF LP Exp RP Stmt ELSE Stmt
| WHILE LP Exp RP Stmt

DefList → Def DefList
| 
Def → Specifier DecList SEMI
DecList → Dec
| Dec COMMA DecList
Dec → VarDec
| VarDec ASSIGNOP Exp

Exp → Exp ASSIGNOP Exp
| Exp AND Exp
| Exp OR Exp
| Exp RELOP Exp
| Exp PLUS Exp
| Exp MINUS Exp
| Exp STAR Exp
| Exp DIV Exp
| LP Exp RP
| MINUS Exp
| NOT Exp
| ID LP Args RP
| ID LP RP
| Exp LB Exp RB
| Exp DOT ID
| ID
| INT
| FLOAT
Args → Exp COMMA Args
| Exp
*/