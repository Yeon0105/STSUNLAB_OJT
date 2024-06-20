#include <stdio.h>
#include <stdlib.h>  // atoi를 쓰기 위한 헤더 파일 선언
#include <ctype.h>   // isdigit를 쓰기 위한 헤더 파일 선언
#include <limits.h>  // INT_MAX, INT_MIN를 쓰기 위한 헤더 파일 선언 (-2,147,483,648 ~ 2,147,483,647)

#define RESULT_FILE "cal_result.txt"

// enum 이용하여 예외상황 처리
typedef enum cal_result_type_e
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
    ERROR_FOPEN_FAIL               = -9,
    ERROR_FSEEK_FAIL               = -10,
    ERROR_FWRITE_FAIL              = -11,
    ERROR_FCLOSE_FAIL              = -12,
    ERROR_DATA_OVERFLOW            = -13,
    ERROR_DATA_UNDERFLOW           = -14
} cal_result_type;

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
int input_validate(int input_count, char **input_param, int *first_operand, int *second_operand)
{
    // 인자 수 4보다 적으면 에러, 4인 이유는 한 개는 프로그램 이름을 포함하기 때문
    if (input_count < 4)
    {
        printf("Error, few operands.\n");
        return ERROR_LESS_THAN_4_OPERANDS;
    }

    // 인자 수 4보다 많으면 에러
    if (input_count > 4)
    {
        printf("Error, many operands.\n");
        return ERROR_MORE_THAN_4_OPERANDS;
    }

    // 첫 번째 피연산자가 숫자가 아닌 경우 에러
    if (!check_number(input_param[1]))
    {
        printf("Error, first operand is not a number.\n");
        return ERROR_FIRST_OPERAND_NOT_NUM;
    }

    // 두 번째 피연산자가 숫자가 아닌 경우 에러
    if (!check_number(input_param[3]))
    {
        printf("Error, second operand is not a number.\n");
        return ERROR_SECOND_OPERAND_NOT_NUM_;
    }

    *first_operand  = atoi(input_param[1]);  // 첫 번째 피연산자, 문자열을 정수로 변환
    *second_operand = atoi(input_param[3]);  // 두 번째 피연산자, 문자열을 정수로 변환

    // 피연산자가 int 범위를 벗어나면 에러
    if ((*first_operand > INT_MAX) || (*second_operand > INT_MAX))
    {
        printf("Error, operand out of range for int type.\n");
        return ERROR_OPERAND_EXCEED_INT_RANGE;
    }
    else if ((*first_operand < INT_MIN) || (*second_operand < INT_MIN))
    {
        printf("Error, operand out of range for int type.\n");
        return ERROR_OPERAND_EXCEED_INT_RANGE;
    }

    // 연산자가 제대로 입력되었는지 확인, ++ -- 두개씩 입력 시 에러
    if (input_param[2][1] != '\0')
    {
        printf("Error, invalid operator.\n");
        return ERROR_INPUT_2_OPERATORS;
    }

    // 0으로 나눌 때 에러
    if ((input_param[2][0] == '/') && (*second_operand == 0))
    {
        printf("Error, cannot be divided by 0.\n");
        return ERROR_NOT_DIVIDE_BY_0;
    }

    return ERROR_NONE;
}

// 계산기 처리 함수
int calculate(int first_operand, char operator, int second_operand, double *result, char *result_string)
{
    // +, -, X, / 연산 수행
    switch (operator)
    {
        case '+':
            *result = (double)first_operand + (double)second_operand;
            break;

        case '-':
            *result = (double)first_operand - (double)second_operand;
            break;

        case 'X':
            *result = (double)first_operand * (double)second_operand;
            break;

        case '/':
            *result = (double)first_operand / (double)second_operand;
            break;

        default:
            printf("Error, use +, -, X, / operator\n");
            return ERROR_NOT_4_BASIC_OPERATORS;
    }

    // overflow, underflow 체크 후 에러메시지 출력
    if (*result > INT_MAX)
    {
        printf("Error, data overflow.\nPlease, input range of -2147483648 ~ 2147483647.\n");
        return ERROR_DATA_OVERFLOW;
    }
    if (*result < INT_MIN)
    {
        printf("Error, data underflow.\nPlease, input range of -2147483648 ~ 2147483647.\n");
        return ERROR_DATA_UNDERFLOW;
    }

    // 결과 출력 및 파일에 저장
    if (operator == '/')
    {
        printf("%d %c %d = %g\n", first_operand, operator, second_operand, *result);
        sprintf(result_string, "%g", *result);
    }
    else
    {
        printf("%d %c %d = %d\n", first_operand, operator, second_operand, (int)*result);
        sprintf(result_string, "%d", (int)*result);
    }

    return ERROR_NONE;
}

