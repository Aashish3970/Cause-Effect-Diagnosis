#include "graph.h"
/***************************************************************************************************************************
 Main Function(Serial and Parallel Fault Simulation)
****************************************************************************************************************************/

void main(int argc, char **argv)
{
  FILE *fisc, *Pat, *fvec, *ffau, *fres, *fout, *ftest, *patternFile, *testSet1, *fwrite; //file pointers used for .isc file, .vec file, .faults file and resultfile
  int Max, Opt, Npi, Npo, Total, Tfs, totalPatterns = 0, erroneousSimulation;             //maxnode id,option,tot no of PIs,tot no of Pos,Tot no of input patterns& faults in.vec in.faults
  int iterations = 0;

  static PATTERN p_vact[Mpt];
  NODE graph[Mnod];
  srand(time(NULL));
  //PATTERN vector[Mpt];                      //structure used to store the input vectors information in .vec file

  //FAULT stuck[Mft];                      //structure used to store the faults information in .faults file
  int a, b, c, d, nLines, numberOfPatterns = 0; //random variables
  int nodeToReplace = 0, newNodeType = 0, NtestPatterns = 0;
  int NpatternsToSelect;
  int totalFaults;
  //Read the .isc file and store the information in graph structure

  fisc = fopen(argv[1], "r"); //file pointer to open .isc file
  Max = 0;
  Max = ReadIsc(fisc, graph); //read .isc file and return index of last node in graph formed
  fclose(fisc);               //close file pointer for .isc file
  PrintCircuit(graph, Max);   //print all members of graph structure
  Npo = countPO(graph, Max);
  Npi = countPI(graph, Max);

  //*************************************Run ALL ***********************************************
  // if (atoi(argv[2])==0)
  // {
      // fout= fopen("c17_erroneous.bench","w");

      // ffau = fopen("faultFile.flt", "w");
      // fprintf(ffau, "%d /0\n", 2 * Max + Max + 1);
      // fclose(ffau);
      // printf("max is %d", Max);
      // testSet1 = fopen("testSet2.test", "w");

      // for(NpatternsToSelect=1; NpatternsToSelect<=4; NpatternsToSelect++)
      // {
      //   for(iterations=0; iterations<500; iterations++) mainPart(nodeToReplace, Max, NtestPatterns, newNodeType, Npi, Npo, graph, fout, fisc, patternFile, ftest, testSet1,NpatternsToSelect);

      // }
      // fclose(testSet1);
  // }
  /****************************************************RUn ALL ***************************************************************/

  /******************************************Simulation BEgin****************************************************************/
   if (atoi(argv[2])==1)
    {
  printf("********Simulation started **********\n");
  Pat = fopen("testSet2.test", "r");
  totalPatterns = ReadVec(Pat, p_vact);
  totalFaults= readTotalFaults(graph,Max);
  // totalFaults=30;
  struct faultList *Flist;

  int k;

  // faultList Flist[60];
  fclose(Pat);

  fwrite = fopen("outputs.txt", "w");
  

  int lineStart = 0;
  int lineEnd = 0;
  
  int testSet = 1;

  while (lineEnd < 10 * totalFaults * 500)
  {
    struct faultList *faultsToBeRemoved = (struct faultList *)malloc(sizeof(struct faultList));
    struct faultList *completedFaults = (struct faultList *)malloc(sizeof(struct faultList));
    Flist = (struct faultList *)malloc(range(lineStart, totalFaults) * Npo * sizeof(struct faultList));
    
    for (k = 0; k < range(lineEnd, totalFaults)* Npo; k++)
    {
      InitializeFaultList(Flist, k);
    }
    
    InitializeFaultList(faultsToBeRemoved,0);
    InitializeFaultList(completedFaults,0);
    
    lineEnd = lineEnd + range(lineEnd, totalFaults);

    
    runSimulate(Max, totalPatterns, graph, p_vact, fwrite, Flist, faultsToBeRemoved,completedFaults, lineStart, lineEnd, testSet, Npo*range(lineStart,totalFaults),totalFaults);
    // printf("Start %d End %d\n", lineStart,lineEnd);
    lineStart = lineEnd;
    testSet++;
    free(faultsToBeRemoved);
    free(Flist);
  }
  
  /**************************************************************End of Simulate Part*******************/ ///

  // fclose(fout);

  // PrintCircuit(graph2,Max);
  //Read the .vec file and store the information in  vector structure

  //Opt=0;
  //Opt=atoi(argv[3]);                          //getting the option from terminal for xval
  //fres=fopen(argv[2],"w");                           //file pointer to open .out file for printing results

  //Perform Logic Simulation for each Input vector and print the Pos .val in output file

  //fclose(fres);                                                  //close file pointer for .out file

  ClearCircuit(graph, Mnod); //clear memeory for all members of graph
  //for(a=0;a<Total;a++){ bzero(vector[a].piv,Mpi); }                //clear memeory for all members of vector
  return;
} //end of main
  /****************************************************************************************************************************/