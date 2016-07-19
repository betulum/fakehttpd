struct sArgs {
	char *ip;
	int port;
	char *dir;
};

void printHelp();
int processArgs(int argc, char **argv, struct sArgs *args);
