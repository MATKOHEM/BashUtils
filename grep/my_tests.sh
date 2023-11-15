#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
DIFF=""

s21_command=(
    "./s21_grep"
    )
sys_command=(
    "grep"
    )

manual=(
"-e and test.txt"
"-i and test.txt"
"-v and test.txt"
"-c and test.txt"
"-l and test.txt"
"-n and test.txt"
"-h and test.txt"
"-s and test.txt"
"-f test2.txt -e it test.txt"
"-o and test.txt"
"-l -c and test.txt"
"-v -c -e like -l -o -i test.txt test2.txt"
"-e clones -i -v -c -l -n -h -s -f test2.txt -o unix test.txt"
"-n -v -s -l -c never test.txt"
"-o -l -i -n code test.txt"
"-s -h -n -l -v -e command -o assembly test2.txt"
"-c -n have test.txt"
"-f test2.txt -e 11 -e : -v test.txt"
"-n -v -c -s -l had test2.txt"
"-n -l first test2.txt"
"-h -f test2.txt -e hello test.txt"
"-c -s -o -h -v -i be test2.txt"
"-n -l -c probably test.txt"
"-iv things test.txt"
"-in print test.txt test2.txt"
)

echo "" > log.txt
echo "


Hello everybody out there using minix -
I'm doing a (free) operating system (just a hobby, won't be big and professional like gnu) for 386(486) AT clones. This has been brewing since april, and is starting to get ready. I'd like any feedback on things people like/dislike in minix, as my OS resembles it somewhat (same physical layout of the file-system (due to practical reasons) among other things).
I've currently ported bash(1.08) and gcc(1.40), and things seem to work. This implies that I'll get something practical within a few months, and I'd like to know what features most people would want. Any suggestions are welcome, but I won't promise I'll implement them :-)
Linus (torvalds@kruuna.helsinki.fi)
PS. Yes - it's free of any minix code, and it has a multi-threaded fs. It is NOT portable (uses 386 task switching etc), and it probably never will support anything other than AT-harddisks, as that's all I have :-(.
Linus Torvalds


<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_\`abcdefghijklmnopqrstuvwxyz{\|}~

" > test.txt
echo "

Thompson wrote the first version in PDP-11 assembly language to help Lee E. McMahon analyze the text of the Federalist Papers to determine authorship of the individual papers. The ed text editor (also authored by Thompson) had regular expression support but could not be used on such a large amount of text, so Thompson excerpted that code into a standalone tool. He chose the name because in ed, the command g/re/p would print all lines matching a specified pattern. grep was first included in Version 4 Unix. Stating that it is "generally cited as the prototypical software tool", McIlroy credited grep with "irrevocably ingraining" Thompson's tools philosophy in Unix.


<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_\`abcdefghijklmnopqrstuvwxyz{\|}~

" > test2.txt

run_test() {
    param=$(echo "$@")
    "${s21_command[@]}" $param > "${s21_command[@]}".log
    "${sys_command[@]}" $param > "${sys_command[@]}".log
    DIFF="$(diff -s "${s21_command[@]}".log "${sys_command[@]}".log)"
    let "COUNTER++"
    if [ "$DIFF" == "Files "${s21_command[@]}".log and "${sys_command[@]}".log are identical" ]
    then
        let "SUCCESS++"
        echo "$COUNTER - Success $param"
    else
        let "FAIL++"
        echo "$COUNTER - Fail $param"
    fi
    rm -f "${s21_command[@]}".log "${sys_command[@]}".log
}

echo "START"
printf "\n"
for i in "${manual[@]}"
do
    var="-"
    run_test "$i"
done
printf "\n"
echo "FAILED: $FAIL"
echo "SUCCESSFUL: $SUCCESS"
echo "ALL: $COUNTER"
printf "\n"

echo "FAILED: $FAIL"
echo "SUCCESSFUL: $SUCCESS"
echo "ALL: $COUNTER"
printf "\n"

rm test.txt test2.txt log.txt

