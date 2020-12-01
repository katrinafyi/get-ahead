FLAGS = -g -std=c++17
CC = g++

programs = parens tree

all: $(programs)

%: %.cpp
	$(CC) $(FLAGS) $^ -o $@

clean:
	rm -fv $(programs)
