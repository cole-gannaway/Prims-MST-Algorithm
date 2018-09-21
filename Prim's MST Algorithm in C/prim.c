/*
 *  Written by Cole Gannaway
 *
 *  Executes with command line arguments. This program can read in
 *  any random graph from a file. It will then run prims algorithm
 *  on the graph and return the resulting Minimum Spanning Tree. 
 *
 *  The graph file must follow the following format using 
 *  the INTEGER class for the numbers: 2 numbers describing 
 *  the value of the vertices with the 3rd number being the weight 
 *  of the edge between the two vertices.
 *  If a 3rd number is not given, it will default the weight as 0.
 *
 *  Ex: 1 3 4 ; 1 2 ;
 *  The above file would read in as vertices: 1,3,2 with weights of
 *  4 and 0 respectively.
 *
 *  The program reads the file as an undirected graph and executes on
 *  positive INTEGERS only.
 *
 *  Reading in functions such as process options etc. was created by 
 *  John C. Lusth, professor at the University of Alabama. 
 *
 *  Use the provided Makefile for compiling and execution purposes.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "integer.h"
#include "real.h"
#include "string.h"
#include "scanner.h"
#include "avl.h"
#include "queue.h"
#include "binomial.h"
#include "vertex.h"
#include "edge.h"

/* options */
int g = 0;    /* option -g*/
int r = 0;    /* option -r default */
int v = 0;    /* option -v*/
// globabl variable

static int processOptions(int,int,char **);
void Fatal(char *,...);

// returns true if string is empty and shouldn't be inserted
int isSemiColon (char * string){
    if (string == 0) return 0;
    if (strcmp(string,";") == 0) return 1;
    return 0;
}

char * readFunct (FILE * fpIN){
    char * string;
    if (stringPending(fpIN)){
        string = readString(fpIN);
    }
    else{
        string = readToken(fpIN);
    }
    return string;
}

static void
update(void *v,void *n) //v is a vertex, n is a binomial heap node
{
    VERTEX * p = v;
    setVERTEXowner(p,n);
}
static int
compareVERTEXprint (void * x, void * y){
    VERTEX * a = x;
    VERTEX * b = y;
    int aNum = getVERTEXnumber(a);
    int bNum = getVERTEXnumber(b);
    return (aNum-bNum);
}

void PrimFunct(BINOMIAL * Q,VERTEX * sv){
    if (sv == 0) return;
    // decrease original key
    setVERTEXkey(sv,0);
    setVERTEXpred(sv,0);
    decreaseKeyBINOMIAL(Q,getVERTEXowner(sv),sv);
    
    VERTEX * u = 0;
    VERTEX * v = 0;
    DLL * weightList = 0;
    DLL * neighborList = 0;
    int weight = 0;
    while (sizeBINOMIAL(Q) != 0){
        u = extractBINOMIAL(Q);
        //printf("U->"); displayVERTEXdebug(u,stdout); printf("\n");
        setVERTEXflag(u,1);
        neighborList = getVERTEXneighbors(u);
        weightList = getVERTEXweights(u);
        firstDLL(neighborList);
        firstDLL(weightList);
        while (moreDLL(neighborList) != 0){
            //printf("running through the neighbors\n");
            v = currentDLL(neighborList);
            weight = getINTEGER((INTEGER *)currentDLL(weightList));
            if (getVERTEXflag(v) != 1 && ((getVERTEXkey(v) == -1) || (getVERTEXkey(v) > weight))){
                setVERTEXpred(v,u);
                setVERTEXkey(v,weight);
                decreaseKeyBINOMIAL(Q,getVERTEXowner(v),v);
            }
            nextDLL(neighborList);
            nextDLL(weightList);
        }
    }
}

