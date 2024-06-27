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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// 예외 상황 처리를 위한 enum 정의
typedef enum error_code_e
{
    SUCCESS                      = 0,
    ERROR_INVALID_ARGUMENTS      = 1,
    ERROR_INVALID_HEAP_SIZE      = 2,
    ERROR_MEMORY_ALLOCATION_FAIL = 3,
    ERROR_FOPEN_FAIL             = 4,
    ERROR_FSEEK_FAIL             = 5,
    ERROR_FTELL_FAIL             = 6,
    ERROR_FWRITE_FAIL            = 7,
    ERROR_FREAD_FAIL             = 8,
    ERROR_FCLOSE_FAIL            = 9
} error_code;

// 파일 처리 관련 정보를 담는 구조체
typedef struct file_variables
{
    char *file_path;            // 파일경로 x.bin
    char *file_name;            // 파일이름 x
    char *file_ext;             // 파일확장자 bin
    int file_div;               // 파일 나누는 갯수
    int heap_size;              // 힙 메모리 크기
    int path_len;               // 파일경로 문자열 길이
    int name_len;               // 파일이름 문자열 길이
    int ext_len;                // 파일확장자 문자열 길이
    int file_dot_idx;           // 확장자의 시작 위치
    FILE *input_file;           // 입력 파일 포인터 
    FILE *output_file;          // 출력 파일 포인터
    char output_file_name[200]; // 출력 파일 이름
    int byte_write;             // 작성할 바이트 수
    int byte_written;           // 작성된 바이트 수
    int cur_ptr;                // 현재 포인터 위치
    int byte_to_read;           // 읽을 바이트 수
    int byte_read;              // 읽은 바이트 수
} file_variables;

typedef struct file_info
{
    int org_size;               // 원본 파일의 크기 409600
    int div_size;               // 분할된 각 파일의 크기
    int div_remain;             // 마지막 분할 파일에 남은 바이트 수
} file_info;

typedef struct divider
{
    file_variables file_var;     // input_params 구조체
    file_info info;              // file_info 구조체
    char *buffer;                // 힙 메모리에 할당된 버퍼
} divider;

// 프로그램 실행 인자 및 파일 정보 터미널에 출력
void print_divider(divider *divider, int print_argc, char **print_argv)
{
    printf("-----------------------------------------------------\n");
    printf("                 File Divied Example                 \n");
    printf("-----------------------------------------------------\n");

    printf("\n//input parameter parsing ---------------------------\n");

    for (int i = 0; i < print_argc; i++)
    {
        printf("argc = %d, argv[] = %s\n", i, print_argv[i]);
    }

    printf("\n//file name, file divide count ----------------------\n");
    printf("input->i_file_path              = %s (len=%d)\n", divider->file_var.file_path, divider->file_var.path_len);
    printf("input->i_file_div               = %d\n", divider->file_var.file_div);
    printf("global_heap_size                = %d byte\n", divider->file_var.heap_size);
}

// 입력한 힙 메모리 터미널에 출력
void print_memory_allocation(divider *divider)
{
    printf("\n//Heap memory check ---------------------------------\n");
    printf("heap check success! malloc()    = %d byte\n", divider->file_var.heap_size);
}

// 파싱한 파일 이름, 파일 사이즈 터미널에 출력
void print_file_info(divider *divider)
{
    printf("\n//file name parsing ---------------------------------\n");
    printf("input->i_file_name.name         = '%s' (len=%d)\n", divider->file_var.file_name, divider->file_var.name_len);

    if (divider->file_var.file_ext)
    {
        printf("input->i_file_name.ext          = '%s' (len=%d)\n", divider->file_var.file_ext, divider->file_var.ext_len);
        printf("input->i_file_name.dot_idx      = [%d]\n", divider->file_var.file_dot_idx);
    }
}

// 원본 파일 경로 및 사이즈 정보 출력 
void print_original_file_size_info(divider *divider)
{
    printf("\n//original file size check --------------------------\n");
    printf("file_name                       = '%s' (len=%d)\n", divider->file_var.file_path, divider->file_var.path_len);
    printf("file_size                       = %d\n", divider->info.org_size);
}

// 분할할 파일 사이즈 및 갯수, 남은 byte 등 분할 파일 정보를 터미널에 출력
void print_divide_info(divider *divider)
{
    printf("\n//output divide size check --------------------------\n");
    printf("output->o_div_info.org_size     = %d\n", divider->info.org_size);
    printf("output->o_div_info.div_size     = %d\n", divider->info.div_size);
    printf("output->o_div_info.div_count    = %d\n", divider->file_var.file_div);
    printf("output->o_div_info.div_remain   = %d\n", divider->info.div_remain);
}

