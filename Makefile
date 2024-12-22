CXX = g++
CXXFLAGS = -std=c++23 -Iinclude -I/opt/homebrew/opt/boost/include \
		   -I/opt/homebrew/opt/openssl/include -O2
LDFLAGS = -L/opt/homebrew/opt/boost/lib -L/opt/homebrew/opt/openssl/lib \
		  -lboost_system -lboost_thread-mt -lssl -lcrypto

SRC_DIR = src
OBJ_DIR = obj
INCLUDE_DIR = include
BIN_DIR = bin

SOURCES = $(wildcard $(SRC_DIR)/*.cpp) \
		  $(wildcard $(SRC_DIR)/DataService/*.cpp) \
		  $(wildcard $(SRC_DIR)/ArbitrageEngine/*.cpp) \
		  $(wildcard $(SRC_DIR)/Config/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))
TARGET = $(BIN_DIR)/arbitrage_engine

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/DataService/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/ArbitrageEngine/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/Config/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
	mkdir -p $(OBJ_DIR)/DataService
	mkdir -p $(OBJ_DIR)/ArbitrageEngine
	mkdir -p $(OBJ_DIR)/Config

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean