/*
 * This program adds a vector binary file that first lists the number of 
 * elements in the vector followed by the values
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	int i, opt, num;
	double *vals, sum = 0;
	char *print_file = NULL;
	FILE *fp = NULL;


	while ((opt = getopt(argc, argv, "i:")) != -1)
	{
		switch (opt)
		{
			case 'i':
				print_file = (char *) malloc (sizeof(char) * 100);
				strcpy(print_file, optarg);
				break;
			case '?':
				printf("Incorrect argument(s), exiting\n");
				exit(1);
		}
	}

	if (print_file == NULL)
	{
		print_file = (char *) malloc (sizeof(char) * 100);
		strcpy(print_file, "default-list-file.dat");
	}

	if ((fp = fopen(print_file, "rb")) == NULL)
	{
		printf("Cannot open file %s for adding, exiting\n", print_file);
		exit(1);
	}

	fread(&num, sizeof(int), 1, fp);
	vals = (double *) malloc (sizeof(double) * num);
	fread(vals, sizeof(double), num, fp);

	for (i = 0; i < num; i++)
	{
		sum += vals[i];
	}

	printf("%f\n", sum);

	free(vals);
	free(print_file);

	return 0;
}
