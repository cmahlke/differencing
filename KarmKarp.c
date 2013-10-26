/*
 *  file:       KarmKarp.c
 *
 *  desc:   Representation I - 
 *          This program is implementation of the Karmarkar-Karp algorithm.  
 *          It is tested against three additional heuristics:
 */

/* To test large integers the linear congruential generator must be commented
 * and the appropriate functions changed in the main body of the code outside the generator.
 */
#if 0
/* linear congruential generator.  Generator x[n+1] = a * x[n] mod m */
#define RAND_INT(l,h) (((int)(random() * ((double)(h)-(l)+1))) + (l))
double random (void);          /* return the next random number x: 0 <= x < 1*/
void rand_seed (unsigned int); /* seed the generator */

static unsigned int SEED = 93186752;

double random() {
	static unsigned int a = 1588635695, m = 4294967291U, q = 2, r = 1117695901;
   SEED = a*(SEED % q) - r*(SEED / q);
   return ((double)SEED / (double)m);
}

void rand_seed(unsigned int init) {
	if (init != 0) 
		SEED = init;
}
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// seed the random generator
void Randomize(void) {
   srand((int) time(NULL));
}

// sum the elelments of the subsets
// usage:	first arg is the name of the array to sum
//			second arg is the index max to stop sum
int sum_subset(int a[], int N) {
	int sum = 0;
	int *p;
	p = &a[0];
	while (p < &a[N])
		sum += *p++;
	return sum;
}

void get_max(int array[], int size_of_array, int* max, int* nextmax, int* maxpos, int *nextmaxpos) {
	int	i;
	
	*maxpos = *nextmaxpos = -1;
	*max = *nextmax = 0;
	for (i = 0; i < size_of_array; i++) {
		if (array[i] > *max) {
			*nextmax = *max;
			*nextmaxpos = *maxpos;
			*max = array[i];
			*maxpos = i;
		} else if (array[i] > *nextmax) {
			*nextmax = array[i];
			*nextmaxpos = i;
		}
	}
}

/* ------ Karmarkar-Karp heuristic algorithm ------
 * compute the difference of the two largest integers in the set
 *		replace the larger integer by the difference
 *		replace the smaller integer w/ zero
 *	repeat
 * The last non-zero integer is the residue
 */
int KarmarkarKarp(int array[], int size_int) {
	int x = 0;
	int y = 0;
	int i = 0;
	int j = 99;
	int residue = 0;

	int	max, nextmax, maxpos, nextmaxpos;
	while (1) {
		get_max(array, size_int, &max, &nextmax, &maxpos, &nextmaxpos);
		if (nextmax == 0)
			{
			residue = max;
			break;
			}
		residue = abs(max - nextmax);
		array[nextmaxpos] = 0;
		array[maxpos] = residue;
	}
	return residue;
}

/* ------ repeated random heuristic ------ */
int* repeated_random(int	loop_cnt) {
	int	set_A[100];
	int	pos_A[100];
	int	neg_A[100];
	int	size_pos;
	int	size_neg;
	int	sum_pos;
	int	sum_neg;
	int	residue;
	int	sub_residue;
	int	i, j;
	int	*ret_int = (int *)malloc(sizeof(int) * 100);	
	int	k;

	residue = -1;
	for (i = 0; i < loop_cnt; i++) {
		// generate random number
		for (j = 0; j < 100; j++)
			set_A[j] = rand();
		
		// generate sign
		for (j = 0; j < 100; j++) {
			if ((rand() % 2) == 0)
				set_A[j] = set_A[j] * -1;
		}

		size_pos = size_neg = 0;
		// create 2 set
		for (j = 0; j < 100; j++) {
			if (set_A[j] < 0) {
				neg_A[size_neg] = set_A[j];
				size_neg++;
			} else {
				pos_A[size_pos] = set_A[j];
				size_pos++;
			} 
		}
		sum_neg = sum_subset(neg_A, size_neg);
		sum_pos = sum_subset(pos_A, size_pos);

		if (residue == -1) {
			residue = abs(sum_neg + sum_pos);
			for (k = 0; k < 100; k++)
				ret_int[k] = set_A[k];
		} else {
			sub_residue = abs(sum_neg + sum_pos);
			if (sub_residue < residue) {
				residue = sub_residue;
				for (k = 0; k < 100; k++)
					ret_int[k] = set_A[k];
			}
		}
	}
	printf("repeated random residue = %d\n", residue);
	return ret_int;
}

