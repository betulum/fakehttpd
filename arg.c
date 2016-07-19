#include<getopt.h>
#include<stdio.h>
#include"arg.h"

void printHelp()
{
	printf("Usage:\n");
	printf("\tfinal  -h <host> -p <port> -d <directory>\n");
}

int processArgs(int argc, char **argv, struct sArgs *args)
{
	if (argc < 2)
		return -1;
	
	int opt;
	const char argstr[] = "h:p:d:";
	while ((opt = getopt(argc, argv, argstr)) != -1) {
		switch (opt) {
		case 'h':
			args->ip = optarg;
			break;
		case 'p':
			args->port = atoi(optarg);
			break;
		case 'd':
			args->dir = optarg;
			break;
		default:
			return -1;
		}
	}
	return 0;
}
