# 17. Вернуть корень из целого числа, либо квадрат целого числа в зависимости
# от запроса клиента

# Compiler and flags
CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lm  # Linker flags for the math library

# Directories
CSRC_DIR = c
SERVER_DIR = $(CSRC_DIR)/server
CLIENT_DIR = $(CSRC_DIR)/client
GOSRC_DIR = go

BUILDDIR = build
BINDIR = bin

# Service executables
SERVICES = server client

# Paths for services
SERVER_SRC = $(SERVER_DIR)/main.c
CLIENT_SRC = $(CLIENT_DIR)/main.c

# Output binaries
SERVER_BIN = $(BINDIR)/server
CLIENT_BIN = $(BINDIR)/client

# Default target: build all services
all: $(SERVICES)

# Create output directories if they don't exist
$(BINDIR):
	mkdir -p $(BINDIR)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Compile and build the server executable
server: $(SERVER_BIN)

$(SERVER_BIN): $(SERVER_SRC) | $(BINDIR)
	$(CC) $(CFLAGS) -o $(SERVER_BIN) $(SERVER_SRC) $(LDFLAGS)

# Compile and build the client executable
client: $(CLIENT_BIN)

$(CLIENT_BIN): $(CLIENT_SRC) | $(BINDIR)
	$(CC) $(CFLAGS) -o $(CLIENT_BIN) $(CLIENT_SRC) $(LDFLAGS)

# Run the server
run-server: server
	./$(SERVER_BIN)

# Run the client
run-client: client
	./$(CLIENT_BIN)

run-go:
	go run ./go

# Clean all generated files
clean:
	rm -rf $(BUILDDIR) $(BINDIR)

.PHONY: all clean server client run-server run-client
