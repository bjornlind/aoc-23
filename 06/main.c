#include <stdio.h>

typedef struct race
{
    unsigned long time;
    unsigned long record;
} race;

int main(int argc, char* args[])
{
    // Problem 1:
    
    // race races[] = {{7, 9}, {15, 40}, {30, 200}};
    race races[] = {{40, 277}, {82, 1338}, {91, 1349}, {66, 1063}};
    int nbrRaces = sizeof(races) / sizeof(races[0]);

    unsigned long sum1 = 1;

    
    for (size_t i = 0; i < nbrRaces; i++)
    {
        unsigned long waysToWin = 0;
        unsigned long raceTime = races[i].time;
        unsigned long raceRecord = races[i].record;

        for (size_t j = 1; j < raceTime; j++)
        {
            unsigned long distance = (raceTime - j) * j;
            waysToWin = distance > raceRecord ? waysToWin + 1 : waysToWin;
        }
        
        sum1 *= waysToWin;
    }   

    // Problem 2:
    unsigned long sum2 = 0;
    // race raceProb2 = {71530, 940200};
    race raceProb2 = {40829166, 277133813491063};

    for (size_t i = 1; i < raceProb2.time; i++)
    {
        unsigned long distance = (raceProb2.time - i) * i;
        sum2 = distance > raceProb2.record ? sum2 + 1 : sum2;
    }
    
    printf("Problem 1: %d\n", sum1);
    printf("Problem 2: %d\n", sum2);
}