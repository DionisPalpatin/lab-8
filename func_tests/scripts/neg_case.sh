#!/bin/bash


#if [[ -n "$USE_VALGRIND" ]]; then
#    echo "Use valgrind"
#fi


if [[ $2 ]]; then
    args="$(cat "$2")"
else
    args=""
fi


# если флаг USE_VALGRIND есть, то программа запускается под его управлением
if [[ -n "$USE_VALGRIND" ]]; then
    command="valgrind --leak-check=full --track-origins=yes --log-file=out/valgrind_report.temp ./app.exe ${args}"
else
    command="./app.exe ${args}"
fi
cd ../../
eval "${command}"


program_exit_code=$?
echo "Exit code: $((program_exit_code - 256))"
script_exit_code=0

# Проверяем результаты valgrind
if [[ -n "$USE_VALGRIND" ]]; then
    if [[ ! (-s out/valgrind_report.temp) ]]; then
        script_exit_code=1
    fi
else
    script_exit_code=2
fi

# Проверяем результаты работы программы и результаты проверки выходных данных
if [[ $program_exit_code == 0 ]]; then
    script_exit_code=$((script_exit_code + 10))
fi


exit $script_exit_code