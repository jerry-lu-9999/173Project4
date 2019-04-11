#include <stdlib.h>
#include <stdio.h>

typedef struct CSG *CSGList;
typedef struct SNAP *SNAPList;
typedef struct CP *CPList;
typedef struct CDH *CDHList;
typedef struct CR *CRList;

struct CSG {
  int studentId;
  char course[5];
  char grade[2];
  CSGList next;
};

struct SNAP{
  int studentId;
  char name[50];
  char address[50];
  long phone;
  SNAPList next;
};

struct CP{
  char course[5];
  char prerequisite[5];
  CPList next;
};

struct CDH{
  char course[5];
  char day[2];
  char hour[4];
  CDHList next;
}

struct CR{
  char course[5];
  char room[100];
  CRList next;
}
typedef CSGList CSGHASHTABLE[1009]; //HASTABLE[1009] is an array
typedef SNAPList SNAPHASHTABLE[1009];
typedef CPList CPHASHTABLE[1009];
typedef CDHList CDHASHTABLE[1009];
typedef CRList CRHASHTABLE[1009];

void insert_CSG(CSG csg, CSGList list){
  int key = csg -> studentID % 1009;
  if (CSGHASHTABLE[key] != null){
      
  }
  struct CSG *csg = (struct CSG*)malloc(sizeof(struct CSG));
  csg ->
}

int main(){
  
}

