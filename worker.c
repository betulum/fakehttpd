#include<ev.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>

#include"arg.h"

void ok(int sock) {
	char buf[64];
	sprintf(buf, "HTTP/1.0 200 OK\r\n");
	send(sock, buf, strlen(buf), MSG_NOSIGNAL);
}

void fail(int sock) {
	char buf[64];
	sprintf(buf, "HTTP/1.0 400 BAD REQUEST\r\n");
	send(sock, buf, strlen(buf), MSG_NOSIGNAL);
}

void nofile(int sock) {
	char buf[64];
	sprintf(buf, "HTTP/1.0 404 RESOURCE NOT AVAILABLE\r\n");
	send(sock, buf, strlen(buf), MSG_NOSIGNAL);
}

void senddata(const char *path, int sock) {
	int fd = open(path, O_RDONLY);
	if (fd < 0) {
		nofile(sock);
	} else {
		char buf[1024*1024];	
		struct stat st;
		stat(path, &st);
		int bytes = read(fd, buf, st.st_size);
		if (bytes < 0) 
			nofile(sock);
		else {
			ok(sock);
			send(sock, buf, bytes, MSG_NOSIGNAL); 
		}
		close(fd);
	}
}

extern int running;
void read_cb(struct ev_loop *loop, struct ev_io *watcher, int revents)
{
	char buf[1024];
	ssize_t rec = recv(watcher->fd, buf, 1024, MSG_NOSIGNAL);
	if (rec < 0) 
		return;
	else if (rec == 0) {
		ev_io_stop(loop, watcher);
		free(watcher);
		return;
	} else {
		char path[256];
		int res = sscanf(buf, "GET %s", path);
 		if (res == 0 || res == EOF) 
			fail(watcher->fd);
		else { 
			senddata(path, watcher->fd);
			//send(watcher->fd, buf, rec, MSG_NOSIGNAL);
		}
	}
}

void accept_cb(struct ev_loop *loop, struct ev_io *watcher, int revents)
{
	int sock = accept(watcher->fd, 0, 0);
	struct ev_io *w_client = malloc(sizeof(struct ev_io));
	
	ev_io_init(w_client, read_cb, sock, EV_READ);
	ev_io_start(loop, w_client);
}

void *worker_function(void *arg) 
{
	struct sArgs *args = (struct sArgs *)arg;
	int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(args->port);
	addr.sin_addr.s_addr = inet_addr(args->ip);
	bind(sock, (struct sockaddr *) &addr, sizeof(addr));
	listen(sock, SOMAXCONN);

	struct ev_loop *loop = ev_loop_new(0);
	struct ev_io w_accept;
	ev_io_init(&w_accept, accept_cb, sock, EV_READ);
	ev_io_start(loop, &w_accept);
	
	while (running) {
		ev_loop(loop, 0);
	}
	ev_loop_destroy(loop);
	return NULL;
}

