GREEN="\033[0;32m"
RED="\033[0;31m"
NC="\033[0m"

S21_GREP=./s21_grep
SYS_GREP=$(which grep)

TEST_FILES=("file1.txt" "file2.txt")
PATTERN_FILE="patterns.txt"


echo -e "Hello world\nHELLO line\nhello again school21" > file1.txt
echo -e "Random text\nAnother hello\nNothing" > file2.txt
echo -e "" > emptyfile.txt
echo -e "hello\nworld" > $PATTERN_FILE

SUCCESS=0
FAIL=0


run_test() {
  local desc="$1"
  shift
  $S21_GREP "$@" > s21_output.txt 2>/dev/null
  $SYS_GREP "$@" > sys_output.txt 2>/dev/null


  if diff -u s21_output.txt sys_output.txt > /dev/null; then
    echo -e "${GREEN}✅ $desc${NC}"
    ((SUCCESS++))
  else
    echo -e "${RED}❌ $desc${NC}"
    diff -u s21_output.txt sys_output.txt
    ((FAIL++))
  fi

  rm -f s21_output.txt sys_output.txt
}

run_test "-e hello file1.txt" -e hello file1.txt
run_test "-i hello file1.txt" -i hello file1.txt
run_test "-v hello file1.txt" -v hello file1.txt
run_test "-c hello file1.txt" -c hello file1.txt
run_test "-n hello file1.txt" -n hello file1.txt
run_test "-h hello file1.txt" -h hello file1.txt
run_test "-l hello file1.txt" -l hello file1.txt


run_test "-iv hello file1.txt" -iv hello file1.txt
run_test "-in hello file1.txt" -in hello file1.txt
run_test "-cv hello file1.txt" -cv hello file1.txt
run_test "-cn hello file1.txt" -cn hello file1.txt
run_test "-lv hello file1.txt" -lv hello file1.txt
run_test "-nv hello file1.txt" -nv hello file1.txt
run_test "-cnh hello file1.txt" -cnh hello file1.txt


run_test "-e hello file1.txt file2.txt" -e hello file1.txt file2.txt
run_test "-c hello file1.txt file2.txt" -c hello file1.txt file2.txt
run_test "-n hello file1.txt file2.txt" -n hello file1.txt file2.txt
run_test "-iv hello file1.txt file2.txt" -iv hello file1.txt file2.txt
run_test "-cv hello file1.txt file2.txt" -cv hello file1.txt file2.txt
run_test "-cn hello file1.txt file2.txt" -cn hello file1.txt file2.txt
run_test "-lv hello file1.txt file2.txt" -lv hello file1.txt file2.txt

run_test "-e "" file1.txt" -e "" file1.txt
run_test "-e [0-9] file1.txt" -e "[0-9]" file1.txt
run_test "-e [0-9] -e "[abc]" -i -n file1.txt file2.txt" -e "[0-9]" -e "[abc]" -i -n file1.txt file2.txt
run_test "-f emptyfile.txt file1.txt" -e emptyfile.txt file1.txt
run_test "-f emptyfile.txt file1.txt" -f emptyfile.txt file1.txt
run_test "-f patterns.txt file1.txt" -f patterns.txt file1.txt
run_test "-f patterns.txt file1.txt file2.txt" -f patterns.txt file1.txt file2.txt
run_test "-i -f patterns.txt file1.txt" -i -f patterns.txt file1.txt




rm -f file1.txt file2.txt $PATTERN_FILE

echo -e "\n${GREEN}Успешных: $SUCCESS${NC}"
echo -e "${RED}Неуспешных: $FAIL${NC}"