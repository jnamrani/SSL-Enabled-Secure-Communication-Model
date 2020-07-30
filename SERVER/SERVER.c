#include<stdio.h>
#include<arpa/inet.h>
#include<errno.h>
#include<unistd.h>
#include<malloc.h>
#include<string.h>
#include<sys/socket.h>
#include<resolv.h>
#include<netdb.h>
#include<openssl/ssl.h>
#include<openssl/err.h>

int create_socket(char hostname, int port)
{
    int s;
    struct hostent *host;
    struct sockaddr_in addr;
    host=gethostbyname(&hostname);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    s = socket(AF_INET, SOCK_STREAM, 0);

    if (s < 0)
    {
    	perror("\n Socket Creation FAILED!");
    	exit(0);
    }
    else printf("\n Socket Creation SUCCESSFUL!");

    if (bind(s, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
    	perror("\n Unable to BIND!");
    	exit(0);
    }

    if (listen(s, 1) < 0)
    {
    	perror("\n Unable to LISTEN!");
    	exit(0);
    }
    else printf("\n LISTENING...");

    fflush(stdout);
    return s;
}

SSL_CTX *create_context()
{
    SSL_METHOD const *method;
    SSL_CTX *ctx;
    SSL_load_error_strings();
    method = SSLv23_server_method();
    ctx = SSL_CTX_new(method);
    if (ctx==NULL)
    {
    	perror("\n Unable to Create/Initialise SSL Context!");
    	ERR_print_errors_fp(stderr);
    	exit(0);
    }
    else printf("\n CONTEXT Initialisation SUCCESSFUL!");
    return ctx;
}

void configure_context(SSL_CTX *ctx)
{
    SSL_CTX_set_ecdh_auto(ctx, 1);

    if (SSL_CTX_use_certificate_file(ctx, "server.crt", SSL_FILETYPE_PEM) <= 0)
    {
        printf("\n Certificate Loading UNSUCCESSFUL!");
    	ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    else printf("\n SERVER's SSL CERTIFICATE LOADED SUCCESSFULLY!");

    if (SSL_CTX_use_PrivateKey_file(ctx, "server.key", SSL_FILETYPE_PEM) <= 0 )
    {
        printf("\n Loading of Private Key FAILED!");
    	ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    else printf("\n ENCRYPTION-DECRYPTION KEYS LOADED SUCCESSFULLY!");
}

int main()
{
long session, seconds;
int bytes, i, sock, port;
char buf[1024];
char hostname;
    printf("\n Welcome, JAYANT NAMRANI! \n You're ready to accept connections from CLIENTS.");
    printf("\n\n Enter IP Address : ");
    scanf("%s", &hostname);
    printf(" Enter the PORT Number : ");
    scanf("%d", &port);
    SSL_library_init();
    printf("\n SSL LIBRARY INITIALISED...");
    OpenSSL_add_all_algorithms();
    SSL_CTX *ctx;
    ctx = create_context();
configure_context(ctx);
sock = create_socket(hostname, port);
struct sockaddr_in addr;
	unsigned int len = sizeof(addr);
	SSL *ssl;
	const char reply[] = "HELLO CLIENT";
	int client = accept(sock, (struct sockaddr*)&addr, &len);
	if (client < 0)
	{
		perror("\n Function 'accept()' returned Negative Value(s) thereby REJECTING Connection Request!");
		exit(EXIT_FAILURE);
	}
	else printf("\n Connection SUCCESSFUL!");

	ssl = SSL_new(ctx);
	SSL_set_fd(ssl, client);
	seconds=60;
	SSL_CTX_set_timeout(ctx, seconds);
	session=SSL_CTX_get_timeout(ctx);
	printf("\n\n *** This session will be timed out after %ld seconds. *** \n", session);
	if (SSL_accept(ssl) <= 0)
	{
		ERR_print_errors_fp(stderr);
		printf("\n ERROR : Cannot establish SECURE Connection. Aborting...");
	}
	else
	{
		for(i=1; i<=5; i++)
		{
			bytes=SSL_read(ssl, buf, sizeof(buf));
			buf[bytes]=0;
			printf("\n\n Message from Client : %s", buf);
			sleep(1);
			printf("\n Replying : %s", reply);
			SSL_write(ssl, reply, strlen(reply));
		}
	}

SSL_free(ssl);
	close(client);
   	close(sock);
    	SSL_CTX_free(ctx);
    	printf("\n\n Connection CLOSED! \n");
}
