#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

int main(int argc, char *argv[]);
int prime_default(int n);

int main(int argc, char *argv[]) {
  int n;
  int n_factor;
  int n_hi;
  int n_lo;
  int primes;
  double t;
  FILE *input;

  if (argc < 2) {
    fprintf(stderr, "Error: missing path to input file\n");
    return 1;
  }

  if ((input = fopen(argv[1], "r")) == NULL) {
    fprintf(stderr, "Error: could not open file\n");
    return 1;
  }

  n_lo = 1;
  n_factor = 2;

 // Do not change this line
  omp_set_num_threads(4);

  fscanf(input, "%d", &n_hi);
  n_hi = 1 << n_hi;

  printf("                    \n");
  printf("         N     Pi(N)\n");
  printf("\n");

  n = n_lo;

  t = omp_get_wtime();

  while (n <= n_hi) {
    primes = prime_default(n);

    printf("  %8d  %8d\n", n, primes);

    n = n * n_factor;
  }

  t = omp_get_wtime() - t;

  /*
    Terminate.
  */
  fprintf(stderr, "%lf\n", t);

  return 0;
}

/*
  Purpose:
   counts primes.
  Licensing:
    This code is distributed under the GNU LGPL license.
  Modified:
    10 July 2010
  Author:
    John Burkardt
  Parameters:
    Input, the maximum number to check.
    Output, the number of prime numbers up to N.
*/
int prime_default(int n) {
	if (n <= 1)
		return 0;
  int total = 1, lim;
#pragma omp parallel for schedule(runtime) private(lim) reduction(+:total)
	for (int i = 3; i <= n; i += 2) {
		total++;
		lim = ceil(sqrt(i));
		for (int j = 2; j <= lim; j++)
			if (i % j == 0) {
				total--;
				break;
			}
	}

  return total;
}
