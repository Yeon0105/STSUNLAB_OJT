#include <stdio.h>

#define LCD_WIDTH 4                          // 가로 4
#define LCD_HEIGHT 8                         // 세로 8
#define DRAW_MODE 1

int display_buffer[LCD_HEIGHT * LCD_WIDTH];  // display buffer

void fill_display_buffer()
{
    // DRAW_MODE 상태라면 조건문 실행
    #if DRAW_MODE
        // 그림 모드 : 'ㄱ' 모양 데이터 삽입
        for (int i = 0; i < LCD_HEIGHT * LCD_WIDTH; i++)
        {
            // 첫 행과 마지막 열 위치인지 확인하는 조건문
            if (i < LCD_WIDTH || i % LCD_WIDTH == LCD_WIDTH - 1)
            {
                display_buffer[i] = 11;
            }
            else
            {
                display_buffer[i] = 0;
            }
        }
    // DRAW_MODE 상태가 아니면 조건문 실행
    #else
        // 숫자 모드 : 1부터 32까지 채우는 함수
        int number = 1;
            for (int i = 0; i < LCD_HEIGHT * LCD_WIDTH; i++)
            {
                display_buffer[i] = number++;
            }
    #endif
}

// 원본
void print_normal()
{
    for (int i = 0; i < LCD_HEIGHT; i++)
    {
        for (int j = 0; j < LCD_WIDTH; j++)
        {
            int index = (i * LCD_WIDTH) + j;       // 01 = [0]
            printf("%02d", display_buffer[index]); // 2자리 수로 정렬하여 출력, 행마다 줄바꿈 수행
        }
        printf("\n");
    }
    printf("\n");
}

// 90도 회전
void print_rotate_90()
{
    for (int i = 0; i < LCD_WIDTH; i++)
    {
        for (int j = LCD_HEIGHT - 1; j >= 0; j--)  // 역순 탐색
        {
            int index = (j * LCD_WIDTH) + i;       // 29 = [28]
            printf("%02d", display_buffer[index]);
        }
        printf("\n");
    }
    printf("\n");
}

// 180도 회전
void print_rotate_180()
{
    for (int i = LCD_HEIGHT - 1; i >= 0; i--)
    {
        for (int j = LCD_WIDTH - 1; j >= 0; j--)
        {
            int index = (i * LCD_WIDTH) + j;     // 32 = [31]
            printf("%02d", display_buffer[index]);
        }
        printf("\n");
    }
    printf("\n");
}

// 270도 회전
void print_rotate_270()
{
    for (int i = LCD_WIDTH - 1; i >= 0; i--)
    {
        for (int j = 0; j < LCD_HEIGHT; j++)
        {
            int index = (j * LCD_WIDTH) + i;     // 04 = [3]
            printf("%02d", display_buffer[index]);
        }
        printf("\n");
    }
    printf("\n");
}

// 매개변수에 따라 함수 호출
void print_display_buffer(int rotation)
{
    if (rotation == 0)
    {
        print_normal();
    }
    else if (rotation == 90)
    {
        print_rotate_90();
    }
    else if (rotation == 180)
    {
        print_rotate_180();
    }
    else if (rotation == 270)
    {
        print_rotate_270();
    }
}

// 메인함수
int main()
{
    fill_display_buffer();

    printf("> normal\n");     // 원본 출력
    print_display_buffer(0);

    printf("> rotate 90\n");  // 90도 회전 출력
    print_display_buffer(90);

    printf("> rotate 180\n");  // 180도 회전 출력
    print_display_buffer(180);

    printf("> rotate 270\n");  // 270도 회전 출력
    print_display_buffer(270);
    return 0;
}