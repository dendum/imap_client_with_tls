# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++14

# Target executable name
TARGET = imapcl

# Arguments
ARGS = eva.fit.vutbr.cz -a auth

# Source files
SRCS = main.cpp parser.cpp error.cpp clientWithoutTLS.cpp

# Object files (generated from source files)
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Linking the object files to create the final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compile the .cpp files into .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up the build (remove object files and executable)
clean:
	rm -f $(OBJS) $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET) $(ARGS)

# Rebuild by cleaning first, then building again
rebuild: clean all