void PrintFunction (VERTEX * sv){
    if (sv == 0){
        printf("EMPTY\n");
    }
    //Create an empty QUEUE//
    QUEUE * vertexList = newQUEUE(0,0);
    BINOMIAL * printHEAP = newBINOMIAL(displayVERTEX,compareVERTEXprint,update,freeVERTEX);
    //Enqueue the source vertex and initialize the height//
    enqueue(vertexList,sv);
    
    VERTEX * ptr = 0;
    VERTEX * v = 0;
    int weight = 0;
    DLL * neighborList = 0;
    DLL * weightList = 0;
    int nodesAtLevel = sizeQUEUE(vertexList);
    int levelNum = 0;
    int totalWeight = 0;
    while (nodesAtLevel > 0){
        for (int a = 0; a < nodesAtLevel; a++){
            ptr = dequeue(vertexList);
            // insert the dequeued vertex
            setVERTEXowner(ptr,insertBINOMIAL(printHEAP,ptr));
            // walk through the list and add only its neighbors that point back to it
            neighborList = getVERTEXneighbors(ptr);
            weightList = getVERTEXweights(ptr);
            firstDLL(neighborList);
            firstDLL(weightList);
            while (moreDLL(neighborList) != 0){
                v = currentDLL(neighborList);
                weight = getINTEGER(currentDLL(weightList));
                // enqueue for the next stuff
                if (getVERTEXpred(v) == ptr){
                    setVERTEXkey(v,weight);
                    enqueue(vertexList,v);
                }
                nextDLL(neighborList);
                nextDLL(weightList);
            }
        }
        // print the previous heap in order
        if (sizeBINOMIAL(printHEAP) != 0) printf("%d: ",levelNum);
        while (sizeBINOMIAL(printHEAP) != 0){
            ptr = extractBINOMIAL(printHEAP);
            displayVERTEX(ptr,stdout);
            if (getVERTEXpred(ptr) != 0 && getVERTEXpred(ptr) != ptr){
                printf("(");
                displayVERTEX(getVERTEXpred(ptr),stdout);
                printf(")");
                printf("%d",getVERTEXkey(ptr));
                totalWeight = totalWeight + getVERTEXkey(ptr);
            }
            if (sizeBINOMIAL(printHEAP) != 0) printf(" ");
        }
        printf("\n");
        levelNum++;
        nodesAtLevel = sizeQUEUE(vertexList);
    }
    printf("weight: %d\n",totalWeight);
}
int
main(int argc,char **argv){
    
    if (argc == 1) Fatal("%d arguments!\n",argc-1);
    
    int argIndex = 1;
    argIndex = processOptions(argIndex,argc,argv);
    
    if (v == 1){
        return 0;
    }
    
    // Initialize Variables
    char * file1 = 0;
    char * fileString = 0;
    int count = 0;
    EDGE * edge = 0;
    // Source vertex
    VERTEX * sourceVertex = 0;
    int toggle = 0;
    VERTEX * v1 = 0;
    VERTEX * v2 = 0;
    int weight = 1;
    int length = 2;
    int * vertices = malloc(sizeof(int) * length);
    vertices[0] = 0;
    vertices[1] = 0;
    /* No corpus or command files */
    if (argIndex == argc) {
        return 0;
    }
    file1 = argv[argIndex];
    FILE * fpIN1 = 0;
    BINOMIAL * b = newBINOMIAL(displayVERTEXdebug,compareVERTEX,update,freeVERTEX);
    // AVL to determine if duplicate edges and vertices
    AVL * edgesTree = newAVL(displayEDGE,compareEDGE,freeEDGE);
    AVL * verticesTree = newAVL(displayVERTEXdebug,compareVERTEX,freeVERTEX);    
    fpIN1 = fopen(file1,"r");
    fileString = readFunct(fpIN1);
    while (!feof(fpIN1)){
        // Read EACH EDGE DESCRIPTION //
        while (!(isSemiColon(fileString)) && count < 3){
            // adding the weight
            if (count == 2){
                weight = atoi(fileString);
            }
            else{
                vertices[count] = atoi(fileString);
            }
            count++;
            fileString = readFunct(fpIN1);
        }
        // Create VERTEX and EDGE objects ///
        if (count >= 2){
            v1 = newVERTEX(vertices[0]);
            v2 = newVERTEX(vertices[1]);
            // order the vertices array to check for DUPLICATES
            if (vertices[0] > vertices[1]){
                int temp = vertices[0];
                vertices[0] = vertices[1];
                vertices[1] = temp;
            }
            edge = newEDGE(vertices[0],vertices[1],weight);
            // Check for loop or Duplicates //
            if (findAVL(edgesTree,edge) == 0) {
                insertAVL(edgesTree,edge);
                VERTEX * foundV1 = 0;
                VERTEX * foundV2 = 0;
                foundV1 = findAVL(verticesTree,v1);
                // new vertex
                if (foundV1 == 0){
                    setVERTEXowner(v1,insertBINOMIAL(b,v1));
                    insertAVL(verticesTree,v1);
                    foundV1 = v1;
                    if (toggle != 1){
                        sourceVertex = v1;
                        toggle = 1;
                    }
                }
                // doesn't add any more if it is a loop
                if (vertices[0] != vertices[1]){
                    foundV2 = findAVL(verticesTree,v2);
                    if (foundV2 == 0){
                        setVERTEXowner(v2,insertBINOMIAL(b,v2));
                        insertAVL(verticesTree,v2);
                        foundV2 = v2;
                    }
                    insertVERTEXneighbor(foundV1,foundV2);
                    insertVERTEXweight(foundV1,weight);
                    insertVERTEXneighbor(foundV2,foundV1);
                    insertVERTEXweight(foundV2,weight);
                }
            }
            else {
                freeVERTEX(v1);
                freeVERTEX(v2);
                freeEDGE(edge);
            }
        }
        // skip this if at least two vertices were not read //
        else{
        }
        // weight is defaulted 1
        weight = 1;
        count = 0;
        fileString = readFunct(fpIN1);
    }
    fclose(fpIN1);
    
    // display EMPTY if empty graph
    if (sizeBINOMIAL(b) == 0){
        printf("EMPTY\n");
        return 0;
    }

    // NOW RUN PRIM ALGORITHIM ///
    
    setVERTEXkey(sourceVertex,0);
    decreaseKeyBINOMIAL(b,getVERTEXowner(sourceVertex),sourceVertex);
    assert(b != 0);
    PrimFunct(b,sourceVertex);
    PrintFunction(sourceVertex);
    return 0;
}

void
Fatal(char *fmt, ...)
{
    va_list ap;
    
    fprintf(stderr,"An error occured: ");
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    
    exit(-1);
}

static int processOptions(int argIndex,int argc, char **argv){
    // looking at the - things
    while (argIndex < argc && *argv[argIndex] == '-'){
        // looks at the first letter after the -
        switch (argv[argIndex][1]){
            case 'v':
                printf("This program is reads in graphs and runs algorithims with binomial heaps as a priority queue\n");
                v = 1;
            case 'r':
                r = 1;
                g = 0;
                break;
            case 'g':
                r = 0;
                g = 1;
                break;
            default:
                Fatal("option %s not understood\n",argv[argIndex]);
        }
        argIndex++;
    }
    // if it is not a -thing
    // we want to return the argIndex
    return argIndex;
}
