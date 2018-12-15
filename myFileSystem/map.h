#ifndef ASSIGNMENT_2_MAP_H

#define ASSIGNMENT_2_MAP_H



/*

 * All function to be stored in the Map must have a fixed

 * arguments order: Tree, String, Element

 */

typedef int (*Func)(Tree, char*);

typedef struct linked_node* LinkNode;

typedef struct Linked_List* Map;



/*

 * Creates a new empty Map ADT.

 */

Map CreateMap();

/*

 * Insert add a new pair of string and function pointer to the map.

 * The string is the key, function pointer is value.

 */

void Insert(Map, char*, Func);

/*

 * Finds the key in the map and removes it.

 */

void RemoveValue(Map, char*);

/*

 * Returns a function pointer according to a given key string.

 */

Func Get (Map, char*);



void DestroyMap(Map);



#endif //ASSIGNMENT_2_MAP_H