// 분할 파일 프로세스 동작 터미널에 출력
void print_divide_process(divider *divider)
{
    printf("\n//file divide process start--------------------------\n\n");

    for (int i = 0; i < divider->file_var.file_div; i++)
    {
        printf("                        %s_%d.%s (cur_ptr=%d)\n", divider->file_var.file_name, i + 1, divider->file_var.file_ext, i * divider->info.div_size);
        printf("case_#1: loop=1\n");
        printf("case_#2: remain=%03d\n\n", divider->info.div_size % 1000);
    }

    if (divider->info.div_remain > 0)
    {
        printf("                        %s_%d.%s (cur_ptr=%d)\n", divider->file_var.file_name, divider->file_var.file_div + 1, divider->file_var.file_ext, divider->file_var.file_div * divider->info.div_size);
        printf("case_#4: remain=%d\n\n", divider->info.div_remain);
    }

    printf("//file divide process end---------------------------\n");
}

// 프로그램 실행 인자를 Divider 구조체에 저장. 
error_code parse_input(int input_argc, char **input_argv, divider *divider)
{
    if (input_argc != 4)
    {
        printf("Error, invalid arguments!\n");
        return ERROR_INVALID_ARGUMENTS;
    }

    // 입력된 heap size가 유효한 정수인지 확인
    for (int i = 0; i < strlen(input_argv[3]); i++)
    {
        if (!isdigit(input_argv[3][i]))
        {
            printf("Error, heap size input check! (argv[3] = %s)\n", input_argv[3]);
            return ERROR_INVALID_HEAP_SIZE;
        }
    }

    // ./ojt_5 x.bin 4 1000 입력한 것을 구조체에 차례로 저장
    divider->file_var.file_path = input_argv[1];                             // 파일경로 저장
    divider->file_var.file_div  = atoi(input_argv[2]);                       // 문자열을 정수로 변환 후 파일 나누는 갯수 저장
    divider->file_var.heap_size = atoi(input_argv[3]);                       // 문자열을 정수로 변환 후 힙 메모리 크기 저장
    divider->file_var.path_len  = strlen(divider->file_var.file_path);       //파일 경로 문자열 길이 구하기

   if (divider->file_var.heap_size <= 0)
    {
        printf("Error, heap size input check! (argv[3] = %s)\n", input_argv[3]);
        return ERROR_INVALID_HEAP_SIZE;
    }
    
    return SUCCESS;
}

// 힙 메모리 할당 후 할당 실패 시 에러메시지 출력
error_code check_heap_memory(divider *divider)
{
    divider->buffer = (char *)malloc(divider->file_var.heap_size);
    if (divider->buffer == NULL)
    {
        printf("Error, memory allocation failed!\n");
        return ERROR_MEMORY_ALLOCATION_FAIL;
    }
    return SUCCESS;
}

// 파일 경로에서 이름과 확장자 분리
error_code parse_file_name(divider *divider)
{
    // 파일 경로의 길이를 계산하여 path_len에 저장
    divider->file_var.path_len = strlen(divider->file_var.file_path);
    // 확장자의 위치를 찾기 위해 -1 설정 (-1은 확장자가 없음을 말함)
    divider->file_var.file_dot_idx = -1;

    // 끝에서부터 마침표(.)를 찾음
    for (int i = divider->file_var.path_len - 1; i >= 0; i--)
    {
        if (divider->file_var.file_path[i] == '.')
        {
            divider->file_var.file_dot_idx = i;
            break;
        }
    }

    // 확장자가 있는 경우
    if (divider->file_var.file_dot_idx != -1)
    {
        divider->file_var.name_len  = divider->file_var.file_dot_idx;
        divider->file_var.ext_len   = divider->file_var.path_len - divider->file_var.file_dot_idx - 1;
        divider->file_var.file_name = (char *)malloc(divider->file_var.name_len + 1);
        divider->file_var.file_ext  = (char *)malloc(divider->file_var.ext_len + 1);

        if ((divider->file_var.file_name == NULL) || (divider->file_var.file_ext == NULL))
        {
            printf("Error, memory allocation failed!\n");
            return ERROR_MEMORY_ALLOCATION_FAIL;
        }

        strncpy(divider->file_var.file_name, divider->file_var.file_path, divider->file_var.file_dot_idx);    // 파일 이름 복사
        divider->file_var.file_name[divider->file_var.name_len] = '\0';
        strcpy(divider->file_var.file_ext, divider->file_var.file_path + divider->file_var.file_dot_idx + 1); // 확장자 복사
    }

    // 확장자가 없는 경우 파일 이름만 저장
    else
    {
        divider->file_var.file_name = (char *)malloc(divider->file_var.path_len + 1);
        if (divider->file_var.file_name == NULL)
        {
            printf("Error, memory allocation failed!\n");
            return ERROR_MEMORY_ALLOCATION_FAIL;
        }
        strcpy(divider->file_var.file_name, divider->file_var.file_path);
        divider->file_var.file_ext = NULL;
        divider->file_var.name_len = divider->file_var.path_len;
    }
    return SUCCESS;
}

