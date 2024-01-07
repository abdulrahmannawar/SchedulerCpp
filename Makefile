CXX := clang++
CXXFLAGS := -std=c++17 -Wall -Wextra
ASFLAGS := -Wall -Wextra

SRC_DIR := src
BUILD_DIR := build
OBJ_DIR := obj
INCLUDE_DIR := include

TASK1_DIR := $(SRC_DIR)/Task1
TASK2_DIR := $(SRC_DIR)/Task2
TASK3_DIR := $(SRC_DIR)/Task3
TESTS_DIR := $(SRC_DIR)/Tests
FIBER_API_DIR := FiberAPI
SIMPLETEST_DIR := simpletest

TASK1_SRC := $(wildcard $(TASK1_DIR)/main.cpp)
TASK1_PSEUDO_SRC := $(wildcard $(TASK1_DIR)/pseudo.cpp)
TASK2_SRC := $(wildcard $(TASK2_DIR)/*.cpp)
TASK3_SRC := $(wildcard $(TASK3_DIR)/main.cpp $(TASK3_DIR)/fiber.cpp $(TASK3_DIR)/scheduler.cpp)
TASK3_API_SRC := $(wildcard $(TASK3_DIR)/main_api.cpp)
TEST_SRC := $(wildcard $(TESTS_DIR)/*.cpp)
FIBER_API_SRC := $(wildcard $(FIBER_API_DIR)/*.cpp)
SIMPLETEST_SRC := $(wildcard $(SIMPLETEST_DIR)/*.cpp)

TASK1_EXE := $(BUILD_DIR)/task1
TASK1_PSEUDO_EXE := $(BUILD_DIR)/task1_pseudo
TASK2_EXE := $(BUILD_DIR)/task2
TASK3_EXE := $(BUILD_DIR)/task3
TASK3_API_EXE := $(BUILD_DIR)/task3_api
TEST_EXE := $(BUILD_DIR)/tests

COMMON_OBJ := $(OBJ_DIR)/context.o

.PHONY: all clean run task1 task2 task3 task1_pseudo task3_api tests


all: update_submodules $(OBJ_DIR) $(BUILD_DIR) $(TASK1_EXE) $(TASK2_EXE) $(TASK3_EXE) $(TASK1_PSEUDO_EXE) $(TASK3_API_EXE) $(TEST_EXE)

$(OBJ_DIR):
	@mkdir -p $@

$(BUILD_DIR):
	@mkdir -p $@

$(BUILD_DIR)/task1: $(TASK1_SRC) $(COMMON_OBJ)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) $^ -o $@

$(BUILD_DIR)/task1_pseudo: $(TASK1_PSEUDO_SRC) $(COMMON_OBJ)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) $^ -o $@

$(BUILD_DIR)/task2: $(TASK2_SRC) $(COMMON_OBJ)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) $^ -o $@

$(BUILD_DIR)/task3: $(TASK3_SRC) $(COMMON_OBJ)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) $^ -o $@

$(BUILD_DIR)/task3_api: $(TASK3_API_SRC) $(COMMON_OBJ)
	$(CXX) $(CXXFLAGS) $(FIBER_API_SRC) -I$(INCLUDE_DIR) $^ -o $@

$(BUILD_DIR)/tests: $(TEST_SRC) $(COMMON_OBJ) $(FIBER_API_SRC) $(SIMPLETEST_SRC)
	$(CXX) $(CXXFLAGS)  -I$(INCLUDE_DIR) $^ -o $@

$(OBJ_DIR)/context.o: context/context.s
	$(CXX) $(ASFLAGS) -c $< -o $@

clean:
	@rm -rf $(BUILD_DIR)/* $(OBJ_DIR)/*

run:
	@./$(BUILD_DIR)/$(shell echo $(filter-out $@,$(MAKECMDGOALS)) | tr A-Z a-z)

task1 task2 task3 task1_pseudo tests task3_api:
	@:

update_submodules:
	@git submodule init
	@git submodule update