/***************************************************************************************************************************
Header Files
****************************************************************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>
#include <math.h>
#include <time.h>
#include <complex.h>
#include <assert.h>
#include <limits.h>
/***************************************************************************************************************************
Constant Declarations 
****************************************************************************************************************************/
// VARIOUS CONSTANTS
#define Mfnam      20			// max size for a file name
#define Mnod    15000 		        // max number of nodes in a graph/node
#define Mlin      200			// max size of characters in a line
#define Mnam       25			// max size of a node name
#define Mtyp       10			// max type of nodes/gates
#define Mout       16		        // max node out degree (Nfo)
#define Min         9			// max node in degree (Nfi)
#define Mpi       233			// max number of primary inputs
#define Mpo       140			// max number of primary outputs
#define Mpt       1000000			// max number of input patterns in .vec file
#define Mft       10			// max number of stuck at faults in .faults file
// NODE TYPE CONSTANTS 
#define INPT 1				// Primary Input
#define AND  2				// AND 
#define NAND 3				// NAND 
#define OR   4				// OR 
#define NOR  5				// NOR 
#define XOR  6				// XOR 
#define XNOR 7				// XNOR 
#define BUFF 8				// BUFFER 
#define NOT  9				// INVERTER 
#define FROM 10				// STEM BRANCH
/***************************************************************************************************************************
Structure Declarations 
****************************************************************************************************************************/
//1.Stucture declaration for LIST
typedef struct LIST {
   int  id;	
   int Mark;	   //id for current element		
   struct LIST *next;  //pointer to next id element( if there is no element, then it will be NULL)		
} LIST;
//2.Stucture declaration for NODE

typedef struct NODE_type
{
  char Name[Mnam];                      //name of the node
  int Type,Nfi,Nfo,Po;                  //type, nooffanins, nooffanouts,primaryo/p
  int Mark,Cval,Fval;                    //marker,correctvalue,faultvalue
  LIST *Fin,*Fot;                      //fanin members, fanout members 
} NODE;
//3.Stucture declaration for PATTERN
struct Node 
{
	char pattern[1000];				// integer data
	struct Node* next;	   // pointer to the next node
};

typedef struct PATTERN
{
  char PI[Mpi];
} PATTERN;

typedef struct faultList
{
  LIST *opFaults;
  int Mark;
  int length;

}faultList;


//4.Stucture declaration for FAULT



/***************************************************************************************************************************
Functions in given.c
****************************************************************************************************************************/
/***************************************************************************************************************************
LIST Structure Functions
****************************************************************************************************************************/
void InsertList(LIST **,int);
void PrintList(LIST *);
void FreeList(LIST **);
/***************************************************************************************************************************
 NODE Structure Functions
****************************************************************************************************************************/
int ReadIsc(FILE *,NODE *);
void InitializeCircuit(NODE *,int);
int AssignTypee(char *);
void PrintCircuit(NODE *,int);
void ClearCircuit(NODE *,int);
void concat (int, int , int*);
int countPI(NODE*,int);
int countPO(NODE*,int);
void makeNewISC(NODE *, NODE *,int,int);

void copyFile(FILE *, NODE *,FILE *, int,int,int,int );
struct Node* readTestFile(FILE *, FILE*,int, int*);

void selectRandomPattern(struct Node*,FILE *,FILE *, int,int,int );
void run(FILE *, FILE* , FILE *, int , int , int ,int);
void createXORbranch(NODE *,LIST *, FILE *,int, int,int);
void mainPart(int , int , int, int,int, int, NODE*, FILE*,FILE*,FILE*, FILE*, FILE*, int);
int ReadVec(FILE *, PATTERN *);

int simulate(int, int, NODE *, PATTERN *, FILE *,int,int, int, char [2][100], faultList*,int);
int charToInt(char);
char intToChar(int);
void runSimulate( int, int, NODE *,PATTERN *, FILE *, faultList *, faultList *,int, int,int,int);
void InitializeFaultList(faultList *, int);
int range (int,int);
/***************************************************************************************************************************
 PATTERN Structure Functions
****************************************************************************************************************************/

/***************************************************************************************************************************
User Defined Functions in user.c
****************************************************************************************************************************/

/****************************************************************************************************************************/
