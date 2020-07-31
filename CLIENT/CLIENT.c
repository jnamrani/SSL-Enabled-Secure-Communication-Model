#include<stdio.h>
#include<errno.h>
#include<unistd.h>
#include<malloc.h>
#include<string.h>
#include<sys/socket.h>
#include<resolv.h>
#include<netdb.h>
#include<openssl/ssl.h>
#include<openssl/err.h>
int OPEN_CONNECTION (char* hostname, int port)
{
	int s;
	struct hostent *host;
	struct sockaddr_in addr;
	host=gethostbyname(hostname);
	if( host == NULL )
	{
		perror(&hostname);
		abort();
	}
	s = socket(PF_INET, SOCK_STREAM, 0);
	bzero(&addr, sizeof(addr));
	addr.sin_family=AF_INET;
	addr.sin_port=htons(port);
	addr.sin_addr.s_addr = *(long*)(host->h_addr);
	if (connect(s, (struct sockaddr*)&addr, sizeof(addr)) != 0)
	{
		close(s);
		perror(hostname);
		abort();
	}
	else printf("\n\n CONNECTION SUCCESSFUL!");
	return s;
}

SSL_CTX *InitCTX(void)
{
	SSL_METHOD const *method;
	SSL_CTX *ctx;
	OpenSSL_add_all_algorithms();
	SSL_load_error_strings();
	method=TLSv1_2_client_method();
	ctx=SSL_CTX_new(method);
	if(ctx==NULL)
	{
		printf("\n ERROR occurred while INITIALISING CONTEXT! ABORTING...");
		ERR_print_errors_fp(stderr);
		abort();
	}
	else
		printf("\n CONTEXT Initialisation SUCCESSFUL!");
	return ctx;
}

int main()
{
	long session, seconds;
	int server, bytes, i, portnum;
	char buf[1024];
	char ClientRequest[1024]={0};
	char hostname[20];
	SSL_CTX *ctx;
	SSL *ssl;
	printf("\n Hello JAYANT NAMRANI! \n You're going to be connected to a SERVER.");
	printf("\n\n Enter IP Address : ");
	scanf("%s", hostname);
	printf(" Enter the PORT Number : ");
	scanf("%d", &portnum);
	SSL_library_init();
	printf("\n SSL LIBRARY INITIALISED...");
	ctx=InitCTX();
	server=OPEN_CONNECTION(hostname, portnum);
	ssl=SSL_new(ctx);
	SSL_set_fd(ssl, server);
	fflush(stdout);
	seconds=60;
	SSL_CTX_set_timeout(ctx, seconds);
	session=SSL_CTX_get_timeout(ctx);
	printf("\n\n *** This session will be timed out after %ld seconds. *** \n", session);
	if (SSL_connect(ssl) < 0)
	{
		printf("\n ERROR : Cannot establish SECURE Connection. Aborting...");
		ERR_print_errors_fp(stderr);
	}
	else
	{
		const char RequestMessage[1024]="HELLO SERVER";
		fflush(stdout);
		printf("\n Connected with %s Encryption...", SSL_get_cipher(ssl));
		for(i=1; i<=5; i++)
		{
			printf("\n\n Sending Message %d '%s'... ", i, RequestMessage);
			sprintf(ClientRequest, RequestMessage);
			SSL_write(ssl, ClientRequest, strlen(ClientRequest));
			sleep(5);
			bytes=SSL_read(ssl, buf, sizeof(buf));
			buf[bytes]=0;
			printf("\n Message from Server : %s", buf);
		}
		SSL_free(ssl);
	}
	close(server);
	SSL_CTX_free(ctx);
	printf("\n\n Connection CLOSED! \n");
	return 0;
}
