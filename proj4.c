#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"

typedef struct Relation *Relation;
struct Relation {
    LinkedList array[1009];
    LinkedList schema;
    int size;
};

typedef struct Tuple *Tuple;
struct Tuple {
    char* str[5];
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

    this->size = size;
    this->schema = new_LinkedList();
    for(int i = 0; i < 1009; i++){
        this->array[i] = new_LinkedList();
    }
    return this;
}

Relation create_CSG(){
  printf("beginning of csg");
    Relation csg = new_Relation(3);
    LinkedList_add_at_front(csg->schema, "Grade");
    LinkedList_add_at_front(csg->schema, "StudentID");
    LinkedList_add_at_front(csg->schema, "Course");
    printf("create_CSG works");
    return csg;
}

Relation create_SNAP(){
    Relation snap = new_Relation(4);
    LinkedList_add_at_front(snap->schema, "Phone");
    LinkedList_add_at_front(snap->schema, "Address");
    LinkedList_add_at_front(snap->schema, "Name");
    LinkedList_add_at_front(snap->schema, "StudentID");
    return snap;
}

Relation create_CP(){
    Relation cp = new_Relation(4);
    LinkedList_add_at_front(cp->schema, "Prerequisite");
    LinkedList_add_at_front(cp->schema, "Course");
    return cp;
}

Relation create_CDH(){
    Relation cdh = new_Relation(4);
    LinkedList_add_at_front(cdh->schema, "Hour");
    LinkedList_add_at_front(cdh->schema, "Day");
    LinkedList_add_at_front(cdh->schema, "Course");
    return cdh;
}

Relation create_CR(){
    Relation cr = new_Relation(4);
    LinkedList_add_at_front(cr->schema, "Room");
    LinkedList_add_at_front(cr->schema, "Course");
    return cr;
}

// create tuple
Tuple create_Tuple(char* strs) {
    Tuple tuple = (Tuple)malloc(sizeof(struct Tuple));
    if(tuple == NULL) {
        return NULL;
    }
    char *ptr = strtok(strs,","); //https://www.codingame.com/playgrounds/14213/how-to-play-with-strings-in-c/string-split
    int size = strlen(strs);
    
    printf("debugging");
    for (int i = 0; i < size; i++){ 
    while(ptr != NULL){
      tuple->str[i] = ptr;
      ptr = strtok(NULL, ",");
    }			   
    return tuple;
}
}

// NEED A HASH FUNCTION FOR STRING
int hash(char* str){
    
    int key;
    return key % 1009;
}

// CHECK IF TWO TUPLES ARE EQUAL
bool equalTuples(Tuple t1, Tuple t2){
  bool test = false;
    for (int j = 0; j < 10; j++){
      if (strcmp(t1->str[j],t2->str[j]) == 0){
        j++;
	test == true;
      } else{
	break;
      }
    }
    return test; 
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

bool check_Tuple(Tuple t, char** pattern){
    bool check = false; 
    for(int i = 1; i < sizeof(pattern); i++){
      if(!strcmp((pattern[i]), "*")){
            if(strcmp(t->str[i], pattern[i])) {
                check = true;
            } else {
                check = false;
            }
        } else {
            check = true;
        }
    }
    return check;
}

// lookpu
Relation lookup(char** strs, Relation r) {
    Relation rela = new_Relation(0);
    // check for the first element in string array
    if(strcmp(strs[0], "*")){
        for(int i = 0; i < r->size; i++){
            LinkedList llist = r->array[i];
            LinkedListIterator it = LinkedList_iterator(llist);
            while(LinkedListIterator_hasNext(it)){
                Tuple tup = LinkedListIterator_next(it);
                if(check_Tuple(tup, strs)) {
                    insert(tup, rela);
                }
            }
        }
    } else {
        int key = hash(strs[0]);
        if(r->array[key] != NULL){
        LinkedListIterator it = LinkedList_iterator(r->array[key]);
        while(LinkedListIterator_hasNext(it)){
            Tuple tup = LinkedListIterator_next(it);
            if(check_Tuple(tup, strs)) {
                insert(tup, rela);
            }
        }
    }
    }
    return rela;
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
int main(){
  Relation csg = create_CSG();
  Tuple t1 = create_Tuple("CS101,12345,A");
  Tuple t2 = create_Tuple("CS102,12345,A");
  // insert(t1,csg);
  // insert(t2,csg);
  equalTuples(t1, t2);
}
