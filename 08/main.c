#include <stdio.h>
#include <string.h>

typedef struct Node
{
    int nbr;
    char loc[3];
    char left[3];
    char right[3];
} Node;

int x1 = 0;
int x2 = 0;

int CalculateIndex(char *str)
{
    return (str[0] - 'A') * x1 + (str[1] - 'A') * x2 + (str[2] - 'A'); 
}

int main(int argc, char const *argv[])
{
    int problemNbr;
    sscanf(argv[2], "%d", &problemNbr);
    const int numAlphabet = 26;
    Node nodes[numAlphabet*numAlphabet*numAlphabet];
    int pathsIndices[6] = {0};
    int nbrPaths = 0;

    // Coefficients:
    x1 = (numAlphabet * (numAlphabet - 1)) + 1;
    x2 = numAlphabet;

    FILE *f = fopen(argv[1], "r");

    // Parse instructions:
    char instruction[300]; 
    fgets(instruction, sizeof(instruction), f);
    long nbrInstructions = (long)(strlen(instruction) - 1);

    // Parse nodes:
    char line[25];
    fgets(line, sizeof(line), f);
    int nbrNodes = 0;

    while(fgets(line, sizeof(line), f) != NULL)
    {
        int idx = CalculateIndex(line);
        nodes[idx].nbr = nbrNodes;
        memcpy(nodes[idx].loc, line, 3);
        memcpy(nodes[idx].left, line+7, 3);
        memcpy(nodes[idx].right, line+12, 3);
        // printf("Node %d: %.3s = (%.3s, %.3s)\n", nodes[idx].nbr, nodes[idx].loc, nodes[idx].left, nodes[idx].right);

        if(nodes[idx].loc[2] == 'A')
        {
            pathsIndices[nbrPaths] = CalculateIndex(nodes[idx].loc);
            nbrPaths++;
        }

        nbrNodes++;
    }

    // Solve problem 1:
    if(problemNbr == 1)
    {
        int idx = 0;
        int nbrSteps = 0;

        while(idx != ((numAlphabet-1)*x1 + (numAlphabet-1)*x2 + (numAlphabet-1)))
        {
            int instructionIdx = nbrSteps % nbrInstructions;
            printf("%c: %.3s = (%.3s, %.3s)\n", instruction[instructionIdx], nodes[idx].loc, nodes[idx].left, nodes[idx].right);
            if(instruction[instructionIdx] == 'L')
            {
                idx = CalculateIndex(nodes[idx].left);
            }
            else
            {
                idx = CalculateIndex(nodes[idx].right);
            }
            nbrSteps++;
        }    

        printf("Problem nbr 1 = %d\n", nbrSteps);
    }

    // Solve problem 2:
    if(problemNbr == 2)
    {
        printf("----------------------\n");
        printf("Problem 2:\n");

        long stepsPerPath[nbrPaths];

        for (size_t i = 0; i < nbrPaths; i++)
        {
            int idx = pathsIndices[i];
            int startIdx = idx;
            long nbrSteps = 0;

            // while(AllLocsEndWithZ(nodes, &idx, 1) == false)
            while(nodes[idx].loc[2] != 'Z')
            {
                int instructionIdx = nbrSteps % nbrInstructions;

                if(instruction[instructionIdx] == 'L')
                {
                    idx = CalculateIndex(nodes[idx].left);
                }
                else
                {
                    idx = CalculateIndex(nodes[idx].right);

                }

                nbrSteps++;
            }

            stepsPerPath[i] = nbrSteps;
        }

        for (size_t i = 0; i < nbrPaths; i++)
        {
            printf("%.3s -> %ld steps\n", nodes[pathsIndices[i]].loc, stepsPerPath[i]);
        }
        
        // Find smallest number which all nbrSteps are evenly divisible by (LCM):
        // https://www.calculatorsoup.com/calculators/math/lcm.php
        // The correct steps (input to LCM) : 21409 11653 19241 12737 14363 15989
    }

    return 0;
}