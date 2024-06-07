#include <stdio.h>
#include <stdlib.h>                         // 동적 메모리 할당 함수 사용을 위한 헤더 파일

// 달팽이 배열을 채우는 함수
void fillSpiralArray(int **arr, int row, int col)
{
    int num    = 0;                         // 배열에 채울 숫자
    int x      = 0;                         // 현재 위치를 나타냄, [0][y]
    int y      = -1;                        // 현재 위치를 나타냄, [x][-1]
    int t      = 1;                         // 방향을 나타내는 변수, 1이면 오른쪽 또는 아래, -1이면 왼쪽 또는 위로 이동
    int maxRow = row;                       // 채워야 할 행의 최대 개수
    int maxCol = col;                       // 채워야 할 열의 최대 갯수 

    while (maxCol > 0 && maxRow > 0)        // 채워야 할 행,열이 남아 있을 때 까지 반복 
    {
        for (int i = 0; i < maxCol; i++)    // 현재 행에서 열을 따라 이동하면서 값 채우기, t가 1이면 오른쪽, -1이면 왼쪽 이동
        {
            y += t;                         // y값을 증가 또는 감소 시켜 이동
            arr[x][y] = num++;
        }
        maxRow--;                           // 한 줄 채웠으면 남은 행 수 줄이기 

        for (int i = 0; i < maxRow; i++)    // 현재 열에서 행을 따라 이동하면서 값 채우기, t가 1이면 아래, -1이면 위로 이동
        {
            x += t;                         // x값을 증가 또는 감소 시켜 이동
            arr[x][y] = num++;
        }
        maxCol--;                           // 한 줄을 채웠으면 남은 열 수 줄이기

        t = -t;                             // 방향 반대 바꾸기
    }
}

// 구분선 출력하는 함수
void printLine(int col)
{
    for (int i = 0; i < col; i++) 
    {
        printf("----");
    }
    printf("\n");    
}

// 배열 출력하는 함수
void printArray(int **arr, int row, int col)
{
    // ---- 출력
    printLine(col);
    // 2차원 배열 출력하는 반복문
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            printf("%3d ", arr[i][j]);
        }
        printf("\n");
    }
    // ---- 출력
    printLine(col);
}

int main(void)
{
    int row, col;

    // 사용자로부터 행과 열 입력 받기
    printf("row? ");
    scanf("%d", &row);
    printf("col? ");
    scanf("%d", &col);

    // 동적 메모리 할당
    int **arr = (int **)malloc(sizeof(int *) * row);
    for (int i = 0; i < row; i++)
    {
        arr[i] = (int *)malloc(sizeof(int) * col);
    }

    // 배열을 달팽이 모양으로 채우기
    fillSpiralArray(arr, row, col);

    // 배열 출력
    printArray(arr, row, col);

    // 동적 메모리 해제
    for (int i = 0; i < row; i++)
    {
        free(arr[i]);
    }
    free(arr);

    return 0;
}