#include <stdio.h>
#include <stdlib.h>                    // atoi를 쓰기 위한 헤더 파일 선언
#include <ctype.h>                     // isdigit를 쓰기 위한 헤더 파일 선언
#include <limits.h>                    // INT_MAX, INT_MAX를 쓰기 위한 헤더 파일 선언

// enum 이용하여 예외상황 처리
enum ERROR_EXCEPTION_HANDLING
{
    ERROR_NONE                     = 0,
    OPERAND_CHECK_NUM_TRUE         = 1,
    ERROR_LESS_THAN_4_OPERANDS     = -1,
    ERROR_MORE_THAN_4_OPERANDS     = -2,
    ERROR_FIRST_OPERAND_NOT_NUM    = -3,
    ERROR_SECOND_OPERAND_NOT_NUM_  = -4,
    ERROR_OPERAND_EXCEED_INT_RANGE = -5,
    ERROR_INPUT_2_OPERATORS        = -6,
    ERROR_NOT_DIVIDE_BY_0          = -7,
    ERROR_NOT_4_BASIC_OPERATORS    = -8,
    ERROR_FOPEN_FAIL               = -9
};

// 덧셈 함수
int add(int operand_1, int operand_2)
{
    return operand_1 + operand_2;
}

// 뺄셈 함수
int subtract(int operand_1, int operand_2)
{
    return operand_1 - operand_2;
}

// 곱셈 함수
int multiply(int operand_1, int operand_2)
{
    return operand_1 * operand_2;
}

// 나눗셈 함수
float divide(int operand_1, int operand_2)
{
    return (float) operand_1 / operand_2;
}

// 입력이 숫자인지 확인하는 함수
int check_number(char *str)
{
    // 음수일 경우도 인식
    if (*str == '-')
    {
        str++;
    }

    // 문자열이 숫자로만 구성되어 있는지 확인
    while (*str)
    {
        // 숫자가 아니면 0 반환, isdigit 함수는 숫자로 변경 실패 시 0 (false) 반환
        if (!isdigit(*str))
        {
            return ERROR_NONE;
        }
        str++;
    }

    // 인자가 숫자이면 1 (true) 반환
    return OPERAND_CHECK_NUM_TRUE;
}

// 문자열 길이 strlen 계산 함수
int my_strlen(char str[])
{
    int len = 0;

    // str[len]이 \0이 아니면 len 증가하면서 반복
    while (str[len] != '\0')
    {
        len++;
    }
    return len;
}

/*
1. [예외처리] argc의 개수를 확인하고, 부족하거나 많은 경우 예외처리로 에러 메시지 출력.
2. [코드구현] main에서 argc, argv로 입력 받은 문자열 인자들을 정수로 변환. (atoi 사용)
3. [예외처리] 피연산자들이 숫자가 아닌 경우 예외처리로 에러 메시지 출력.
4. [예외처리] 피연산자들이 int의 범위를 초과하면 예외처리로 에러 메시지 출력.
5. [예외처리] 연산자가 잘못됐을 때 예외처리로 에러 메시지 출력.
6. [예외처리] 나눗셈 연산 시 두번째 피연산자가 0이면 예외처리로 에러 메시지 출력.
7. [코드구현] 사칙연산 계산 처리 및 결과 출력.
8. [코드구현] 사칙연산 계산기를 실행한 결과가 cal_result.txt 파일에 저장. (계산 결과값이 마지막 줄에 누적되어 저장됨)
9. [enum 이용 예외처리] enum type을 이용하여 각 예외처리 상황의 return 값 처리.
*/

int main(int argc, char *argv[])
{
    // 인자 수 4보다 적으면 에러, 4인 이유는 한 개는 프로그램 이름을 포함하기 때문
    if (argc < 4)
    {
        printf("Error, few operands.\n");
        return ERROR_LESS_THAN_4_OPERANDS;
    }

    // 인자 수 4보다 많으면 에러
    if (argc > 4)
    {
        printf("Error, many operands.\n");
        return ERROR_MORE_THAN_4_OPERANDS;
    }
    
    int first_operand  = atoi(argv[1]);  // 첫 번째 피연산자, 문자열을 정수로 변환
    int second_operand = atoi(argv[3]);  // 두 번째 피연산자, 문자열을 정수로 변환
    char operator      = argv[2][0];     // 연산자

    int result_int     = 0;              // 덧셈, 뺄셈, 곱셈 결과 값 (정수)
    float result_float = 0.0;            // 나눗셈 결과 값 (소수점)

    char result_string[200];             // 충분한 크기 버퍼 할당
    
    // 첫 번째 피연산자가 숫자가 아닌 경우 에러
    if (!check_number(argv[1]))
    {
        printf("Error, first operand is not a number.\n");
        return ERROR_FIRST_OPERAND_NOT_NUM;
    }

    // 두 번째 피연산자가 숫자가 아닌 경우 에러
    if (!check_number(argv[3]))
    {
        printf("Error, second operand is not a number.\n");
        return ERROR_SECOND_OPERAND_NOT_NUM_;
    }

    // 피연산자가 int 범위를 벗어나면 에러
    if ((first_operand > INT_MAX) || (second_operand > INT_MAX))
    {
        printf("Error, operand out of range for int type.\n");
        return ERROR_OPERAND_EXCEED_INT_RANGE;
    }
    else if ((first_operand < INT_MIN) || (second_operand < INT_MIN))
    {
        printf("Error, operand out of range for int type.\n");
        return ERROR_OPERAND_EXCEED_INT_RANGE;
    }

    // 연산자가 제대로 입력되었는지 확인, ++ -- 두개씩 입력 시 에러
    if (argv[2][1] != '\0')
    {
        printf("Error, invalid operator.\n");
        return ERROR_INPUT_2_OPERATORS;
    }

    // 0으로 나눌 때 에러
    if ((operator == '/') && (second_operand == 0))
    {
        printf("Error, cannot be divided by 0.\n");
        return ERROR_NOT_DIVIDE_BY_0;
    }

    // +, -, X, / 연산자에 따라 해당 함수 호출
    switch (operator)
    {
        case '+':
            result_int = add(first_operand, second_operand);
            printf("%d + %d = %d\n", first_operand, second_operand, result_int);
            break;

        case '-':
            result_int = subtract(first_operand, second_operand);
            printf("%d - %d = %d\n", first_operand, second_operand, result_int);
            break;

        case 'X':
            result_int = multiply(first_operand, second_operand);
            printf("%d X %d = %d\n", first_operand, second_operand, result_int);
            break;

        case '/':
            result_float = divide(first_operand, second_operand);
            printf("%d / %d = %.1f\n", first_operand, second_operand, result_float);
            break;

        default:
            printf("Error, use +, -, X, / operator\n");   // +, -, X, /가 아니면 오류 출력
            return ERROR_NOT_4_BASIC_OPERATORS;
    }

    // 파일에 결과를 저장
    FILE *fp = fopen("cal_result.txt", "a");              // "a" 모드로 파일 열기 (이어쓰기 모드)

    if (fp == NULL)
    {
        printf ("Error open file");
        return ERROR_FOPEN_FAIL;
    }

    // 정수를 문자열로 변환하여 파일에 쓰기 (sprintf 사용)
    if (operator == '/')
    {
        sprintf(result_string, "%.1f\n", result_float);   // 소수점 한 자리까지 출력
    }
    else
    {
        sprintf(result_string, "%d\n", result_int);
    }

    // 파일 쓰기 및 닫기
    fwrite(result_string, sizeof(char), my_strlen(result_string), fp);
    fclose(fp);

    return ERROR_NONE;
}