
#include <stdio.h>
#include <math.h>

int main() {
	int size = 15, j;
	int ar[size] = { 7,9,2,5,11,14,0,3,25,1,4,6,33,95,8 };
	int k = 1;
	int gap = (int)(size / pow(2, k));
	int i, temp;
	while (gap > 0) {
		printf("\n%d\n", gap);

		for (j = 0;j< (gap);j++) {
			i = j;
			while ((i + gap) < (size)) {
				if (ar[i] > ar[i + gap]) {
					//std::iter_swap(population.begin() + i, population.begin() + (i + gap));
					temp = ar[i];
					ar[i] = ar[i + gap];
					ar[i + gap] = temp;
				}
				i += gap;
			}
		}
		k++;
		gap = (int)(size / pow(2, k));
	}

	for (int i = 0;i < size;i++) {

		printf("%d ", ar[i]);
	}
	printf("\n%d", gap);

}