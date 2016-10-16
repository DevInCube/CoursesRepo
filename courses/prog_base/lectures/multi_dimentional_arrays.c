#include <stdio.h>

int main(void) {
	int arr[2][3][4] = {
		{
			{1, 2, 6, 5},
			{2, 2, 6, 7},
			{1, 8, 6, 7},
		},
		{
			{1, 2, 6, 7},
			{5, 2, 5, 3},
			{1, 2, 2, 7},
		}
	};	
	int sum = 0;
	int index = 0;
	int i = 0;
	int j = 0;
	int k = 0;
	int val = 0;
	for (index = 0; index < 2*3*4; index++) {
	
		val = arr[0][0][index];  /* use only 1 index */
		printf("arr[0][0][%i] = %i;", index, val);
		
		i = index / (3 * 4);
		j = index % (3 * 4) / (4);
		k = index % (4);
		val = arr[i][j][k];  /* or use 3 indices from one index */
		printf("\tarr[%i][%i][%i] = %i;", i, j, k, val);
		
		sum += val;
		printf("\tsum = %i;\n", sum);
	}
	printf("Total sum: %i\n", sum);

	return 0;
}
