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
            Map *m = (Map*)malloc(sizeof(Map));
            sscanf(line, "%ld%ld%d", &m->dest, &m->source, &m->length);
            mapIdx++;

            // Insert mapping into mappings array, keeping it sorted by the source
            int idx = 0;

            // Find which index to insert the mapping into:
            for (size_t i = 1; i < mapIdx; i++)
            {   
                Map *mapInArr = &maps[i-1];
                if(m->source < mapInArr->source)
                {
                    idx = i-1;
                    break;
                }
                
                idx++;
            }

            if(idx < mapIdx - 1)
            {
                // Move all elements in larger indices to the right by one position:
                for (size_t i = mapIdx; i > idx; i--)
                {
                    maps[i] = maps[i-1];
                }
            }

            // Insert the new mapping:
            maps[idx] = *m;            
        }
    }    

    mapGroup->maps = maps;
    mapGroup->size = mapIdx;
}

Map* FindMapByBinarySearch(MapGroup mg, long seed)
{
    int lowIdx = 0;
    int highIdx = mg.size - 1;
  
    while(lowIdx <= highIdx)
    {
        int mid = lowIdx + (highIdx - lowIdx) / 2;

        Map *m = mg.maps + mid;
        if(seed >= m->source && seed < (m->source + m->length))
        {
            return m;
        }

        if(seed < m->source)
        {
            highIdx = mid - 1;
        }
        else
        {
            lowIdx = mid + 1;
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

        for (size_t j = 0; j < nbrMapGroups; j++)
        {
            Map *m = FindMapByBinarySearch(*(mapGroups+j), loc);

            long diff = loc - m->source;
            long mapping = m->dest + diff;
            loc = mapping;
        }
        
        minloc = (minloc < 0 || minloc > loc) ? loc : minloc;
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

    // Parse seeds
    fgets(seedsLine, sizeof(seedsLine), f);
    long nbrSeedsProblem1 = ParseSeeds(seedsLine, seeds);
    fgets(seedsLine, sizeof(seedsLine), f);

    // Parse mappings
    while(!feof(f))
    {
        ParseMapGroup(f, &mapGroups[mapGroupIdx]);
        mapGroups[mapGroupIdx].idx = mapGroupIdx;
        mapGroupIdx++;
    }

    // Solve problem 1:
    long lowestLocProblem1 = Solve(seeds, nbrSeedsProblem1, mapGroups, mapGroupIdx);

    // Solve problem 2:
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