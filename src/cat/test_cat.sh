GREEN="\033[0;32m"
RED="\033[0;31m"
NC="\033[0m" 

S21_CAT=./s21_cat

TEST_FILES=("test1.txt" "test2.txt" "empty.txt")

FLAGS=(
  "-b"
  "-e"
  "-n"
  "-s"
  "-t"
  "-v"
  "-b -e"
  "-n -s"
  "-t -v"
  "--number"
  "--number-nonblank"
  "--squeeze-blank"
)

SUCCESS=0
FAIL=0

echo -e "Hello\n\nWorld\n\tTab\tLine\nBye" > test1.txt
echo -e "\n\n\nOnly\nBlank\n\nLines\n" > test2.txt
> empty.txt

run_test() {
  local flag="$1"
  local file="$2"
  local test_name="Test: '$flag' on '$file'"

  $S21_CAT $flag $file > s21_output.txt 2> /dev/null
  cat $flag $file > cat_output.txt 2> /dev/null

   if diff -q s21_output.txt cat_output.txt > /dev/null; then
    echo -e "${GREEN}✅ $test_name${NC}"
    ((SUCCESS++))
  else
    echo -e "${RED}❌ $test_name${NC}"
    diff -u s21_output.txt cat_output.txt
    ((FAIL++))
  fi

  rm -f s21_output.txt cat_output.txt
}

for flag in "${FLAGS[@]}"; do
  for file in "${TEST_FILES[@]}"; do
    run_test "$flag" "$file"
  done
done

echo -e "\n${GREEN}Успешных: $SUCCESS${NC}"
echo -e "${RED}Неуспешных: $FAIL${NC}"

rm -f test1.txt test2.txt empty.txt