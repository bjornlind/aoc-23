#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int GetWins(int* wins, int* winningNbrs, int nbrWinningNbrs, int* playerNbrs, int nbrPlayerNbrs)
{
    int nbrWins = 0;

    for (size_t i = 0; i < nbrWinningNbrs; i++)
    {
        int winNbr = *(winningNbrs+i);
        bool isWin = false;

        for (size_t j = 0; j < nbrPlayerNbrs; j++)
        {
            if(*(playerNbrs+j) == winNbr)
            {
                isWin = true;
                break;
            }
        }

        if(isWin)
        {
            *(wins+nbrWins) = winNbr;
            nbrWins++;
        }
    }

    return nbrWins;
}

int max(int num1, int num2)
{
    return (num1 >= num2) ? num1 : num2;
}

void ParseLine(char* line, int *gameNbr, int *winningNbrs, int *playerNbrs)
{
    int winIdx = 0;
    int playerIdx = 0;

    // Parse into three strings
    char *gameNbrString = strtok(line, ":");
    char *winningNbrsString = strtok(NULL, "|");
    char *playersNbrsString = strtok(NULL, "");

    // Parse game number:
    sscanf(gameNbrString, "%*s%d", gameNbr);
    printf("Game number: %d\n", *gameNbr);

    // Parse winning numbers:
    char* token = strtok(winningNbrsString, " ");
    while(token)
    {
        sscanf(token, "%d", winningNbrs+winIdx);
        token = strtok(NULL, " ");
        winIdx++;
    }

    printf("Winning numbers: ");
    for (size_t i = 0; i < winIdx; i++)
    {
        printf("%d ", *(winningNbrs+i));
    }

    printf("\n");

    // Parse player numbers:
    token = strtok(playersNbrsString, " ");
    while(token)
    {
        sscanf(token, "%d", playerNbrs+playerIdx);
        token = strtok(NULL, " ");
        playerIdx++;
    }

    printf("Player numbers: ");
    for (size_t i = 0; i < playerIdx; i++)
    {
        printf("%d ", *(playerNbrs+i));
    }

    printf("\n");
}

int main(int argc, char* argv[])
{
    int nbrWinningNbrs = 10;
    int nbrPlayerNbrs = 25;

    // int nbrWinningNbrs = 5;
    // int nbrPlayerNbrs = 8;

    int lineNbr = 0;
    char line[150];
    int sumProblem1 = 0;
    int cards[250] = {0};
    int cardsSum = 0;

    printf("Input file: %s\n", argv[1]);
    FILE *f = fopen(argv[1], "r");

    while(fgets(line, sizeof(line), f) != NULL)
    {
        // Parse game number, winning numbers and player numbers
        int gameNbr;
        int* winningNbrs = (int*) calloc(nbrWinningNbrs, sizeof(int));
        int* playerNbrs = (int*) calloc(nbrPlayerNbrs, sizeof(int));
        ParseLine(line, &gameNbr, winningNbrs, playerNbrs);
        
        cards[gameNbr]++;

        // Find matches and calculate win sum:
        int *playerWins = (int*)calloc(max(nbrWinningNbrs, nbrPlayerNbrs), sizeof(int));
        int nbrWins = GetWins(playerWins, winningNbrs, nbrWinningNbrs, playerNbrs, nbrPlayerNbrs);
        printf("%d wins: ", nbrWins);
        
        // Calculate win sum, problem 1:
        int winSum = nbrWins > 0 ? pow(2, nbrWins-1) : 0;
        sumProblem1 += winSum;       

        for (size_t k = 0; k < nbrWins; k++)
        {
            int nbr = *(playerWins+k);
            printf("%d ", nbr); 
        }
        printf(" = %d\n", winSum);

        // Adding cards, problem 2:
        if(nbrWins > 0)
        {
            printf("Adding the following cards: ");
            for (size_t i = 1; i <= nbrWins; i++)
            {
                cards[gameNbr + i] += cards[gameNbr];
                printf("%d ", gameNbr + i);
            }
            printf("\n");
        }

        printf("Number of cards for game %d = %d\n", gameNbr, cards[gameNbr]);
        cardsSum += cards[gameNbr];

        free(playerWins);
        free(winningNbrs);
        free(playerNbrs);

        printf("\n");
    }

    printf("Problem 1 sum: %d\n", sumProblem1);
    printf("Problem 2 sum: %d\n", cardsSum);

    fclose(f);
    return 0;
}