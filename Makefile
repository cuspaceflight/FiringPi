# - inc/
#     - *.h
# - src/
#     - *.c
#     - *.cpp
# - obj/
#     - *.o
# - main
ifeq ($(shell echo $$OS),$$OS)
    MAKEDIR = if not exist "$(1)" mkdir "$(1)"
    RM = if exist "$(1)" del /F /S /Q "$(1)"
else
    MAKEDIR = '$(SHELL)' -c "mkdir -p \"$(1)\""
    RM = '$(SHELL)' -c "rm -rf \"$(1)\""
endif

TARGET := main
BUILD := build
SRCDIR := src
INCDIR := include

SOURCES := $(wildcard $(SRCDIR)/*.c $(SRCDIR)/*.cpp)
OBJECTS := $(patsubst $(SRCDIR)%, $(BUILD)%, $(patsubst %.c,%.o, $(patsubst %.cpp,%.o,$(SOURCES))))

HEADERPATH := /usr/include/hdf5/serial
INCLUDE := -I. -I./$(INCDIR) -I$(HEADERPATH)
LIBPATH :=
LIBS := -lncursesw -lwiringPi -li2c -lpthread -lhdf5

FLAGS := -Wall -g -otest
CCFLAGS := $(FLAGS) -std=c99
CXXFLAGS := $(FLAGS) -std=c++2a

CC := gcc
CXX := g++

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(OBJECTS) -o $(TARGET) $(LIBPATH) $(LIBS)

$(BUILD)/%.o: $(SRCDIR)/%.c | $(BUILD)
	$(CC) $(CCFLAGS) $(INCLUDE) -c $< -o $@

$(BUILD):
	$(call MAKEDIR,$(BUILD))

$(BUILD)/%.o: $(SRCDIR)/%.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

.PHONY: clean help

clean:
	$(call RM,$(BUILD))
	$(call RM,$(TARGET))
	
help:
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'
