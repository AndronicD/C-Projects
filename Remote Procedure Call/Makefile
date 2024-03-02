build:
	gcc client.c application_clnt.c application_xdr.c headers/inputReader.c headers/userData.c -Wall -lnsl -ltirpc -g -I/usr/include/tirpc -o client
	gcc server.c application_svc.c application_xdr.c headers/inputReader.c headers/verify.c headers/userData.c -Wall -lnsl -ltirpc -g -I/usr/include/tirpc -o server
clean:
	rm client server client.out server.out