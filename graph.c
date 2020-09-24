#include "graph.h"

/***************************************************************************************************************************
Insert an element "x" at end of LIST "l", if "x" is not already in "l". 
****************************************************************************************************************************/
void InsertList(LIST **l, int x)
{
  LIST *p, *tl;

  if ((p = (LIST *)malloc(sizeof(LIST))) == NULL)
  {
    printf("LIST: Out of memory\n");
    exit(1);
  }
  else
  {
    p->id = x;
    p->next = NULL;
    if (*l == NULL)
    {
      *l = p;
      return;
    }
    tl = NULL;
    tl = *l;
    while (tl != NULL)
    {
      if (tl->id == x)
      {
        break;
      }
      if (tl->next == NULL)
      {
        tl->next = p;
      }
      tl = tl->next;
    }
  }
  return;
} //end of InsertList
/***************************************************************************************************************************
Print the elements in LIST "l"          
***************************************************************************************************************************/
void PrintList(LIST *l)
{
  LIST *temp = NULL;

  temp = l;
  while (temp != NULL)
  {
    printf("%d ", temp->id);
    temp = temp->next;
  }
  return;
} //end of PrintList
/***************************************************************************************************************************
Free all elements in  LIST "l"  
****************************************************************************************************************************/
void FreeList(LIST **l)
{
  LIST *temp = NULL;

  if (*l == NULL)
  {
    return;
  }
  temp = (*l);
  while ((*l) != NULL)
  {
    temp = temp->next;
    free(*l);
    (*l) = temp;
  }
  (*l) = NULL;
  return;
} //end of FreeList
/*****************************************************************************************************************************
 Routine to read the Bench Markk(.isc files)
*****************************************************************************************************************************/
int ReadIsc(FILE *fisc, NODE *graph)
{
  char c, noty[Mlin], from[Mlin], str1[Mlin], str2[Mlin], name[Mlin], line[Mlin];
  int i, id, fid, Fin, fout, mid = 0;

  // intialize all nodes in graph structure
  for (i = 0; i < Mnod; i++)
  {
    InitializeCircuit(graph, i);
  }
  //skip the comment lines
  do
    fgets(line, Mlin, fisc);
  while (line[0] == '*');
  // read line by line
  while (!feof(fisc))
  {
    //initialize temporary strings
    bzero(noty, strlen(noty));
    bzero(from, strlen(from));
    bzero(str1, strlen(str1));
    bzero(str2, strlen(str2));
    bzero(name, strlen(name));
    //break line into data
    sscanf(line, "%d %s %s %s %s", &id, name, noty, str1, str2);
    //fill in the Typee
    strcpy(graph[id].Name, name);
    graph[id].Type = AssignTypee(noty);
    //fill in fanin and fanout numbers
    if (graph[id].Type != FROM)
    {
      fout = atoi(str1);
      Fin = atoi(str2);
    }
    else
    {
      Fin = fout = 1;
      strcpy(from, str1);
    }
    if (id > mid)
    {
      mid = id;
    }
    graph[id].Nfo = fout;
    graph[id].Nfi = Fin;
    if (fout == 0)
    {
      graph[id].Po = 1;
    }
    //create fanin and fanout lists
    switch (graph[id].Type)
    {
    case 0:
      printf("ReadIsc: Error in input file (Node %d)\n", id);
      exit(1);
    case INPT:
      break;
    case AND:
    case NAND:
    case OR:
    case NOR:
    case XOR:
    case XNOR:
    case BUFF:
    case NOT:
      for (i = 1; i <= Fin; i++)
      {
        fscanf(fisc, "%d", &fid);
        InsertList(&graph[id].Fin, fid);
        InsertList(&graph[fid].Fot, id);
      }
      fscanf(fisc, "\n");
      break;
    case FROM:
      for (i = mid; i > 0; i--)
      {
        if (graph[i].Type != 0)
        {
          if (strcmp(graph[i].Name, from) == 0)
          {
            fid = i;
            break;
          }
        }
      }
      InsertList(&graph[id].Fin, fid);
      InsertList(&graph[fid].Fot, id);
      break;
    } //end case
    bzero(line, strlen(line));
    fgets(line, Mlin, fisc);
  } // end while
  return mid;
} //end of ReadIsc
/****************************************************************************************************************************
Initialize the paricular memeber of graph structure
****************************************************************************************************************************/
void InitializeCircuit(NODE *graph, int num)
{
  bzero(graph[num].Name, Mnam);
  graph[num].Type = graph[num].Nfi = graph[num].Nfo = graph[num].Po = graph[num].Mark = 0;
  graph[num].Cval = graph[num].Fval = 3;
  graph[num].Fin = graph[num].Fot = NULL;
  return;
} //end of InitializeCircuit
/****************************************************************************************************************************
Convert (char *) Typee read to (int)     
****************************************************************************************************************************/
int AssignTypee(char *Type)
{
  if ((strcmp(Type, "inpt") == 0) || (strcmp(Type, "INPT") == 0))
    return 1;
  else if ((strcmp(Type, "and") == 0) || (strcmp(Type, "AND") == 0))
    return 2;
  else if ((strcmp(Type, "nand") == 0) || (strcmp(Type, "NAND") == 0))
    return 3;
  else if ((strcmp(Type, "or") == 0) || (strcmp(Type, "OR") == 0))
    return 4;
  else if ((strcmp(Type, "nor") == 0) || (strcmp(Type, "NOR") == 0))
    return 5;
  else if ((strcmp(Type, "xor") == 0) || (strcmp(Type, "XOR") == 0))
    return 6;
  else if ((strcmp(Type, "xnor") == 0) || (strcmp(Type, "XNOR") == 0))
    return 7;
  else if ((strcmp(Type, "buff") == 0) || (strcmp(Type, "BUFF") == 0))
    return 8;
  else if ((strcmp(Type, "not") == 0) || (strcmp(Type, "NOT") == 0))
    return 9;
  else if ((strcmp(Type, "from") == 0) || (strcmp(Type, "FROM") == 0))
    return 10;
  else
    return 0;
} //end of AssignTypee
/****************************************************************************************************************************
Print all members of graph structure(except Type=0) after reading the bench file
*****************************************************************************************************************************/
void PrintCircuit(NODE *graph, int Max)
{
  LIST *temp;
  int i;
  printf("\nID\tNAME\tTypeE\tPO\tIN#\tOUT#\tCVAL\tFVAL\tMarkK\tFANIN\tFANOUT\n");
  for (i = 0; i <= Max; i++)
  {
    if (graph[i].Type != 0)
    {
      printf("%d\t%s\t%d\t%d\t%d\t%d\t", i, graph[i].Name, graph[i].Type, graph[i].Po, graph[i].Nfi, graph[i].Nfo);
      printf("%d\t%d\t%d\t", graph[i].Cval, graph[i].Fval, graph[i].Mark);
      temp = NULL;
      temp = graph[i].Fin;
      if (temp != NULL)
      {
        PrintList(temp);
      }
      printf("\t");
      temp = NULL;
      temp = graph[i].Fot;
      if (temp != NULL)
      {
        PrintList(temp);
      }
      printf("\n");
    }
  }
  return;
} //end of PrintCircuit
/****************************************************************************************************************************
Free the memory of all member of graph structure
*****************************************************************************************************************************/
void ClearCircuit(NODE *graph, int i)
{
  int num = 0;
  for (num = 0; num < i; num++)
  {
    graph[num].Type = graph[num].Nfi = graph[num].Nfo = graph[num].Po = 0;
    graph[num].Mark = graph[num].Cval = graph[num].Fval = 0;
    if (graph[num].Type != 0)
    {
      bzero(graph[num].Name, Mnam);
      if (graph[num].Fin != NULL)
      {
        FreeList(&graph[num].Fin);
        graph[num].Fin = NULL;
      }
      if (graph[num].Fot != NULL)
      {
        FreeList(&graph[num].Fot);
        graph[num].Fot = NULL;
      }
    }
  }
  return;
} //end of ClearCircuit
/*****************************************************************************************************************************
 Routine to read the .vec files
*****************************************************************************************************************************/

