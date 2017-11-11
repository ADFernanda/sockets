all: clienteFTP servidorFTPc
	
clienteFTP:
	gcc clienteFTP.c -o clienteFTP

servidorFTPc:
	gcc servidorFTP.c -o servidorFTP
