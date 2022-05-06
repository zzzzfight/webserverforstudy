CXX = g++
CFLAGS = -std=c++11 -Wall -g -v -I/usr/include/cppconn -L/usr/lib -lmysqlcppconn -o
TARGET = test
OBJS = ./temptest.cpp
test:
	$(CXX) $(CFLAGS) $(TARGET) $(OBJS)