CC	= g++ 
CXX = $(CC)

CFLAGS = -ansi  -pedantic -Werror -Wall 
CFLAGS += -g3

CPPFLAGS = $(addprefix -I,$(INC_DIR) $(INC_TEST_DIR))
CPPFLAGS += -MMD -MP

CXXFLAGS = $(CFLAGS) -std=c++03

LDFLAGS =
LDLIBS = -pthread -ldl

SOURCES = $(TARGET).cpp
SOURCES += $(filter-out $(SRC_DIR)/agent_initializer.cpp, $(wildcard $(SRC_DIR)/*.cpp))
SOURCES += $(wildcard $(SRC_TEST_DIR)/*.cpp)

OBJS = $(SOURCES:.cpp=.o)

TARGET = ./test/mvp/test

INC_DIR = ./inc
SRC_DIR = ./src
INC_TEST_DIR = ./test/inc
SRC_TEST_DIR = ./test/src

DEPENDS = $(OBJS:%.o=%.d)

$(TARGET): $(OBJS)

all: $(TARGET)

check: $(TARGET)
	./$(TARGET) -v

valgrind:
	valgrind --leak-check=full ./$(TARGET)

clean: 
	rm -f $(OBJS) $(TARGET) $(DEPENDS)

-include $(DEPENDS)

.PHONY: clean check all valgrind