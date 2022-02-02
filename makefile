oac.exe: *.cpp *.hpp
	g++ main.cpp -o oac.exe

tests: test.exe
	./test.exe

test.exe: *.hpp *.cpp
	g++ test.cpp -o test.exe