TARGET = build/oac.exe

CC = g++

SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(patsubst src/%,build/objs/%,$(SOURCES:%.cpp=%.o))

TEST_SOURCES = $(filter-out src/main.cpp,$(SOURCES)) $(wildcard test/*.test.cpp)
TEST_OBJECTS = $(addprefix build/objs/, $(notdir $(TEST_SOURCES:%.cpp=%.o)))

# Commands

build: $(TARGET)

.PHONY: run
run: $(TARGET)
	./$(TARGET)

.PHONY: test
test: build/test.exe
	./build/test.exe

.PHONY: clean
clean:
	rm -f build/objs/*.o

.PHONY: reset
reset: clean
	rm -f build/*.exe


# Build Rules

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^

build/objs/%.test.o: test/%.test.cpp test/*.hpp src/*.hpp
	$(CC) -I./src -I./test -c -o $@ $<

build/objs/%.o: src/%.cpp src/*.hpp
	$(CC) -I./src -I./test -c -o $@ $<

build/test.exe: $(TEST_OBJECTS)
	$(CC) -o $@ $^
