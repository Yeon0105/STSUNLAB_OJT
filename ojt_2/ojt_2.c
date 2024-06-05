#include <stdio.h>

//#define DRAW_MODE                           // DRAW_MODE 매크로 

#define LCD_WIDTH    4                        // 가로 4
#define LCD_HEIGHT   8                        // 세로 8

int display_buffer[LCD_HEIGHT * LCD_WIDTH];    // display buffer

void fill_display_buffer(void)
{
#ifdef DRAW_MODE
    // DRAW_MODE 매크로가 정의되어 있으면 조건문 실행
    // 그림 모드 : 'ㄱ' 모양 데이터 삽입
    for (int i = 0; i < (LCD_HEIGHT * LCD_WIDTH); i++)
    {
        // 첫 행과 마지막 열 위치인지 확인하는 조건문
        if ((i < LCD_WIDTH) || ((i % LCD_WIDTH) == (LCD_WIDTH - 1)))
        {
            display_buffer[i] = 11;
        }
        else
        {
            display_buffer[i] = 0;
        }
    }
#else
    // DRAW_MODE 매크로가 정의되어 않으면 조건문 실행
    // 숫자 모드 : DRAW_MODE가 아닐 때는 항상 00부터 출력
    for (int i = 0; i < (LCD_HEIGHT * LCD_WIDTH); i++)
    {
        display_buffer[i] = i;
    }
#endif
}

// 원본
void print_normal(void)
{
    for (int i = 0; i < LCD_HEIGHT; i++)
    {
        for (int j = 0; j < LCD_WIDTH; j++)
        {
            printf("%02d", display_buffer[(i * LCD_WIDTH) + j]);  // 2자리 수로 정렬하여 출력, 행마다 줄바꿈 수행, 01 = [0]
        }
        printf("\n");
    }
    printf("\n");
}

// 90도 회전
void print_rotate_90(void)
{
    for (int i = 0; i < LCD_WIDTH; i++)
    {
        for (int j = LCD_HEIGHT - 1; j >= 0; j--)                 // 역순 탐색
        {
            printf("%02d", display_buffer[(j * LCD_WIDTH) + i]);  // 29 = [28]
        }
        printf("\n");
    }
    printf("\n");
}

// 180도 회전
void print_rotate_180(void)
{
    for (int i = LCD_HEIGHT - 1; i >= 0; i--)
    {
        for (int j = LCD_WIDTH - 1; j >= 0; j--)
        {
            printf("%02d", display_buffer[(i * LCD_WIDTH) + j]);  // 32 = [31]
        }
        printf("\n");
    }
    printf("\n");
}

// 270도 회전
void print_rotate_270(void)
{
    for (int i = LCD_WIDTH - 1; i >= 0; i--)
    {
        for (int j = 0; j < LCD_HEIGHT; j++)
        {
            printf("%02d", display_buffer[(j * LCD_WIDTH) + i]);  // 04 = [3]
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
int main(void)
{
    fill_display_buffer();

    printf("> normal\n");      // 원본 출력
    print_display_buffer(0);

    printf("> rotate 90\n");   // 90도 회전 출력
    print_display_buffer(90);

    printf("> rotate 180\n");  // 180도 회전 출력
    print_display_buffer(180);

    printf("> rotate 270\n");  // 270도 회전 출력
    print_display_buffer(270);

    return 0;
}