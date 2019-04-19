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
    Relation snap = new_Relation(3);
    LinkedList_add_at_front(snap->schema, "Phone");
    LinkedList_add_at_front(snap->schema, "Address");
    LinkedList_add_at_front(snap->schema, "Name");
    LinkedList_add_at_front(snap->schema, "StudentID");
    return snap;
}

Relation create_CP(){
    Relation cp = new_Relation(8);
    LinkedList_add_at_front(cp->schema, "Prerequisite");
    LinkedList_add_at_front(cp->schema, "Course");
    return cp;
}

Relation create_CDH(){
    Relation cdh = new_Relation(8);
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
    for (int i = 0; i < t1->tupSize; i++){
        for (int j = 0; j < t2->tupSize; j++){
            if (strcmp(t1->str[i],t2->str[i]) == 0){
                test = true;
            } else {
                test = false;
                break;
            }
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
    if(LinkedList_elementAt(r->array[key], 0) == NULL){
        LinkedList_add_at_front(r->array[key], t);
    } else {
        LinkedListIterator it = LinkedList_iterator(r->array[key]);
        while(LinkedListIterator_hasNext(it)){
            Tuple tup = LinkedListIterator_next(it);
            if(!equalTuples(tup, t)){
                LinkedList_add_at_end(r->array[key], t);
            } 
            return;
        }
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
void getStudentCourseGrade (Relation snap, Relation csg){
    char name[50],  course[20];;
    printf("Example 8.6\n");
    printf("Answer to the query \"What grade did StudentName get in CourseName\"?\n");
    printf("Enter student name\n");
    fgets(name, 50, stdin);
    strtok(name, "\n");
    printf("Enter course\n");
    fgets(course, 50, stdin); 
    strtok(course, "\n");
    for(int i = 0; i < 1009; i++){
        LinkedList llist = snap->array[i];
        LinkedListIterator it = LinkedList_iterator(llist);
        while(LinkedListIterator_hasNext(it)){
            Tuple tup = LinkedListIterator_next(it);
            if(strcmp(tup->str[1], name) == 0){
                char* id = tup->str[0];
                for(int j = 0; j < 1009; j++){
                    LinkedList llist2 = csg->array[j];
                    LinkedListIterator it2 = LinkedList_iterator(llist2);
                    while(LinkedListIterator_hasNext(it2)){
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

void getStudentatTimeonDay(Relation snap, Relation csg, Relation cdh, Relation cr){
    char name[50], time[10], day[5];
    printf("Answer to the query \"Where is StudentName at Time on Day\"?\n");
    printf("Enter student name\n");
    fgets(name, 50, stdin);
    strtok(name, "\n");
    printf("Enter time\n");
    fgets(time, 10, stdin);
    strtok(time, "\n");
    printf("Enter day\n");
    fgets(day, 5, stdin);
    strtok(day, "\n");
    LinkedList courses = new_LinkedList();
    for(int i = 0; i < 1009; i++){
        LinkedList llist = snap->array[i];
        LinkedListIterator it = LinkedList_iterator(llist);
        while(LinkedListIterator_hasNext(it)){
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
                    break;
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

// PART 3   
Relation selection(char** strs, Relation r){
    LinkedList schema = r->schema;
    LinkedListIterator it = LinkedList_iterator(schema);
    int count = 0;
    char* lookupQ[10];
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
        printf("%s ", lookupQ[i]);
    }
    Relation selection = lookup(lookupQ, r);
    return selection;
}

int getIndex (LinkedList list, char* str){
    int count = 0;
    LinkedListIterator it = LinkedList_iterator(list);
    while(LinkedListIterator_hasNext(it)){
        char* e = LinkedListIterator_next(it);
        if(strcmp(e, str) == 0){
            break;
        }
        count++;
    }
    return count;
}
LinkedList intersectList(LinkedList l1, LinkedList l2){
    LinkedListIterator it1 = LinkedList_iterator(l1);
    
    LinkedList intersection = new_LinkedList();
    while(LinkedListIterator_hasNext(it1)){
        char* att1 = (void*) LinkedListIterator_next(it1);
        LinkedListIterator it2 = LinkedList_iterator(l2);
        while(LinkedListIterator_hasNext(it2)){
            char* att2 = (void*) LinkedListIterator_next(it2);
            if(strcmp(att1, att2) == 0){
                LinkedList_add_at_end(intersection, att1);
            }
        }
    }
    return intersection;
}

LinkedList unionList (LinkedList l1, LinkedList l2){
    LinkedListIterator it1 = LinkedList_iterator(l1);
    LinkedListIterator it2 = LinkedList_iterator(l2);
    LinkedList unionList = new_LinkedList();
    
    while(LinkedListIterator_hasNext(it1)){
        char* att1 = LinkedListIterator_next(it1);
        LinkedList_add_at_end(unionList, att1);
    }
    while(LinkedListIterator_hasNext(it2)){
        char* att2 = LinkedListIterator_next(it2);
        if(!LinkedList_contains(unionList, att2)){
            LinkedList_add_at_end(unionList, att2);
        }
    }
    return unionList;
}

//this returns the indexes of the attributes that are the same
//this can be stored as arrays of size 2, index 0 being the index i of schema 1
//index 1 being the index j of schema 2
LinkedList indexPairs (LinkedList schema1, LinkedList schema2){
    LinkedList pair = new_LinkedList();
    LinkedList newSchema = intersectList(schema1, schema2);
    LinkedListIterator itnew = LinkedList_iterator(newSchema);
    while(LinkedListIterator_hasNext(itnew)){
        char* att = LinkedListIterator_next(itnew);
        int index1 = getIndex(schema1, att);
        int index2 = getIndex(schema2, att);
        LinkedList_add_at_front(pair, index1);
        LinkedList_add_at_end(pair, index2);
    }
    return pair;
}

Tuple mergeTuple(Tuple tup1, Tuple tup2, LinkedList indexPairs){
    char* tupArr[100];
    int curCount = 0;
    for(int i = 0; i < tup1->tupSize; i++){
        tupArr[curCount] = tup1->str[i];
        curCount++;
    }
    int ind1 = (int) LinkedList_elementAt(indexPairs, 0);
    int ind2 = (int) LinkedList_elementAt(indexPairs, 1);
    char* att1 = tup1->str[ind1];
    char* att2 = tup2->str[ind2];
    if(strcmp(att1, att2) != 0){
        return NULL;
    }
    
    int at2 = 0;
    for(int i = 0; i < tup2->tupSize; i++){
            if(i != at2){
                tupArr[curCount] = tup2->str[i];
                curCount++;
            }
    }
    int newSize = curCount;
    Tuple newTup = create_Tuple(tupArr, newSize);
    return newTup;
}


//projection
Relation projection(char** strs, Relation r){
    LinkedList schema = r-> schema;
    LinkedList newSchema = new_LinkedList(); //maybe no need for this????????
    LinkedListIterator it = LinkedList_iterator(schema);
    int colID[4];
    int count = 0;

    //go through schema and count the number of 
    while(LinkedListIterator_hasNext(it)){
        char* attribute = LinkedListIterator_next(it);
        for(int i = 0; i< r->size; i++){
            // printf("\nwhat's wrong");
            // printf("\nattribute: %s", attribute);
            // printf("\nstrs: %s", strs[i]);
            if(strs[i]==NULL){
                // printf("\nwill break");
                break;
            }else if(strcmp(attribute, strs[i]) == 0){ //if the attribute and
                // printf("colID insert"); 
                colID[count] = i; //create and array of column ID 
                count++;
                LinkedList_add_at_end(newSchema, strs[i]);
            }
        }
    }

    //now there is a new schema
    Relation projection = new_Relation(count);
    projection->schema = newSchema;
    int counter = 0;
    char* temp [4];

    for(int i = 0; i < 1009; i++){
        LinkedListIterator it = LinkedList_iterator(r->array[i]); //go through all relation
        while(LinkedListIterator_hasNext(it) && it != NULL){//for every tuple in the list
            //if it contains column id, then put it into a new relation
            Tuple tup = LinkedListIterator_next(it);
            for(int k = 0; k < tup->tupSize; k++){//tuple 
                for(int j = 0; j < 4; j++){ //go through column id    
                // printf("stuck?"); 
                    //int tupleSize = tup->tupSize;
                    if(colID[j] == k){//at the place where the colID matches the tuple loc
                        temp[counter] = tup->str[k];
                        counter++;
                        // printf("\nit entered: %s\n\n",temp[counter-1]);
                    }
                        // printf("this is counts: %d", count);
                }
            }
        }
    }
                // int t = count;
                // printf("count: %d", count);
            char* input[10];
            for(int c = 0; c < counter; c++){
                input[0] = temp[c];
                    // printf("counter::: %d", counter);
                    // printf("\nTHISIS TEMP[%d]: %s\n\n",c,temp[c]);
                    // printf("\nTHISIS INPUT[%d]: %s\n\n",t,input[t]);
                Tuple t1 = create_Tuple(input, count);
                insert(t1, projection);
            }
    printf("\n\n");
    print_Relation(projection);
    return projection;
}

// join
Relation join(Relation r1, Relation r2){
    LinkedList newSchema = unionList(r1->schema, r2->schema);
    //LinkedList joinedAtt = intersectList(r1->schema, r2->schema);
    //Calculate index pairs here
    if(LinkedList_isEmpty(newSchema)){
        printf("These two relations can't be joined. \n");
        return NULL;
    }
    Relation joinedRela = new_Relation(19);

    for(int i = 0; i < 1009; i++){//for all tuples in R1
        LinkedList l1 = r1->array[i];
        LinkedListIterator it1 = LinkedList_iterator(l1);
        while(LinkedListIterator_hasNext(it1)){//All the tuples in the list at the position i from R1
            Tuple tup1 = LinkedListIterator_next(it1);
            for(int j = 0; j < 1009; j++){//For all tuples in R2
                LinkedList l2 = r2->array[j];
                LinkedListIterator it2 = LinkedList_iterator(l2);
                while(LinkedListIterator_hasNext(it2)){//All the tuples in the list at the position i from R2
                    Tuple tup2 = LinkedListIterator_next(it2);
                    LinkedList pair = indexPairs(r1->schema, r2->schema);
                    Tuple newTup = mergeTuple(tup1, tup2, pair);
                    if(newTup == NULL){
                        break;
                    }
                    insert(newTup, joinedRela);
                }
            }
        }
    }
    print_Relation(joinedRela);
    return joinedRela;
}
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

  Database db = new_Database();
  db->CSG = create_CSG();
  db->SNAP = create_SNAP();
  db->CDH = create_CDH();
  db->CP = create_CP();
  db->CR = create_CR();

  fgets(line,256,fptr);
 // if(strcmp("Course StudentID Grade\n",line) == 0){
    for (int i = 0; i < 6; i++){
         fgets(line, 256, fptr);
	       char* CSGtuple[3];
         token = strtok(line, "|");
	     for (int m = 0; m < 3; m++){
            while(token != NULL ){		    
		            CSGtuple[m] = token;
		            printf("%s ",CSGtuple[m]);
              	token = strtok(NULL, "|");
		        }
	     }
	      Tuple t = create_Tuple(CSGtuple, 3);
		    insert(t,db->CSG);
      
    }
    fgets(line, 256, fptr);   //this fgets is to consume the empty line between two relations
    fgets(line,256, fptr);   //read in the schema
    printf("\n");
    //printf("%s",line);
    //if (strcmp("StudentID Name Address Phone\n", line) == 0){}
    for (int j = 0; j < 3; j++){
        fgets(line,256,fptr);
       // printf("%s\n", line);
        char* SNAPtuple[4];
        token = strtok(line, "|");
           for (int n = 0; n < 4; n++){
          	while (token != NULL){
	              SNAPtuple[n] = token;
	              printf("%s ", SNAPtuple[n]);
	              token = strtok(NULL, "|");
	         }
      }
	        //Tuple ab = create_Tuple(SNAPtuple, 4);
	        //insert(ab, db->SNAP);
         //  printf("insert success\n");
    }

    fgets(line,256,fptr);
    fgets(line,256,fptr); //this line reads in Course Day Hour
    printf("\n");
      for (int a = 0; a < 3; a++){
	         fgets(line, 256, fptr);
	         char* CDHtuple[3];
	         token = strtok(line,"|");
           //printf("%s\n", line);
	         for (int b = 0; b < 3; b++){
	             while (token != NULL){
	                CDHtuple[b] = token;
	                printf("%s", CDHtuple[b]);
	                token = strtok(NULL, "|");
	  }
	}
		//Tuple t = create_Tuple(CDHtuple, 3);
    //insert(t, db->CDH);
    }

      printf("\n");
      fgets(line,256,fptr);
      fgets(line,256,fptr);
	    //printf("%s", line);
    for (int c = 0; c < 4; c++){
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
      Tuple t = create_Tuple(CRtuple,2);
      insert(t, db->CR);
   
    }
    
    fgets(line,256,fptr);
    fgets(line,256, fptr);
    //printf("\n%s",line);
    for (int e = 0; e < 7; e++){
      fgets(line, 256, fptr);
      char* CPtuple[2];
      token = strtok(line, "|");
      for (int f = 0; f < 2; f++){
          while (token != NULL){
                 CPtuple[f] = token;
	               printf("%s", CPtuple[f]);
	               token = strtok(NULL, "|");
	        }  
      }
      Tuple t = create_Tuple(CPtuple,2);
      insert(t, db->CP);
      //printf("insert success");
    }
  }
  

  

void writeFile(Database db){
  FILE *fptr;
  fptr = fopen("Database.txt","w+");

  // print_Relation(db->CSG);
   fputs("Course StudentID Grade", fptr);
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
   fputs("\nStudentID Name Address Phone", fptr);
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
   fputs("\nCourse Day Hour", fptr);
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
   fputs("\nCourse Room", fptr);
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
   fputs("\nCourse Prerequisite", fptr);
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
    readFile("Database.txt");
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

    printf("PART 1\n");
    printf("Demonstrate LOOK UP function.\n");
    printf("1. (lookup(\"CS101\", 12345, *), Course-StudentId-Grade)\n");
    char* lookup1 [3] = {"CS101", "12345", "*"};
    lookup(lookup1, db->CSG);
    printf("2. (lookup(\"CS205\", \"CS120\"), Course-Prerequisite)\n");
    char* lookup2 [2] = {"CS205", "CS120"};
    lookup(lookup2, db->CP);

    /* COMMENT THIS PART OUT TO KEEP THE CR RELATION INTACT FOR TESTING IN PART 2
    printf("Demonstrate DELETE function.\n");
    printf("3. (delete(\"CS101\", *), Course-Room)\n");
    char* delete3 [2] = {"CS101", "*"};
    delete(delete3, db->CR);
    */
    printf("\nPART 2\n"); 
    // getStudentCourseGrade(db->SNAP, db->CSG);
    // getStudentatTimeonDay(db->SNAP, db->CSG, db->CDH, db->CR);

    printf("\nPART 3\n");
    printf("\nExample 8.12.\n");
    printf("Demonstrating selection functions for any query in the from of C=a (i.e Course=\"CS101\").\n");
    char* selectionQuery [3] = {"Room", "=", "Turing Aud."};
    selection(selectionQuery, db->CR);
    printf("\nExample 8.15\n");
    printf("Computing JOIN function for every 2 relations\n");
    join(db->CR, db->CDH);

    printf("\nProjection Demonstration CSG:");
    char* csg1TEST [2] = {"Course"};
    projection(csg1TEST, db->CSG);
    printf("\nProjection Demonstration SNAP:");

    char* snap1TEST [2] = {"Name"};
    projection(snap1TEST, db->CSG);
    printf("\nProjection Demonstration CDH:");

    char* cdh1TEST [2] = {"Hour"};
    projection(cdh1TEST, db->CDH);
    printf("\nProjection Demonstration CR:");

    char* cr1TEST [2] = {"Course"};
    projection(cr1TEST, db->CR);

    // printf("Example 8.15\n");
    char* prjQuery [2] = {"Day", "Hour"};
    char* selectQuery [3] = {"Room", "=", "Turing Aud."};
    projection(prjQuery, selection(selectQuery,join(db->CR, db->CDH)));
}