// 원본 파일의 크기 구함
error_code get_original_file_size(divider *divider)
{
    //파일 읽기 모드로 열기
    divider->file_var.input_file = fopen(divider->file_var.file_path, "rb");
    if (divider->file_var.input_file == NULL)
    {
        printf("Error, fopen failed!\n");
        return ERROR_FOPEN_FAIL;
    }

    //파일 포인터를 끝으로 이동
    if (fseek(divider->file_var.input_file, 0, SEEK_END) != 0)
    {
        printf("Error, fseek failed!\n");
        fclose(divider->file_var.input_file);
        return  ERROR_FSEEK_FAIL;
    }

    //파일 크기 계산
    divider->info.org_size = ftell(divider->file_var.input_file);
    if (divider->info.org_size == -1)
    {
        printf("Error, ftell failed!\n");
        fclose(divider->file_var.input_file);
        return ERROR_FTELL_FAIL;
    }

    //파일 포인터를 다시 처음으로 이동
    if (fseek(divider->file_var.input_file, 0, SEEK_SET) != 0)
    {
        printf("Error, fseek failed!\n");
        fclose(divider->file_var.input_file);
        return ERROR_FSEEK_FAIL;
    }

    return SUCCESS;
}

// 파일 사이즈 및 남은 byte 크기 구함
void calculate_division_info(divider *divider)
{
    divider->info.div_size   = divider->info.org_size / divider->file_var.file_div;
    divider->info.div_remain = divider->info.org_size % divider->file_var.file_div;
}

