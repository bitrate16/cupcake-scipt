g++ -std=c++11 -w -g GIL2.cpp exceptions.cpp parser.cpp translator.cpp ast.cpp main_test.cpp -o test && valgrind --leak-check=full --track-origins=yes ./test