/****************************************************************************************************************************/
/****** Copy the isc file to new isc file ********/

int countPI(NODE *graph, int Max)
{
  int i, PIcount = 0;

  for (i = 0; i <= Max; i++)
  {
    if (graph[i].Type != 0)
    {
      if (graph[i].Nfi == 0)
        PIcount += 1;
    }
  }
  return PIcount;
}

int countPO(NODE *graph, int Max)
{
  int i, POcount = 0;

  for (i = 0; i <= Max; i++)
  {
    if (graph[i].Type != 0)
    {
      if (graph[i].Nfo == 0)
        POcount += 1;
    }
  }
  return POcount;
}

const char *gateType(int Type)
{
  if (Type == 8)
    return "BUFF";
  else if (Type == 9)
    return "NOT";
  else if (Type == 2)
    return "AND";
  else if (Type == 3)
    return "NAND";
  else if (Type == 4)
    return "OR";
  else if (Type == 5)
    return "NOR";
  else if (Type == 6)
    return "XOR";
  else if (Type == 7)
    return "XNOR";
}

void append(struct Node **head_ref, char *pattern)
{

  struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
  struct Node *last = *head_ref; /* used in step 5*/
  strcpy(new_node->pattern, pattern);
  new_node->next = NULL;

  if (*head_ref == NULL)
  {
    *head_ref = new_node;
    return;
  }

  while (last->next != NULL)
    last = last->next;

  last->next = new_node;
  return;
}

