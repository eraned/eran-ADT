/*
 * tree.c
 *
 *  Created on: Dec 22, 2017
 *      Author: ise
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "tree.h"

void PrintHelper(LinkedListNode root, printFunction print, char* tabs);

Tree TreeCreate(copyFunction copy, freeFunction free, printFunction printfunc, compareFunction compare){
    Tree tree = (Tree)malloc(sizeof(Tree));
    if(tree==NULL)
        return NULL;
    tree->copyFunc = copy;
    tree->freeFunc = free;
    tree->printFunc = printfunc;
    tree->compareFunc = compare;
    GenericNode treeRoot = (GenericNode)malloc(sizeof(GenericNode));
    if (treeRoot==NULL)
        exit(1);
    char* rootLocation = (char*)calloc(1, sizeof(char));
    strcpy(rootLocation, "/");
    treeRoot->Location = rootLocation;
    treeRoot->element = NULL;
    treeRoot->head = NULL;
    tree->root=treeRoot;
    tree->printFunc = printfunc;
    return tree;
}

char* ElementNameToString(Element element, printFunction print){
    return print(element);
}

/* Gets GenericNode and new GenericNode (which initilized and allocated) and Insert ...  */
void sortedInsert(LinkedListNode curhead, LinkedListNode newNode, compareFunction cmp){
    if ((cmp((curhead->data->element),(newNode->data->element))) >= 0){
        struct Node temp = *curhead;
        *curhead = *newNode;
        *newNode = temp;
        curhead->next = newNode;
        curhead->next->prev = newNode;

        //curhead = newNode;
    }

    else{
        LinkedListNode current = curhead;
        while ((current->next != NULL) && ((cmp(((current->next->data->element)),((newNode->data->element)))) <= 0)){
            current = current->next;
        }
        newNode->next = current->next;
        if (current->next != NULL)
            newNode->next->prev = newNode;

        current->next = newNode;
        newNode->prev = current;
    }
}

char** str_split(char* a_str, const char a_delim)
{

    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str; // changed from a_str to tmpStr
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = (char**)calloc(count, sizeof(char*));

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx) = strdup(token);
            idx++;
            token = strtok(0, delim);
        }
        //assert(idx == count - 1);
        *(result + idx) = 0;
    }
    return result;
}

GenericNode SearchGenNode(Tree tree, char* location, Boolean* flag){
    char* tmpStr = (char*)(malloc(sizeof(location)));
    strcpy(tmpStr, location); // because strtok problem (modifies the original string)
    tmpStr++;
    char **tempResult;
    tempResult = str_split(tmpStr, '/');
    GenericNode tmpRoot = tree->root;
    LinkedListNode tmpNode = tree->root->head;
    int foundCounter = 0;
    if (tmpRoot->NumOfChilds == 0){
        char* missing;
        missing = (char*)malloc(sizeof(*(tempResult)));
        strcpy(missing, *(tempResult));
        *flag = FALSE;
        GenericNode missRoot = (GenericNode)malloc(sizeof(GenericNode));
        missRoot->Location = missing;
       // free(tmpStr);
        return missRoot;
    }

    if (tempResult){
        int i = 0;
        for (i = 0; *(tempResult + i) && (foundCounter == i); i++)
        {
            for (int j=0; j < tmpRoot->NumOfChilds; j++, tmpNode = tmpNode->next){
                char* pch = strstr(tmpNode->data->Location, *(tempResult + i));
                if (pch){
                    {
                        tmpRoot = tmpNode->data;
                        tmpNode = tmpNode->data->head;
                    }
                    foundCounter++;
                    break; // found the right location
                }
            }
            //free(*(tempResult + i));
        }
        //free(tempResult);
        if (foundCounter != i){
            char* missing;
            if (!*(tempResult + foundCounter))
                i--;
            missing = (char*)malloc(sizeof(*(tempResult + foundCounter))); // changed from i to foundcounter
            strcpy(missing, *(tempResult + foundCounter));
            *flag = FALSE;
            GenericNode missRoot = (GenericNode)malloc(sizeof(GenericNode));
            missRoot->Location = missing;
            return missRoot;
            }
    }
    *flag = TRUE;
    return tmpRoot;
}
/*Starting location always starts with "/" */

