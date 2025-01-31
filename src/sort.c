
void reisInsertionSort( int arr[], int len ) {
		for ( int i = 0; i < len; i++ ) {
				int curr = arr[i];
				int j = i - 1;
				while ( j >= 0 && arr[j] > curr ) {
						arr[j + 1] = arr[j];
						j--;
				}
				arr[j + 1] = curr;
		}	
}


void reisMergeSort( int arr[], int len ) {

}

void reisKaratsuba( int dividend, int divisor ) {

}
