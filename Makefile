CXX ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -pedantic
all: mouse
mouse: Main.cpp API.cpp API.h Maze.h RobotIO.h MmsRobot.h
	$(CXX) $(CXXFLAGS) Main.cpp API.cpp -o mouse
clean:
	rm -f mouse
