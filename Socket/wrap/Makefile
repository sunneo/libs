DEP= sckwrap.c
all: factor_client factor_server
clean:
	rm -fv factor_client factor_server

factor_client: factor_client.c
	gcc -o factor_client factor_client.c $(DEP)

factor_server: factor_server.c
	gcc -o factor_server factor_server.c $(DEP)