Result AddIn(Tree tree,Element elem, char* Location) {
    Boolean bool = TRUE;
    GenericNode Parent = SearchGenNode(tree, Location, &bool);
    if (bool == FALSE)
        return FAILURE;
    GenericNode NewGenericNode = (GenericNode)malloc(sizeof(GenericNode));
    NewGenericNode->element = elem;
    NewGenericNode->head = NULL;
    char* ElementLabel = ElementNameToString(elem, tree->printFunc);
    char* NewLocation = (char*)malloc((strlen(Parent->Location)+strlen(ElementLabel)+3)*sizeof(char));
    strcpy(NewLocation, Parent->Location);
    strcat(NewLocation, "/");
    strcat(NewLocation, ElementLabel);
    NewGenericNode->Location=NewLocation;
    LinkedListNode NewLinkedListNode = (LinkedListNode)malloc(sizeof(LinkedListNode));
    NewLinkedListNode->data = NewGenericNode;
    NewLinkedListNode->next = NULL;
    NewLinkedListNode->prev = NULL;


    if (Parent->head == NULL) {
        Parent->head = NewLinkedListNode;
        Parent->NumOfChilds++;
        return SUCCESS;
    } else if (Parent->head != NULL) {
        sortedInsert(Parent->head, NewLinkedListNode, tree->compareFunc);
        Parent->NumOfChilds++;
        return SUCCESS;
    }
    return FAILURE;
}


Result Add(Tree tree,Element elem) {
    GenericNode NewGenericNode = (GenericNode)malloc(sizeof(GenericNode));
    NewGenericNode->element = elem;
    NewGenericNode->head = NULL;
    char* ElementLabel = ElementNameToString(elem, tree->printFunc);
    char* NewLocation = (char*)malloc(sizeof(tree->root->Location));
    strcpy(NewLocation, tree->root->Location);
    strcat(NewLocation, ElementLabel);
    NewGenericNode->Location=NewLocation;
    LinkedListNode NewLinkedListNode = (LinkedListNode)malloc(sizeof(LinkedListNode));
    NewLinkedListNode->data = NewGenericNode;
    NewLinkedListNode->next = NULL;
    NewLinkedListNode->prev = NULL;

    if (tree->root->head == NULL) {
        tree->root->head = NewLinkedListNode;
        tree->root->NumOfChilds++;
       // free(NewLocation);
      //  free(NewLinkedListNode);
        //free(newElem);
        return SUCCESS;
    }
    else
    {
        LinkedListNode tempCur;
        tempCur = tree->root->head;
        sortedInsert(tempCur, NewLinkedListNode, tree->compareFunc);

    }
        tree->root->NumOfChilds++;
    }



void Remove(Tree tree, char* location) {
    char* fullPathCpy = calloc(strlen(location)+1, sizeof(char));
    strcpy(fullPathCpy, location);
    Boolean flag = TRUE;
    GenericNode tmpRoot = SearchGenNode(tree, location, &flag);
    if (tmpRoot->head != NULL) {
        LinkedListNode tmpHead = tmpRoot->head;
        /* deletes and free everything under it */
        RemoveMaster(tmpHead, tree->freeFunc);
    }
    //char* labelNodeToDelete = tree->printFunc(tmpRoot->element);
    char* tmp = strrchr(fullPathCpy, '/');
    tmp++;
    /* calcaluate the substring of path without name of element */
    size_t index;
    index = (size_t)(tmp - fullPathCpy);
    char* fatherLocation = malloc(sizeof(fullPathCpy));
    strncpy(fatherLocation, fullPathCpy, index);
    //tmp++;
    GenericNode father = SearchGenNode(tree, fatherLocation, &flag);
    free(fatherLocation);
    int childsNum = father->NumOfChilds;
    char* NodeLocation = tmpRoot->Location;
    /* search for LinkedListNode which represents node to delete */
    LinkedListNode curr = father->head;
    for (int i = 0; i < childsNum; curr = curr->next, i++) {
        char *currLocation = curr->data->Location;
        if (strcmp(NodeLocation, currLocation) == 0) {
            /* found the Node which points on tmpRoot */
            if (curr->next != NULL && curr->prev != NULL) {
                curr->prev->next = curr->next;
                curr->next->prev = curr->prev;
            } else if (curr->next != NULL && curr->prev == NULL) {
                father->head = curr->next;
                curr->next->prev = NULL;
            }
            else if (curr->next == NULL && curr->prev != NULL) {
                curr->prev->next = NULL;
            }
            else{
                free(father->head);
                father->head = NULL;
            }
            father->NumOfChilds--;
            break;
        }
    }
    free(fullPathCpy);
}
/* function which removes GenericeNode and all that under it */
void RemoveMaster(LinkedListNode tmpHead, freeFunction freeFunc) {
    return;
}


