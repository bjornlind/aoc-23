#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define nCOL 150

FILE *fp;
int line = 1;
bool finalLine = false;

int sumProblem1 = 0;
int sumProblem2 = 0;

int extractNbr(int nbrDigits, int* arr)
{
    int nbr = 0;

    for (size_t i = 0; i < nbrDigits; i++)
    {
        nbr += pow(10, nbrDigits - i - 1) * arr[i];
    }

    return nbr;
}

bool isValid(char c)
{
    if(!isdigit(c) && c != '.' && c != '\n' && c != EOF)
    {
        // printf("%c is a symbol!\n", c);
        return true;
    }

}

bool CheckCurrentLine(int idx, char* line)
{
    if(idx > 0)
    {
        // Check to the left:
        char cL = line[idx-1];

        if(isValid(cL))
        {
            printf("Char to the left is valid: %c\n", cL);
            return true;
        }
    }

    // Check to the right:
    char cR = line[idx+1];
    if(isValid(cR))
    {
        return true;
    }
}

bool CheckOtherLine(int idx, char* line)
{
    if(idx > 0)
    {
        // Check diagonally to the left:
        if(isValid(line[idx-1]))
        {
            return true;
        }
    }

    // Check same column and diagonally to the right:
    if(isValid(line[idx]) || isValid(line[idx+1]))
    {
        return true;
    }
}



void SolveProblem1(char* prevLine, char* currLine, char* nextLine)
{
    int currNbrs[nCOL];
    int currNbrIdx = 0;
    bool currNbrIncluded = false;

    for (size_t i = 0; i < nCOL; i++)
    {
        char c = currLine[i];
        
        if(isdigit(c))
        {
            currNbrs[currNbrIdx] = (int)(c - '0');
            currNbrIdx++;

            if(!currNbrIncluded)
            {
                if(CheckCurrentLine(i, currLine))
                {
                    int n = extractNbr(currNbrIdx, currNbrs);
                    currNbrIncluded = true;
                }
                else if(line > 1 && CheckOtherLine(i, prevLine))
                {
                    int n = extractNbr(currNbrIdx, currNbrs);
                    currNbrIncluded = true;
                }
                else if(!finalLine && CheckOtherLine(i, nextLine))
                {
                    int n = extractNbr(currNbrIdx, currNbrs);
                    currNbrIncluded = true;
                }
            }
        }
        else
        {
            if(currNbrIdx > 0)
            {
                // We have finished finding a complete number, check if it should be added to the sum:
                if(currNbrIncluded)
                {
                    // Extract the number
                    int nbr = extractNbr(currNbrIdx, currNbrs);
                    sumProblem1 += nbr;
                    printf("%d should be included!\n", nbr);                       
                }
            }

            currNbrIdx = 0;
            currNbrIncluded = false;
        }

        if(c == '\n' || c == EOF)
        {
            break;
        }
    }
}

void SolveProblem2(char* prevLine, char* currLine, char* nextLine)
{
    for (size_t i = 0; i < nCOL; i++)
    {
        if(currLine[i] == '*')
    }
}

void Solve()
{
    char prevLine[nCOL];
    char currLine[nCOL];
    char nextLine[nCOL];
    
    while(!finalLine)
    {
        if(line == 1)
        {
            fgets(currLine, nCOL, fp);
            fgets(nextLine, nCOL, fp);
        }
        else if(line > 1)
        {
            memcpy(prevLine, currLine, nCOL);
            memcpy(currLine, nextLine, nCOL);

            if(fgets(nextLine, nCOL, fp) == NULL)
            {
                finalLine = true;
            };
        }

        SolveProblem1(prevLine, currLine, nextLine);
        SolveProblem2(prevLine, currLine, nextLine);

        line++;
    }

    printf("Problem 1 sum = %d\n", sumProblem1);
}


int main(int argc, char const *argv[])
{
    fp = fopen(argv[1], "r");
    if(fp == NULL)
    {
        perror("Error opening file\n");
        return -1;
    }

    Solve();
    fclose(fp);
    return 0;
}