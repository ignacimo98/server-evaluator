SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

EXE_SERVER := $(BIN_DIR)/echoserver
EXE_SERVERT := $(BIN_DIR)/servert
EXE_CLIENT := $(BIN_DIR)/echoclient

# OBJ_SERVERT := $(filter-out $(OBJ_DIR)/echoclient.o $(OBJ_DIR)/echoserver.o $(OBJ_DIR)/servert.o $(OBJ_DIR)/test_client.o, $(OBJ))
# OBJ_SERVER := $(filter-out $(OBJ_DIR)/echoclient.o $(OBJ_DIR)/echoservert.o $(OBJ_DIR)/servert.o $(OBJ_DIR)/test_client.o, $(OBJ))
# OBJ_CLIENT := $(filter-out $(OBJ_DIR)/echoservert.o $(OBJ_DIR)/echoserver.o $(OBJ_DIR)/sobel.o $(OBJ_DIR)/servert.o $(OBJ_DIR)/test_client.o, $(OBJ))

OBJ_SERVERT := $(filter-out $(OBJ_DIR)/echoclient.o $(OBJ_DIR)/echoserver.o $(OBJ_DIR)/echoservert.o $(OBJ_DIR)/test_client.o $(OBJ_DIR)/server.o, $(OBJ)) 
OBJ_CLIENT := $(filter-out $(OBJ_DIR)/echoservert.o $(OBJ_DIR)/echoserver.o $(OBJ_DIR)/sobel.o $(OBJ_DIR)/servert.o $(OBJ_DIR)/echoclient.o $(OBJ_DIR)/server.o, $(OBJ))
OBJ_SERVER := $(filter-out $(OBJ_DIR)/echoclient.o $(OBJ_DIR)/echoserver.o $(OBJ_DIR)/echoservert.o $(OBJ_DIR)/servert.o $(OBJ_DIR)/test_client.o,  $(OBJ))


CPPFLAGS := -Iinclude -MMD -MP
CFLAGS   := -Wall -g
LDFLAGS  := -Llib
LDLIBS   := -lpthread

.PHONY: all create_image_directory exe clean

all: create_image_directory $(EXE_CLIENT) $(EXE_SERVER) $(EXE_SERVERT)


$(EXE_CLIENT): $(OBJ_CLIENT) | $(BIN_DIR)
		$(CC) $(LDFLAGS) $^  -o $@ $(LDLIBS)

$(EXE_SERVER): $(OBJ_SERVER) | $(BIN_DIR)
		$(CC) $(LDFLAGS) $^  -o $@ $(LDLIBS)

$(EXE_SERVERT): $(OBJ_SERVERT) | $(BIN_DIR)
		$(CC) $(LDFLAGS) $^  -o $@ $(LDLIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
		$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@


$(BIN_DIR) $(OBJ_DIR):
		mkdir -p $@

create_image_directory:
		mkdir -p received_images

clean:
		@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)

-include $(OBJ:.o=.d)
