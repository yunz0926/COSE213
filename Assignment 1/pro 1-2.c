int count = 0; //전역변수 선언

void multi(int a[][MAX_SIZE], int b[][MAX_SIZE], int c[][MAX_SIZE])
{
	int i, j, k;
	for (i = 0; i < MAX_SIZE; i++) {
		for (j = 0; j < MAX_SIZE; j++) {
			c[i][j] = 0;
			count += 2;  //for j loop & assignment
			for (k = 0; k < MAX_SIZE; k++) {
				c[i][j] += a[i][k] * b[k][j];
				count += 2;  //for k loop & assignment
			}
			count++;  //for last of k loop
		}
		count += 2;  //for i loop & last of j loop
	}
	count++;  //for last of i loop
}