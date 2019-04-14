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
    Relation csg = new_Relation(3);
    LinkedList_add_at_front(csg->schema, "Grade");
    LinkedList_add_at_front(csg->schema, "StudentID");
    LinkedList_add_at_front(csg->schema, "Course");
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
Tuple create_Tuple(char* strs[]) {
    Tuple tuple = (Tuple)malloc(sizeof(struct Tuple));
    if(tuple == NULL) {
        return NULL;
    }
    //int size = *(&strs + 1) - strs;
    //printf("\nseizeof %d", sizeof(*strs)/sizeof(*strs[0]));
    //printf("\nsize %d",size);
   int i = 0;
    // for(char* s = *strs; s != "\0"; *s++){
    for (int i = 0; i < 3; i++){ //SIZE MUST BE FIXED
        tuple->str[i] = strs[i];
 //     printf("'%s'\n",tuple->str[i]);
    }
//	printf("\n\nTHis is tuple at 0: %s \n1: %s \n, 2: %s \n", tuple->str[0], tuple->str[1], tuple->str[2]);
    return tuple;   
}

// NEED A HASH FUNCTION FOR STRING
int hash(char* str){
    int key=0;
    for(int i = 0; i < strlen(str); i++) {
        key += str[i];
    }
    return key % 1009;
}

// CHECK IF TWO TUPLES ARE EQUAL
bool equalTuples(Tuple t1, Tuple t2){
    int size1 = 0; int size2 = 0;
    for (int i = 0; i < 5; i++){
        if (t1->str[i]!=NULL){
            size1++;
        }
        if (t1->str[i] != NULL){
            size2++;
        }
    }
    bool test = false;
    for (int j = 0; j < size1; j++){
      if (strcmp(t1->str[j],t2->str[j]) == 0){
        	test = true;
      } else {
          test = false;
          break;
      }
    }
    return test; 
}

void print_Tuple(Tuple tup){
    int sizetup = sizeof(tup->str)/sizeof(tup->str[0]);
    // printf("Size of tup %d", sizetup);
    for(int i = 0; i < 3; i++){ // size again
        printf("%s\n", tup->str[i]);
    }
}

void print_Relation(Relation r){
    int sizearr = sizeof(r->array)/sizeof(r->array[0]);
    for(int i = 0; i < sizearr; i++){
        LinkedListIterator it = LinkedList_iterator(r->array[i]);
        while(LinkedListIterator_hasNext(it) && it != NULL){
            Tuple tup = LinkedListIterator_next(it);
            print_Tuple(tup);
        }
    }
}

// insert 
void insert(Tuple t, Relation r) {
    int key = hash(t->str[0]);
    if(LinkedList_elementAt(r->array[key], 0) != NULL){
        LinkedListIterator it = LinkedList_iterator(r->array[key]);
        while(LinkedListIterator_hasNext(it)){
            Tuple tup = LinkedListIterator_next(it);
            if(equalTuples(tup, t)){
            printf("\nTuple already exists");
            } else {
            LinkedList_add_at_end(r->array[key], t);
            }
        }
    } else {
        LinkedList_add_at_front(r->array[key], t);
    }
}

// check if a tuple matches a pattern query
bool check_Tuple(Tuple t, char** pattern){
    bool check = false; 
    int sizepat = sizeof(pattern)/sizeof(pattern[0]);
    // printf("pattern size %d\n", sizepat);
    for(int i = 1; i < 3; i++){
        if(strcmp((pattern[i]), "*") != 0){
            if(strcmp(t->str[i], pattern[i]) == 0) {
                check = true;
            } else {
                check = false;
            }
        } else {
            check = true;
        }
    }
    // printf("check is %d", check);
    return check;
}

// lookup
Relation lookup(char** strs, Relation r) {
    printf("LOOKING UP\n");
    Relation rela = new_Relation(0);
    // check for the first element in string array
    if(strcmp(strs[0], "*") == 0){
        for(int i = 0; i < 1009; i++){
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
        if(LinkedList_elementAt(r->array[key], 0) != NULL){
            LinkedListIterator it = LinkedList_iterator(r->array[key]);
            while(LinkedListIterator_hasNext(it)){
                Tuple tup = LinkedListIterator_next(it);
                if(check_Tuple(tup, strs)) {
                    insert(tup, rela);
                }
            }
        }

    }
    print_Relation(rela);
    return rela;
}
// print relation

// delete
void delete(char** strs, Relation r) {
    printf("DELETING\n");
    // check for the first element in string array
    if(strcmp(strs[0], "*") == 0){
        for(int i = 0; i < 1009; i++){
            LinkedList llist = r->array[i];
            LinkedListIterator it = LinkedList_iterator(llist);
            while(LinkedListIterator_hasNext(it)){
                Tuple tup = LinkedListIterator_next(it);
                if(check_Tuple(tup, strs)) {
                    LinkedList_remove(r->array[i], tup);
                }
            }
        }
    } else { 
        int key = hash(strs[0]);
        if(LinkedList_elementAt(r->array[key], 0) != NULL){
            LinkedListIterator it = LinkedList_iterator(r->array[key]);
            while(LinkedListIterator_hasNext(it)){
                Tuple tup = LinkedListIterator_next(it);
                if(check_Tuple(tup, strs)) {
                    LinkedList_remove(r->array[key], tup);
                }
            }
        }

    }
    print_Relation(r);
}
// selection
//Relation selection(char** strs, Relation r){

//}

// projection
//Relation projection(char** strs, Relation r){

//}

// join
int main(){
  Relation csg = create_CSG();

  char* csg1 [3] = {"CS101","12345","A"};
  char* csg2 [3] = {"CS101","67890","B"};
  char* csg3 [3] = {"EE200","12345","C"};
  char* csg4 [3] = {"EE200","22222","B+"};
  char* csg5 [3] = {"CS101","33333","A-"};
  char* csg6 [3] = {"PH100","67890","C+"};
  char* snap1 [4] = {"12345","C. Brown", "12 Apple St.", "555-1234"};
  char* snap2 [4] = {"67890","L. Van Pelt", "34 Pear Ave.", "555-5678"};
  char* snap3 [4] = {"22222","P. Patty", "56 Grape Blvd.", "555-9999"};
  char* cp1 [2] = {"CS101","CS100"};
  char* cp2 [2] = {"EE200","EE005"};
  char* cp3 [2] = {"EE200","CS100"};
  char* cp4 [2] = {"CS120","CS101"};
  char* cp5 [2] = {"CS121","CS120"};
  char* cp6 [2] = {"CS205","CS101"};
  char* cp7 [2] = {"CS206","CS121"};
  char* cp8 [2] = {"CS206","CS205"};
  char* cdh1 [3] = {"CS101","M","9AM"};
  char* cdh2 [3] = {"CS101","W","9AM"};
  char* cdh3 [3] = {"CS101","F","9AM"};
  char* cdh4 [3] = {"EE200","Tu","10AM"};
  char* cdh5 [3] = {"EE200","W","1PM"};
  char* cdh6 [3] = {"EE200","Th","10AM"};
  char* cr1 [2] = {"CS101","Turing Aud."};
  char* cr2 [2] = {"EE200","25 Ohm Hall"};
  char* cr3 [2] = {"PH100","Newton Lab."};
  Tuple t1 = create_Tuple(csg1);
  Tuple t2 = create_Tuple(csg2);
  Tuple t3 = create_Tuple(csg3);
    insert(t1,csg);
    insert(t2,csg);
    equalTuples(t1, t2);
    print_Relation(csg);
    char* query [3] = {"CS101", "*", "*"};
    lookup(query, csg);
    delete(query, csg);
}
