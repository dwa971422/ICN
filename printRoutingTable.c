#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define INFINITE 1000000
#define FILE_NAME_LENGTH 20
#define OPERATION_NAME_LENGTH 5
#define REMOVE_ROUTER_NAME_LENGTH 10

void releaseAllocatedResource(int**, int);
int minimalDistanceIndex(int*, bool*, int);
void printNextRouter(int*, int*, int, int, int*, FILE*);
void printRoutingTable(int*, int*, int, int, int, int, int*, FILE*);
void DijkstraAlgorithm(int**, int, int, int, int, int*, FILE*);
void removeCertainRouter(int**, int, int);
int** NewMatrixAndRemainingRouter(int**, int, int, int**);

int main() {
    char operationName[OPERATION_NAME_LENGTH];
    char fileName[FILE_NAME_LENGTH];

    //load input operation name and file name
    scanf("%s", operationName);
    scanf("%s", fileName);

    //open the file and store all information of the file
    FILE* file = fopen(fileName, "r");

    int routerNumber;
    fscanf(file, "%d", &routerNumber);

    int** adjacencyMatrix = (int**)malloc(sizeof(int*) * routerNumber);
    for (int i = 0; i < routerNumber; i++) {
        adjacencyMatrix[i] = (int*)malloc(sizeof(int) * routerNumber);
        for (int j = 0; j < routerNumber; j++)
            fscanf(file, "%d", &adjacencyMatrix[i][j]);
    }

    //load input operation name and check whether it is "rm"
    int removeRouterOrNot = 0;
    int remainingRouterCnt = routerNumber;
    int* remainingRouterIndex;

    scanf("%s", operationName);
    //if operation name is not "rm"
    if (strncmp(operationName, "rm", 2) != 0) {
        fileName[strlen(fileName) - strlen(".txt")] = '\0';
        strcat(fileName, "_out2.txt");
        FILE* result = fopen(fileName, "w");

        for (int i = 0; i < routerNumber; i++)
            DijkstraAlgorithm(adjacencyMatrix, routerNumber, routerNumber, i, removeRouterOrNot, remainingRouterIndex, result);

        releaseAllocatedResource(adjacencyMatrix, routerNumber);
        fclose(file);
        fclose(result);
    }
    //if operation name is "rm"
    else {
        removeRouterOrNot = 1;
        remainingRouterCnt--;

        char removeRouterName[REMOVE_ROUTER_NAME_LENGTH];

        scanf("%s", removeRouterName);
        char* digitPtr;
        for (int i = 0; i < strlen(removeRouterName); i++) {
            if (isdigit(removeRouterName[i])) {
                digitPtr = &removeRouterName[i];
                break;
            }
        }
        int removedRouterIndex = atoi(digitPtr);
        
        removeCertainRouter(adjacencyMatrix, routerNumber, removedRouterIndex);
        int** newMatrix = NewMatrixAndRemainingRouter(adjacencyMatrix, routerNumber, remainingRouterCnt, &remainingRouterIndex);

        while (1) {
            scanf("%s", operationName);
            if (strncmp(operationName, "rm", 2) != 0)
                break;
            else {
                remainingRouterCnt--;
                releaseAllocatedResource(newMatrix, remainingRouterCnt);
                free(remainingRouterIndex);

                scanf("%s", removeRouterName);
                for (int i = 0; i < strlen(removeRouterName); i++) {
                    if (isdigit(removeRouterName[i]) != 0) {
                        digitPtr = &removeRouterName[i];
                        break;
                    }
                }
                removedRouterIndex = atoi(digitPtr);

                removeCertainRouter(adjacencyMatrix, routerNumber, removedRouterIndex);
                newMatrix = NewMatrixAndRemainingRouter(adjacencyMatrix, routerNumber, remainingRouterCnt, &remainingRouterIndex);
            }
        }

        fileName[strlen(fileName) - strlen(".txt")] = '\0';
        strcat(fileName, "_out2.txt");
        FILE* result = fopen(fileName, "w");

        if (remainingRouterCnt <= 0)
            fprintf(result, "there is no router left! every router is removed!\n");
        else {
            for (int i = 0; i < remainingRouterCnt; i++)
                DijkstraAlgorithm(newMatrix, routerNumber, remainingRouterCnt, i, removeRouterOrNot, remainingRouterIndex, result);
        }
        
        releaseAllocatedResource(adjacencyMatrix, routerNumber);
        releaseAllocatedResource(newMatrix, remainingRouterCnt);
        free(remainingRouterIndex);
        fclose(file);
        fclose(result);
    }

    return 0;
}

void releaseAllocatedResource(int** adjacencyMatrix, int routerNumber) {
    for (int i = 0; i < routerNumber; i++)
        free(adjacencyMatrix[i]);
    free(adjacencyMatrix);

    return;
}

int minimalDistanceIndex(int* distFromSource, bool* shortestPathSet, int remainingRouterCnt) {
    int minimalDist = INFINITE;
    int minDistIndex;

    for (int i = 0; i < remainingRouterCnt; i++) {
        if (shortestPathSet[i] == false && distFromSource[i] <= minimalDist) {
            minimalDist = distFromSource[i];
            minDistIndex = i;
        }
    }

    return minDistIndex;
}

