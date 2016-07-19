#include"arg.h"
#include"worker.h"
#include<stdlib.h>
#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<pthread.h>

int running = 1;
void ctrlc (int dummy) { running = 0; };

int main(int argc, char **argv)
{
	struct sArgs args;
	if (processArgs(argc, argv, &args) < 0) {
		printHelp();
		exit(EXIT_FAILURE);
	}

	if (chdir(args.dir) < 0) {
		perror("chdir");
		return EXIT_FAILURE;
	}	
	
	if (daemon(1,0) < 0) {
		perror("daemon");
		return EXIT_FAILURE;
	}

	sigset_t sigset;
	sigemptyset(&sigset);
	sigaddset(&sigset, SIGINT| SIGTERM);
	if (pthread_sigmask(SIG_BLOCK, &sigset, NULL) != 0) {
		perror("pthread_sigmask");
		return EXIT_FAILURE;
	}

	int cpu_number = sysconf(_SC_NPROCESSORS_ONLN); 
	pthread_t *worker = malloc(cpu_number * sizeof(pthread_t));
	for(int i=0; i<cpu_number; i++) {
		if (pthread_create(worker + i, NULL, worker_function, &args) != 0) {
			perror("pthread_create");
			return EXIT_FAILURE;
		}
	}
	
	struct sigaction sigact;
	sigact.sa_handler = ctrlc;
	sigaction(SIGINT|SIGTERM, &sigact, NULL);
	pthread_sigmask(SIG_UNBLOCK, &sigset, NULL);

	for(int i=0; i<cpu_number; i++)
		pthread_join(worker[i], NULL);
	
	return EXIT_SUCCESS;
}	

