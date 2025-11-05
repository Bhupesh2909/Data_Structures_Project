CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude


SRC_DIR = source
OBJ_DIR = obj
INC_DIR = include

SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))
TARGET = h2optimizer


$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)