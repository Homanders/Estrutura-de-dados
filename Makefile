CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

TARGET = sistema

SRC = main.cpp \
      src/Generator.cpp \
      src/DataTreatment.cpp \
      src/AVL.cpp \
      src/HashTable.cpp \
      src/LinkedList.cpp \
      src/SkipNode.cpp \
      src/SkipList.cpp \
      src/BloomFilter.cpp \
      src/OptimizedHashTable.cpp \
      src/SensorAnalytics.cpp \
      src/Visualization.cpp

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET) sistema.exe