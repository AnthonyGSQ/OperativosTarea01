# Variables
CXX := g++
SRC_DIR := src
BIN_DIR := bin
BUILD_DIR := build
TARGET := $(BIN_DIR)/main
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
CXXFLAGS := -Wall -Wextra -std=c++17

# Default target
.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run target
.PHONY: run
run: all
	$(TARGET)

# Clean target
.PHONY: clean
clean:
	rm -rf $(BIN_DIR) $(BUILD_DIR)

# Install dependencies
.PHONY: instdeps
instdeps:
	@echo "Instalando dependencias..."
	@sudo apt-get update && sudo apt-get install -y g++

# Help target
.PHONY: help
help:
	@echo "Opciones disponibles:"
	@echo "  make         Compila el proyecto (por defecto)"
	@echo "  make run     Compila y ejecuta el proyecto"
	@echo "  make clean   Elimina archivos compilados"
	@echo "  make instdeps Instala dependencias necesarias (g++)"
	@echo "  make help    Muestra esta ayuda"
