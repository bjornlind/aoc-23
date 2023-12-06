#include <stdio.h>
#include <string.h>

enum COLOR {
    UNKNOWN,
    RED,
    GREEN,
    BLUE
};

static void Solve();
static void SolveProblem2();
static void SolveOneLine(char* s);
char* ColorToString(enum COLOR c);

FILE* fp;
int problemNbr;

const int NbrRedCubes = 12;
const int NbrGreenCubes = 13;
const int NbrBlueCubes = 14;

int possibleGamesSum = 0;
int powerSum = 0;

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

static void Solve()
{
    printf("MAX CUBES:\n\tRED: %d\n\tGREEN: %d\n\tBLUE: %d\n", NbrRedCubes, NbrGreenCubes, NbrBlueCubes);
    char line[255];

    while(fgets(line, sizeof(line)/sizeof(line[0]), fp) != NULL)
    {
        // printf("Line: %s", line);
        SolveOneLine(line);
    }

    printf("Sum: %d\n", possibleGamesSum);
    printf("Power: %d\n", powerSum);
}

static void SolveOneLine(char* str)
 {    
    const char delims[2] = " ";
    char *token = strtok(str, delims);
    int gameId;
    int nbrCubesInHand = -1;
    int nbrHandsWithExcessCubes = 0;
    enum COLOR cubeColorInHand = UNKNOWN;

    int minNbrReds = 0;
    int minNbrGreens = 0;
    int minNbrBlues = 0;

    while(token != NULL)
    {
        if(strstr(token, ":"))
        {
            // Parse game id:
            int size = strlen(token);
            token[size-1] = '\0';
            sscanf(token, "%d", &gameId);
        }
        else if(strstr(token, ";") != NULL || strstr(token, ",") != NULL || strstr(token, "\n") != NULL)
        {
            // Parse the color
            if(strstr(token, "red"))
            {
                cubeColorInHand = RED;
            }
            else if(strstr(token, "green"))
            {
                cubeColorInHand = GREEN;
            }
            else if(strstr(token, "blue"))
            {
                cubeColorInHand = BLUE;
            }
        }
        else if(!strstr(token, "Game"))
        {
            // Parse the number of cubes
            sscanf(token, "%d", &nbrCubesInHand);
        }
        
        if(cubeColorInHand != UNKNOWN && nbrCubesInHand > -1)
        {
            char *colorStr = ColorToString(cubeColorInHand);

            switch (cubeColorInHand)
            {
            case RED:
                if(nbrCubesInHand > minNbrReds)
                {
                    minNbrReds = nbrCubesInHand;
                }

                if(nbrCubesInHand > NbrRedCubes)
                {
                    nbrHandsWithExcessCubes++;
                }
                break;

            case GREEN:
                if(nbrCubesInHand > minNbrGreens)
                {
                    minNbrGreens = nbrCubesInHand;
                }
                
                if(nbrCubesInHand > NbrGreenCubes)
                {
                    nbrHandsWithExcessCubes++;
                }
                break;

            case BLUE:
                if(nbrCubesInHand > minNbrBlues)
                {
                    minNbrBlues = nbrCubesInHand;
                }
                                
                if(nbrCubesInHand > NbrBlueCubes)
                {
                    nbrHandsWithExcessCubes++;
                }
                break;

            default:
                break;
            }

            cubeColorInHand = UNKNOWN;
            nbrCubesInHand = -1;
        }
        
        token = strtok(NULL, delims);
    }

    // Problem 1:
    if(nbrHandsWithExcessCubes < 1)
    {
        possibleGamesSum += gameId;
    }

    // Problem 2:
    int power = minNbrReds * minNbrGreens * minNbrBlues;
    powerSum += power;
 }

 char* ColorToString(enum COLOR c)
 {
     char *str;

     switch (c)
     {
     case RED:
         str = "red";
         break;

        case GREEN:
            str = "green";
            break;
        case BLUE:
            str = "blue";
            break;
     default:
         break;
     }

     return str;
 }