// 파일 저장 함수
int save_result_to_file(char *result_string)
{
    int len         = 0;                    // 문자열 길이를 담는 변수
    char newline[2] = {0x0d, 0x0a};         // 윈도우 줄바꿈 \n 에 해당하는 hex 값

    // "r+" 모드로 파일을 열어서 파일이 없을 경우 파일을 생성
    // "r+" 모드는 기존의 내용이 존재할 경우, 기존의 내용을 지우지 않으면서 열기 때문에 기존 내용에 새로운 내용을 추가하거나 할 수 있다.
    FILE *fp = fopen(RESULT_FILE, "r+");

    // "r+" 모드로 연 파일이 존재 하지 않으면 파일을 "w+" 모드로 열고, "w+" 모드로 연 파일이 존재하지 않으면 에러 출력
    // "w+" 모드는 기존의 내용이 존재할 경우, 기존의 내용을 모두 지우기 때문에 기존 파일에 덧붙여서 파일에 쓰고자 할 때 문제가 발생한다.
    if (fp == NULL)
    {
        // 파일이 없을 경우 "w+" 모드로 생성
        fp = fopen(RESULT_FILE, "w+");
        if (fp == NULL)
        {
            printf("Error, file opening failed.\nCheck the file permissions.\n");
            return ERROR_FOPEN_FAIL;
        }
    }
    else
    {
        // 파일이 있을 경우 파일 끝으로 이동
        if (fseek(fp, 0, SEEK_END) != 0)
        {
            printf("Error, file seeking failed.\nCheck the file permissions.\n");
            return ERROR_FSEEK_FAIL;
        }
    }

    len = my_strlen(result_string); // 문자열의 길이가 들어있음

    // 계산 결과 저장 된 값을 파일에 쓰기, fwrite 함수는 성공적으로 쓴 바이트 수를 반환
    if (fwrite(result_string, sizeof(char), len, fp) != len)
    {
        printf("Error, file writing failed.\nCheck the file permissions.\n");
        fclose(fp);
        return ERROR_FWRITE_FAIL;
    }

    // 줄바꿈 문자 쓰기. "\r\n" 2개를 쓰므로 2 사용
    if (fwrite(newline, sizeof(char), 2, fp) != 2)
    {
        printf("Error, writing newline to file failed.\nCheck the file permissions.\n");
        fclose(fp);
        return ERROR_FWRITE_FAIL;
    }

    // fclose는 성공적으로 닫으면 0 반환
    if (fclose(fp) != 0)
    {
        printf("Error, file closing failed.\nCheck the file permissions.\n");
        return ERROR_FCLOSE_FAIL;
    }

    return ERROR_NONE;
}

/*
1. [enum 사용] enum type을 이용하여 각 예외처리 상황의 return 값 설정
2. [예외처리] 입력 유효성 검사 및 예외처리 하는 함수로 이동, 예외처리 할 상황이면 에러 메시지 출력
3. [atoi 사용] 사칙연산 계산을 하기 위해 문자열 피연산자들을 정수로 변환
4. [overflow, underflow 검사] calculate 함수에서 연산 함수의 반환값으로 에러 처리
5. [사칙연산 수행] 연산자를 기준으로 사칙연산을 수행하고, 결과 처리
6. [파일 저장 수행] 계산 결과를 파일에 저장, 파일을 열고, 파일의 끝으로 이동한 후 계산 결과를 파일에 작성
7. [파일 저장 예외처리] 파일을 저장하고 출력하는 중, 오류 발생 시 에러 메시지 출력 후 enum에 있는 값 반환
*/

int main(int argc, char **argv)
{
    int first_operand, second_operand = 0;           // 피연산자
    double result                     = 0;           // 계산 결과 값을 저장하는 변수
    char operator                     = argv[2][0];  // 연산자
    char result_string[20]            = {0};         // 결과를 문자열로 변환하여 저장할 버퍼

    int error_validate = input_validate(argc, argv, &first_operand, &second_operand); // 입력 유효성 검사

    if (error_validate != ERROR_NONE)                // 0이 아니면 enum에서 설정한 return 값 출력
    {
        return error_validate;                       // 유효성 검사 실패 시 enum에서 정의한 오류 코드 반환
    }

    // calculate 함수에서 연산 처리
    int calc_result_cause = calculate(first_operand, operator, second_operand, &result, result_string);

    if (calc_result_cause != ERROR_NONE)
    {
        return calc_result_cause;
    }

    // save_result_to_file 함수에서 결과 출력 및 파일 저장
    int save_result_cause = save_result_to_file(result_string);

    if (save_result_cause != ERROR_NONE)
    {
        return save_result_cause;                    // save_result_cause 함수에서 오류가 발생하면 enum에서 설정한 값 반환
    }

    return ERROR_NONE;
}