const char *GetNth(struct Node *head, int index)
{
  struct Node *current = head;

  // the index of the
  // node we're currently
  // looking at
  int count = 1;
  while (current != NULL)
  {
    if (count == index)
      return (current->pattern);
    count++;
    current = current->next;
  }

  /* if we get to this line,  
       the caller was asking 
       for a non-existent element 
       so we assert fail */
  assert(0);
}

// This function prints contents of linked list starting from head
void printList(struct Node *node)
{
  while (node != NULL)
  {
    printf(" %s ", node->pattern);
    node = node->next;
  }
}

void createXORbranch(NODE *graph, LIST *faninList, FILE *fout, int newNodeType, int nodeToReplace, int Max)
{
  LIST *temp1;
  temp1 = faninList;
  int fanInArray[20];
  int j = 0, k = 0, count = 0, increase = 2, i = 0;

  while (temp1 != NULL)
  {
    fanInArray[j] = temp1->id;
    temp1 = temp1->next;
    j = j + 1;
  }
  count = j;
  printf("The count is %d\n", count);
  for (j = 0; j < count; j++)
  {

    if (graph[fanInArray[j]].Nfi == 0)
    {
      fanInArray[j] = fanInArray[j];
    }
    else if (graph[fanInArray[j]].Type == 10)
    {
      if (graph[graph[fanInArray[j]].Fin->id].Nfi == 0)
      {
        fanInArray[j] = graph[fanInArray[j]].Fin->id;
      }
      else
      {
        fanInArray[j] = graph[fanInArray[j]].Fin->id + Max;
      }
    }
    else
    {
      fanInArray[j] = fanInArray[j] + Max;
    }
  }

  i = fanInArray[k];
  while (k < count - 1)
  {
    j = fanInArray[k + 1];
    if (k == count - 2)
      fprintf(fout, "%d= %s(%d, %d)\n", nodeToReplace + Max, gateType(newNodeType), i, j);
    else
      fprintf(fout, "%d= %s(%d, %d)\n", 2 * Max + Max + increase, gateType(newNodeType), i, j);
    i = 2 * Max + Max + increase;
    increase++;
    k++;
  }
}

void copyFile(FILE *fisc, NODE *graph, FILE *fout, int Max, int Npo, int nodeToReplace, int newNodeType)
{
  fout = fopen("c17_erroneous.bench", "w");
  int i = 0, count = 0, value = 0, POnumber = 0;
  LIST *temp;
  char inputs[1000] = "";
  char add[10];
  char inputs_2[1000] = "";
  char add_2[10];
  char xor_outputs[1000] = "";
  char add_3[10];
  fprintf(fout, "#BenchFile\n");
  for (i = 0; i <= Max; i++)
  {
    if (graph[i].Type != 0)
    {
      if (graph[i].Nfi == 0)
        fprintf(fout, "INPUT(%d)\n", i);
    }
  }
  for (i = 0; i <= Max; i++)
  {
    if (graph[i].Type == 8 || graph[i].Type == 9 || graph[i].Type == 2 || graph[i].Type == 3 || graph[i].Type == 4 || graph[i].Type == 5 || graph[i].Type == 6 || graph[i].Type == 7)
    {
      temp = NULL;
      bzero(inputs, 1000);
      bzero(inputs_2, 1000);
      temp = graph[i].Fin;
      while (temp != NULL)
      {
        bzero(add, 10);
        bzero(add_2, 10);
        if (graph[temp->id].Type == 10)
          sprintf(add, "%d", graph[temp->id].Fin->id);
        else
          sprintf(add, "%d", temp->id);

        if (graph[temp->id].Nfi == 0)
          sprintf(add_2, "%d", temp->id);
        else if (graph[temp->id].Type == 10)
        {
          if (graph[graph[temp->id].Fin->id].Nfi == 0)
            sprintf(add_2, "%d", graph[temp->id].Fin->id); //added later
          else
            sprintf(add_2, "%d", graph[temp->id].Fin->id + Max);
        }
        else
          sprintf(add_2, "%d", temp->id + Max);

        strcat(inputs, add);
        strcat(inputs_2, add_2);
        count += 1;

        if (temp->next != NULL)
        {
          strcat(inputs, ",");
          strcat(inputs_2, ",");
        }
        temp = temp->next;
      }

      if (i == nodeToReplace && graph[nodeToReplace].Nfi > 2 && (newNodeType == 6 || newNodeType == 7))
      {
        fprintf(fout, "%d = %s(%s)\n", i, gateType(graph[i].Type), inputs);
        createXORbranch(graph, graph[nodeToReplace].Fin, fout, newNodeType, nodeToReplace, Max);
      }
      else
      {
        fprintf(fout, "%d = %s(%s)\n", i, gateType(graph[i].Type), inputs);
        if (i != nodeToReplace)
          fprintf(fout, "%d = %s(%s)\n", i + Max, gateType(graph[i].Type), inputs_2);
        else
          fprintf(fout, "%d = %s(%s)\n", i + Max, gateType(newNodeType), inputs_2);
      }
    }

    if (graph[i].Type != 0)
    {
      if (graph[i].Nfo == 0)
      {
        fprintf(fout, "%d= XOR(%d,%d)\n", 2 * Max + i, i, i + Max);
        bzero(add_3, 10);
        value = 2 * Max + i;
        sprintf(add_3, "%d", value);
        strcat(xor_outputs, add_3);
        POnumber += 1;
        if (POnumber != Npo)
          strcat(xor_outputs, ",");
      }
    }
  }
  fprintf(fout, "OUTPUT(%d)\n", 2 * Max + Max + 1);
  fprintf(fout, "%d = OR(%s)\n", 2 * Max + Max + 1, xor_outputs);
  fclose(fout);
}

