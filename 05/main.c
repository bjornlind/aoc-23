#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct Map
{
    long source;
    long dest;
    int length;
} Map;

typedef struct MapGroup
{
    int idx;
    int size;
    Map *maps;
} MapGroup;


long ParseSeeds(char *line, long *seeds)
{
    int nbrSeeds = 0;

    char *token = strtok(line, ":");
    token = strtok(NULL, " ");
    while(token)
    {
        sscanf(token, "%ld", seeds+nbrSeeds);
        nbrSeeds++;
        token = strtok(NULL, " ");
    }

    return nbrSeeds;
}

void ParseMapGroup(FILE *f, MapGroup *mapGroup)
{
    int mapGroupIdx = 0;
    int mapIdx = 0;
    char line[100];
    Map *maps = calloc(100, sizeof(Map)); 

    while(fgets(line, sizeof(line), f) != NULL)
    {
        if(line[0] == '\n')
        {
            break;
        }

        if(isdigit(line[0]))
        {
            // printf("Extracting mapping from: %s", line);
            sscanf(line, "%ld%ld%d", &maps[mapIdx].dest, &maps[mapIdx].source, &maps[mapIdx].length);
            // printf("\t-> %ld %ld %d\n", maps[mapIdx].dest, maps[mapIdx].source, maps[mapIdx].length);
            mapIdx++;
        }
    }

    mapGroup->maps = maps;
    mapGroup->size = mapIdx;
}

void printArray(long *arr, size_t length, char delim)
{
    for (size_t i = 0; i < length; i++)
    {
        printf("%ld", *(arr+i));
        if(i < length - 1)
        {
            printf("%c", delim);
        }
    }
}

Map* FindMap(MapGroup mg, long seed)
{
    // printf("\tLooking for mapping for seed %ld in map group %d\n", seed, mg.idx);
    
    for (size_t i = 0; i < mg.size; i++)
    {
        Map *m = mg.maps + i;
        // printf("\t\tAnalyzing map: Src= %ld, Dst= %ld, Len= %d\n", m->source, m->dest, m->length);
        if(seed >= m->source && seed < m->source + m->length)
        {
            // printf("\tFound mapping: Src = %ld, Dst = %ld, Len = %d\n", m->source, m->dest, m->length);
            return m;
        }
    }

    Map *reflectMap = malloc(sizeof(Map));
    reflectMap->source = seed;
    reflectMap->dest = seed;
    reflectMap->length = 1;
    return reflectMap;
}

long Solve(long *seeds, long nbrSeeds, MapGroup *mapGroups, int nbrMapGroups)
{
    long minloc = -1;

    // Loop through all seeds and calculate mappings
    for (size_t i = 0; i < nbrSeeds; i++)
    {
        long loc = seeds[i];
        // printf("Mapping seed %ld:\n", loc);

        for (size_t j = 0; j < nbrMapGroups; j++)
        {
            Map *m = FindMap(*(mapGroups+j), loc);
            
            long diff = loc - m->source;
            long mapping = m->dest + diff;
            // printf("\t%d: Mapped %ld to %ld\n\n", j, loc, mapping);
            loc = mapping;
        }
        
        minloc = (minloc < 0 || minloc > loc) ? loc : minloc; 
        // printf("\tSeed %ld -> Location %ld\n", seeds[i], loc);
    }

    return minloc;
}

int main(int argc, char* argv[])
{
    long seeds[50];
    char seedsLine[220];
    int mapGroupIdx = 0;
    MapGroup mapGroups[10];

    FILE *f = fopen(argv[1], "r");

    // Parse seeds problem 1
    fgets(seedsLine, sizeof(seedsLine), f);
    long nbrSeedsProblem1 = ParseSeeds(seedsLine, seeds);
    fgets(seedsLine, sizeof(seedsLine), f);

    // Parse seeds problem 2


    // Parse mappings
    while(!feof(f))
    {
        ParseMapGroup(f, &mapGroups[mapGroupIdx]);
        mapGroups[mapGroupIdx].idx = mapGroupIdx;
        mapGroupIdx++;
    }

    // for (size_t i = 0; i < mapGroupIdx; i++)
    // {
    //     printf("Map group %d:\n", i);
    //     printf("\t%d maps:\n", mapGroups[i].size);
    //     for (size_t j = 0; j < mapGroups[i].size; j++)
    //     {
    //         Map *m = mapGroups[i].maps+j;
    //         printf("\t\tSource: %ld,\t Dest: %ld,\t Length: %d\n", m->source, m->dest, m->length);
    //     }    
    // }

    long lowestLocProblem1 = Solve(seeds, nbrSeedsProblem1, mapGroups, mapGroupIdx);

    long lowestLocProblem2 = -1;

    for (size_t i = 0; i < nbrSeedsProblem1; i+=2)
    {
        long firstSeed = seeds[i];
        long nbrSeeds = seeds[i+1];

        for (size_t j = 0; j < nbrSeeds; j++)
        {
            long seed = firstSeed + j;
            long loc = Solve(&seed, 1, mapGroups, mapGroupIdx);
            lowestLocProblem2 = (lowestLocProblem2 < 0 || loc < lowestLocProblem2) ? loc : lowestLocProblem2;
        }
        
    }
    
    printf("Problem 1: Lowest location number = %ld\n", lowestLocProblem1);
    printf("Problem 2: Lowest location number = %ld\n", lowestLocProblem2);
    
    fclose(f);
}