/*
 * main.c
 *
 *  Created on: Dec 22, 2017
 *      Author: ise
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tree.h"
#include "map.h"
#include "assert.h"

struct system_obj{
    char* name;
    Boolean isFile;
};
typedef struct system_obj* Object;

typedef Result (*commandFunc)(Tree, char*);

/*Comparison function*/

Element cpy(Element dest, Element elem){
    Object tempdest = (Object)dest;
    Object tempelem = (Object)elem;
    char* tempName;
    tempName = (char*)malloc((strlen(tempelem->name)+1)*sizeof(char));
    strcpy(tempName, tempelem->name);
    tempdest->name = tempName;
    tempdest->isFile = tempelem->isFile;
    //free(elem);
    return dest;
}

char* getlbl(Element e){
    Object object = (Object)e;
    char* tempName = object->name;
    return object->name;
}

char* getDetails(Element e){
    Object object = (Object)e;
    char* tempName = object->name;
    char* type;
    type = (char*)malloc(sizeof(tempName));
    if (object->isFile){
        strcpy(type, "File: ");
    }
    else
        strcpy(type, "Folder: ");
    char* sub;
    sub = (char*)malloc(sizeof(type)+sizeof(tempName));
    strcat(type, tempName);
    strcpy(sub, type);
    return sub;
}


int cmp(Element data1, Element data2){
    Object folder1 = (Object)data1;
    Object folder2 = (Object)data2;
    return strcmp(folder1->name, folder2->name);
}


void fre(Element e){
    Object temp = ((Object)e);
    free(temp->name);
    //free(temp->isFile);
    free(temp);
}

Result OnError0(char* wrongCommand){
    printf("Unknown command: %s \n", wrongCommand);
    return FAILURE;
}

Result OnError1(GenericNode missingPath, printFunction print){
    //char* missPathLabel = print();
    printf("Path %s does not exist \n", missingPath->Location);
    return FAILURE;
}

Result OnError2(char* missingPath){
    //char* missPathLabel = print();
    printf("File %s does not exist \n", missingPath);
    return FAILURE;
}

Result OnError3(char* isFolderPath){
    printf("The path %s describes a folder \n", isFolderPath);
    return FAILURE;
}


Result mkdir(Tree tree, char* fullPath){
    fullPath++; // to ignore the ' '
    char* tmpStr = (char*)(calloc(strlen(fullPath)+1, sizeof(char)));
    strcpy(tmpStr, fullPath);
    //tmpStr++;
    char* path = (char*)(malloc(sizeof(tmpStr)));
    //if (*(tmpStr) == '/')

    /* Locate last occurrence of character in string - Element name */
    char* tmp = strrchr(tmpStr, '/');
    tmp++;
    char* element = (char*)(malloc(sizeof(tmp)));


    /* calcaluate the substring of path without name of element */
    size_t index;
    index = (size_t)(tmp - tmpStr);

    strncpy(path, tmpStr, index);
    //tmp++;
    strcpy(element, tmp);

    Object newFolder;
    newFolder = (Object)malloc(sizeof(Object));
    newFolder->name = element;
    newFolder->isFile = FALSE;

    if (strcmp(path, "/") == 0){
        Add(tree, newFolder);
    }
    else
    {
        Boolean flag = FALSE;
        //tmpStr++;
        GenericNode tmpRoot = SearchGenNode(tree, path, &flag); // changed from tmpStr to path 21:54
        if (flag == FALSE){
            //free(newFolder->name);
            //fre(newFolder);
            return OnError1(tmpRoot, tree->printFunc);
        }
        AddIn(tree, newFolder, path);
        free(tmpStr);
        free(path);
     //   free(tmp);

    }
}

Result touch(Tree tree, char* fullPath){
    fullPath++; // to ignore the ' '
    char* tmpStr = (char*)(calloc(strlen(fullPath)+1, sizeof(char)));
    strcpy(tmpStr, fullPath);


    /* Locate last occurrence of character in string - Element name */
    char* objectName = strrchr(tmpStr, '/');
    objectName++;
    char* path = (char*)(malloc(sizeof(objectName)+1));

    /* calcaluate the substring of path without name of element */
    size_t index;
    index = (size_t)(objectName - tmpStr);

    strncpy(path, tmpStr, index);

    char* element = (char*)(malloc(50* sizeof(char)));

    strcpy(element, objectName);

    Object newFolder;
    newFolder = (Object)malloc(sizeof(Object));
    newFolder->name = element;
    newFolder->isFile = TRUE;

    if (strcmp(path, "/") == 0){
        Add(tree, newFolder);
    }
    else
    {
        Boolean flag = FALSE;
        //tmpStr++;
        GenericNode tmpRoot = SearchGenNode(tree, path, &flag);
        if (flag == FALSE){
            //free(newFolder->name);
            //fre(newFolder);
            return OnError1(tmpRoot, tree->printFunc);
        }
        AddIn(tree, newFolder, path);
    }
}

