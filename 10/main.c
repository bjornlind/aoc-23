#include <stdio.h>
#include <stdbool.h>

#define MAXCOLS 150
#define MAXROWS 150

typedef struct Point
{
    int row;
    int col;
} Point;

char chars[MAXROWS][MAXCOLS];

bool GetNextPoint(Point *prevPoint, Point *currPoint, Point *nextPoint)
{
    char currChar = chars[currPoint->row][currPoint->col];
    printf("\tCurrent char = %c\n", currChar);

    switch (chars[currPoint->row][currPoint->col])
    {
    case '|':
        if(prevPoint->row > currPoint->row)
        {
            nextPoint->row = currPoint->row - 1;
            nextPoint->col = currPoint->col;
        }
        else
        {
            nextPoint->row = currPoint->row + 1;
            nextPoint->col = currPoint->col;
        }
        break;
    
    case '-':
        if(prevPoint->col < currPoint->col)
        {
            nextPoint->row = currPoint->row;
            nextPoint->col = currPoint->col + 1;
        }
        else
        {
            nextPoint->row = currPoint->row;
            nextPoint->col = currPoint->col - 1;
        }

        break;

    case 'L':
        if(prevPoint->row < currPoint->row)
        {
            nextPoint->row = currPoint->row;
            nextPoint->col = currPoint->col + 1;
        }
        else
        {
            nextPoint->row = currPoint->row - 1;
            nextPoint->col = currPoint->col;
        }
        break;

    case 'J':
        if(prevPoint->row < currPoint->row)
        {
            nextPoint->row = currPoint->row;
            nextPoint->col = currPoint->col - 1;
        }
        else
        {
            nextPoint->row = currPoint->row - 1;
            nextPoint->col = currPoint->col;
        }
        break;

    case '7':
        if(prevPoint->row == currPoint->row)
        {
            nextPoint->row = currPoint->row + 1;
            nextPoint->col = currPoint->col;
        }
        else
        {
            nextPoint->row = currPoint->row;
            nextPoint->col = currPoint->col - 1;
        }
        break;

    case 'F':
        if(prevPoint->row > currPoint->row)
        {
            nextPoint->row = currPoint->row;
            nextPoint->col = currPoint->col + 1;
        }
        else
        {
            nextPoint->row = currPoint->row + 1;
            nextPoint->col = currPoint->col;
        }
        break;

    case 'S':
        nextPoint->row = currPoint->row;
        nextPoint->col = currPoint->col;
        break;

    default:
        return false;
    }

    return true;
}

bool ComparePoints(Point *p1, Point *p2)
{
    // printf("P1 = (%d, %d), P2 = (%d, %d)\n", p1->row, p1->col, p2->row, p2->col);
    if(p1->row == p2->row && p1->col == p2->col)
    {
        // printf("\tP1 and P2 are equal!\n");
        return true;
    }

    return false;
}

int NbrStepsToLoop(Point *startPoint, Point *startSearchPoint)
{
    // Calculate how many steps to go around the loop once:
    int nbrSteps = 1;
    Point prevPoint = {startPoint->row, startPoint->col};
    Point currPoint = {startSearchPoint->row, startSearchPoint->col};
    Point nextPoint;

    do
    {
        nbrSteps++;

        if(GetNextPoint(&prevPoint, &currPoint, &nextPoint) == false)
        {
            nbrSteps = -1;
            break;
        }
        printf("Next point = (%d, %d) = %c\n", nextPoint.row, nextPoint.col, chars[nextPoint.row][nextPoint.col]);

        prevPoint.row = currPoint.row;
        prevPoint.col = currPoint.col;
        currPoint.row = nextPoint.row;
        currPoint.col = nextPoint.col;
    } while(ComparePoints(startPoint, &currPoint) == false);

    return nbrSteps;
}

bool IsValidStartPoint(Point *searchPoints, char *validStartChars, int idx, int nbrValids)
{
    Point searchPoint = searchPoints[idx];
    char c = chars[searchPoint.row][searchPoint.col];

    for (size_t i = 0; i < nbrValids; i++)
    {
        if(validStartChars[i] == c)
        {
            return true;
        }
    }

    return false;    
}

int main(int argc, char const *argv[])
{
    FILE *f = fopen(argv[1], "r");

    char line[MAXCOLS];
    int rows = 0;
    int columns = 0;

    Point startPoint;
    int startRow = -1;
    int startCol = -1;

    int currCol = 0;
    char c;

    while((c = fgetc(f)) != EOF)
    {
        if(c == '\n')
        {
            rows++;
            columns = currCol + 1;
            currCol = 0;
            // printf("\n");
        }
        else
        {
            if(c == 'S')
            {
                startPoint.row = rows;
                startPoint.col = currCol;
            }
            
            // printf("%c", c);
            chars[rows][currCol] = c;
            currCol++;
        }
    }

    int maxLoopLength = 0;

    Point p1 = {startPoint.row, startPoint.col + 1};
    Point p2 = {startPoint.row + 1, startPoint.col};
    Point p3 = {startPoint.row, startPoint.col - 1};
    Point p4 = {startPoint.row - 1, startPoint.col};
    Point searchPoints[] = {p1, p2, p3, p4};

    char validSearchPointStarts[4][3] = {{'-', 'J', '7'}, {'|', 'L', 'J'}, {'-', 'L', 'F'}, {'|', 'F', '7'}};

    for (size_t i = 0; i < 4; i++)
    {
        Point p = searchPoints[i];
        if(IsValidStartPoint(searchPoints, validSearchPointStarts[i], i, 3))
        {
            printf("Start point = (%d, %d) :: Search point = (%d, %d)\n", startPoint.row, startPoint.col, p.row, p.col);
            if(chars[p.row][p.col] != '.' && p.row >= 0 && p.col >= 0)
            {
                int loopLength = NbrStepsToLoop(&startPoint, &p);
                maxLoopLength = loopLength > maxLoopLength ? loopLength : maxLoopLength;
                printf("Loop length = %d\n", loopLength);
            }
            
            printf("\n");
        }
    }
    
    printf("\n");
    printf("Longest loop = %d\n", maxLoopLength);
    printf("Furthest point = %d\n", maxLoopLength / 2);


    printf("\n");



    // printf("Current point = (%d, %d) = %c\n", currPoint.row, currPoint.col, chars[currPoint.row][currPoint.col]);
    // printf("Next point = (%d, %d) = %c\n", nextPoint.row, nextPoint.col, chars[nextPoint.row][nextPoint.col]);

    fclose(f);
    return 0;
}
