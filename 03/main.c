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
            // printf("Char to the left is valid: %c\n", cL);
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
                    // printf("%d should be included!\n", nbr);                       
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

int findWholeNumber(int idx, char* line)
{
    int startIdx = idx;
    int endIdx = idx;
    
    // find start idx
    while(startIdx > 0 && isdigit(line[startIdx-1]))
    {
        startIdx--;
    }

    // find end idx
    while(line[endIdx+1] != '\n' && line[endIdx+1] != EOF && isdigit(line[endIdx+1]))
    {
        endIdx++;
    }

    int arr[endIdx-startIdx+1];

    for (size_t i = startIdx; i <= endIdx; i++)
    {
        arr[i-startIdx] = (int) line[i] - '0';
    }

    int nbr = extractNbr(endIdx-startIdx+1, arr);
    // printf("Found %d in %s", nbr, line);
    return nbr;    
}

int FindGearNumber(int idx, char* prevLine, char* currLine, char* nextLine)
{
    int num1 = -1;
    int num2 = -1;

    // Check previous line
    if(line > 1)
    {
        if(isdigit(prevLine[idx]))
        {
            // Straight above
            num1 = findWholeNumber(idx, prevLine);
        }
        else
        {
            if(idx > 0 && isdigit(prevLine[idx-1]))
            {
                // Above to the left
                int num = findWholeNumber(idx-1, prevLine);
                if(num1 < 0)
                {
                    num1 = num;
                }
                else if (num2 < 0)
                {
                    num2 = num;
                }
            }
            
            if(isdigit(prevLine[idx+1]))
            {
                // Above to the right
                int num = findWholeNumber(idx+1, prevLine);
                if(num1 < 0)
                {
                    num1 = num;
                }
                else if (num2 < 0)
                {
                    num2 = num;
                }
            }
        }
    }

    // Check left
    if(idx > 0 && isdigit(currLine[idx-1]))
    {
        int num = findWholeNumber(idx-1, currLine);
        if(num1 < 0)
        {
            num1 = num;
        }
        else if (num2 < 0)
        {
            num2 = num;
        }
    }

    // Check right
    if(isdigit(currLine[idx+1]))
    {
        int num = findWholeNumber(idx+1, currLine);
        if(num1 < 0)
        {
            num1 = num;
        }
        else if (num2 < 0)
        {
            num2 = num;
        }
    }

    // Check below
    if(!finalLine)
    {
        if(isdigit(nextLine[idx]))
        {
            // Straight below
            int num = findWholeNumber(idx, nextLine);
            if(num1 < 0)
            {
                num1 = num;
            }
            else if (num2 < 0)
            {
                num2 = num;
            }
        }
        else
        {
            if(idx > 0 && isdigit(nextLine[idx-1]))
            {
                // Below to the left
                int num = findWholeNumber(idx-1, nextLine);
                if(num1 < 0)
                {
                    num1 = num;
                }
                else if (num2 < 0)
                {
                    num2 = num;
                }              
            }

            if(isdigit(nextLine[idx+1]))
            {
                // Below to the right
                int num = findWholeNumber(idx+1, nextLine);
                if(num1 < 0)
                {
                    num1 = num;
                }
                else if (num2 < 0)
                {
                    num2 = num;
                }
            }
        }
    }

    if(num1 > -1 && num2 > -1)
    {
        return num1 * num2;
    }

    return -1;
}

void SolveProblem2(char* prevLine, char* currLine, char* nextLine)
{
    for (size_t i = 0; i < nCOL; i++)
    {
        if(currLine[i] == '*')
        {
            int g = FindGearNumber(i, prevLine, currLine, nextLine);
            if(g > -1)
            {
                sumProblem2 += g;
                // printf("Found gear number %d on line %d\n", g, line);   
            }
        }
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
    printf("Problem 2 sum = %d\n", sumProblem2);
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