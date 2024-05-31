#include <stdio.h>

// 중복 코드 함수 (문자열 길이 계산)
int my_strlen(char str[])
{
    int len = 0;
    while(str[len] != '\0')             // str[len]이 \0이 아니면 len 증가하면서 반복
    {
        len++;
    }
    return len;
}

// 문자열 복사 함수
void my_strcpy(char dest[], char src[])
{
    // len는 my_strlen함수를 통해 입력한 문자열의 길이가 담겨있음
    int len = my_strlen(src);

    for (int i = 0; i < len; i++)       // 문자열의 길이만큼 i가 증가하면서 반복
    {
        dest[i] = src[i];               // 원본이 사본에 복사
    }
}

// 문자열 뒤집기 함수
void my_strrev(char str[]) 
{
    // len는 my_strlen함수를 통해 입력한 문자열의 길이가 담겨있음
    int len = my_strlen(str);

    // 문자열 뒤집기
    for (int i = 0; i < len / 2; i++)       // i<len/2는 길이가 6개면 3번 바꾸고, 길이가 5개면 2번 바꾸기 때문
    {
        char temp        = str[i];
        str[i]           = str[len - 1 - i]; // 뒤 단어가 앞으로 온다
        str[len - 1 - i] = temp;             // 앞 단어가 뒤로 간다
    }
}

int main(int argc, char *argv[])
{
    char copy[100];

    // 문자열 복사
    my_strcpy(copy, argv[1]);
    printf("my_strcpy: %s\n", copy);

    // 문자열 뒤집기
    my_strrev(argv[1]);
    printf("my_strrev: %s\n", argv[1]);

    return 0;
}