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
    RM = rmdir /S /Q "$(1)"
else
    MAKEDIR = '$(SHELL)' -c "mkdir -p \"$(1)\""
    RM = '$(SHELL)' -c "rm -rf \"$(1)\""
endif

TARGET := main
OBJDIR := obj
SOURCES := $(wildcard src/*.c src/*.cpp)
OBJECTS := $(patsubst src%,obj%, $(patsubst %.c,%.o, $(patsubst %.cpp,%.o,$(SOURCES))))

INCLUDE := -I. -I./inc
LIBPATH :=
LIBS := -lncursesw

FLAGS := -Wall
CCFLAGS := $(FLAGS) -std=c99
CXXFLAGS := $(FLAGS) -std=c++2a

CC := gcc
CXX := g++


$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(OBJECTS) -o $(TARGET) $(LIBPATH) $(LIBS)


%.o: ../src/%.c
	$(CC) $(CCFLAGS) $(INCLUDE) -c $< -o $@

%.o: ../src/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@
	
.PHONY: clean help

clean:
	$(call RM,$(OBJDIR))
	$(call MAKEDIR,$(OBJDIR))
	$(call RM,$(TARGET))
	
help:
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'
