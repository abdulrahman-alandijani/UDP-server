server: UDP_Server.c
	gcc -w UDP_Server.c -o server
	chmod +x server

client: UDP_Client.c
	gcc -w UDP_Client.c -o client
	chmod +x client

clean:
	rm server client