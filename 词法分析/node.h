#ifndef NODE_H
#define NODE_H

// #include <unistd.h>
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// define node type
typedef enum nodeType {
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_ID,
    TOKEN_TYPE,
    TOKEN_OTHER,
    NOT_A_TOKEN

} NodeType;


#define TRUE 1
#define FALSE 0


// node type declared
typedef struct node {
    int lineNo;  
    NodeType type;  
    char* name;
    char* val; 
    struct node* child;  //  non-terminals node first child node
    struct node* next;   //  non-terminals node next brother node

} Node;

typedef Node* aNode;

static inline aNode notTokenNode(int lineNo, NodeType type, char* name, int argc,...) {
    aNode curNode = NULL;
    int nameLength = strlen(name) + 1;

    curNode = (aNode)malloc(sizeof(Node));

    assert(curNode != NULL);

    curNode->name = (char*)malloc(sizeof(char) * nameLength);

    assert(curNode->name != NULL);

    curNode->lineNo = lineNo;
    curNode->type = type;
    strncpy(curNode->name, name, nameLength);

    va_list vaList;
    va_start(vaList, argc);

    aNode tempNode = va_arg(vaList, aNode);

    curNode->child = tempNode;

    for (int i = 1; i < argc; i++) {
        tempNode->next = va_arg(vaList, aNode);
        if (tempNode->next != NULL) {
            tempNode = tempNode->next;
        }
    }

    va_end(vaList);
    return curNode;
}

static inline aNode tokenNode(int lineNo, NodeType type, char* tokenName,char* tokenText) {
    aNode tokenNode = (aNode)malloc(sizeof(Node));
    int nameLength = strlen(tokenName) + 1;
    int textLength = strlen(tokenText) + 1;

    assert(tokenNode != NULL);

    tokenNode->lineNo = lineNo;
    tokenNode->type = type;

    tokenNode->name = (char*)malloc(sizeof(char) * nameLength);
    tokenNode->val = (char*)malloc(sizeof(char) * textLength);

    assert(tokenNode->name != NULL);
    assert(tokenNode->val != NULL);

    strncpy(tokenNode->name, tokenName, nameLength);
    strncpy(tokenNode->val, tokenText, textLength);

    tokenNode->child = NULL;
    tokenNode->next = NULL;

    return tokenNode;
}

static inline void freeNode(aNode node) {
    if (node == NULL) return;
    while (node->child != NULL) {
        aNode temp = node->child;
        node->child = node->child->next;
        freeNode(temp);
    }
    free(node->name);
    free(node->val);
    free(node);
    node->name = NULL;
    node->val = NULL;
    node = NULL;
}

static inline void printTree(aNode curNode, int height) {
    if (curNode == NULL) {
        return;
    }
    for (int i = 0; i < height; i++) {
        printf("  ");
    }
    printf("%s", curNode->name);
    if (curNode->type == NOT_A_TOKEN) {
        printf(" (%d)", curNode->lineNo);
    } else if (curNode->type == TOKEN_TYPE || curNode->type == TOKEN_ID || curNode->type == TOKEN_INT) {
        printf(": %s", curNode->val);
    } else if (curNode->type == TOKEN_FLOAT) {
        printf(": %lf", atof(curNode->val));
    }
    printf("\n");
    printTree(curNode->child, height + 1);
    printTree(curNode->next, height);
}

#endif