Result ls (Tree tree, char* fullInput){
    fullInput++; // ? for ls long touch
    char* tmpStr = (char*)(calloc(strlen(fullInput)+1, sizeof(char)));
    strcpy(tmpStr, fullInput);
        /* ls */
    if (strcmp(tmpStr, "") == 0){
        Print(tree);
        free(tmpStr);
    }
        /* ls -r dirPath */
    else if (strstr(tmpStr, "-r")){
        tmpStr += 3;
        Tree tmpTree = (Tree)malloc(sizeof(Tree));
        Boolean flag = FALSE;
        GenericNode tmpRoot = SearchGenNode(tree, tmpStr, &flag);
        if (flag == FALSE)
            return OnError1(tmpRoot, tree->printFunc);
        tmpTree->printFunc = tree->printFunc;
        tmpTree->root = tmpRoot;
        Print(tmpTree);
        //free(tmpStr);
       // free(tmpTree);
    }
    else{
        struct tree_rec *tmpTree;
        Boolean flag = FALSE;
        GenericNode tmpRoot = SearchGenNode(tree, tmpStr, &flag);
        if (flag == FALSE)
            return OnError1(tmpRoot, tree->printFunc);
        LinkedListNode tmpHead = tmpRoot->head;
        for (int i = 0; i < tmpRoot->NumOfChilds; i++, tmpHead = tmpHead->next){
            char* tmpLabel;
            tmpLabel = (char*)calloc(strlen(getlbl(tmpHead->data->element)), sizeof(char));
            strcpy(tmpLabel, getlbl(tmpHead->data->element));
            printf("%s \t", tmpLabel);
            if (tmpHead->data->NumOfChilds > 0)
                printf("%d \n", tmpHead->data->NumOfChilds);
            printf("\n");
        }
        free(tmpStr);
    }
}



char** Split(char* a_str, const char a_delim)
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

Result mv (Tree tree, char* usrInput) {
    usrInput++;
    //char* path;
    char **splited = Split(usrInput, ' ');
    Boolean flag = FALSE;
    GenericNode tmpFrom = SearchGenNode(tree, *(splited), &flag);
    if (flag == FALSE){

        return OnError1(tmpFrom, tree->printFunc);
    }
    Object tmpElement = (Object)tmpFrom->element;
    /* if the element isn't a file */
    if (tmpElement->isFile == FALSE)
        return OnError2(*(splited));
    Object elementCpy = (Object)malloc(sizeof(tmpElement));
    cpy(elementCpy, tmpElement);
    Remove(tree, *(splited));

    Boolean toFlag = FALSE;
    GenericNode tmpTo = SearchGenNode(tree, *(splited+1), &toFlag);
    /* the toPath is missing or it ends in file name which do not exist */
    if (toFlag == FALSE)
    {
        char* ptr;
        ptr = (char*)malloc(sizeof(char));
        char* tmp = strrchr(*(splited+1), '/');
        tmp++;
        strcpy(ptr, tmp);
        /* calcaluate the substring of path without name of element */
        size_t index;
        index = (size_t)(tmp - *(splited+1));
        char* tempFatherPath;
        tempFatherPath = malloc((index+1)* sizeof(char));
        strncpy(tempFatherPath, *(splited+1), index);
        //tmp++;
        Boolean fatherFlag = FALSE;
        GenericNode tmpFather = SearchGenNode(tree, tempFatherPath, &fatherFlag);
        /* the toPath is missing */
        if (fatherFlag == FALSE){
            return OnError1(tmpFather, tree->printFunc);
        }
        /* the toPath ends in file name which do not exits */
        else{
            //tmp++;
            elementCpy->name = ptr;
            AddIn(tree, elementCpy, tempFatherPath);
        }
    }
    /* the toPath is existing Folder */
    else
        return AddIn(tree, elementCpy, *(splited+1));
    }