void printNextRouter(int* distFromSource, int* parentOfRouter, int destination, int removeRouterOrNot, int* remainingRouterIndex, FILE* outputFile) {
    if (distFromSource[destination] == INFINITE) {
        fprintf(outputFile, "%d\n", -1);
        return;
    }

    if (parentOfRouter[destination] != -1) {
        while (parentOfRouter[parentOfRouter[destination]] != -1)
            destination = parentOfRouter[destination];
    }
        
    if (removeRouterOrNot == 0)
        fprintf(outputFile, "%d\n", destination + 1);
    else
        fprintf(outputFile, "%d\n", remainingRouterIndex[destination] + 1);

    return;
}

void printRoutingTable(int* distFromSource, int* parentOfRouter, int originRouterCnt, int remainingRouterCnt, int sourceRouter, int removeRouterOrNot, int* remainingRouterIndex, FILE* outputFile) {
    if (removeRouterOrNot == 0)
        fprintf(outputFile, "Routing table of router %d:\n", sourceRouter + 1);
    else
        fprintf(outputFile, "Routing table of router %d:\n", remainingRouterIndex[sourceRouter] + 1);

    for (int j = 0; j < remainingRouterCnt; j++) {
        if (distFromSource[j] == INFINITE)
            fprintf(outputFile, "%d ", -1);
        else
            fprintf(outputFile, "%d ", distFromSource[j]);
        printNextRouter(distFromSource, parentOfRouter, j, removeRouterOrNot, remainingRouterIndex, outputFile);
    }
    fprintf(outputFile, "\n");

    return;
}

void DijkstraAlgorithm(int** adjacencyMatrix, int originRouterCnt, int remainingRouterCnt, int sourceRouter, int removeRouterOrNot, int* remainingRouterIndex, FILE* outputFile) {
    int* distFromSource = (int*)malloc(sizeof(int) * remainingRouterCnt);
    bool* shortestPathSet = (bool*)malloc(sizeof(bool) * remainingRouterCnt);
    int* parentOfRouter = (int*)malloc(sizeof(int) * remainingRouterCnt);

    for (int i = 0; i < remainingRouterCnt; i++) {
        distFromSource[i] = INFINITE;
        shortestPathSet[i] = false;
    }
    parentOfRouter[sourceRouter] = -1;
    distFromSource[sourceRouter] = 0;

    for (int i = 0; i < remainingRouterCnt - 1; i++) {
        int minDistIndex = minimalDistanceIndex(distFromSource, shortestPathSet, remainingRouterCnt);
        shortestPathSet[minDistIndex] = true;

        for (int j = 0; j < remainingRouterCnt; j++) {
            if (!shortestPathSet[j] && adjacencyMatrix[minDistIndex][j] > 0 && distFromSource[minDistIndex] != INFINITE
               && distFromSource[minDistIndex] + adjacencyMatrix[minDistIndex][j] < distFromSource[j]) {
               distFromSource[j] = distFromSource[minDistIndex] + adjacencyMatrix[minDistIndex][j];
               parentOfRouter[j] = minDistIndex;
            }
        }
    }

    printRoutingTable(distFromSource, parentOfRouter, originRouterCnt, remainingRouterCnt, sourceRouter, removeRouterOrNot, remainingRouterIndex, outputFile);
        
    free(distFromSource);
    free(shortestPathSet);
    free(parentOfRouter);

    return;
}

void removeCertainRouter(int** adjacencyMatrix, int routerNumber, int removedRouterIndex) {
    for (int i = 0; i < routerNumber; i++)
        adjacencyMatrix[i][removedRouterIndex - 1] = -1;
    for (int i = 0; i < routerNumber; i++)
        adjacencyMatrix[removedRouterIndex - 1][i] = -1;
        
    return;
}

int** NewMatrixAndRemainingRouter(int** adjacencyMatrix, int routerNumber, int remainingRouterCnt, int** remainingRouterIndex) {
    bool* remainingRouter = (bool*)malloc(sizeof(bool) * routerNumber);
    for (int i = 0; i < routerNumber; i++)
        remainingRouter[i] = false;

    for (int i = 0; i < routerNumber; i++) {
        for (int j = 0; j < routerNumber; j++) {
            if (adjacencyMatrix[i][j] != -1) {
                remainingRouter[i] = true;
                remainingRouter[j] = true;
            }
        }
    }

    int** newMatrix = (int**)malloc(sizeof(int*) * remainingRouterCnt);
    for (int i = 0; i < remainingRouterCnt; i++)
        newMatrix[i] = (int*)malloc(sizeof(int) * remainingRouterCnt);

    int rowIndex = 0, colIndex = 0;
    for (int i = 0; i < routerNumber; i++) {
        for (int j = 0; j < routerNumber; j++) {
            if (remainingRouter[i] == true && remainingRouter[j] == true) {
                newMatrix[rowIndex][colIndex] = adjacencyMatrix[i][j];
                colIndex++;
            }
            if (colIndex == remainingRouterCnt) {
                colIndex = 0;
                rowIndex++;
                break;
            }
        }
        if (rowIndex == remainingRouterCnt)
            break;
    }

    int* adjustRouterIndex = (int*)malloc(sizeof(int) * remainingRouterCnt);
    int key = 0;
    for (int i = 0; i < routerNumber; i++) {
        if (remainingRouter[i] == true) {
            adjustRouterIndex[key] = i;
            key++;
        }
    }

    *remainingRouterIndex = adjustRouterIndex;
    free(remainingRouter);

    return newMatrix;
}