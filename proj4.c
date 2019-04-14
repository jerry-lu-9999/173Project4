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
    for(char* s = *strs; s != "\0"; *s++){
    //for (int i = 0; i < 3; i++){ //SIZE MUST BE FIXED
        //tuple->str[i] = strs[i];
	i++;
	s = strs[i];
 //     printf("'%s'\n",tuple->str[i]);
    }

//	printf("\n\nTHis is tuple at 0: %s \n1: %s \n, 2: %s \n", tuple->str[0], tuple->str[1], tuple->str[2]);
    return tuple;   
}

// NEED A HASH FUNCTION FOR STRING
int hash(char* str){
  int key=0;
  for(int i = 0; i < strlen(str); i++)
  {
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


// insert 
void insert(Tuple t, Relation r) {
    int key = hash(t->str[0]);
    if(LinkedList_elementAt(r->array[key], 0) != NULL){
       // printf("\n\ninsert if loop");
        LinkedListIterator it = LinkedList_iterator(r->array[key]);
       // printf("%d\n\n", key);
        // LinkedList_print_string_list(r->array[key]);
        while(LinkedListIterator_hasNext(it)){
            Tuple tup = LinkedListIterator_next(it);
            if(equalTuples(tup, t)){
            //    printf("\nTuple already exists");
            } else {
                tup -> next = t;

            }
        }
    } else {
       // printf("\n\n....else \n\n");
        LinkedList_add_at_front(r->array[key], t);
        //LinkedList_print_string_list(r->array[key]);

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

// lookup
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
// print relation

void print_Relation(Relation r){
	printf("PRINT RELATIONS");
    for(int i = 0; i < sizeof(r->array); i++){
        LinkedListIterator it = LinkedList_iterator(r->array[i]);
        while(LinkedListIterator_hasNext(it)){
            Tuple tup = LinkedListIterator_next(it);
		printf("SIZEOF %d", sizeof(tup->str));
            for(int j = 0; j < sizeof(tup->str); j++){
                printf("\n J: %d \n STRING: %s ", j, tup->str[j]);
            }
        }
    }
}

// delete
void delete(char** strs, Relation r) {
    if(strcmp(strs[0], "*")){
        for(int i = 0; i < r->size; i++){
            LinkedList llist = r->array[i];
            LinkedListIterator it = LinkedList_iterator(llist);
            while(LinkedListIterator_hasNext(it)){
                Tuple tup = LinkedListIterator_next(it);
                if(check_Tuple(tup, strs)) {
                    tup = tup->next;
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
                    tup = tup->next;
                }
            }
        }
    }
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
  char* sa1 [3] = {"CS101","12345","A"};
  char* sa2 [3] = {"CS101","12345","B"};
  printf("--->%d", sizeof(sa1)/sizeof(sa1[0]));
  Tuple t1 = create_Tuple(sa1);
  Tuple t2 = create_Tuple(sa2);
  insert(t1,csg);
  insert(t2,csg);
  equalTuples(t1, t2);
  print_Relation(csg);
}
