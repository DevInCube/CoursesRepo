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
		i = index / (3 * 4);
		j = index % (3 * 4) / (4);
		k = index % (4);
		printf("arr[%i][%i][%i] = %i;\t", i, j, k, arr[i][j][k]);
		val = arr[0][0][index];
		sum += val;
		printf("arr[0][0][%i] = %i; \tsum = %i\n", index, val, sum);
	}
	printf("Total sum: %i\n", sum);

	return 0;
}
