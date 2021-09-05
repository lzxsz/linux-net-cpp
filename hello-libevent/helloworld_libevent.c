/*
 * helloworld_libevent.c
 *
 * This is a libEvent HelloWorld example. Compiled and run in windows and Linux.
 *
 * Linux compile command: helloworld_libevent.c -levent -o helloworld_libevent 
 * Windows compile tools: VS2010 + libevent-2.0.22-stable
 */

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#include <winsock2.h>

#else /* Linux */
  #include <sys/socket.h>
  #include <errno.h>
#endif

#include <event.h>
#include <evhttp.h>

/* The response function of HTTP root request */
void root_handler(struct evhttp_request *req, void *arg)
{
    struct evbuffer *buf = evbuffer_new();
    if(!buf){
        puts("failed to create response buffer");
        return;
    }

    evbuffer_add_printf(buf, "Hello: %s\n", evhttp_request_uri(req));
    evhttp_send_reply(req, HTTP_OK, "OK", buf);
}

/* The response function of HTTP generic request */
void generic_handler(struct evhttp_request *req, void *arg)
{
    struct evbuffer *buf = evbuffer_new();
    if(!buf){
        puts("failed to create response buffer");
        return;
    }

    evbuffer_add_printf(buf, "Requested: %s\n", evhttp_request_uri(req));
    evhttp_send_reply(req, HTTP_OK, "OK", buf);
}

/* Main function */
int main(int argc, wchar_t* argv[])
{
	struct evhttp *httpd;

#ifdef _WIN32
	WSADATA wsaData;
	DWORD Ret;

	if ((Ret = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0) { /* Windows Sockets Asynchronous Startup */
		printf("WSAStartup failed with error %d\n", Ret);
		return -1;
	}
#else /* Linuix */
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
	{
		printf("The socket startup failed with error %d\n", sock);
		exit(1);
	}
#endif

    event_init(); //Init libEvent

    httpd = evhttp_start("0.0.0.0", 8080); /* Start HTTP server */

    if(!httpd){
	return 1;
    }

    evhttp_set_cb(httpd, "/", root_handler, NULL);  /* Set the response function of HTTP root request */
    evhttp_set_gencb(httpd, generic_handler, NULL); /* Set the response function of HTTP generic request */

	printf("httpd server start OK!\n");

    event_dispatch(); /*  Event Loop. Wait! */
    evhttp_free(httpd); /* Free HTTP server */

#ifdef _WIN32
	WSACleanup(); /* Windows Sockets Asynchronous Cleanup */
#endif

    return 0;
}
