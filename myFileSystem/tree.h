/*
 * tree.h
 *
 *  Created on: Dec 22, 2017
 *      Author: ise
 */

#ifndef TREE_H_
#define TREE_H_

typedef enum {FALSE,TRUE} Boolean;
typedef enum {FAILURE,SUCCESS} Result;
typedef void* Element;
typedef Element (*copyFunction)(Element, Element);
typedef void (*freeFunction)(Element);
typedef char* (*printFunction)(Element);
typedef int (*compareFunction)(Element, Element);

typedef struct node_rec* GenericNode;

struct tree_rec{
    copyFunction copyFunc;
    freeFunction freeFunc;
    compareFunction compareFunc;
    printFunction printFunc;
    GenericNode root;
};

typedef struct tree_rec* Tree;

typedef struct Node* LinkedListNode;

struct Node  {
    GenericNode data;  // pointer to GenericNode
    LinkedListNode next;
    LinkedListNode prev;
};

struct node_rec{
    Element element;
    LinkedListNode head; // DoublyLinkedList Node to children linkedlist, NULL if no childs.
    int NumOfChilds;
    char* Location;
};



Tree TreeCreate(copyFunction, freeFunction, printFunction, compareFunction);

Result Add(Tree,Element);

Result AddIn(Tree,Element , char*);

void Remove(Tree, char*);

Result Reassign(Tree,Element, char*);

void Print(Tree);

GenericNode SearchGenNode(Tree, char*, Boolean*);

char* ElementNameToString(Element, printFunction);


#endif /* TREE_H_ */