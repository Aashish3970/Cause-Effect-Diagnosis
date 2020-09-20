#include "graph.h"
/***************************************************************************************************************************
 Main Function(Serial and Parallel Fault Simulation)
****************************************************************************************************************************/

void main(int argc, char **argv)
{
    FILE *fisc, *fvec, *ffau, *fres, *fout, *ftest, *patternFile, *testSet1; //file pointers used for .isc file, .vec file, .faults file and resultfile
    int Max, Opt, Npi, Npo, Total, Tfs;                                      //maxnode id,option,tot no of PIs,tot no of Pos,Tot no of input patterns& faults in.vec in.faults

    NODE graph[Mnod];
    NODE graph2[Mnod]; //structure used to store the ckt information in .isc file
    //PATTERN vector[Mpt];                      //structure used to store the input vectors information in .vec file

    //FAULT stuck[Mft];                      //structure used to store the faults information in .faults file
    int a, b, c, d, nLines, numberOfPatterns = 0; //random variables
    int nodeToReplace = 0, newNodeType = 0, NtestPatterns = 0;

    //Read the .isc file and store the information in graph structure
    fisc = fopen(argv[1], "r"); //file pointer to open .isc file
    Max = 0;
    Max = ReadIsc(fisc, graph); //read .isc file and return index of last node in graph formed
    fclose(fisc);               //close file pointer for .isc file
    PrintCircuit(graph, Max);   //print all members of graph structure
    Npo = countPO(graph, Max);
    Npi = countPI(graph, Max);
    // fout= fopen("c17_erroneous.bench","w");

    ffau = fopen("faultFile.flt", "w");
    fprintf(ffau, "%d /0\n", 2 * Max + Max + 1);
    fclose(ffau);
    printf("max is %d", Max);
    testSet1 = fopen("testSet1.test", "w");
    copyFile(fisc, graph, fout, Max,Npo,815, XNOR);
    // fclose(fout);

    // system("/opt/net/apps/atalanta/atalanta -A -f /home/grad/siu856300090/Downloads/ECE524/Project1/faultFile.flt /home/grad/siu856300090/Downloads/ECE524/Project1/c17_erroneous.bench");
    // patternFile= fopen("TestPatterns.test","w");

    // ftest= fopen("c17_erroneous.test","r");
    // NtestPatterns=readTestFile(ftest, patternFile, Max);
    // fclose(ftest);
    // fclose(patternFile);

    // patternFile= fopen("TestPatterns.test","r");

    // testSet1= fopen("testSet1.test","w");
    // selectRandomPattern(patternFile,testSet1,NtestPatterns, Npi);

    

    // for (nodeToReplace = 0; nodeToReplace <= Max; nodeToReplace++)
    // {
    //     if (graph[nodeToReplace].Type != 0 )
    //     {
    //         if (graph[nodeToReplace].Type == 8)
    //         {
    //             newNodeType = 9;

    //             copyFile(fisc, graph, fout, Max, Npo, nodeToReplace, newNodeType);
    //             run(patternFile, ftest, testSet1, Npi, Max, NtestPatterns);
    //         }
    //         else if (graph[nodeToReplace].Type == 9)
    //         {
    //             newNodeType = 8;
    //             copyFile(fisc, graph, fout, Max, Npo, nodeToReplace, newNodeType);
    //             run(patternFile, ftest, testSet1, Npi, Max, NtestPatterns);
    //         }
    //         else if (graph[nodeToReplace].Type == 2)
    //         {
    //             for (newNodeType = 3; newNodeType <= 7; newNodeType++)
    //             {
    //                 copyFile(fisc, graph, fout, Max, Npo, nodeToReplace, newNodeType);
    //                 run(patternFile, ftest, testSet1, Npi, Max, NtestPatterns);
    //             }
    //         }
    //         else if (graph[nodeToReplace].Type == 7)
    //         {
    //             for (newNodeType = 2; newNodeType <= 6; newNodeType++)
    //             {
    //                 copyFile(fisc, graph, fout, Max, Npo, nodeToReplace, newNodeType);
    //                 run(patternFile, ftest, testSet1, Npi, Max, NtestPatterns);
    //             }
    //         }
    //         else if (graph[nodeToReplace].Type == 3)
    //         {
    //             newNodeType = 2;
    //             copyFile(fisc, graph, fout, Max, Npo, nodeToReplace, newNodeType);
    //             run(patternFile, ftest, testSet1, Npi, Max, NtestPatterns);
    //             for (newNodeType = 4; newNodeType <= 7; newNodeType++)
    //             {
    //                 copyFile(fisc, graph, fout, Max, Npo, nodeToReplace, newNodeType);
    //                 run(patternFile, ftest, testSet1, Npi, Max, NtestPatterns);
    //             }
    //         }
    //         else if (graph[nodeToReplace].Type == 4)
    //         {
    //             for (newNodeType = 2; newNodeType <= 3; newNodeType++)
    //             {
    //                 copyFile(fisc, graph, fout, Max, Npo, nodeToReplace, newNodeType);
    //                 run(patternFile, ftest, testSet1, Npi, Max, NtestPatterns);
    //             }
    //             for (newNodeType = 5; newNodeType <= 7; newNodeType++)
    //             {
    //                 copyFile(fisc, graph, fout, Max, Npo, nodeToReplace, newNodeType);
    //                 run(patternFile, ftest, testSet1, Npi, Max, NtestPatterns);
    //             }
    //         }
    //         else if (graph[nodeToReplace].Type == 5)
    //         {
    //             for (newNodeType = 2; newNodeType <= 4; newNodeType++)
    //             {
    //                 copyFile(fisc, graph, fout, Max, Npo, nodeToReplace, newNodeType);
    //                 run(patternFile, ftest, testSet1, Npi, Max, NtestPatterns);
    //             }
    //             for (newNodeType = 6; newNodeType <= 7; newNodeType++)
    //             {
    //                 copyFile(fisc, graph, fout, Max, Npo, nodeToReplace, newNodeType);
    //                 run(patternFile, ftest, testSet1, Npi, Max, NtestPatterns);
    //             }
    //         }
    //         else if (graph[nodeToReplace].Type == 6)
    //         {
    //             for (newNodeType = 2; newNodeType <= 5; newNodeType++)
    //             {
    //                 copyFile(fisc, graph, fout, Max, Npo, nodeToReplace, newNodeType);
    //                 run(patternFile, ftest, testSet1, Npi, Max, NtestPatterns);
    //             }
    //             newNodeType = 7;
    //             copyFile(fisc, graph, fout, Max, Npo, nodeToReplace, newNodeType);
    //             run(patternFile, ftest, testSet1, Npi, Max, NtestPatterns);
    //         }
            
    //     } 
    // }

    // fclose(testSet1);

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
