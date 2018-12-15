/*

 *  The map is a linked list based ADT. Each node in the list holds four variables.

 *  String KEY, Function pointer VALUE, next node ptr and previous node ptr.

 *  head and tail are global variables.

 */

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include "tree.h"

#include "map.h"



static LinkNode Create_link(Map, char*, Func);

static LinkNode Lookup(Map , char *);



struct linked_node{

    char *key;

    void* value;

    LinkNode next;

    LinkNode prev;

};



struct Linked_List{

    LinkNode head;

    LinkNode tail;

};

//LinkNode head;

//LinkNode tail;



Map CreateMap()

{

    Map new_ll = (Map) malloc(sizeof(struct Linked_List));

    //if(new_ll == NULL) PRINT_ERR;

    new_ll->head = NULL; //(LinkNode) malloc(sizeof(LinkNode));

    new_ll->tail = NULL; //(LinkNode) malloc(sizeof(LinkNode));

}



void Insert(Map list, char* key, Func func)

{

    LinkNode new_n = Create_link(list, key, func);

}



void RemoveValue(Map list, char* key)

{

    LinkNode t_rm = Lookup(list, key);

    if(t_rm == NULL) {

        printf("KEY NOT FOUND IN MAP");

        return;

    }

    if(t_rm == list->head){

        list->head = t_rm->next;

        free(t_rm->key);

        free(t_rm->value);

        free(t_rm);

        return;

    }



    if(t_rm == list->tail){

        list->tail = t_rm->prev;

        free(t_rm->key);

        free(t_rm->value);

        free(t_rm);

        return;

    }



    else{

        t_rm->prev->next = t_rm->next;

        t_rm->next->prev = t_rm->prev;

        free(t_rm->key);

        free(t_rm->value);

        free(t_rm);

    }

}



Func Get (Map list, char* key)

{
    LinkNode ans = Lookup(list, key);
    if (ans == NULL)
        return NULL;
    return ans->value;

}



void DestroyMap(Map m){

    if(m->head != NULL){

        LinkNode curr = m->head;

        LinkNode ptr = m->head;

        while (ptr != NULL){

            ptr = curr->next;

            free(curr);

        }

    }

    else{

        //do nothing

    }

}



static LinkNode Create_link (Map list, char* key, Func func)

{

    LinkNode new_n = (struct linked_node *) malloc(sizeof(struct linked_node));

    //if (new_n == NULL) PRINT_ERR;

    size_t k_len = strlen(key);



    if(list->head == NULL && list->tail == NULL) {

        list->head = new_n;

        list->tail = new_n;

        new_n->prev = NULL;

        new_n->next = NULL;

        new_n->key = (char*) malloc((sizeof(char)*(k_len+1)));

        //if(new_n->key == NULL) PRINT_ERR;

        strcpy(new_n->key, key);

        new_n->value = func;



        return new_n;

    }



    new_n->key = (char*) malloc((sizeof(char)*(k_len+1)));

    //if(new_n->key == NULL) PRINT_ERR;

    strcpy(new_n->key, key);

    new_n->value = func;

    list->tail->next = new_n;

    new_n->prev = list->tail;

    new_n->next = NULL;

    list->tail = new_n;



    return new_n;

}



static LinkNode Lookup(Map list, char *key)

{

    LinkNode tmp = list->head;

    while (tmp != NULL){

        if(strcmp(key, tmp->key) == 0)

            return tmp;

        tmp = tmp->next;

        /*if(tmp->next == list->tail){

            if(strcmp(key, list->tail->key))

                return list->tail;

        }*/

    }

    return NULL;

}