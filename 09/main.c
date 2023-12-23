#include <stdio.h>
#include <string.h>

int FindNextValue(int nbrNumbers, int *numbers, int problemNbr)
{
    int newNumbers[nbrNumbers - 1];
    int sum = 0;

    for (size_t i = 0; i < nbrNumbers - 1; i++)
    {
        int diff = numbers[i + 1] - numbers[i];
        newNumbers[i] = diff;
        sum += diff;
    }

    if(sum == 0)
    {
        if(problemNbr == 1)
        {
            return numbers[nbrNumbers - 1] + newNumbers[nbrNumbers - 2];
        }
        else
        {
            return numbers[0];
        }
    }
    else
    {
        if(problemNbr == 1)
        {
            return numbers[nbrNumbers - 1] + FindNextValue(nbrNumbers - 1, newNumbers, 1);
        }
        else
        {
            return numbers[0] - FindNextValue(nbrNumbers - 1, newNumbers, 2);
        }
    }    
}

int main(int argc, char const *argv[])
{
    int sumProb1 = 0;
    int sumProb2 = 0;
    FILE* f = fopen(argv[1], "r");
    char lineStr[150] = {};

    while(fgets(lineStr, sizeof(lineStr), f) != NULL)
    {
        // Parse line into array
        int numbersOrig[20];
        int nbrNumbers = 0;

        char *token = strtok(lineStr, " ");

        while(token != NULL)
        {
            sscanf(token, "%d", &numbersOrig[nbrNumbers++]);
            token = strtok(NULL, " ");
        }

        int nextNumberProb1 = FindNextValue(nbrNumbers, numbersOrig, 1);
        int nextNumberProb2 = FindNextValue(nbrNumbers, numbersOrig, 2);
        
        sumProb1 += nextNumberProb1;
        sumProb2 += nextNumberProb2;
    }

    printf("Sum problem 1 = %d\n", sumProb1);
    printf("Sum problem 2 = %d\n", sumProb2);
    return 0;
}