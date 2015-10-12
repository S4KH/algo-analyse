
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/time.h>

/*
   Compile - gcc −Wall −O3 −o knapsack knapsack.c
   Execute - ./knapsack −v −n 8 −W 200
*/

/*
 * Data structure for the set of items, which each have a weight, value. 
 * n is the number of items in our set.
 */
typedef struct {
	uint8_t* weight;
	uint8_t* value;
	int32_t n;
} items_t;

/* Macro to get maximum of two values. */
#define MAX(a, b) (((a) > (b)) ? (a) : (b))


/*
 * Can be used to get timing information.
 */
unsigned long
gettime()
{
    struct timeval tp;
    gettimeofday(&tp ,NULL);
    return (tp.tv_sec*1E6) + tp.tv_usec;
}


/* 
 * Generate a random set of items, could be used for testing.
 */
items_t*
generate_random_items(int32_t n,int32_t W)
{
	int32_t i;
	items_t* items;

	/* init rand() */
	srand( getpid() );

	/* malloc */
	items = (items_t*) malloc(sizeof(items_t));
	if(!items) {
		fprintf(stderr, "Cannot malloc().\n");
		exit(EXIT_FAILURE);
	}
	items->weight = (uint8_t*) malloc(n*sizeof(uint8_t));
	items->value = (uint8_t*) malloc(n*sizeof(uint8_t));
	if(!items->weight || !items->value) {
		fprintf(stderr, "Cannot malloc().\n");
		exit(EXIT_FAILURE);
	}

	/* generate */
	for (i = 0; i < n; ++i) {
		items->weight[i] = 1 + (rand() % (W>>1));
		items->value[i] = 1 + (rand() % 255);
	}

	items->n = n;
	return items;
}



/*
 * Print out the set of items. 
 */
void
print_items(items_t* items) {
	uint32_t i;
	fprintf(stdout," item | weight | value \n");
	for(i=0;i<items->n;i++) {
		fprintf(stdout,"%4u %7u %8u\n",i,items->weight[i],items->value[i]);
	}
    fflush(stdout);
}


/*
 * Print out dynamic programming table.
 */
void
print_table(int** T,int32_t x,int32_t y)
{
    int32_t i,j;
    for(i=0;i<x;i++) {
        for(j=0;j<y;j++) fprintf(stdout,"%4d ",T[i][j]);
        fprintf(stdout, "\n");
    }
    fflush(stdout);
}

/*
 * Top-down Knapsack implementation. 
 */
int32_t
mfknapsack(items_t* items,uint32_t W)
{
	int32_t i,j;
	int32_t maxvalue =  0;


	/* malloc table */
    int32_t ** f= (int32_t**)malloc(items->n*sizeof(int32_t*));

    for(i=0;i<=items->n;i++){
        f[i] = (int32_t*)malloc(W*sizeof(int32_t));        
    }

    /* init table */

    for(j=1;j<=W;j++){
        f[0][j] = -1;
    }

    /* solve */

    for(i=1;i<=items->n;i++){
        f[i][0]=-1;
        for(j=1;j<=W;j++){
            if(items->weight[i]>j){                
                f[i][j] = f[i-1][j];
            }else{
                f[i][j] = MAX(f[i-1][j], items->value[i]+f[i-1][j-items->weight[i]]);
            }
        }
    }

    maxvalue = f[items->n][W];

	/* cleanup */

    for(i=0;i<=items->n;i++){
        free(f[i]);
    }

    free(f);

	return maxvalue;
}


/*
 * Bottom up knapsack implementation.
 */  
int32_t
dpknapsack(items_t* items,uint32_t W)
{
    int32_t i,j;
    int32_t maxvalue = 0;

    /* malloc table */

    int32_t ** v = (int32_t**)malloc(items->n*sizeof(int32_t*));

    for(i=0;i<=items->n;i++){
        v[i] = (int32_t*)malloc(W*sizeof(int32_t));
        v[i][0] = 0;
    }

    /* init table */

    for(j=1;j<=W;j++){
        v[0][j] = 0;
    }

    /* solve */

    for(i=1;i<=items->n;i++){
        for(j=1;j<=W;j++){
            if(j-items->weight[i]>=0){                
                v[i][j] = MAX(v[i-1][j], (items->value[i]+v[i-1][j-items->weight[i]]));                
            }else{
                v[i][j] = v[i-1][j];
            }
        }
    }

    maxvalue = v[items->n][W];

    /* cleanup */

    for(i=0;i<=items->n;i++){
        free(v[i]);
    }
    free(v);

    return maxvalue;
}

/* **************************************************** */

/*
 * Main function. 
 */
int main(int argc,char** argv)
{
	items_t* items;
	int32_t n,W,verbose,op;
	int32_t maxvalue;
    unsigned long start,stop;

	/* parse args */
    n = -1;W = -1;verbose = 0;
    while( (op=getopt(argc,argv,"n:W:v")) != -1 ) {
        switch (op) {
            case 'n':
                n = atoi(optarg);
                break;
            case 'W':
                W = atoi(optarg);
                break;
            case 'v':
                verbose = 1;
                break;
            default:
                fprintf(stdout, "Usage: %s -v -n <items> -W <size>\n",argv[0]);
                return EXIT_FAILURE;
        }
    }
    if(n < 1 || W < 1) {
        fprintf(stdout, "Usage: %s -v -n <items> -W <size>\n",argv[0]);
        return EXIT_FAILURE;
    }

	/* generate random items */
	items = generate_random_items(n,W);

    if(verbose) {
        fprintf(stdout, "n = %d W = %d\n",n,W);
        /* print items */
        print_items(items);
    }

	/* solve */
	maxvalue = 0;

    start = gettime();
	maxvalue = mfknapsack(items,W);
    stop = gettime();
    fprintf(stdout, "mfknapsack() time taken = %.2f sec\n",
                    (double)(stop-start)/1E6);
    fprintf(stdout, "W %u maximum value: %d\n",W,maxvalue);

    start = gettime();
	maxvalue = dpknapsack(items,W);
    stop = gettime();
    fprintf(stdout, "dpknapsack() time taken = %.2f sec\n",
                    (double)(stop-start)/1E6);
    fprintf(stdout, "W %u maximum value: %d\n",W,maxvalue);

	/* cleanup */
	free(items->weight);
	free(items->value);
	free(items);
	return EXIT_SUCCESS;
}
