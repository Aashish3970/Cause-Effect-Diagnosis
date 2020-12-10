#include "graph.h"
/***************************************************************************************************************************
 Main Function(Serial and Parallel Fault Simulation)
****************************************************************************************************************************/

void main(int argc, char **argv)
{
  FILE *fisc, *Pat, *fvec, *ffau, *fres, *fout, *ftest, *patternFile, *testSet1, *fwrite, *resolutionOut; //file pointers used for .isc file, .vec file, .faults file and resultfile
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
  int totalTests = 0;
  //*************************************Run ALL ***********************************************
  // if (atoi(argv[2]) == 0)
  // {

    fout = fopen("c17_erroneous.bench", "w");

    ffau = fopen("faultFile.flt", "w");
    fprintf(ffau, "%d /0\n", 2 * Max + Max + 1);
    fclose(ffau);
    printf("max is %d", Max);
    testSet1 = fopen(argv[2], "w");
    
    // for(NpatternsToSelect=1; NpatternsToSelect<=4; NpatternsToSelect++)
    // {
    NpatternsToSelect = 1;
    for (iterations = 0; iterations < 3; iterations++)
    {
      totalTests=0;
      // fprintf(testSet1,"Test Set %d \n",iterations+1);
      mainPart(nodeToReplace, Max, NtestPatterns, newNodeType, Npi, Npo, graph, fout, fisc, patternFile, ftest, testSet1, NpatternsToSelect, &totalTests);
      printf("totaalPatterns= %d ", totalTests);
      
    }

    // }
    fclose(testSet1);
  // }
  /****************************************************RUn ALL ***************************************************************/

  /******************************************Simulation BEgin****************************************************************/
  // if (atoi(argv[2]) == 1)
  // {
    printf("********Simulation started **********\n");
    Pat = fopen(argv[2], "r");
    totalPatterns = ReadVec(Pat, p_vact);
    totalFaults = readTotalFaults(graph, Max);
    struct faultList *Flist;
    
    int k;


    fclose(Pat);
    
    fwrite = fopen("outputs.txt", "w");

    int lineStart = 0;
    int lineEnd = 0;

    int testSet = 1;
    resolutionOut= fopen(argv[3],"w");
    // while (lineEnd < 10 * totalFaults * 2)
    while (lineEnd < totalTests * 3)
    {
      struct faultList *faultsToBeRemoved = (struct faultList *)malloc(sizeof(struct faultList));
      struct faultList *completedFaults = (struct faultList *)malloc(sizeof(struct faultList));
      Flist = (struct faultList *)malloc(range(lineStart, totalTests) * Npo * sizeof(struct faultList));
      for (k = 0; k < range(lineEnd, totalTests) * Npo; k++)
      {
        InitializeFaultList(Flist, k);
      }

      InitializeFaultList(faultsToBeRemoved, 0);
      InitializeFaultList(completedFaults, 0);

      lineEnd = lineEnd + range(lineEnd, totalTests);
    
      runSimulate(Max, totalPatterns, graph, p_vact, fwrite, Flist,faultsToBeRemoved, completedFaults, lineStart, lineEnd, testSet, Npo * totalTests, totalFaults, resolutionOut);
      // printf("Start %d End %d\n", lineStart,lineEnd);
      lineStart = lineEnd;
      testSet++;
      free(faultsToBeRemoved);
      free(Flist);
      
    }
    fclose(resolutionOut);
  // }
  /**************************************************************End of Simulate Part*******************/ ///
  ClearCircuit(graph, Mnod); //clear memeory for all members of graph
  //for(a=0;a<Total;a++){ bzero(vector[a].piv,Mpi); }                //clear memeory for all members of vector
  return;
} //end of main
  /****************************************************************************************************************************/