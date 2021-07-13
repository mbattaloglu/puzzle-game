#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char* word;
    char* clue;
    int x;
    int y;
    char direction;
    int f;
}Word_t;

Word_t* loadTextFile(FILE *fPtr, int nrWords);
char** createArray(int rows, int cols);
char** createBoard(int rows, int cols, int nrWords, char** myBoard, Word_t *words);
char** updateBoard(char** myBoard, Word_t *words, int solve);
void displayBoard(int rows, int cols, char** myBoard);
int isBoardFilled(int rows, int cols, char** myBoard);
void playGame(char** myBoard, int nrWords, Word_t *words, int x, int y, int countToFinish);
void displayQuestions(int nrWords, Word_t *words);
int findIndex(int arr[], int size, int val);

int main()
{
    char filename[150];
    printf("Enter filename:");
    scanf("%s",filename);

    int rows;
    int cols;
    int number_of_questions;

    char line[150];
    FILE *file;

    file = fopen(filename,"r");
    if (file == NULL)
    {
        printf("Wrong Filename, Try Again...\n");
        return 0;
    }
    else
    {
        fscanf(file,"%[^\n]\n",&line);  // Reading row, col and number of questions.
    }
    
    
    rows = atoi(strtok(line," "));
    cols = atoi(strtok(NULL, " "));
    number_of_questions = atoi(strtok(NULL, " "));

    int countToFinish = number_of_questions;

    printf("\n\nGame is %d rows x %d cols with %d words.\n",rows,cols,number_of_questions);
    
    Word_t *words = loadTextFile(file,number_of_questions);
    char **board = createArray(rows,cols);
    board = createBoard(rows,cols,number_of_questions,board,words);
    printf("Words and clues are read!\n");
    printf("Current Puzzle:\n\n");
    displayBoard(rows,cols,board);
    
    playGame(board,number_of_questions,words,rows,cols,countToFinish);
    
    
    return 0;
}

Word_t* loadTextFile(FILE *file, int nrWords)
{
    char line[150];
    size_t user = 4096;
    Word_t *words = (Word_t*)malloc(nrWords * user);
    
    for (int i = 0; i < nrWords; i++)
    {
        
        char *clue = (char*)malloc(user);
        char *word = (char*)malloc(user);

        fscanf(file,"%[^\n]\n",&line);
        //Splitting words
        char *piece = strtok(line," "); 
        (words+i)->f = 0;
        (words+i)->direction = piece[0];
        piece = strtok(NULL," ");
        (words+i)->x = atoi(piece);
        piece = strtok(NULL, " ");
        (words+i)->y = atoi(piece);
        piece = strtok(NULL, " ");
        strcpy(word,piece);
        (words+i)->word = word;
        piece = strtok(NULL, " ");
        strcpy(clue,piece);
        

        while (piece = strtok(NULL, " "))
        {
            strcat(clue, " ");
            strcat(clue,piece);
        }
        
        (words+i)->clue = clue;
        
    }

    fclose(file);

    return words;
}

char** createArray(int rows, int cols)
{
    char **arr = (char **)malloc(rows * sizeof(char*));
    for (int i = 0; i < rows; i++)
    {
        arr[i] = (char *)malloc(cols*sizeof(char));
    }
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            arr[i][j] = '#';
        }   
    }
    return arr;
}

void displayBoard(int rows, int cols, char** board)
{
    printf("     ");
    for (int i = 0; i < cols; i++)
    {
        printf("  %d ",i+1);
    }
    printf("\n     ");
    for (int i = 0; i < cols; i++)
    {
        printf("  --");
    }
    printf("\n");
    for (int i = 0 ; i < cols ; i++)
    {
        printf("%d    | ",i+1);
        for (int j = 0; j < rows; j++)
        {   
            printf("%c   ",board[i][j]);
        }
        printf("\n");   
    } 
    printf("     ********************\n");
}

char** createBoard(int rows, int cols, int nrWords, char** myBoard, Word_t *words)
{
    for(int i = 0 ; i < nrWords ; i++)
    {
        char direction = (words+i)->direction;
        int length = strlen((words+i)->word);
        int x = (words+i)->x;
        int y = (words+i)->y;
        if (direction == 'H')
        {
            for (int j = 0; j < length; j++) 
            {
                myBoard[x-1][y-1+j] = '_';
            }
        }
        else if (direction == 'V')
        {
            for (int j = 0; j < length; j++)
            {
                myBoard[x-1+j][y-1] = '_';
            }
            
        }
    }
    return myBoard;
}

char** updateBoard(char** myBoard, Word_t *words, int solve)
{
    char *word = (words+solve)->word;
    char direction = (words+solve)->direction;
    int x = (words+solve)->x;
    int y = (words+solve)->y;
    int length = strlen(word);
    if (direction == 'H')
    {
        for (int i = 0; i < length; i++)
        {
            char letter = word[i];
            myBoard[x-1][y-1+i] = letter;
        }
    }
    else if (direction == 'V')
    {
        for (int i = 0; i < length; i++)
        {
            char letter = word[i];
            myBoard[x-1+i][y-1] = letter;
        }
    }
    return myBoard;
}

int isBoardFilled(int rows, int cols, char** myBoard)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (myBoard[i][j] == '_')
            {
                return 0;
            }
        }
    }
    return 1;
}

void playGame(char** myBoard, int nrWords, Word_t *words, int x, int y, int countToFinish)
{
    int questionNumber = 0;
    while (1)
    {   
        printf("Ask For Hint:\n");
        displayQuestions(nrWords, words);
        printf("Enter -1 to exit\n");
        printf("Which word to solve next?: ");

        int test = scanf("%d",&questionNumber);
        int temp;
        while (test != 1)
        {
            while((temp=getchar()) != EOF && temp != '\n');
		    printf("Invalid input... please enter a number: ");
		    test = scanf("%d", &questionNumber);
        }
        
        
        if (questionNumber == -1)
        {
            printf("Game finished...\n");
            break;
        }
        
        questionNumber--;
        if ((words+questionNumber)->f == 1)
        {
            printf("\n\nThat word has been solved before!\n\n");  
        }
        else
        {
            char *clue = (words+questionNumber)->clue;
            printf("Hint : %s",clue);
            printf("\nEnter your solution:");
            char solution[5];
            scanf("%s",solution);
            strlwr(solution);
            char *word = (words+questionNumber)->word;
            strlwr(word);
            if (!strcmp(solution,word))
            {
                (words+questionNumber)->f = 1;
                strupr(word);
                updateBoard(myBoard,words,questionNumber);
                printf("\nCorrect!\n\n");
            }
            else
            {
                printf("\nWRONG ANSWER\n\n");
            }
        
            printf("Current Puzzle:\n\n");
            displayBoard(x,y,myBoard);
            if (isBoardFilled(x,y,myBoard))
            {
                printf("Congratulations, you beat the puzzle\n");
                break;
            }
        }
    }
}

void displayQuestions(int nrWords, Word_t *words)
{
    printf("#  Direction    Row   Col\n");
    printf("--------------------------\n");
    for (int  i = 0; i < nrWords; i++)
    {
        char *direction = (words+i)->direction == 'H' ? "Horizontal" : "Vertical  ";
        if ((words+i)->f == 0)
        {
            printf("%d: %s    %d    %d\n",i+1,direction,(words+i)->x,(words+i)->y);
        }   
    }
}

int findIndex(int arr[], int size, int val)
{
    for(int i = 0 ; i < size ; i++)
    {
        if (arr[i] = val)
        {
            return 1;
        }
    }
    return 0;
}

