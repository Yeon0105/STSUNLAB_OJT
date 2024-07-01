#include "ojt_5.h"

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

    if (divider->file_var.file_ext != NULL)
    {
        printf("input->i_file_name.ext          = '%s' (len=%d)\n", divider->file_var.file_ext, divider->file_var.ext_len);
        printf("input->i_file_name.dot_idx      = [%d]\n", divider->file_var.file_dot_idx);
    }
    else
    {
        printf("input->i_file_name.ext          = '' (len=%d)\n", 0);
        printf("input->i_file_name.dot_idx      = [0]\n");
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
        if (divider->file_var.file_ext)
        {
            printf("                        %s_%d.%s (cur_ptr=%d)\n", divider->file_var.file_name, i + 1, divider->file_var.file_ext, i * divider->info.div_size);
            printf("case_#1: loop=1\n");
            printf("case_#2: remain=%03d\n\n", divider->info.div_size % 1000);
        }
        else
        {
            printf("                        %s_%d (cur_ptr=%d)\n", divider->file_var.file_name, i + 1, i * divider->info.div_size);
            printf("case_#1: loop=1\n");
            printf("case_#2: remain=%03d\n\n", divider->info.div_size % 1000);
        }
    }

    if (divider->info.div_remain > 0)
    {
        if (divider->file_var.file_ext)
        {
            printf("                        %s_%d.%s (cur_ptr=%d)\n", divider->file_var.file_name, divider->file_var.file_div + 1, divider->file_var.file_ext, divider->file_var.file_div * divider->info.div_size);
        }
        else
        {
            printf("                        %s_%d (cur_ptr=%d)\n", divider->file_var.file_name, divider->file_var.file_div + 1, divider->file_var.file_div * divider->info.div_size);
        }
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

    // 입력된 나눌 갯수가 유효한 정수인지 확인
    for (int i = 0; i < strlen(input_argv[2]); i++)
    {
        if (!isdigit(input_argv[2][i]))
        {
            printf("Error, file divide count input check! (argv[2] = %s)\n", input_argv[2]);
            return ERROR_INVALID_FILE_DIV;
        }
    }

    // ./ojt_5 x.bin 4 1000 입력한 것을 구조체에 차례로 저장
    divider->file_var.file_path = input_argv[1];                             // 파일경로 저장
    divider->file_var.file_div  = atoi(input_argv[2]);                       // 문자열을 정수로 변환 후 파일 나누는 갯수 저장
    divider->file_var.heap_size = atoi(input_argv[3]);                       // 문자열을 정수로 변환 후 힙 메모리 크기 저장
    divider->file_var.path_len  = strlen(divider->file_var.file_path);       //파일 경로 문자열 길이 구하기

    if (divider->file_var.file_div <= 0)
    {
        printf("Error, file divide count input check! (argv[2] = %s)\n", input_argv[2]);
        return ERROR_INVALID_FILE_DIV;
    }

    if (divider->file_var.heap_size <= 0)
    {
        printf("Error, heap size input check! (argv[3] = %s)\n", input_argv[3]);
        return ERROR_INVALID_HEAP_SIZE;
    }

    // 문자열에서 '.' 문자의 위치를 찾아 인덱스 값 저장
    divider->file_var.file_dot_idx = -1;

    // 파일 경로의 끝부터 검색을 시작해 '.' 문자를 찾도록 변경, 파일 맨 뒤에 있는 확장자를 찾기 수월
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
        // 확장자 위치에 따른 파일 이름 및 확장자 저장
        divider->file_var.name_len = divider->file_var.file_dot_idx;
        divider->file_var.ext_len  = divider->file_var.path_len - divider->file_var.file_dot_idx - 1;

        // '.' 문자를 기준으로 파일 이름과 확장자를 분리하여 저장
        divider->file_var.file_name = strndup(divider->file_var.file_path, divider->file_var.file_dot_idx);
        divider->file_var.file_ext  = strdup(&divider->file_var.file_path[divider->file_var.file_dot_idx + 1]);
    }
    else
    {
        // 확장자가 없는 경우 파일 이름만 저장
        divider->file_var.file_name = strdup(divider->file_var.file_path);
        divider->file_var.file_ext = NULL;
        divider->file_var.name_len = divider->file_var.path_len;
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

// 원본 파일의 크기 구함
error_code get_original_file_size(divider *divider)
{
    // 파일 읽기 모드로 열기
    divider->file_var.input_file = fopen(divider->file_var.file_path, "rb");
    if (divider->file_var.input_file == NULL)
    {
        printf("Error, fopen failed!\n");
        return ERROR_FOPEN_FAIL;
    }

    // 파일 포인터를 끝으로 이동
    if (fseek(divider->file_var.input_file, 0, SEEK_END) != F_SUCCESS)
    {
        printf("Error, fseek failed!\n");
        fclose(divider->file_var.input_file);
        return ERROR_FSEEK_FAIL;
    }

    // 파일 크기 계산
    divider->info.org_size = ftell(divider->file_var.input_file);
    if (divider->info.org_size == -1)
    {
        printf("Error, ftell failed!\n");
        fclose(divider->file_var.input_file);
        return ERROR_FTELL_FAIL;
    }

    //파일 포인터를 다시 처음으로 이동
    if (fseek(divider->file_var.input_file, 0, SEEK_SET) != F_SUCCESS)
    {
        printf("Error, fseek failed!\n");
        fclose(divider->file_var.input_file);
        return ERROR_FSEEK_FAIL;
    }

    return SUCCESS;
}

// 파일 사이즈 및 남은 byte 크기 구함
error_code calculate_division_info(divider *divider)
{
    // 파일 크기보다 큰 분할 개수 입력 시 오류 처리
    if (divider->file_var.file_div > divider->info.org_size)
    {
        printf("Error, file divide count exceeds file size!\n");
        return ERROR_FILE_DIV_EXCEED;
    }

    divider->info.div_size   = divider->info.org_size / divider->file_var.file_div;
    divider->info.div_remain = divider->info.org_size % divider->file_var.file_div;

    return SUCCESS;
}

// 파일을 지정된 수만큼 분할하여 새로운 파일로 저장 ex) x_1.bin x_2.bin
error_code divide_file(divider *divider)
{

    // 파일 분할
    for (int i = 0; i < divider->file_var.file_div; i++)
    {
        // 출력 파일 이름 생성
        if (divider->file_var.file_ext)
        {
            snprintf(divider->file_var.output_file_name, sizeof(divider->file_var.output_file_name), "%s_%d.%s", divider->file_var.file_name, i + 1, divider->file_var.file_ext);
        }
        else
        {
            snprintf(divider->file_var.output_file_name, sizeof(divider->file_var.output_file_name), "%s_%d", divider->file_var.file_name, i + 1);
        }

        // 출력 파일 열기
        divider->file_var.output_file = fopen(divider->file_var.output_file_name, "wb");
        if (divider->file_var.output_file == NULL)
        {
            printf("Error, file open failed!\n");
            fclose(divider->file_var.input_file);
            return ERROR_FOPEN_FAIL;
        }

        // 분할된 파일 크기만큼 데이터 읽기 및 쓰기
        for (int j = 0; j < divider->info.div_size; j += divider->file_var.heap_size)
        {
            if (divider->info.div_size - j < divider->file_var.heap_size)
            {
                divider->file_var.byte_to_read = divider->info.div_size - j;
            }
            else
            {
                divider->file_var.byte_to_read = divider->file_var.heap_size;
            }

            divider->file_var.byte_read = fread(divider->buffer, 1, divider->file_var.byte_to_read, divider->file_var.input_file);
            if (divider->file_var.byte_read != divider->file_var.byte_to_read)
            {
                printf("Error, fread failed!\n");
                fclose(divider->file_var.input_file);
                fclose(divider->file_var.output_file);
                return ERROR_FREAD_FAIL;
            }

            divider->file_var.byte_written = fwrite(divider->buffer, 1, divider->file_var.byte_read, divider->file_var.output_file);
            if (divider->file_var.byte_written != divider->file_var.byte_read)
            {
                printf("Error, fwrite failed!\n");
                fclose(divider->file_var.input_file);
                fclose(divider->file_var.output_file);
                return ERROR_FWRITE_FAIL;
            }
        }

        if (fclose(divider->file_var.output_file) != F_SUCCESS)
        {
            printf("Error, closing failed!\n");
            return ERROR_FCLOSE_FAIL;
        }
    }

    // 남은 바이트 처리, 남은 바이트가 있는지 확인
    if (divider->info.div_remain > 0)
    {
        // 출력 파일 이름 생성
        if (divider->file_var.file_ext)
        {
            snprintf(divider->file_var.output_file_name, sizeof(divider->file_var.output_file_name), "%s_%d.%s", divider->file_var.file_name, divider->file_var.file_div + 1, divider->file_var.file_ext);
        }
        else
        {
            snprintf(divider->file_var.output_file_name, sizeof(divider->file_var.output_file_name), "%s_%d", divider->file_var.file_name, divider->file_var.file_div + 1);
        }

        // 출력 파일 열기
        divider->file_var.output_file = fopen(divider->file_var.output_file_name, "wb");
        if (divider->file_var.output_file == NULL)
        {
            printf("Error, file open failed!\n");
            fclose(divider->file_var.input_file);
            return ERROR_FOPEN_FAIL;
        }

        // 남은 바이트 쓰기
        divider->file_var.byte_to_read = divider->info.div_remain;

        divider->file_var.byte_read = fread(divider->buffer, 1, divider->file_var.byte_to_read, divider->file_var.input_file);
        if (divider->file_var.byte_read != divider->file_var.byte_to_read)
        {
            printf("Error, fread failed!\n");
            fclose(divider->file_var.input_file);
            fclose(divider->file_var.output_file);
            return ERROR_FREAD_FAIL;
        }

        divider->file_var.byte_written = fwrite(divider->buffer, 1, divider->file_var.byte_read, divider->file_var.output_file);
        if (divider->file_var.byte_written != divider->file_var.byte_read)
        {
            printf("Error, fwrite failed!\n");
            fclose(divider->file_var.input_file);
            fclose(divider->file_var.output_file);
            return ERROR_FWRITE_FAIL;
        }

        // 출력 파일 닫기
        if (fclose(divider->file_var.output_file) != F_SUCCESS)
        {
            printf("Error, fclose failed!\n");
            return ERROR_FCLOSE_FAIL;
        }
    }

    // 입력 파일 닫기
    if (fclose(divider->file_var.input_file) != F_SUCCESS)
    {
        printf("Error, fclose failed!\n");
        return ERROR_FCLOSE_FAIL;
    }

    return SUCCESS;
}

// 메모리 해제
void memory_free(divider *divider)
{
    if (divider->file_var.file_name != NULL)
    {
        free(divider->file_var.file_name);
    }

    if (divider->file_var.file_ext != NULL)
    {
        free(divider->file_var.file_ext);
    }

    if (divider->buffer != NULL)
    {
        free(divider->buffer);
    }
}


error_code main(int argc, char **argv)
{
    divider file_divider;
    memset(&file_divider, 0, sizeof(divider));
    error_code status = SUCCESS;

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
    print_file_info(&file_divider);

    status = get_original_file_size(&file_divider);
    if (status != SUCCESS)
    {
        return status;
    }

    status = calculate_division_info(&file_divider);
    if (status != SUCCESS)
    {
        return status;
    }

    print_original_file_size_info(&file_divider);

    status = divide_file(&file_divider);
    if (status != SUCCESS)
    {
        return status;
    }

    print_divide_info(&file_divider);
    print_divide_process(&file_divider);

    // 메모리해제
    memory_free(&file_divider);

    return SUCCESS;
}