struct Node *readTestFile(FILE *ftest, FILE *patternFile, int Max, int *NtestPatterns)
{
  struct Node *head = NULL;

  char fline[Mlin], scan1[50], inputPattern[1000], scan3[50], *i, skipUpto[100], checkString[100];
  bzero(scan1, 50);
  bzero(inputPattern, 1000);
  bzero(scan3, 50);
  bzero(fline, Mlin);

  sprintf(skipUpto, "%d /%d", 2 * Max + Max + 1, 0);
  while (strncmp(fline, skipUpto, strlen(skipUpto)) != 0)
  {
    fgets(fline, Mlin, ftest);
  }
  while (!feof(ftest))
  {
    fgets(fline, Mlin, ftest);

    sscanf(fline, "%s %s %s", scan1, inputPattern, scan3);
    sprintf(checkString, "/0");
    if (strncmp(inputPattern, checkString, strlen(checkString)) == 0)
      break;

    i = inputPattern;

    append(&head, inputPattern);
    fprintf(patternFile, "%s\n", inputPattern);
    *NtestPatterns = *NtestPatterns + 1;
  }

  return head;
}

void selectRandomPattern(struct Node *patternHead, FILE *patternFile, FILE *testSet1, int NtestPattern, int Npi, int NpatternsToSelect)
{

  char line[Mlin], testPattern[1000];
  const char *i;
  bzero(testPattern, 1000);
  srand(time(0));
  int targetLine = 0;
  
  printf("the number of test pattern is %d\n", NtestPattern);
  

  while (NpatternsToSelect != 0)
  {

    targetLine = (rand() % NtestPattern + 1);
    i = GetNth(patternHead, targetLine);
    while (*i != '\0')
    {
      if (*i == 'x')
      {
        const int randomBit = rand() % 2;
        fprintf(testSet1, "%d", randomBit);
      }
      else
        fprintf(testSet1, "%c", *i);
      i++;
    }
    fprintf(testSet1, "\n");
    NpatternsToSelect--;
  }
}

