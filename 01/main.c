#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

void SolveProblem1(FILE* fp);
void SolveProblem2(FILE* fp);
int ExtractFirstAndLastNumber(const char *s, int n, const char **matches);

const char *numbers[] = {"one", "two", "three", "four", "five", 
                            "six", "seven", "eight", "nine"};

int main(int argc, char const *argv[])
{     
    // Open the file
    FILE* fp; 
    fp = fopen(argv[1], "r");
    if(fp == NULL)
    {
        perror("Error opening file\n");
        return -1;
    }

    // Parse problem to solve
    if(strcmp(argv[2], "01") == 0)
    {
        SolveProblem1(fp);
    }
    else
    {
        SolveProblem2(fp);
    }
    
    fclose(fp);
    return 0;
}

void SolveProblem1(FILE* fp)
{
    printf("Solving problem 1 ...\n");

    int sum = 0;
    int num1 = -1;
    int num2 = -1;
    char c;

    do
    {
        c = fgetc(fp);

        if(c == '\n' || c == EOF)
        {
            // printf("num1 = %d, num2 = %d\n", num1, num2);

            if(num2 > -1)
            {
                sum += num1 * 10 + num2;
            }
            else
            {
                sum += num1 * 10 + num1;
            }

            num1 = -1;
            num2 = -1;
            continue;
        }

        if(isdigit(c))
        {
            if(num1 < 0)
            {
                int n;
                sscanf(&c, "%d", &n);
                // printf("Setting num1 to %d\n", n);
                num1 = n;
            }
            else
            {
                int n;
                sscanf(&c, "%d", &n);
                // printf("Setting num2 to %d\n", n);
                num2 = n;
            }
        }
    }while(c != EOF);

    printf("Answer: %d\n", sum);
}

void SolveProblem2(FILE* fp)
{   
    printf("Solving problem 2 ...\n");

    char c;
    int num1 = -1;
    int num2 = -1;
    int sum = 0;
    int lineNbr = 1;

    #define MATCH_SIZE 255

    char match[MATCH_SIZE];
    //char match[5];
    int matchIdx = 0;

    do
    {
        c = fgetc(fp);


        if(c == '\n' || c == EOF)
        {
            int n1 = num1;
            int n2 = num2 < 0 ? num1 : num2;
            sum += n1 * 10 + n2;

            printf("Line %d: %d + %d -> %d\n", lineNbr++, n1, n2, n1*10+n2);

            num1 = -1;
            num2 = -1;
            matchIdx = 0;
            memset(match, 0, MATCH_SIZE);
        }
        else
        {
            if(isdigit(c))
            {
                matchIdx = 0;
                memset(match, 0, MATCH_SIZE);
                
                if(num1 < 0)
                {
                    num1 = c - '0';
                }
                else
                {
                    num2 = c - '0';
                }
            }
            else
            {
                // append to match
                match[matchIdx++] = c;
                int m = VerifyMatch(match);
                if(m != 0)
                {
                    // printf("Found number %d in string %s\n", m, match);

                    if(num1 < 0)
                    {
                        num1 = m;
                    }
                    else
                    {
                        num2 = m;
                    }

                    matchIdx = 0;
                    memset(match, 0, MATCH_SIZE);
                    fseek(fp, -1L, SEEK_CUR);
                }
            }
        }

    } while (c != EOF);
    
    printf("Problem 2 sum: %d\n", sum);

}

int VerifyMatch(char* s)
{
    for (size_t i = 0; i < 9; i++)
    {
        char* nbr = numbers[i];
        if(strstr(s, nbr) != NULL)
        {
            return i + 1;
        }
    }

    return 0;
    
}

int ExtractFirstAndLastNumber(const char *s, int n, const char **matches)
{
    for (size_t i = 0; i < n; i++)
    {
        char* nbr = matches[i];
        strstr(s, nbr);    
    }

    return 0;    
}