// 파일을 지정된 수만큼 분할하여 새로운 파일로 저장 ex) x_1.bin x_2.bin
error_code divide_file(divider *divider)
{
    // 원본 파일 열기 
    divider->file_var.input_file = fopen(divider->file_var.file_path, "rb");
    if (divider->file_var.input_file == NULL)
    {
        printf("Error, file open failed!\n");
        return  ERROR_FOPEN_FAIL;
    }

    // 파일 분할
    for (int i = 0; i < divider->file_var.file_div; i++)
    {
        // 출력 파일 이름 생성
        snprintf(divider->file_var.output_file_name, sizeof(divider->file_var.output_file_name), "%s_%d.%s", divider->file_var.file_name, i + 1, divider->file_var.file_ext);

        // 출력 파일 열기
        divider->file_var.output_file = fopen(divider->file_var.output_file_name, "wb");
    
        if (divider->file_var.output_file == NULL)
        {
            printf("Error, file open failed!\n");
            fclose(divider->file_var.input_file);
            return ERROR_FOPEN_FAIL;
        }

        // 분할된 각 파일에 쓸 바이트 수 설정
        divider->file_var.byte_write   = divider->info.div_size; // 작성할 바이트 수 = 분할된 각 파일의 크기
        divider->file_var.byte_written = 0;                      // 작성된 바이트 수 0
        divider->file_var.cur_ptr = i * divider->info.div_size;  // 현재 포인터 위치 = i * 분할된 각 파일의 크기

        // 원본 파일의 현재 위치 설정 (파일의 처음 위치를 기준)
        if (fseek(divider->file_var.input_file, divider->file_var.cur_ptr, SEEK_SET) != 0)
        {
            printf("Error, file seek failed!\n");
            fclose(divider->file_var.input_file);
            fclose(divider->file_var.output_file);
            return ERROR_FSEEK_FAIL;
        }

        // 버퍼를 사용하여 파일 읽기 및 쓰기
        while (divider->file_var.byte_written < divider->file_var.byte_write)
        {
            // 현재 남은 바이트 수 계산
            if ((divider->file_var.byte_write - divider->file_var.byte_written) < divider->file_var.heap_size)
            {
                divider->file_var.byte_to_read = divider->file_var.byte_write - divider->file_var.byte_written;
            }
            else
            {
                divider->file_var.byte_to_read = divider->file_var.heap_size;
            } 

            // 파일에서 데이터 읽기
            divider->file_var.byte_read = fread(divider->buffer, 1, divider->file_var.byte_to_read, divider->file_var.input_file);
            if (divider->file_var.byte_read < 0)
            {
                printf("Error, fread failed!\n");
                fclose(divider->file_var.input_file);
                fclose(divider->file_var.output_file);
                return ERROR_FREAD_FAIL;
            }

            // 읽은 데이터를 파일에 쓰기
            if (fwrite(divider->buffer, 1, divider->file_var.byte_read, divider->file_var.output_file) != divider->file_var.byte_read)
            {
                printf("Error, fwrite failed!\n");
                fclose(divider->file_var.input_file);
                fclose(divider->file_var.output_file);
                return ERROR_FWRITE_FAIL;
            }

            // 쓴 바이트 수 업데이트
            divider->file_var.byte_written += divider->file_var.byte_read;
        }

        if (fclose(divider->file_var.output_file) != 0)
        {
            printf("Error, closing failed!\n");
            return ERROR_FCLOSE_FAIL;
        }
    }

    // 남은 바이트 처리, 남은 바이트가 있는지 확인
    if (divider->info.div_remain > 0)
    {
        // 남은 데이터를 위한 파일 이름 생성
        snprintf(divider->file_var.output_file_name, sizeof(divider->file_var.output_file_name), "%s_%d.%s", divider->file_var.file_name, divider->file_var.file_div + 1, divider->file_var.file_ext);

        // 출력 파일 열기
        divider->file_var.output_file = fopen(divider->file_var.output_file_name, "wb");
        if (divider->file_var.output_file == NULL)
        {
            printf("Error, file open failed!\n");
            fclose(divider->file_var.input_file);
            return ERROR_FOPEN_FAIL;

        }

        // 남은 바이트 읽기 및 쓰기
        // fseek을 사용하여 입력 파일의 포인터 위치를 설정, 위치는 이전 파일들의 크기를 모두 합친 값
        if (fseek(divider->file_var.input_file, divider->file_var.file_div * divider->info.div_size, SEEK_SET) != 0)
        {
            printf("Error, fseek failed!\n");
            fclose(divider->file_var.input_file);
            fclose(divider->file_var.output_file);
            return ERROR_FSEEK_FAIL;
        }

        // fread를 사용하여 남은 바이트를 입력 파일에서 읽어 버퍼에 저장
        divider->file_var.byte_read = fread(divider->buffer, 1, divider->info.div_remain, divider->file_var.input_file);

        if ((divider->file_var.byte_read < 0))
        {
            printf("Error, fread failed!\n");
            fclose(divider->file_var.input_file);
            fclose(divider->file_var.output_file);
            return ERROR_FREAD_FAIL;
        }

        // fwrite를 사용하여 버퍼에 저장된 데이터를 출력 파일에 씀
        if (fwrite(divider->buffer, 1, divider->file_var.byte_read, divider->file_var.output_file) != divider->file_var.byte_read)
        {
            printf("Error, fwrite failed!\n");
            fclose(divider->file_var.input_file);
            fclose(divider->file_var.output_file);
            return ERROR_FWRITE_FAIL;
        }

        // 출력 파일 닫기
        if (fclose(divider->file_var.output_file) != 0)
        {
            printf("Error, fclose failed!\n");
            fclose(divider->file_var.input_file);
            return ERROR_FCLOSE_FAIL;
        }
    }

    // 입력 파일 닫기
    if (fclose(divider->file_var.input_file) != 0)
    {
        printf("Error, fclose failed!\n");
        return ERROR_FCLOSE_FAIL;
    }
    return SUCCESS;
}

// 메모리 해제
void free_memory(divider *divider)
{
    free(divider->buffer);
    free(divider->file_var.file_name);
    free(divider->file_var.file_ext);
}

error_code main(int argc, char **argv)
{
    divider file_divider;
    error_code status;

    status = parse_input(argc, argv, &file_divider);
    if (status != SUCCESS)
    {
        return status;
    }

    print_divider(&file_divider, argc, argv);

    status = check_heap_memory(&file_divider);
    if (status != SUCCESS)
    {
        return status;
    }
    print_memory_allocation(&file_divider);

    status = parse_file_name(&file_divider);
    if (status != SUCCESS)
    {
        return status;
    }
    print_file_info(&file_divider);

    status = get_original_file_size(&file_divider);
    if (status != SUCCESS)
    {
        return status;
    }

    calculate_division_info(&file_divider);
    print_original_file_size_info(&file_divider);

    status = divide_file(&file_divider);
    if (status != SUCCESS)
    {
        return status;
    }

    print_divide_info(&file_divider);
    print_divide_process(&file_divider);

    free_memory(&file_divider);

    return SUCCESS;
}