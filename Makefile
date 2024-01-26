CC = gcc
CFLAGS = -fdiagnostics-color=always
INCLUDE = -I./src/include/
SRC = ./src/lib/str.c 
SRC += ./src/lib/chat_node.c 
SRC += ./src/lib/UI_params.c 
SRC += ./src/lib/socket.c 
SRC += ./src/app.c
TARGET = ./target/app.exe
LIBS = -luv
default:
	$(CC) $(SRC) -o $(TARGET) $(LIBS) -lpdcurses_wincon -lwinmm $(CFLAGS) $(INCLUDE)
	$(TARGET)
extern:
	$(CC) $(SRC) -o $(TARGET) $(LIBS) -lpdcurses $(CFLAGS) $(INCLUDE)
	$(TARGET)