#ifndef OJT_5_H
#define OJT_5_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
[enum 사용] enum type을 이용하여 각 예외처리 상황의 return 값 설정
[구조체 정의] 지역변수와 전역변수를 대신할 구조체 정의
[예외처리] 입력이 올바른지, 파일이 존재하는 지 등 예외처리하는 함수 작성 후, 예외처리 할 상황이면 에러 메시지 출력
[인자 값 받아오기] int argc, char *argv[]를 통해 프로그램 실행 인자 받아오기
[함수 작성] 힙 메모리 확인 및 할당하는 함수 작성
[함수 작성] 입력 인자를 구조체에 저장하는 함수 작성
[함수 작성] 파일 이름 구조체에 저장하는 함수 작성
[함수 작성] 파일 크기를 확인하고 구조체에 저장하는 함수 작성
[함수 작성] 분할할 파일 크기를 계산하는 함수 작성
[함수 작성] 파일을 분할하여 저장하는 함수 작성
[메인 함수] 메인함수에서 위 함수들 호출
*/

// 예외 상황 처리를 위한 enum 정의
typedef enum error_code_e {
    SUCCESS = 0,
    ERROR_INVALID_ARGUMENTS = 1,
    ERROR_INVALID_HEAP_SIZE = 2,
    ERROR_MEMORY_ALLOCATION_FAIL = 3,
    ERROR_FOPEN_FAIL = 4,
    ERROR_FSEEK_FAIL = 5,
    ERROR_FTELL_FAIL = 6,
    ERROR_FWRITE_FAIL = 7,
    ERROR_FREAD_FAIL = 8,
    ERROR_FCLOSE_FAIL = 9,
    ERROR_INVALID_FILE_DIV = 10,
    ERROR_FILE_DIV_EXCEED = 11
} error_code;

// 파일 처리 관련 정보를 담는 구조체
typedef struct file_variables {
    char *file_path;             // 파일경로 x.bin
    char *file_name;             // 파일이름 x
    char *file_ext;              // 파일확장자 bin
    int file_div;                // 파일 나누는 갯수
    int heap_size;               // 힙 메모리 크기
    int path_len;                // 파일경로 문자열 길이
    int name_len;                // 파일이름 문자열 길이
    int ext_len;                 // 파일확장자 문자열 길이
    int file_dot_idx;            // 확장자의 시작 위치
    FILE *input_file;            // 입력 파일 포인터
    FILE *output_file;           // 출력 파일 포인터
    char output_file_name[200];  // 출력 파일 이름
    int byte_write;              // 작성할 바이트 수
    int byte_written;            // 작성된 바이트 수
    int cur_ptr;                 // 현재 포인터 위치
    int byte_to_read;            // 읽을 바이트 수
    int byte_read;               // 읽은 바이트 수
} file_variables;

typedef struct file_info {
    int org_size;                // 원본 파일의 크기 409600
    int div_size;                // 분할된 각 파일의 크기
    int div_remain;              // 마지막 분할 파일에 남은 바이트 수
} file_info;

typedef struct divider {
    file_variables file_var;     // input_params 구조체
    file_info info;              // file_info 구조체
    char *buffer;                // 힙 메모리에 할당된 버퍼
} divider;

#endif // OJT_5_H
