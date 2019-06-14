#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>

#define N (20)
#define M (20)
#define L (N * M)
#define True 1
#define False 0

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

typedef struct mark
{
    int x;
    int y;
} bodySnake;

char scoreFile[] = "score.txt";

void drawMap(bodySnake snake[], char map[][M], const int headIndex,
             const int endIndex, const int food_x, const int food_y, int score, const int *maxScore);
void initSnake();
void snakeMove(bodySnake snake[], char map[][M], int *headIndex, int *endIndex, int direction[], int *food_x, int *food_y, int *score, int *maxScore, int *pause);

void move(bodySnake snake[], int *headIndex, int direction[], const int snakeHead);
void artificialMove(bodySnake snake[], int *headIndex,
                    int *endIndex, int direction[], char click, int snakeHead, int *pause);
int snakeEatFood(bodySnake snake[], const int headIndex,
                 const int food_x, const int food_y, int *score, int *maxScore);
void markMap(bodySnake snake[], char map[][M], const int headIndex,
             const int endIndex, const int food_x, const int food_y);
void foodConflictSnake(bodySnake snake[], int *food_x, int *food_y,
                       const int headIndex, const int endIndex);
void gameOver(int score);
void snakeDead(bodySnake snake[], int headIndex, int endIndex, int *active);
int viewScore(const char scoreFile[]);
void updateScore(int score, int *maxScore);
int readMaxScore(const char scoreFile[]);
void help();
void returnMenu();
void menu();
void initFile(const char scoreFile[]);

int main()
{
    menu();
    return 0;
}

void initSnake()
{

    //地图
    char map[N][M] = {0};
    //初始蛇的身体
    bodySnake snake[L]; //装蛇的数组
    snake[0].x = 3, snake[0].y = 3;
    snake[1].x = 3, snake[1].y = 4;
    snake[2].x = 3, snake[2].y = 5;

    map[3][3] = 1, map[3][4] = 1, map[3][5] = 1;

    int endIndex = 0;  //蛇尾
    int headIndex = 2; //蛇头

    //随机食物
    srand((unsigned)time(NULL));
    int food_x = rand() % N;
    int food_y = rand() % M;

    int direction[4] = {0, 0, 0, 1};
    int maxScore = readMaxScore(scoreFile);
    int score = 0;
    int active = True;
    int pause = False;
    while (active)
    {
        drawMap(snake, map, headIndex, endIndex, food_x, food_y, score, &maxScore);
        snakeMove(snake, map, &headIndex, &endIndex,
                  direction, &food_x, &food_y, &score, &maxScore, &pause);
        snakeDead(snake, headIndex, endIndex, &active);
    }
    gameOver(score);
}

void menu()
{
    system("cls");
    initFile(scoreFile);
    printf("*****************************************\n");
    printf("*                                       *\n");
    printf("*                1.开始游戏.            *\n");
    printf("*                2.查看帮助.            *\n");
    printf("*                3.查看历史最高分.      *\n");
    printf("*                                       *\n");
    printf("*****************************************\n");
    int choose;
    scanf("%1d", &choose);
    while (getchar() != '\n')
    {
        continue;
    }

    if (choose == 1)
    {
        initSnake();
    }
    else if (choose == 2)
    {
        help();
        returnMenu();
    }
    else if (choose == 3)
    {
        viewScore(scoreFile);
        returnMenu();
    }
    else
    {
        menu();
    }
}

void help()
{
    system("cls");
    printf("w s a d对应上下左右，空格键暂停，q直接退出（不保存分数）\n");
    printf("祝你好运！\n");
}

void returnMenu()
{
    int choose;
    printf("\n\n1.返回菜单\t\t\t2.退出\n");
    scanf("%1d", &choose);
    while (getchar() != '\n')
    {
        continue;
    }
    menu();
}

void initFile(const char scoreFile[])
{
    FILE *fp;
    if ((fp = fopen(scoreFile, "r")) == NULL)
    {
        int score = 0;
        fp = fopen(scoreFile, "w");
        fprintf(fp, "%d\n", score);
    }
    fclose(fp);
}

int viewScore(const char scoreFile[])
{
    system("cls");
    int score;
    FILE *fp;
    if ((fp = fopen(scoreFile, "r")) != NULL)
    {
        fscanf(fp, "%d", &score);
        printf("你的历史最高分为%d.", score);
    }
    fclose(fp);
    return score;
}

int readMaxScore(const char scoreFile[])
{
    int score = 0;
    FILE *fp;
    if ((fp = fopen(scoreFile, "r")) != NULL)
    {
        fscanf(fp, "%d", &score);
    }
    else
    {
        printf("打开失败！");
    }
    fclose(fp);
    return score;
}

void markMap(bodySnake snake[], char map[][M], const int headIndex,
             const int endIndex, const int food_x, const int food_y)
{
    if (headIndex > endIndex)
    {
        for (int i = endIndex; i <= headIndex; i++)
        {
            map[snake[i].x][snake[i].y] = 1;
        }
    }
    else
    {
        for (int i = endIndex; i < L; i++)
        {
            map[snake[i].x][snake[i].y] = 1;
        }
        for (int i = 0; i <= headIndex; i++)
        {
            map[snake[i].x][snake[i].y] = 1;
        }
    }
    map[food_x][food_y] = 2;
}

void drawMap(bodySnake snake[], char map[][M], const int headIndex,
             const int endIndex, const int food_x, const int food_y, int score, const int *maxScore)
{
    markMap(snake, map, headIndex, endIndex, food_x, food_y);
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            if (map[i][j] == 0)
            {
                printf("--");
            }
            else if (map[i][j] == 1)
            {
                printf("★");
            }
            else if (map[i][j] == 2)
            {
                printf("●");
            }
        }
        putchar(10);
    }
    printf("你的当前分数：%d.\t\t\t历史最高分为：%d\n", score, *maxScore);
    Sleep(40);
    system("cls");
}

