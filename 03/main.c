#include <stdio.h>

#define nCOL 140

void Solve();
FILE *fp;

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

void Solve()
{
    char prevLine[nCOL];
    char currLine[nCOL];
    char nextLine[nCOL];
    
    char c = fgetc(fp);

    while(c != EOF && c != '.')
    {

    }
}