Result Reassign(Tree tree, Element elem, char* location) {
    Boolean flag = FALSE;
    GenericNode current = SearchGenNode(tree, location, &flag);
    if (flag == FALSE)
        return FAILURE;

    char *tmp = strrchr(location, '/');
    tmp++;
    size_t index;
    index = (size_t) (tmp - location);
    char *path;
    path = (char) malloc(sizeof(index));
    strncpy(path, location, index);
    GenericNode tmpFather = SearchGenNode(tree, path, &flag);
    if (flag == FALSE) { // no parent - no possible
    }
    free(current->Location);
    tree->copyFunc(current->element, elem);
    size_t length = strlen(tmpFather->Location) + strlen(ElementNameToString(elem, tree->printFunc));
    current->Location = (char *) malloc(sizeof(char) * (length + 2));
    strcpy(current->Location, tmpFather->Location);
    if (tmpFather != tree->root) { strcat(current->Location, "/"); }
    else { /*do nothing*/ }
    strcat(current->Location, ElementNameToString(elem, tree->printFunc));

    if (current->head != NULL) {
        LinkedListNode tmpHead = current->head;
        for (int i = 0; i < current->NumOfChilds; i++) {
            ReassignMaster(tree, tmpHead);
            tmpHead = tmpHead->next;
        }
        return SUCCESS;
    }
}

void ReassignMaster(Tree tree, LinkedListNode head) {
    Boolean flag = TRUE;
    GenericNode current = SearchGenNode(tree, head->data->Location, &flag);
    char *tmp = strrchr(current->Location, '/');
    tmp++;
    size_t index;
    index = (size_t) (tmp - current->Location);
    char *path;
    path = (char) malloc(sizeof(index));
    strncpy(path, current->Location, index);
    GenericNode tmpFather = SearchGenNode(tree, path, &flag);
    if (flag == FALSE) { // no parent - no possible
    }

    size_t length = strlen(tmpFather->Location) + strlen(ElementNameToString(current->element, tree->printFunc));
    current->Location = (char *) malloc(sizeof(char) * (length + 2));

    strcpy(current->Location, tmpFather->Location);
    strcat(current->Location, "/");
    strcat(current->Location, ElementNameToString(current->element, tree->printFunc));

    if (current->head != NULL) {
        LinkedListNode tmpHead = current->head;
        for (int i = 0; i < current->NumOfChilds; i++) {
            ReassignMaster(tree, tmpHead);
            tmpHead = tmpHead->next;
        }

    }
}

void Print(Tree tree) {
    LinkedListNode tmpHead = tree->root->head;
    char* tabs = (char*)malloc(30*sizeof(char));
    for (int i = 0; i < tree->root->NumOfChilds; i++, tmpHead=tmpHead->next) {
        strcpy(tabs, "");
        PrintHelper(tmpHead, tree->printFunc, tabs);
    }
}

void PrintHelper(LinkedListNode root, printFunction print, char* tabs){

    int numOfChilds = root->data->NumOfChilds;
    char* tmpLbl = ElementNameToString(root->data->element, print);
    printf(" %s %s %d\n", tabs, tmpLbl, numOfChilds);
    //printf("%d ", numOfChilds);

    if (root->data->NumOfChilds > 0) {
        strcat(tabs, "\t");
        LinkedListNode tmpRoot = root->data->head;
        for (int j = 0; j < root->data->NumOfChilds ; j++, tmpRoot = tmpRoot->next) {
                    PrintHelper(tmpRoot, print, tabs);
            }
        }
}

