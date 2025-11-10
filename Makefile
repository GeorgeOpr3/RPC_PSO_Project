
CC = gcc
CFLAGS = -Wall -Wextra -pthread
OBJ_CLIENT = rpc_client.o rpc_lib.o main_client.o
OBJ_SERVER = rpc_server.o
DEPS = rpc_common.h rpc_lib.h rpc_client.h


PORT = 5000



all: server client

server: $(OBJ_SERVER)
	$(CC) $(CFLAGS) -o server $(OBJ_SERVER)

client: $(OBJ_CLIENT)
	$(CC) $(CFLAGS) -o client $(OBJ_CLIENT)



rpc_client.o: rpc_client.c $(DEPS)
	$(CC) $(CFLAGS) -c rpc_client.c

main_client.o: main_client.c $(DEPS)
	$(CC) $(CFLAGS) -c main_client.c

rpc_server.o: rpc_server.c $(DEPS)
	$(CC) $(CFLAGS) -c rpc_server.c

rpc_lib.o: rpc_lib.c rpc_lib.h rpc_common.h
	$(CC) $(CFLAGS) -c rpc_lib.c

clean:
	rm -f *.o server client

run_server: server
	./server

run_client: client
	./client
