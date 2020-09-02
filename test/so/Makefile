CC	= g++ 
CXX = $(CC)

CFLAGS = -ansi -pedantic -Werror -Wall -fPIC
CFLAGS += -g3 

CPPFLAGS = $(addprefix -I,$(INC_DIR) $(TEST_INC_DIR))
CPPFLAGS += -MMD -MP 

CXXFLAGS = $(CFLAGS) -std=c++03 

SO_FLAGS = -shared
LDFLAGS = -Wl,-rpath . -L.
LDLIBS = -ldl -lpthread

SOURCES = $(TARGET).cpp
SOURCES += $(wildcard $(SRC_DIR)/*.cpp)
SOURCES += $(wildcard $(TEST_SRC_DIR)/*.cpp)

SO_SENSOR_TARGET = libSensorAgent.so
SO_CONTROLLER_TARGET = libControllerAgent.so

OBJS = $(SOURCES:.cpp=.o)

SO_CONTROLLER_OBJS = $(SO_DIR)/controller_agent.o
SO_SENSOR_OBJS = $(SO_DIR)/sensor_agent.o

INC_DIR = ../../inc
SRC_DIR = ../../src
SO_DIR = ../../so

TEST_INC_DIR = ../inc
TEST_SRC_DIR = ../src

TARGET = test

DEPENDS = $(OBJS:%.o=%.d)
DEPENDS += $(SO_SENSOR_OBJS:%.o=%.d)
DEPENDS += $(SO_CONTROLLER_OBJS:%.o=%.d)

all: $(SO_SENSOR_TARGET) $(SO_CONTROLLER_TARGET) $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LDLIBS)

$(SO_SENSOR_TARGET): $(OBJS) $(SO_SENSOR_OBJS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(SO_FLAGS) $(LDFLAGS) -o $(SO_SENSOR_TARGET) $(OBJS) $(SO_SENSOR_OBJS) $(LDLIBS)

$(SO_CONTROLLER_TARGET) : $(OBJS) $(SO_CONTROLLER_OBJS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(SO_FLAGS) $(LDFLAGS) -o $(SO_CONTROLLER_TARGET) $(OBJS) $(SO_CONTROLLER_OBJS) $(LDLIBS)

check: $(TARGET) $(SO_SENSOR_TARGET) $(SO_CONTROLLER_TARGET)
	./$(TARGET) -v

valgrind:
	valgrind --leak-check=full ./$(TARGET)

clean: 
	rm -f $(OBJS) $(TARGET) $(DEPENDS) $(SO_SENSOR_TARGET) $(SO_SENSOR_OBJS) $(SO_CONTROLLER_TARGET) $(SO_CONTROLLER_OBJS)

debug:
	gdb ./test

-include $(DEPENDS)

.PHONY: clean check all valgrind