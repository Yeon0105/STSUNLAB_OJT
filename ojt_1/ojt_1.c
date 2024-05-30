#include <stdio.h>

//문자열 복사 함수
void my_strcpy(char dest[], char src[]) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
}

//문자열 뒤집기 함수
void my_strrev(char str[]) {
    int len = 0; 

    //문자열 길이 계산
    while (str[len] != '\0') {
        len++;
    }

    //문자열 뒤집기
    for (int i = 0; i < len / 2; i++) { //i<len/2인 이유는 길이가 6개면 3번 바꾸고, 길이가 5개면 2번 바꾸기 때문
        char temp = str[i];
        str[i] = str[len - 1 - i]; //뒤 단어가 앞으로 온다
        str[len - 1 - i] = temp; //앞 단어가 뒤로 간다
    }
}

int main(int argc, char *argv[]) {

    char copy[100]; 

    //문자열 복사
    my_strcpy(copy, argv[1]);
    printf("my_strcpy: %s\n", copy);

    //문자열 뒤집기
    my_strrev(argv[1]);
    printf("my_strrev: %s\n", argv[1]);

    return 0;
}
