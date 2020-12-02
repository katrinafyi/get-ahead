FLAGS = -g -std=c++17
CC = g++

programs = parens parens2 tree rect words

all: $(programs)

%: %.cpp
	$(CC) $(FLAGS) $^ -o $@

clean:
	rm -fv $(programs)