void mainPart(int nodeToReplace, int Max, int NtestPatterns, int newNodeType, int Npi, int Npo, NODE *graph, FILE *fout, FILE *fisc, FILE *patternFile, FILE *ftest, FILE *testSet1, int NpatternsToSelect)
{

  for (nodeToReplace = 0; nodeToReplace <= Max; nodeToReplace++)
  {
    if (graph[nodeToReplace].Type != 0)
    {
      if (graph[nodeToReplace].Type == 8)
      {
        newNodeType = 9;

        copyFile(fisc, graph, fout, Max, Npo, nodeToReplace, newNodeType);
        run(patternFile, ftest, testSet1, Npi, Max, NtestPatterns, NpatternsToSelect);
      }
      else if (graph[nodeToReplace].Type == 9)
      {
        newNodeType = 8;
        copyFile(fisc, graph, fout, Max, Npo, nodeToReplace, newNodeType);
        run(patternFile, ftest, testSet1, Npi, Max, NtestPatterns, NpatternsToSelect);
      }
      else if (graph[nodeToReplace].Type == 2)
      {
        for (newNodeType = 3; newNodeType <= 7; newNodeType++)
        {
          copyFile(fisc, graph, fout, Max, Npo, nodeToReplace, newNodeType);
          run(patternFile, ftest, testSet1, Npi, Max, NtestPatterns, NpatternsToSelect);
        }
      }
      else if (graph[nodeToReplace].Type == 7)
      {
        for (newNodeType = 2; newNodeType <= 6; newNodeType++)
        {
          copyFile(fisc, graph, fout, Max, Npo, nodeToReplace, newNodeType);
          run(patternFile, ftest, testSet1, Npi, Max, NtestPatterns, NpatternsToSelect);
        }
      }
      else if (graph[nodeToReplace].Type == 3)
      {
        newNodeType = 2;
        copyFile(fisc, graph, fout, Max, Npo, nodeToReplace, newNodeType);
        run(patternFile, ftest, testSet1, Npi, Max, NtestPatterns, NpatternsToSelect);
        for (newNodeType = 4; newNodeType <= 7; newNodeType++)
        {
          copyFile(fisc, graph, fout, Max, Npo, nodeToReplace, newNodeType);
          run(patternFile, ftest, testSet1, Npi, Max, NtestPatterns, NpatternsToSelect);
        }
      }
      else if (graph[nodeToReplace].Type == 4)
      {
        for (newNodeType = 2; newNodeType <= 3; newNodeType++)
        {
          copyFile(fisc, graph, fout, Max, Npo, nodeToReplace, newNodeType);
          run(patternFile, ftest, testSet1, Npi, Max, NtestPatterns, NpatternsToSelect);
        }
        for (newNodeType = 5; newNodeType <= 7; newNodeType++)
        {
          copyFile(fisc, graph, fout, Max, Npo, nodeToReplace, newNodeType);
          run(patternFile, ftest, testSet1, Npi, Max, NtestPatterns, NpatternsToSelect);
        }
      }
      else if (graph[nodeToReplace].Type == 5)
      {
        for (newNodeType = 2; newNodeType <= 4; newNodeType++)
        {
          copyFile(fisc, graph, fout, Max, Npo, nodeToReplace, newNodeType);
          run(patternFile, ftest, testSet1, Npi, Max, NtestPatterns, NpatternsToSelect);
        }
        for (newNodeType = 6; newNodeType <= 7; newNodeType++)
        {
          copyFile(fisc, graph, fout, Max, Npo, nodeToReplace, newNodeType);
          run(patternFile, ftest, testSet1, Npi, Max, NtestPatterns, NpatternsToSelect);
        }
      }
      else if (graph[nodeToReplace].Type == 6)
      {
        for (newNodeType = 2; newNodeType <= 5; newNodeType++)
        {
          copyFile(fisc, graph, fout, Max, Npo, nodeToReplace, newNodeType);
          run(patternFile, ftest, testSet1, Npi, Max, NtestPatterns, NpatternsToSelect);
        }
        newNodeType = 7;
        copyFile(fisc, graph, fout, Max, Npo, nodeToReplace, newNodeType);
        run(patternFile, ftest, testSet1, Npi, Max, NtestPatterns, NpatternsToSelect);
      }
    }
  }
}

void run(FILE *patternFile, FILE *ftest, FILE *testSet1, int Npi, int Max, int NtestPatterns, int NpatternsToSelect)
{
  struct Node *head;

  system("/home/aashish/Atalanta/atalanta -D 50 -f /home/aashish/Downloads/Cause-Effect-Diagnosis/faultFile.flt /home/aashish/Downloads/Cause-Effect-Diagnosis/c17_erroneous.bench");
  // system("/opt/net/apps/atalanta/atalanta -D 100 -f  /home/grad/siu856300090/Downloads/ECE524/Project1/faultFile.flt /home/grad/siu856300090/Downloads/ECE524/Project1/c17_erroneous.bench");
  patternFile = fopen("TestPatterns.test", "w");
  ftest = fopen("c17_erroneous.test", "r");
  head = readTestFile(ftest, patternFile, Max, &NtestPatterns);
  fclose(ftest);
  fclose(patternFile);
  patternFile = fopen("TestPatterns.test", "r");

  if (NtestPatterns != 0)
    selectRandomPattern(head, patternFile, testSet1, NtestPatterns, Npi, NpatternsToSelect);
  fclose(patternFile);
}
