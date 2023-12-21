#include <stdio.h>
#include <string.h>

int problemNbr;

typedef struct Hand
{
    int idx;
    char cards[5];
    int bid;
    int typeScore;
} Hand;

int GetLabelScore(char c)
{
    switch (c)
    {
    case 'A':
        return 12;
    case 'K':
        return 11;
    case 'Q':
        return 10;
    case 'J':
        return 9;
    case 'T':
        return 8;
    default:
        return (int)(c-'0') - 2;
    }
}

int GetLabelScoreWithJoker(char c)
{
    switch (c)
    {
    case 'J':
        return 0;
    
    default:
        int score = GetLabelScore(c);
        if(score < 9)
        {
            return score + 1;
        }
        else
        {
            return score;
        }
    }
}

int CalculateHandTypeScore(Hand *h)
{
    int labels[13] = {0};
    int first = 0;
    int second = 0;

    for (size_t i = 0; i < 5; i++)
    {
        int score = problemNbr > 1 
            ? GetLabelScoreWithJoker(h->cards[i])
            : GetLabelScore(h->cards[i]);
        labels[score]++;
    }

    for (size_t i = problemNbr == 1 ? 0 : 1; i < 13; i++)
    {
        int cnt = labels[i];

        if(cnt >= first)
        {
            second = first;
            first = cnt;
        }
        else if(cnt >= second)
        {
            second = cnt;
        }
    }

    if(problemNbr == 2)
    {
        first += labels[0];
    }

    if(first == 5)
    {
        // Five of a kind
        return 7;
    }
    else if(first == 4)
    {
        // Four of a kind
        return 6;
    }
    else if(first == 3 && second == 2)
    {
        // Full house
        return 5;
    }
    else if(first == 3 && second < 2)
    {
        // Three of a kind
        return 4;
    }
    else if(first == 2 && second == 2)
    {
        // Two pair
        return 3;
    }
    else if(first == 2 && second < 2)
    {
        // One pair
        return 2;
    }
    else
    {
        // High card
        return 1;
    }
    
}

// Returns:
//      0 : handToInsert has equal rank to handInArray
//      1 : handToInsert has higher rank than handInArray
//     -1 : handToInsert has lower rank than handInArray
int CompareHandsRank(Hand *handToInsert, Hand *handInArray)
{
    if(handToInsert->typeScore > handInArray->typeScore)
    {
        return 1;
    }

    if(handToInsert->typeScore < handInArray->typeScore)
    {
        return -1;
    }

    for (size_t i = 0; i < 5; i++)
    {
        int handToInsertLabelScore = problemNbr == 1 
            ? GetLabelScore(handToInsert->cards[i])
            : GetLabelScoreWithJoker(handToInsert->cards[i]);

        int handInArrayLabelScore = problemNbr == 1 
            ? GetLabelScore(handInArray->cards[i])
            : GetLabelScoreWithJoker(handInArray->cards[i]);

        if(handToInsertLabelScore > handInArrayLabelScore)
        {
            return 1;
        }

        if(handToInsertLabelScore < handInArrayLabelScore)
        {
            return -1;
        }
    }

    printf("These are the same !!\n");
    return 0;
    
}

// Not working binary search:
// int FindIndexBinarySearch(Hand *h, Hand *hands, int nbrHands)
// {
//     int lowIdx = 0;
//     int highIdx = nbrHands - 1;
//     int mid = lowIdx + (highIdx - lowIdx) / 2;
     

//     while(lowIdx <= highIdx)
//     {
//         printf("\tLow idx = %d, High idx = %d, Mid idx = %d\n", lowIdx, highIdx, mid);

//         int comparedRank = CompareHandsRank(h, hands + mid);
//         printf("\tCompared hand %d (%d) and hand %d (%d)\n", h->idx, h->typeScore, (hands+mid)->idx, (hands+mid)->typeScore);
//         printf("\tHand %d has higher score\n", comparedRank > 0 ? h->idx : (hands + mid)->idx);

//         if(comparedRank < 0)
//         {
//             // handToInsert has lower rank than handInArray, cut to left half
//             highIdx = mid - 1;            
//         }
//         else
//         {
//             // handToInsert has higher rank than handInArray, cut to right half
//             lowIdx = mid + 1;
//         }

//         mid = lowIdx + (highIdx - lowIdx) / 2;
//     }

//     return mid;
// }

int FindIndex(Hand *h, Hand *hands, int nbrHands)
{
    if(nbrHands == 0)
    {
        return 0;
    }

    for (size_t i = 0; i < nbrHands; i++)
    {
        int comparedRank = CompareHandsRank(h, hands+i);
        if(comparedRank > 0)
        {
            return i;
        }
    }

    return nbrHands;
}

void InsertIntoArray(Hand *h, Hand *hands, int idx, int nbrHands)
{
    if(nbrHands > 0)
    {
        for (size_t i = nbrHands; i > idx; i--)
        {
            hands[i] = hands[i - 1];
        }
    }

    hands[idx] = *h;
}

void AddHand(Hand *h, Hand *hands, int nbrHands)
{
    // Find what index to insert the hand at:
    // int idx = FindIndexBinarySearch(h, hands, nbrHands);
    int idx = FindIndex(h, hands, nbrHands);
    // printf("\tHand %d should be inserted into index %d\n", h->idx, idx);

    // Insert into array:
    InsertIntoArray(h, hands, idx, nbrHands);
}

int main(int argc, char* args[])
{
    FILE *f = fopen(args[1], "r");

    sscanf(args[2], "%d", &problemNbr);
    printf("Problem nbr: %d\n", problemNbr);
    int nbrHands = 0;
    int totalWinnings = 0;
    Hand hands[1010];

    char handString[25];

    printf("Hand\tCards\tBid\tTypeScore\n");

    
    while(fgets(handString, sizeof(handString), f) != NULL)
    {
        // Parse hand
        Hand h;
        h.idx = nbrHands;
        char *token = strtok(handString, " ");
        memcpy(h.cards, token, 5);

        token = strtok(NULL, "\n");
        sscanf(token, "%d", &h.bid);
        h.typeScore = CalculateHandTypeScore(&h);

        printf("%d\t%.5s\t%d\t%d\n", h.idx, h.cards, h.bid, h.typeScore);
        AddHand(&h, hands, nbrHands);
        nbrHands++;
    }

    printf("\n\nAfter sorting:\n");
    printf("Hand\tCards\tBid\tTypeScore\tTotal\n");

    int sum = 0;

    for (size_t i = 0; i < nbrHands; i++)
    {
        Hand h = hands[i];
        int totalScore = (nbrHands - i) * h.bid;
        sum += totalScore;
        printf("%d\t%.5s\t%d\t%d\t\t%d\n", h.idx, h.cards, h.bid, h.typeScore, totalScore);
    }

    printf("\n");
    printf("Problem %d: Total winnings = %d\n", problemNbr, sum);    

    fclose(f);
    return 0;
}