Result cp (Tree tree, char* usrInput) {
    usrInput++;
    /* check if file's father is exist */
    char *ptr;
    ptr = (char *) malloc(sizeof(char));
    char **splited = Split(usrInput, ' ');
    char *tmp = strrchr(*(splited), '/');
    tmp++;
    strcpy(ptr, tmp);
    size_t index;
    index = (size_t) (tmp - *(splited));
    char *tempFatherPath;
    tempFatherPath = malloc((index + 1) * sizeof(char));
    strncpy(tempFatherPath, *(splited), index);
    Boolean fatherFlag = FALSE;
    GenericNode tmpFather = SearchGenNode(tree, tempFatherPath, &fatherFlag);
    /* the fromPath is missing */
    if (fatherFlag == FALSE)
        return OnError1(tmpFather, tree->printFunc);
        /*the file is not exist or file is exist*/
    else {
        Boolean fileFlag = FALSE;
        GenericNode tmpFileNode = SearchGenNode(tree, *(splited), &fileFlag);
        /* file is not exist */
        if (fileFlag == FALSE)
            return OnError2(*(splited));
            /* file is exist */
        else {
            char *ptrTo;
            ptrTo = (char *)malloc(sizeof(char));
            char *tmpTo = strrchr(*(splited+1), '/');
            tmpTo++;
            strcpy(ptrTo, tmpTo);
            size_t toIndex;
            toIndex = (size_t) (tmpTo - *(splited+1));
            char *tempToFatherPath;
            tempToFatherPath = malloc((toIndex + 1) * sizeof(char));
            strncpy(tempToFatherPath, *(splited+1), toIndex);

            Boolean toPathFlag = FALSE;
            GenericNode toPathNode = SearchGenNode(tree, tempToFatherPath, &toPathFlag);
            /* <to> path isn't exist */
            if (toPathFlag == FALSE)
                return OnError1(toPathNode, tree->printFunc);
            /* start to work on the file copy */
            Object toCpy;
            toCpy = (Object)(tmpFileNode->element);
            Object fileCpy = (Object) malloc(sizeof(toCpy));
            cpy(fileCpy, toCpy);
            fileCpy->name = tmpTo;
            return AddIn(tree, fileCpy, toPathNode->Location);

        }
    }
}




Result rm (Tree tree, char* usrInput) {
    usrInput++;
    char* path;
    char **splited = Split(usrInput, ' ');
    if (strcmp(*(splited), "-r") != 0)
    {
        path = (char*)(calloc(strlen(*(splited)), sizeof(char)));
        strcpy(path, *(splited));
    }
    else
    {
        path = (char*)(calloc(strlen(*(splited+1))+1, sizeof(char)));
        strcpy(path, *(splited+1));
    }

    Boolean flag = FALSE;
    GenericNode tmpRoot = SearchGenNode(tree, path, &flag);
    char* details;
    details = getDetails(tmpRoot->element);
    char* pch = strstr(details, "Folder:");
    if (pch) {
        if (strcmp(*(splited), "-r") != 0)
            return OnError3(tmpRoot->Location);
        else
            Remove(tree, path);
    }
    else
        Remove(tree, path);
}



Result Exit(Tree tree, char* userInput){
    //Remove(tree, tree->root->Location);
    exit(0);
}


int main() {
    Tree mainTree = TreeCreate(cpy, fre, getlbl, cmp);
    printf("\t %s \n", "Welcome to objects simultation system by eran & nadav");
    Map map = CreateMap();
    Insert(map, "mkdir", mkdir);
    Insert(map, "ls", ls);
    Insert(map, "touch", touch);
    Insert(map, "cp", cp);
    Insert(map, "mv", mv);
    Insert(map, "rm", rm);
    Insert(map, "exit", Exit);
    Boolean isExit=FALSE;
    while (!isExit) {
        printf("@> ");
        char *tmpUserInput, buffer[2000];
        scanf(" %[^\n]s", buffer);
        char *userInput;
        userInput = (char *) (malloc(strlen(buffer) + 1));
        strcpy(userInput, buffer);
        char *userInputCpy;
        userInputCpy = (char *) (malloc(strlen(buffer) + 1));
        strcpy(userInputCpy, userInput);
        char **splited = Split(userInput, ' ');
        //free(userInput);
        char *command;
        command = (char *) (malloc(sizeof(*(splited) + 1)));
        strcpy(command, *(splited));
        commandFunc func;
        func = (commandFunc)Get(map, command);
        if (func == Exit)
            isExit = TRUE;
        if (func == NULL) {
            OnError0(command);
            //free(command);
            //command = NULL;
        }
        else {
            char *temp = (char *) (malloc(strlen(userInputCpy) + 1));
            strcpy(temp, userInputCpy);
            temp += strlen(command);
            char *path = (char *) malloc(strlen(temp) + 1);
            strcpy(path, temp);
            func(mainTree, path);
            memset(buffer, 0, 2000);
            //free(temp);
            free(command);
            //free(userInputCpy);
            //free(userInput);
            //path == NULL;
            //free(temp);
            //temp == NULL;
        }
    }
}