int* hill_climbing(int loop_cnt) {
	int i = 0;
	int j = 0;
	int k = 0;
	int l = 0;
   int set_A[100];	// initial "solution" of 100 random integers
   int pos_arr[100]; // + subset values
	int pos_size;
   int neg_arr[100]; // - subset values
	int neg_size;
	int sum_pos;
	int sum_neg;
	int residue;
	int sub_residue;
	int *ret_int = (int *)malloc(sizeof(int) * 100);	

	// generate 100 random numbers and store them in set_A
   for (i = 0; i < 100 ; i++) {
		set_A[i] = rand();
   }

	//	Change the sign of S
	for (i = 0; i < 100; i++) {
		if ((rand() % 2) == 0) {	/* negative */
			set_A[i] = set_A[i] * -1;
		} 
	}

	// initialize the return
	for (k = 0; k < 100; k++)
		ret_int[k] = set_A[k];

	/*	partition the array
	 * mod 2 == 0 is negative
	 *	mod 2 == 1 is positive
	 */
	pos_size = neg_size = 0;
	for (i = 0; i < 100; i++) {
		if (set_A[i] < 0) {
			neg_arr[neg_size] = set_A[i];
			neg_size++;
		} else {
			pos_arr[pos_size] = set_A[i];
			pos_size++;
		}
	}

	// get negative sum
	sum_neg = sum_subset(neg_arr, neg_size);
	sum_pos = sum_subset(pos_arr, pos_size);

	residue = abs(sum_neg + sum_pos);

	for (l = 0; l < loop_cnt; l++) {
		i = (rand() % 100);
		j = (rand() % 100);

		set_A[i] = set_A[i] * -1;
		if ((rand() % 2) == 0)
			set_A[j] = set_A[j] * -1;

		// split the array
		pos_size = neg_size = 0;
		for (i = 0; i < 100; i++) {
			if (set_A[i] < 0) {
				neg_arr[neg_size] = set_A[i];
				neg_size++;
			} else {
				pos_arr[pos_size] = set_A[i];
				pos_size++;
			}
		}	
		// get negative sum
		sum_neg = sum_subset(neg_arr, neg_size);
		sum_pos = sum_subset(pos_arr, pos_size);

		sub_residue = abs(sum_neg + sum_pos);
		if (sub_residue < residue) {
			residue = sub_residue;
			for (k = 0; k < 100; k++)
				ret_int[k] = set_A[k];
		}
	}
	printf("hill climbing residue = %d\n", residue);
	return ret_int;
}

/* ------ simulated annealing ------ */
int* simulated_annealing(int loop_cnt) {
   int set_A[100];	// initial "solution" of 100 random integers
   int pos_arr[100]; // + subset values
	int pos_size;
   int neg_arr[100]; // - subset values
	int neg_size;
	int sum_pos;
	int sum_neg;
	int residue;
	int sub_residue;
	double prob;
	int	*ret_int = (int *)malloc(sizeof(int) * 100);	
	int i, k, l, j;

	// generate 100 random numbers and store them in set_A
   for (i = 0; i < 100 ; i++) {
		set_A[i] = rand();
   }

	//	Change the sign of S
	for (i = 0; i < 100; i++) {
		if ((rand() % 2) == 0) {	// negative
			set_A[i] = set_A[i] * -1;
		} 
	}

	// initialize the return
	for (k = 0; k < 100; k++)
		ret_int[k] = set_A[k];

	/*	partition the array
	 * mod 2 == 0 is negative
	 *	mod 2 == 1 is positive
	 */
	pos_size = neg_size = 0;
	for (i = 0; i < 100; i++) {
		if (set_A[i] < 0) {
			neg_arr[neg_size] = set_A[i];
			neg_size++;
		} else {
			pos_arr[pos_size] = set_A[i];
			pos_size++;
		}
	}

	// get negative sum
	sum_neg = sum_subset(neg_arr, neg_size);
	sum_pos = sum_subset(pos_arr, pos_size);

	residue = abs(sum_neg + sum_pos);

	for (l = 0; l < loop_cnt; l++) {
		i = (rand() % 100);
		j = (rand() % 100);

		set_A[i] = set_A[i] * -1;
		if ((rand() % 2) == 0)
			set_A[j] = set_A[j] * -1;

		// split the array
		pos_size = neg_size = 0;
		for (i = 0; i < 100; i++) {
			if (set_A[i] < 0) {
				neg_arr[neg_size] = set_A[i];
				neg_size++;
			} else {
				pos_arr[pos_size] = set_A[i];
				pos_size++;
			}
		}	
		// get negative sum
		sum_neg = sum_subset(neg_arr, neg_size);
		sum_pos = sum_subset(pos_arr, pos_size);

		sub_residue = abs(sum_neg + sum_pos);
		if (sub_residue < residue) {
			residue = sub_residue;
			for (k = 0; k < 100; k++)
				ret_int[k] = set_A[k];
		} else {
			prob = exp(((-1) * (sub_residue - residue))/(pow(10, 7)*pow(0.8, (l/300))));
			/* NOTE: not sure how the prob works here, assuming > .5, set ret_int */
			if (prob > 0.5) {
				for (k = 0; k < 100; k++)
					ret_int[k] = set_A[k];
			}
		}
	}
	printf("simulated annealing residue = %d\n", residue);
	return ret_int;
}

/********** DEBUGGING ***********************/
void NEWLINE(void) { printf("-------------------------------------------\n"); }
static void PrintArray(const int array[], int n) {
	int i;
	for (i = 0; i < n; i++) { printf("%d: ", i); printf("%d\n", array[i]); }
}

/**********************************************************
 * main							                               *
 **********************************************************/
main() {
	int	loop_cnt;
   int	*random_arr;	// initial "solution" of 100 random integers
	int	*hill_climing_arr;
	int	*simulated_arr;
	int	i;
	int	set_A[100];

	loop_cnt = 25000;	/* at least 25000 */

	// generate 100 random numbers and store them in set_A
   for (i = 0; i < 100 ; i++) {
		set_A[i] = rand();
   }

	// seed the random number generator
   Randomize();
   printf("On this computer RAND_MAX: %7d\n", RAND_MAX);
	printf("Number of iterations: %12d\n\n", loop_cnt);

	random_arr = repeated_random(loop_cnt);
	hill_climing_arr = hill_climbing(loop_cnt);
	simulated_arr = simulated_annealing(loop_cnt);
	NEWLINE();
	printf("KarmarkarKap residue = %d\n", KarmarkarKarp(set_A, 100));
	NEWLINE();
}
