
JMiniShell: main.o AliasSet.o VariableSet.o CmdLineSeparator.o Jobs.o
	g++ -o MiniShell main.o AliasSet.o VariableSet.o CmdLineSeparator.o Jobs.o

main.o: main.cpp
	g++ -c -std=c++0x main.cpp
CmdLineSeparator.o: CmdLineSeparator.cpp CmdLineSeparator.h
	g++ -c CmdLineSeparator.cpp
AliasSet.o: AliasSet.cpp AliasSet.h
	g++ -c AliasSet.cpp
VariableSet.o: VariableSet.cpp VariableSet.h
	g++ -c VariableSet.cpp
Jobs.o: Jobs.h Jobs.cpp
	g++ -c Jobs.cpp

clean :
	rm main.o AliasSet.o VariableSet.o CmdLineSeparator.o Jobs.o
