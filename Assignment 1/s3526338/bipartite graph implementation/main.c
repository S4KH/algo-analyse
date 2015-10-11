/*
 * main.c
 *
 *  Created on: 01/07/2015
 *      Author: Jeffrey Chan
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/time.h>

#include "bipartiteGraph.h"
#include "commonDefs.h"


/* declarations */

#define GETOPT_FINISHED			-1

/* command line options, you may add more to this if you wish (but remember we will use this main.c for testing). */
struct cmdline_opt {
	int verbose;	/* if true, print out results */
	int profile;	/* optional flag, do what you want with this */
};

/* current time */
unsigned long
gettime()
{
        struct timeval tp;
        gettimeofday(&tp, NULL);
        return (tp.tv_sec*1E6) + tp.tv_usec;
}

/* Static function declarations */
void print_usage(const char *name);
int parse_args(int argc, char **argv, struct cmdline_opt *optvals);
void process_datafile(const char *fname, bpGraph_t **ppGraph,
	struct cmdline_opt *optvals);


/* ************************************************************************************* */
/* functions defintions */

/*
 * Main function, the entry point of program.
 */
int main(int argc, char *argv[])
{
	int i = 0;
	int arg_data_index = 0;
	struct cmdline_opt options;
	bpGraph_t *pGraph = NULL;

	/* process the command line arguments */
	arg_data_index = parse_args(argc, argv, &options);

	/* process the commands */
	for (i = arg_data_index; i < argc; i++) {
		process_datafile(argv[i], &pGraph, &options);
	}


	return EXIT_SUCCESS;
} /* end of main() */


void process_datafile(const char *fname, bpGraph_t **ppGraph,
	struct cmdline_opt *optvals)
{
	FILE *file;
	char cmd, cmdType;
	int retVal, val1, val2, val3;
	int outputFlag;
	unsigned long start, end;

	/* sanity check for pointers */
	assert(optvals);

	outputFlag = optvals->verbose;


	if ((file = fopen(fname, "r")) == NULL) {
		fprintf(stderr,"Error: cannot open file %s.\n",	fname);
		return;
	}

	/* we are assuming a well-formed datafile for this project */
	/* However you are welcome to do some error checking (note that the given implemenation can return error codes, which you may want to handle).  But when we test, we will only provide well formed input. */
	while (fscanf(file, "%c %c %d %d %d\n", &cmd, &cmdType, &val1, &val2, &val3) > 0) {
		switch (cmd) {
			/* create graph */
			case 'C':
				if (*ppGraph == NULL) {
					*ppGraph = bipartGraphCreate(val1, val2);
					if (outputFlag) {
						printf("create graph: %d %d\n", val1, val2);
					}
				}
				break;
			/* destroy graph */
			case 'D':
				if (*ppGraph != NULL) {
					bipartGraphDestroy(*ppGraph);
					if (outputFlag) {
						printf("destroyed graph\n");
					}
				}
				break;
			/* insert vertex or edge */
			case 'I':
				switch (cmdType) {
					/* insert vertex */
					case 'V':
                        start = gettime();
						retVal = bipartGraphInsertVertex(*ppGraph, val1, val2);
						if (outputFlag) {
							printf("insert vertex: %d %d %s\n", val1, val2,
								(retVal > 0 ? "new" : "repeat"));
						}
						end = gettime();
						printf("time taken = %.5f sec\n", (double)(end-start)/1E6);
						break;
					/* insert edge */
					case 'E':
                        start = gettime();
						retVal = bipartGraphInsertEdge(*ppGraph, val1, val2, val3);
						if (outputFlag) {
							printf("insert edge: %d %d %d %s\n", val1, val2, val3,
								(retVal > 0 ? "new" : "failed"));
						}
						end = gettime();
						printf("time taken = %.5f sec\n", (double)(end-start)/1E6);
						break;
				}
				break;
			/* remove vertex or edge */
			case 'R':
				switch (cmdType) {
					/* delete vertex */
					case 'V':
                        start = gettime();
						retVal = bipartGraphDeleteVertex(*ppGraph, val1, val2);
						if (outputFlag) {
							printf("removed vertex: %d %d %s\n", val1, val2,
								(retVal > 0 ? "delete" : "missing"));
						}
						end = gettime();
						printf("time taken = %.5f sec\n", (double)(end-start)/1E6);
						break;
					/* delete edge */
					case 'E':
                        start = gettime();
						retVal = bipartGraphDeleteEdge(*ppGraph, val1, val2, val3);
						if (outputFlag) {
							printf("removed edge: %d %d %d %s\n", val1, val2, val3,
								(retVal > 0 ? "delete" : "missing"));
						}
						end = gettime();
						printf("time taken = %.5f sec\n", (double)(end-start)/1E6);
						break;
				}
				break;
			case 'S':
				/* search */
				switch (cmdType) {
					case 'V':
						retVal = bipartGraphFindVertex(*ppGraph, val1, val2);
						if (outputFlag) {
							printf("search: %d %d %s\n", val1, val2,
								(retVal > 0 ? "present" : "absent"));
						}
						break;
					case 'E':
						retVal = bipartGraphFindEdge(*ppGraph, val1, val2, val3);
						if (outputFlag) {
							printf("search: %d %d %d %s\n", val1, val2, val3,
								(retVal > 0 ? "present" : "absent"));
						}
						break;
				}
				break;
			/* define your own commands if it helps with testing */
			case 'P':
				/* print, useful for debugging */
				if (outputFlag) {
					bipartGraphPrint(*ppGraph);
				}
				break;
			default:
				/* ignore any unrecognised commands */
				printf("Unknown command: %c %c", cmd, cmdType);
				break;
		}
	}

	fclose(file);
} /* end of process_datafile() */


/**
 * Parse the command line arguments.
 */
int parse_args(int argc, char **argv, struct cmdline_opt *optvals)
{
	extern int optind;

	int opt = GETOPT_FINISHED;

	if (argc <= 1) {
		print_usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	/* set options to default */
	assert(optvals);
	optvals->verbose = 1;
	optvals->profile = 0;

	/*
		This uses getopt to read command line options.
		See the man page to learn more about getopt.
	*/
	while ((opt = getopt(argc, argv, "qvp")) != GETOPT_FINISHED) {
		switch (opt) {
			case 'q':
				/* quiet mode */
				optvals->verbose = 0;
				break;
			case 'v':
				/* verbose mode (default) */
				optvals->verbose = 1;
				break;
			case 'p':
				/* profiling mode (can do what you like) */
				optvals->profile = 1;
				break;
			default:
				print_usage(argv[0]);
				exit(EXIT_FAILURE);
			}
	}

	return optind;
} /* End of parse_args() */


/**
 * Print usage message.
 */
void print_usage(const char *name)
{
	fprintf(stderr, "Usage: %s [OPTIONS]... [FILES]...\n", name);
	fprintf(stderr, "Implements a basic bipartite graph ADT, "
		"performing the\noperations defined in FILES.\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "Options:\n");
	fprintf(stderr, "  -q\t\tquiet mode "
		"(no output from operations)\n");
	fprintf(stderr, "  -v\t\tverbose mode "
		"(default, output from operations)\n");
	fprintf(stderr, "  -p\t\tprofile mode "
		"(unimplemented, can do what you want)\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "Multiple files can be specified "
		"and they will run sequentially.\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "Example:\n  "
		"%s insert_list.txt search_list.txt\n", name);

	return;
} /* End of print_usage() */
