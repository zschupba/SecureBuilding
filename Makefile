CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

# Source files for local build
LOCAL_SRCS = serverInputHandler.cpp logAppend.cpp logRead.cpp

# Source files for server build
SERVER_SRCS = serverNetworking.cpp serverInputHandler.cpp logAppend.cpp logRead.cpp

# Output binaries
LOCAL_BIN = local
SERVER_BIN = server

# Default target
all: local server

# Build for local machine
local: $(LOCAL_SRCS)
	$(CXX) $(CXXFLAGS) -o $(LOCAL_BIN) $(LOCAL_SRCS)

# Build for server
server: $(SERVER_SRCS)
	$(CXX) $(CXXFLAGS) -o $(SERVER_BIN) $(SERVER_SRCS)

# Clean compiled files
clean:
	rm -f $(LOCAL_BIN) $(SERVER_BIN)
