#include <stdio.h>
#include <stdlib.h>                    // atoi를 쓰기 위한 헤더 파일 선언
#include <ctype.h>                     // isdigit를 쓰기 위한 헤더 파일 선언
#include <limits.h>                    // INT_MAX, INT_MAX를 쓰기 위한 헤더 파일 선언

// enum 이용하여 예외상황 처리
enum ERROR_EXCEPTION_HANDLING
{
    SUCCESS_OPERAND_CHECK_NUM      = 1,
    ERROR_NONE                     = 0,
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
    return SUCCESS_OPERAND_CHECK_NUM;
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

// 예외처리 함수
int input_validate(int argc, char *argv[])
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

    int first_operand  = atoi(argv[1]);  // 첫 번째 피연산자, 문자열을 정수로 변환
    int second_operand = atoi(argv[3]);  // 두 번째 피연산자, 문자열을 정수로 변환

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
    if ((argv[2][0] == '/') && (second_operand == 0))
    {
        printf("Error, cannot be divided by 0.\n");
        return ERROR_NOT_DIVIDE_BY_0;
    }

    return ERROR_NONE;
}

// 계산기 처리 함수
int calculate(int first_operand, char operator, int second_operand)
{
    int result_int     = 0;              // 덧셈, 뺄셈, 곱셈 결과 값 (정수)
    float result_float = 0.0;            // 나눗셈 결과 값 (소수점)

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
        
        // +, -, X, /가 아니면 오류 출력
        default:
            printf("Error, use +, -, X, / operator\n");
            return ERROR_NOT_4_BASIC_OPERATORS;
    }
}

// 결과 파일 저장 함수
int save_result_to_file(int first_operand, char operator, int second_operand)
{
    int result_int     = 0;              // 덧셈, 뺄셈, 곱셈 결과 값 (정수)
    float result_float = 0.0;            // 나눗셈 결과 값 (소수점)

    char result_string[200];             // 충분한 크기 버퍼 할당

    // 파일에 결과를 저장, "a" 모드로 파일 열기 (이어쓰기 모드)
    FILE *fp = fopen("cal_result.txt", "a");

    if (fp == NULL)
    {
        printf("Error open file");
        return ERROR_FOPEN_FAIL;
    }

    // fwrite 쓰려면 정수를 문자열로 변환 필요 -> sprintf 사용
    switch (operator)
    {
        case '+':
            result_int = add(first_operand, second_operand);
            sprintf(result_string, "%d\n", result_int);
            break;

        case '-':
            result_int = subtract(first_operand, second_operand);
            sprintf(result_string, "%d\n", result_int);
            break;

        case 'X':
            result_int = multiply(first_operand, second_operand);
            sprintf(result_string, "%d\n", result_int);
            break;

        case '/':
            result_float = divide(first_operand, second_operand);
            sprintf(result_string, "%.1f\n", result_float);   // 소수점 한 자리까지 출력
            break;
    }

    // 파일 쓰기 및 닫기
    fwrite(result_string, sizeof(char), my_strlen(result_string), fp);
    fclose(fp);

    return ERROR_NONE;
}

/*
1. [enum 사용] enum type을 이용하여 각 예외처리 상황의 return 값 설정
2. [예외처리] 입력 유효성 검사 및 예외처리 하는 함수로 이동, 예외처리 할 상황이면 에러 메시지 출력
3. [atoi 사용] 사칙연산 계산을 하기 위해 문자열 피연산자들을 정수로 변환
4. [사칙연산 수행] +, -, X, / 연산자에 따라 계산하는 함수 호출
5. [파일 저장 수행] 계산 결과를 파일에 저장하는 함수 호출, fwrite는 정수를 문자열로 반환해서 써야 하기 때문에 sprintf 사용
*/

int main(int argc, char *argv[])
{
    int error_validate = input_validate(argc, argv);            // 입력 유효성 검사
    if (error_validate != ERROR_NONE)                           // 0이 아니면 enum에서 설정한 return 값 출력
    {
        return error_validate;                                  // 유효성 검사 실패 시 enum에서 정의한 오류 코드 반환
    }

    int first_operand  = atoi(argv[1]);                           // 첫 번째 피연산자, 문자열을 정수로 변환
    int second_operand = atoi(argv[3]);                           // 두 번째 피연산자, 문자열을 정수로 변환
    char operator      = argv[2][0];                              // 연산자

    calculate(first_operand, operator, second_operand);           // +, -, X, / 연산자에 따라 계산하는 함수 호출
    save_result_to_file(first_operand, operator, second_operand); // 계산 결과를 파일에 저장하는 함수 호출

    return ERROR_NONE;
}