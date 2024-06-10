#include <stdio.h>
#include <stdlib.h>                         // 동적 메모리 할당 함수 사용을 위한 헤더 파일

// 달팽이 배열을 채우는 함수
void fill_spiral_array(int **arr, int row, int col)
{
    int num    = 0;                         // 배열에 채울 숫자
    int x      = 0;                         // 현재 위치를 나타냄, [0][y]
    int y      = -1;                        // 현재 위치를 나타냄, [x][-1]
    int target = 1;                         // 방향을 나타내는 변수, 1이면 오른쪽 또는 아래, -1이면 왼쪽 또는 위로 이동
    int maxRow = row;                       // 채워야 할 행의 최대 개수
    int maxCol = col;                       // 채워야 할 열의 최대 갯수 

    while ((maxCol > 0) && (maxRow > 0))    // 채워야 할 행,열이 남아 있을 때 까지 반복 
    {
        for (int i = 0; i < maxCol; i++)    // 현재 행에서 열을 따라 이동하면서 값 채우기, t가 1이면 오른쪽, -1이면 왼쪽 이동
        {
            y += target;                    // y값을 증가 또는 감소 시켜 이동
            arr[x][y] = num++;
        }
        maxRow--;                           // 한 줄 채웠으면 남은 행 수 줄이기 

        for (int i = 0; i < maxRow; i++)    // 현재 열에서 행을 따라 이동하면서 값 채우기, t가 1이면 아래, -1이면 위로 이동
        {
            x += target;                    // x값을 증가 또는 감소 시켜 이동
            arr[x][y] = num++;
        }
        maxCol--;                           // 한 줄을 채웠으면 남은 열 수 줄이기

        target = -target;                   // 방향 반대 바꾸기
    }
}

// 구분선 출력하는 함수
void print_line(int col)
{
    for (int i = 0; i < col; i++) 
    {
        printf("----");
    }
    printf("\n");    
}

// 배열 출력하는 함수
void print_array(int **arr, int row, int col)
{
    // ---- 출력
    print_line(col);

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
    print_line(col);
}

// 입력 버퍼 비우는 함수
void clear_input_buffer(void)
{
    while (getchar () != '\n');
}

// 유효한 정수를 입력받는 함수
int get_valid_integer(char *prompt)
{
    char c    = '\n';
    int value = 0;

    while (1)
    {
        printf("%s", prompt);                                                           // 전달된 문자열 row? 또는 col?
        if ((scanf("%d%c", &value, &c) == 2) && (c == '\n') && (value > 0))             // 2개의 값을 읽어 2를 반환, 문자가 \n, 정수가 양수여야 하는 조건문
        {
            if((value > 0) &&(value <= 10))                                             // 유효범위 1~10 설정, 최대 10행 10열까지 가능
            {
                return value;
            }
            else
            {
                printf("Error, input integer too big!\nPlease, input integer 1~10.\n"); // 10 이상 입력 시 에러 출력 
            }
        }
        else
        {
            printf("Error, invalid input!\nPlease, input positive integer.\n");         // 위 조건을 만족하지 않으면 에러 출력
        }
        clear_input_buffer();
    }
}

int main(void)
{
    // 사용자로부터 행과 열 입력 받기
    int row = get_valid_integer("row? ");
    int col = get_valid_integer("col? ");

    // 동적 메모리 할당
    int **arr = (int **)malloc(sizeof(int *) * row);
    for (int i = 0; i < row; i++)
    {
        arr[i] = (int *)malloc(sizeof(int) * col);
    }

    // 배열을 달팽이 모양으로 채우기
    fill_spiral_array(arr, row, col);

    // 배열 출력
    print_array(arr, row, col);

    // 동적 메모리 해제
    for (int i = 0; i < row; i++)
    {
        free(arr[i]);
    }
    free(arr);

    return 0;
}