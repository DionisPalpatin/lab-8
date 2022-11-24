#!/bin/bash


#if [[ -n "$USE_VALGRIND" ]]; then
#    echo "Use valgrind"
#fi


RED="\033[0;31m"
GREEN="\033[0;32m"
DEFAULT="\033[0m"
GREY=""


# Cпособ кодирования кода выхода pos_case.sh для позитивных тестов:
#   * первая цифра 0/1 - флаг отсутствия/наличия ошибок в рантайме программы (неправильный код выхода)
#   * вторая цифра 0/1/2 - флаг отсутствия/наличия ошибок в тесте или отсутствия проверки
#   * третья цифра 0/1/2 - флаг отсутствия/наличия ошибок или отсутствия проверки памяти
# Первый параметр функции - код выхода тестовых скриптов, второй - номер теста
function print_pos_case_result()
{
    memory_errors=$(($1 % 10))
    mem_err_str=""
    if [[ $memory_errors -eq 0 ]]; then
        mem_err_str="${GREEN}OK${DEFAULT}"
    elif [[ $memory_errors -eq 1 ]]; then
        mem_err_str="${RED}ERROR${DEFAULT}"
    else
        mem_err_str="${GREY}NOT CHECKED${DEFAULT}"
    fi

    test_errors=$(($1 / 10 % 10))
    test_err_str=""
    if [[ $test_errors -eq 0 ]]; then
        test_err_str="${GREEN}OK${DEFAULT}"
    elif [[ $test_errors -eq 1 ]]; then
        test_err_str="${RED}ERROR${DEFAULT}"
    else
        test_err_str="${GREY}NOT CHECKED${DEFAULT}"
    fi

    runtime_errors=$(($1 / 100))
    runtime_err_str=""
    if [[ $runtime_errors -eq 0 ]]; then
        runtime_err_str="${GREEN}OK${DEFAULT}"
    else
        runtime_err_str="${RED}ERROR${DEFAULT}"
    fi

    printf "Test №%s:\n" "$2"
    echo -e "\tRuntime: ${runtime_err_str}"
    echo -e "\tOutput: ${test_err_str}"
    echo -e "\tMemory: ${mem_err_str}"
}


# Cпособ кодирования кода выхода neg_case.sh для негативных тестов:
#   * первая цифра 0/1 - флаг наличия/отсутствия ошибок в рантайме программы
#   * вторая цифра 0/1/2 - флаг отсутствия/наличия ошибок c памятью или отсутствия проверки памяти
# Первый параметр функции - код выхода тестовых скриптов, второй - номер теста
function print_neg_case_result()
{
    memory_errors=$(($1 % 10))
    mem_err_str=""
    if [[ $memory_errors -eq 0 ]]; then
        mem_err_str="${GREEN}OK${DEFAULT}"
    elif [[ $memory_errors -eq 1 ]]; then
        mem_err_str="${RED}ERROR${DEFAULT}"
    else
        mem_err_str="${GREY}NOT CHECKED${DEFAULT}"
    fi

    runtime_errors=$(($1 / 10))
    runtime_err_str=""
    if [[ $runtime_errors -eq 0 ]]; then
        runtime_err_str="${GREEN}OK${DEFAULT}"
    else
        runtime_err_str="${RED}ERROR${DEFAULT}"
    fi

    printf "Test №%s:\n" "$2"
    echo -e "\tRuntime: ${runtime_err_str}"
    echo -e "\tMemory: ${mem_err_str}"
}


failed_pos_tests=0
failed_neg_tests=0


printf "\n\n\n---------------  TESTING IS STARTED  ---------------"
printf "\n\n\n---------------  RUN POSITIVE TESTS  ---------------\n\n\n"


iter=1
test_number="0${iter}"


# пока есть файлы с подходящей маской в папке, циклично выполняется подстановка файла с входными данными
# на вход в pos_case.sh
# если код возврата не равен нулю, то выводится сообщение об этом и увеличивается счетчик не пройденных тестов
# если же все нормально, то выводится сообщение о том, что тест успешно пройден
while [[ -f "../data/pos_${test_number}_in.txt" || -f "../data/pos_${test_number}_in1.txt" ]]; do
    input_stream="../data/pos_${test_number}_in.txt"
    output_stream="../data/pos_${test_number}_out.txt"
    args_file="../data/pos_${test_number}_args.txt"

    eval "./pos_case.sh $input_stream $output_stream $args_file"

    exit_code=$?
    print_pos_case_result $exit_code $iter
    if [[ $exit_code -gt 2 ]]; then
        failed_pos_tests=$((failed_pos_tests+1))
    fi

    iter=$((iter+1))
    if [[ $iter -lt 10 ]]; then
        test_number="0${iter}"
    else
        test_number="$iter"
    fi
done


printf "\n\n\n---------------  RUN NEGATIVE TESTS  ---------------\n\n\n"


iter=1
test_number="0${iter}"


# все то же самое, только для негативных тестов
# и не передаются в поток ввода neg_out файлов
while [[ -f "../data/neg_${test_number}_in.txt" ]]; do
    input_stream="../data/neg_${test_number}_in.txt"
    args_file="../data/neg_${test_number}_args.txt"

    eval "./neg_case.sh $input_stream $args_file"

    exit_code=$?
    print_neg_case_result $exit_code $iter
    if [[ $exit_code -gt 2 ]]; then
        failed_neg_tests=$((failed_neg_tests+1))
    fi

    iter=$((iter+1))
    if [[ $iter -lt 10 ]]; then
        test_number="0${iter}"
    else
        test_number="$iter"
    fi
done


printf "\n\n\n---------------  RESULTS  ---------------\n\n\n"


# вывод общей информации
# если не пройден хотя бы один тест
# то выводится кол-во не пройденных тестов
# иначе выводится сообщение о том, что все тесты пройдены успешно
# для негативных и позитивных тестов данные операции выполняются по отдельности
if [ $failed_pos_tests -eq 0 ]; then
    printf "All positive tests were successfully\n"
else
    printf "%s positive tests are failed\n" "$failed_pos_tests"
fi

if [[ $failed_neg_tests -eq 0 ]]; then
    printf "All negative tests were successfully"
else
    printf "%s negative tests are failed" "$failed_neg_tests"
fi


printf "\n\n\n---------------  TESTING HAS FINISHED  ---------------\n\n\n"


# скрипт возвращает общее количество не пройденных тестов
exit $((failed_pos_tests + failed_neg_tests))