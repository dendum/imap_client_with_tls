# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++11

LDFLAGS = -lssl -lcrypto

# Target executable name
TARGET = imapcl

# Arguments
#ARGS = imap.pobox.sk -a auth -o cmake-build-debug/msgs -n
ARGS = imap.pobox.sk -p 993 -T -a auth -o cmake-build-debug/msgs -h

# Source files
SRCS = main.cpp parser.cpp client.cpp error.cpp clientWithoutTLS.cpp clientWithTLS.cpp

# Object files (generated from source files)
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Linking the object files to create the final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

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
