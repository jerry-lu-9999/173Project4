#include <stdio.h>
#include <stdlib.h>
#include "LinkedList.h"

typedef struct Relation *Relation;
struct Relation {
    LinkedList array[1009];
    LinkedList schema;
    int size;
};

typedef struct Tuple *Tuple;
struct Tuple {
    char* str[10];
    Tuple next;
};

// typedef struct SNAP *SNAPLIST;
// struct SNAP {
//     int StudentID;
//     char Name[30];
//     char Phone[8];
//     SNAPLIST next;
// };
// typedef SNAPLIST SNAPTABLE[1009];

// void insert_SNAP(SNAPLIST snap, SNAPTABLE relation){

// }

// constructor for relation
Relation new_Relation(int size) {
    Relation this = (Relation)malloc(sizeof(struct Relation));
    if(this == NULL) {
        return NULL;
    }
    this->size = size;
    this->schema = new_LinkedList();
    for(int i = 0; i < size; i++){
        this->array[i] = new_LinkedList();
    }
    return this;
}

// create tuple
Tuple create_Tuple(char** strs) {
    Tuple tuple = (Tuple)malloc(sizeof(struct Tuple));
    if(tuple == NULL) {
        return NULL;
    }
    return tuple;
}

// NEED A HASH FUNCTION FOR STRING
int hash(char* str){
    int key;

    return key % 1009;
}

// CHECK IF TWO TUPLES ARE EQUAL
bool equalTuples(Tuple t1, Tuple t2){
    return false;
}
// insert 
void insert(Tuple t, Relation r) {
    int key = hash(t->str[0]);
    if(r->array[key] != NULL){
        LinkedListIterator it = LinkedList_iterator(r->array[key]);
        while(LinkedListIterator_hasNext(it)){
            Tuple tup = LinkedListIterator_next(it);
            if(equalTuples(tup, t)){
                printf("Tuple already exists");
            } else {
                tup -> next = t;
            }
        }
    } else {
        LinkedList_add_at_front(r->array[key], t);
    }
}

// lookup
Relation lookup(char** strs, Relation r) {

}

// delete
void delete(char** strs, Relation r) {

}

// selection
Relation selection(char** strs, Relation r){

}

// projection
Relation projection(char** strs, Relation r){

}

// join