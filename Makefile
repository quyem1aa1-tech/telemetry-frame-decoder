.PHONY: setup-fifo run-sim read-sim test-pipe run-test

# Default target executed when running 'make' without arguments
.DEFAULT_GOAL := help

# Build directory and generator configuration
CXX ?= g++
BUILD_DIR ?= build
GENERATOR ?= Unix Makefiles

.PHONY: help build test setup-fifo run-e2e clean

help: ## Display this list of available commands
	@echo "Usage: make [target]"
	@echo ""
	@echo "Available targets:"
	@awk 'BEGIN {FS = ":.*## "}; /^[a-zA-Z_-]+:.*## / {printf "  \033[36m%-15s\033[0m %s\n", $$1, $$2}' $(MAKEFILE_LIST)

build: ## Configure and compile the project using CMake
	@echo "Configuring project using compiler: $(CXX)"
	@cmake -B $(BUILD_DIR) -S . -G "$(GENERATOR)" \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
		-DCMAKE_CXX_COMPILER=$(CXX)
	@cmake --build $(BUILD_DIR) --parallel

build-clang: ## Compile the project using Clang compiler specifically
	@echo "Switching to Clang toolchain..."
	@$(MAKE) build CXX=clang++ BUILD_DIR=build-clang

test: build ## Run unit tests using CTest
	@ctest --test-dir $(BUILD_DIR) --output-on-failure

setup-fifo: ## Create the Linux Named Pipe (FIFO) for testing
	@bash scripts/setup_fifo.sh

run-e2e: build setup-fifo ## Run end-to-end integration tests (Binary -> Pipe -> Decoder)
	@bash scripts/run_e2e.sh

clean: ## Remove build artifacts and temporary files
	@echo "Cleaning build artifacts..."
	@rm -rf $(BUILD_DIR) build-clang compile_commands.json


# ==============================================================================
# Update / Rebuild Targets
# ==============================================================================

update: build ## Update (incrementally recompile) the default GCC build

update-clang: build-clang ## Update (incrementally recompile) the Clang build

update-all: update update-clang ## Update both GCC and Clang builds sequentially
	@echo "Both GCC and Clang builds have been updated successfully."

# ==============================================================================
# Simulation & Testing Targets
# ==============================================================================

setup-fifo: ## Create the Linux Named Pipe for data stream
	@echo "Setting up FIFO pipe..."
	@bash scripts/setup_fifo.sh

run-sim: setup-fifo ## Generate telemetry frames into FIFO
	@echo "Starting Python sensor simulator..."
	@python3 tests/python/test_pipe.py

read-sim: setup-fifo ## Wait and read telemetry	 from FIFO
	@echo "Waiting for telemetry stream from FIFO..."
	@hexdump -v -e '7/1 "%02X " "\n"' tests/tmp/telemetry_fifo.bin

test-pipe: setup-fifo ## Test reading and writing concurrently in one command
	@echo "Testing pipe concurrently..." 
	@python3 tests/python/test_pipe.py & \
	sleep 0.1 && \
	hexdump -v -e '7/1 "%02X " "\n"' tests/tmp/telemetry_fifo.bin


run-test: ## Run main
	@bash scripts/run_test.sh