void snakeMove(bodySnake snake[], char map[][M], int *headIndex, int *endIndex, int direction[], int *food_x, int *food_y, int *score, int *maxScore, int *pause)
{

    int snakeHead = (*headIndex + 1) % L;
    if (kbhit())
    {
        char click = getch();
        artificialMove(snake, headIndex, endIndex, direction, click, snakeHead, pause);
    }
    else if (!*pause)
    {
        move(snake, headIndex, direction, snakeHead);
    }
    //是否吃了食物
    int eat = snakeEatFood(snake, *headIndex, *food_x, *food_y, score, maxScore);
    if (eat)
    {
        *food_x = rand() % N;
        *food_y = rand() % M;
        foodConflictSnake(snake, food_x, food_y, *headIndex, *endIndex);
    }
    //没吃到食物且没暂停，才去掉尾巴。即暂停或吃到食物才不去掉尾巴
    else if (!eat && !*pause)
    {
        int x = snake[*endIndex].x;
        int y = snake[*endIndex].y;
        //去掉尾巴
        map[x][y] = 0;
        //尾巴前行
        *endIndex = (*endIndex + 1) % L;
    }
}

void move(bodySnake snake[], int *headIndex, int direction[], const int snakeHead)
{
    snake[snakeHead].x = (snake[*headIndex].x + direction[UP] + direction[DOWN] + N) % N;
    snake[snakeHead].y = (snake[*headIndex].y + direction[LEFT] + direction[RIGHT] + M) % M;
    *headIndex = snakeHead;
}

void artificialMove(bodySnake snake[], int *headIndex,
                    int *endIndex, int direction[], char click, int snakeHead, int *pause)
{
    if ((click == 'w' || click == 'W') && direction[DOWN] != 1)
    {
        direction[UP] = -1, direction[DOWN] = 0, direction[LEFT] = 0, direction[RIGHT] = 0;
    }
    else if ((click == 's' || click == 'S') && direction[UP] != -1)
    {
        direction[UP] = 0, direction[DOWN] = 1, direction[LEFT] = 0, direction[RIGHT] = 0;
    }
    else if ((click == 'a' || click == 'A') && direction[RIGHT] != 1)
    {
        direction[UP] = 0, direction[DOWN] = 0, direction[LEFT] = -1, direction[RIGHT] = 0;
    }
    else if ((click == 'd' || click == 'D') && direction[LEFT] != -1)
    {
        direction[UP] = 0, direction[DOWN] = 0, direction[LEFT] = 0, direction[RIGHT] = 1;
    }
    else if (click == ' ')
    {
        if (*pause == False)
        {
            *pause = True;
        }
        else
        {
            *pause = False;
        }
    }
    else if (click == 'q')
    {
        exit(0);
    }
    if (!*pause)
    {
        move(snake, headIndex, direction, snakeHead);
    }
}

int snakeEatFood(bodySnake snake[], const int headIndex,
                 const int food_x, const int food_y, int *score, int *maxScore)
{
    if (snake[headIndex].x == food_x && snake[headIndex].y == food_y)
    {
        *score += 10;
        updateScore(*score, maxScore);
        return True;
    }
    return False;
}

void updateScore(int score, int *maxScore)
{

    if (score > *maxScore)
    {
        *maxScore = score;
    }
}

void foodConflictSnake(bodySnake snake[], int *food_x, int *food_y,
                       const int headIndex, const int endIndex)
{
    int conflict = False;
    do
    {
        conflict = False;
        if (headIndex > endIndex)
        {
            for (int i = endIndex; i <= headIndex; i++)
            {
                if (*food_x == snake[i].x && *food_y == snake[i].y)
                {
                    conflict = True;
                    break;
                }
            }
        }
        else
        {
            for (int i = endIndex; i < L; i++)
            {
                if (*food_x == snake[i].x && *food_y == snake[i].y)
                {
                    conflict = True;
                    break;
                }
            }
            for (int i = 0; i <= headIndex; i++)
            {
                if (*food_x == snake[i].x && *food_y == snake[i].y)
                {
                    conflict = True;
                    break;
                }
            }
        }
        if (conflict)
        {
            *food_x = rand() % N;
            *food_y = rand() % M;
        }
    } while (conflict);
}

void snakeDead(bodySnake snake[], int headIndex, int endIndex, int *active)
{
    if (headIndex > endIndex)
    {
        for (int i = endIndex; i < headIndex; i++)
        {
            if (snake[i].x == snake[headIndex].x &&
                snake[i].y == snake[headIndex].y)
            {
                *active = False;
                break;
            }
        }
    }
    else
    {
        for (int i = endIndex; i < L; i++)
        {
            if (snake[i].x == snake[headIndex].x &&
                snake[i].y == snake[headIndex].y)
            {
                *active = False;
                break;
            }
        }
        for (int i = 0; i < headIndex; i++)
        {
            if (snake[i].x == snake[headIndex].x &&
                snake[i].y == snake[headIndex].y)
            {
                *active = False;
                break;
            }
        }
    }
}

void gameOver(int score)
{
    int historyScore = readMaxScore(scoreFile);
    if (score > historyScore)
    {
        FILE *fp;
        fp = fopen(scoreFile, "w");
        fprintf(fp, "%d\n", score);
        fclose(fp);
        printf("\t\t\t恭喜你打破了历史记录！\t\t\t\n");
    }
    printf("\t\t\tGame Over!\t\t\t\n");
}
