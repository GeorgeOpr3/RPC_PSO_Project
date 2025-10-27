all:
	gcc rpc_client.c -o client
	gcc rpc_server.c -o server

clean:
	rm server
	rm client