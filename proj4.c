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
    int tupSize;
    Tuple next;
};

typedef struct Database *Database;
struct Database {
  Relation CSG;
  Relation SNAP;
  Relation CP;
  Relation CDH;
  Relation CR;
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
    Relation csg = new_Relation(7);
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

Database new_Database(){
    Database this = (Database)malloc(sizeof(struct Database));
    this->CSG = create_CSG();
    this->SNAP = create_SNAP();
    this->CP = create_CP();
    this->CDH = create_CDH();
    this->CR = create_CR();
    return this;
}

// create tuple
Tuple create_Tuple(char* strs[], int tupSize) {
    Tuple tuple = (Tuple)malloc(sizeof(struct Tuple));
    if(tuple == NULL) {
        return NULL;
    }
    tuple->tupSize = tupSize;
    // for(char* s = *strs; s != "\0"; *s++){
    for (int i = 0; i < tupSize; i++){ //SIZE MUST BE FIXED
        tuple->str[i] = strs[i];
 //     printf("'%s'\n",tuple->str[i]);
    }
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
    if(t1->tupSize != t2->tupSize) return false;
    bool test = false;
    for (int j = 0; j < t1->tupSize; j++){
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
    for(int i = 0; i < 3; i++){ // size again
        printf("%s ", tup->str[i]);
    }
    printf("\n");
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

char* Tuple_toString(Tuple tup, int i){
  printf("%s", tup->str[i]);
  return tup->str[i];
}

// insert
void insert(Tuple t, Relation r) {
    int key = hash(t->str[0]);
    if(LinkedList_elementAt(r->array[key], 0) != NULL){
        LinkedListIterator it = LinkedList_iterator(r->array[key]);
        while(LinkedListIterator_hasNext(it)){
            Tuple tup = LinkedListIterator_next(it);
            if(equalTuples(tup, t)){
	      //  printf("\nTuple already exists");
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
    // printf("pattern size %d\n", sizepat);
    for(int i = 1; i < t->tupSize; i++){
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
// PART 2
void getStudentCourseGrade (char* name, char* course, Relation snap, Relation csg){
    //printf("goes in here\n");
    for(int i = 0; i < 1009; i++){
        LinkedList llist = snap->array[i];
        LinkedListIterator it = LinkedList_iterator(llist);
        while(LinkedListIterator_hasNext(it)){
            //printf("goes in here 2\n");
            Tuple tup = LinkedListIterator_next(it);
            if(strcmp(tup->str[1], name) == 0){
                char* id = tup->str[0];
                for(int j = 0; j < 1009; j++){
                    LinkedList llist2 = csg->array[j];
                    LinkedListIterator it2 = LinkedList_iterator(llist2);
                    while(LinkedListIterator_hasNext(it2)){
                        // printf("goes in here 3\n");
                        Tuple tup2 = LinkedListIterator_next(it2);
                        if((strcmp(tup2->str[1], id) == 0) && (strcmp(tup2->str[0], course) == 0)){
                            printf("The grade for student %s in course %s is %s\n", name, course, tup2->str[2]);
                        }
                    }
                }
            }
        }
    }
}

void getStudentatTimeonDay(char* name, char* time, char* day, Relation snap, Relation csg, Relation cdh, Relation cr){
    LinkedList courses = new_LinkedList();
    for(int i = 0; i < 1009; i++){
        LinkedList llist = snap->array[i];
        LinkedListIterator it = LinkedList_iterator(llist);
        while(LinkedListIterator_hasNext(it)){
            //printf("goes in here 2\n");
            Tuple tup = LinkedListIterator_next(it);
            if(strcmp(tup->str[1], name) == 0){
                // get the id of the student from SNAP
                char* id = tup->str[0];
                for(int j = 0; j < 1009; j++){
                    LinkedList llist2 = csg->array[j];
                    LinkedListIterator it2 = LinkedList_iterator(llist2);
                    while(LinkedListIterator_hasNext(it2)){
                        Tuple tup2 = LinkedListIterator_next(it2);
                        if((strcmp(tup2->str[1], id) == 0)){
                            // get all of courses of that student from CSG
                            LinkedList_add_at_front(courses, tup2->str[0]);
                        }
                    }
                }
            }
        }
    }
    int keyCourse = 0;
    // get all of the courses from the linked list that has corresponding hours and days from CDH
    for(int i = 0; i < 1009; i++){
        LinkedList clist = cdh->array[i];
        LinkedListIterator itCDH = LinkedList_iterator(clist);
        while(LinkedListIterator_hasNext(itCDH)){
            Tuple tupCDH = LinkedListIterator_next(itCDH);
            LinkedListIterator itCourse = LinkedList_iterator(courses);
            while(LinkedListIterator_hasNext(itCourse)){
                char* curCourse = LinkedListIterator_next(itCourse);
                if((strcmp(tupCDH->str[0], curCourse) == 0)&&(strcmp(tupCDH->str[1], day) == 0)&&(strcmp(tupCDH->str[2], time)==0)){
                    keyCourse = hash(curCourse);
                }
            }
        }
    }
    LinkedList listRoom = cr->array[keyCourse];
    LinkedListIterator itRoom = LinkedList_iterator(listRoom);
    while(LinkedListIterator_hasNext(itRoom)){
        Tuple tupRoom = LinkedListIterator_next(itRoom);
        printf("The student %s is in room %s\n", name, tupRoom->str[1]);
    }
}

//selection
Relation selection(char** strs, Relation r){
    LinkedList schema = r->schema;
    LinkedListIterator it = LinkedList_iterator(schema);
    int count = 0;
    char* lookupQ[4];
    while(LinkedListIterator_hasNext(it)){
        char* attribute = LinkedListIterator_next(it);
        if(strcmp(attribute, strs[0]) == 0){//if there is no value in the schema then break
            break;
        }
        count++;
    }
    for(int i = 0; i < r->size; i++){
        if(i==count) {
            lookupQ[count] = (char*) strs[2];
        } else {
            lookupQ[i] = (char*) "*";
        }
        
    }
    Relation selection = lookup(lookupQ, r);
    return selection;
}

// projection
//Relation projection(char** strs, Relation r){

//}

// join
void readFile(char name[]){
  FILE *fptr;
  char *token;
  
  fptr = fopen(name,"r");

  char line[256];//store each line
  
  if (fptr == NULL){
    printf("open file error!");
    exit(1);
  } else {
    printf("Reading success! Loading...\n");
  }

  //Database db = new_Database();
  
  fgets(line,256,fptr);
  if(strcmp("CSG\n",line) == 0){
    for (int i = 0; i < 7; i++){
         fgets(line, 256, fptr);
	 char* CSGtuple[3];
             token = strtok(line, "|");
	     for (int m = 0; m < 3; m++){
                 while(token != NULL ){		    
		    CSGtuple[m] = token;
		    printf("%s",CSGtuple[m]);
              	    token = strtok(NULL, "|");
		 }
	     }
	     // Tuple t = create_Tuple(CSGtuple, 3);
		    // insert(t,db->CSG);
    }
    printf("\n");
    fgets(line, 256, fptr);
    fgets(line,256,fptr);
    printf("%s",line);
    for (int j = 0; j < 3; j++){
        fgets(line,256,fptr);
        char* SNAPtuple[4];
        token = strtok(line, "|");
           for (int n = 0; n < 3; n++){
          	while (token != NULL){
	          SNAPtuple[n] = token;
	          printf("%s", SNAPtuple[n]);
	          token = strtok(NULL, "|");
	         }
              }
	   //Tuple t = create_Tuple(SNAPtuple, 4);
	   //
    }
    fgets(line,256,fptr);
    fgets(line,256,fptr);
    printf("\n%s", line);
      for (int a = 0; a < 8; a++){
	fgets(line, 256, fptr);
	char* CDHtuple[3];
	token = strtok(line,"|");
	for (int b = 0; b < 3; b++){
	  while (token != NULL){
	    CDHtuple[b] = token;
	    printf("%s", CDHtuple[b]);
	    token = strtok(NULL, "|");
	  }
	}
	//	Tuple t = create_Tuple(CDHtuple, 3);
    }
      printf("\n");
      fgets(line,256,fptr);
         fgets(line,256,fptr);
	 printf("%s", line);
    for (int c = 0; c < 3; c++){
      fgets(line, 256, fptr);
      char* CRtuple[3];
      token = strtok(line, "|");
      for (int d = 0; d < 2; d++){
	while (token != NULL){
          CRtuple[d] = token;
	  printf("%s", CRtuple[d]);
	  token = strtok(NULL, "|");
	}
      }
      //Tuple t = create_Tuple(CRtuple,2);
    }
    
    fgets(line,256,fptr);
    fgets(line,256, fptr);
    printf("\n%s",line);
    for (int e = 0; e < 7; e++){
      fgets(line, 256, fptr);
      char* CPtuple[2];
      token = strtok(line, "|");
      for (int f = 0; f < 2; f++){
        while (token!=NULL){
          CPtuple[f] = token;
	  printf("%s", CPtuple[f]);
	  token = strtok(NULL, "|");
	}
      }
      //Tuple t = create_Tuple(CPtuple,2);
    }
  }
  
}
  

void writeFile(Database db){
  FILE *fptr;
  fptr = fopen("Database","w+");

  // print_Relation(db->CSG);
   fputs("CSG", fptr);
   int CSGsizearr = sizeof(db->CSG->array)/sizeof(db->CSG->array[0]);
   for(int i = 0; i < CSGsizearr; i++){
   LinkedListIterator it = LinkedList_iterator(db->CSG->array[i]);
        while(LinkedListIterator_hasNext(it) && it != NULL){
            Tuple tup = LinkedListIterator_next(it);
	    fputs("\n",fptr);
	    printf("\n");
	        for(int j = 0; j < tup->tupSize; j++){ // size again
		  // printf("\n this is i: %d \n",j);
		  
		  fputs(Tuple_toString(tup,j),fptr);
		  fputs("|",fptr);
		  printf(" | ");
		}
	    }    
	
   }

     // print_Relation(db->SNAP);
   fputs("\n",fptr);
   fputs("\nSNAP", fptr);
   int SNAPsizearr = sizeof(db->SNAP->array)/sizeof(db->SNAP->array[0]);
   for(int i = 0; i < SNAPsizearr; i++){
   LinkedListIterator it = LinkedList_iterator(db->SNAP->array[i]);
        while(LinkedListIterator_hasNext(it) && it != NULL){
            Tuple tup = LinkedListIterator_next(it);
	    fputs("\n",fptr);
	    printf("\n");
	        for(int j = 0; j < tup->tupSize; j++){ // size again
		  // printf("\n this is i: %d \n",j);
		  fputs(Tuple_toString(tup,j),fptr);
		  fputs("|",fptr);
		  printf(" | ");
		}
	     
   }
   }

        // print_Relation(db->CDH);
   fputs("\n",fptr);
   fputs("\nCDH", fptr);
   int CDHsizearr = sizeof(db->CDH->array)/sizeof(db->CDH->array[0]);
   for(int i = 0; i < CDHsizearr; i++){
   LinkedListIterator it = LinkedList_iterator(db->CDH->array[i]);
        while(LinkedListIterator_hasNext(it) && it != NULL){
            Tuple tup = LinkedListIterator_next(it);
	    fputs("\n",fptr);
	    printf("\n");
	        for(int j = 0; j < tup->tupSize; j++){ // size again
		  // printf("\n this is i: %d \n",j);
		  fputs(Tuple_toString(tup,j),fptr);
		  fputs("|",fptr);
		  printf(" | ");
		}
	     
   }
   }


        // print_Relation(db->CR);
   fputs("\n",fptr);
   fputs("\nCR", fptr);
   int CRsizearr = sizeof(db->CR->array)/sizeof(db->CR->array[0]);
   for(int i = 0; i < CRsizearr; i++){
   LinkedListIterator it = LinkedList_iterator(db->CR->array[i]);
        while(LinkedListIterator_hasNext(it) && it != NULL){
            Tuple tup = LinkedListIterator_next(it);
	    fputs("\n",fptr);
	    printf("\n");
	        for(int j = 0; j < tup->tupSize; j++){ // size again
		  // printf("\n this is i: %d \n",j);
		  fputs(Tuple_toString(tup,j),fptr);
		  fputs("|",fptr);
		  printf(" | ");
		}
	     
	}	
   }

   fputs("\n",fptr);
   fputs("\nCP", fptr);
   int CPsizearr = sizeof(db->CP->array)/sizeof(db->CP->array[0]);
   for (int i = 0; i < CPsizearr; i++){
     LinkedListIterator it = LinkedList_iterator(db->CP->array[i]);
     while(LinkedListIterator_hasNext(it) && it != NULL){
       Tuple tup = LinkedListIterator_next(it);
       fputs("\n", fptr);
       printf("\n");
       for (int j = 0; j < tup->tupSize; j++){
	 fputs(Tuple_toString(tup,j),fptr);
	 fputs("|",fptr);
	 printf(" | ");
       }
     }
   }
   fclose(fptr);
}

int main(){
    Database db = new_Database();
    printf("Here is reading file");
    readFile("Database");
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
    Tuple t1 = create_Tuple(csg1, 3);
    Tuple t2 = create_Tuple(csg2, 3);
    Tuple t3 = create_Tuple(csg3, 3);
    Tuple t4 = create_Tuple(csg4, 3);
    Tuple t5 = create_Tuple(csg5, 3);
    Tuple t6 = create_Tuple(csg6, 3);
    
    Tuple s1 = create_Tuple(snap1, 4);
    Tuple s2 = create_Tuple(snap2, 4);
    Tuple s3 = create_Tuple(snap3, 4);
    
    Tuple cd1 = create_Tuple(cdh1, 3);
    Tuple cd2 = create_Tuple(cdh2, 3);
    Tuple cd3 = create_Tuple(cdh3, 3);
    Tuple cd4 = create_Tuple(cdh4, 3);
    Tuple cd5 = create_Tuple(cdh5, 3);
    Tuple cd6 = create_Tuple(cdh6, 3);

    Tuple c1 = create_Tuple(cr1, 2);
    Tuple c2 = create_Tuple(cr2, 2);
    Tuple c3 = create_Tuple(cr3, 2);
    Tuple CP1 = create_Tuple(cp1,2);
    Tuple CP2 = create_Tuple(cp2,2);
    Tuple CP3 = create_Tuple(cp3,2);
    Tuple CP4 = create_Tuple(cp4,2);
    Tuple CP5 = create_Tuple(cp5,2);
    Tuple CP6 = create_Tuple(cp6,2);
    Tuple CP7 = create_Tuple(cp7,2);
    Tuple CP8 = create_Tuple(cp8,2);
    
    insert(t1,db->CSG);
    insert(t2,db->CSG);
    insert(t3,db->CSG);
    insert(t4,db->CSG);
    insert(t5,db->CSG);
    insert(t6,db->CSG);
    
    insert(s1,db->SNAP);
    insert(s2,db->SNAP);
    insert(s3,db->SNAP);
    insert(cd1,db->CDH);
    insert(cd2,db->CDH);
    insert(cd3,db->CDH);
    insert(cd4,db->CDH);
    insert(cd5,db->CDH);
    insert(cd6,db->CDH);
    
    insert(c1,db->CR);
    insert(c2,db->CR);
    insert(c3,db->CR);
    
    insert(CP1,db->CP);
    insert(CP2,db->CP);
    insert(CP3,db->CP);
    insert(CP4,db->CP);
    insert(CP5,db->CP);
    insert(CP6,db->CP);
    insert(CP7,db->CP);
    insert(CP8,db->CP);
   
    printf("\nHere is writing the file");
    writeFile(db);
    printf("\n");
    printf("\n");

    char str[20], course[20];
    char *token;
    printf("PART 1\n");

    char* query [3] = {"CS101", "", ""};
    lookup(query, db->CSG);
    delete(query, db->CSG);

    printf("PART 2\n");

    printf("PART 3\n");
    
    scanf("%s", str);
    char* temp[3];
    temp[0] = strtok(str,"");
    for (int i = 1; i < 3; i++){
        if (token != NULL){
	  temp[i] = strtok(NULL,"");
      }	  
    }
    
    char* selectionQuery [3] = {temp[0], temp[1], temp[2]};
    
    selection(selectionQuery, db->CSG);
    
    printf("put in a specific course");
    scanf("%s", course);

    lookup(query, db->CSG);

    printf("Give me a student name\n");
    char name[20], class[20];
    scanf("%s", name);
    printf("then give me a class he is in:\n");
    scanf("%s", class);
    getStudentCourseGrade(name, class ,db->SNAP, db->CSG);
 
    char time[10], day[1];
    printf("Give me a time of a day and a specific day\n");
    scanf("%s", time);
    scanf("%s", day);
    getStudentatTimeonDay(name, time, day, db->SNAP, db->CSG, db->CDH, db->CR);

    printf("\nProjection Demonstration CSG:")
    char* csg1TEST [2] = {"Course"};
    Relation prj1 = projection(csg1TEST, db->CSG);
    printf("\nProjection Demonstration SNAP:")

    char* snap1TEST [2] = {"Name"};
    Relation prj2 = projection(csg1TEST, db->CSG);
    printf("\nProjection Demonstration CDH:")

    char* cdh1TEST [2] = {"Hour"}
    Relation prj3 = projection(cdh1TEST, db->CDH);
    printf("\nProjection Demonstration CR:")

    char* cr1TEST [2] = {"Course"};
    Relation prj4 = projection(cr1TEST, db->CR);
}
