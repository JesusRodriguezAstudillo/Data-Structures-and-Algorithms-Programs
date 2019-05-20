/*
Author:		Jesus Rodriguez
Program:	program3.cpp
Date;		4 December 2017
Description:	This program is used to perform the radix sort for bases 2, 8, 10, and 16. The program
		loops four times in order to increase the size of the array that will be sorted. The 
		radix sort is run on each array 10 times and the runtime of each radix sort is calculated.
		In the end, the average of the runtimes is calculated and displayed.
*/
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <ctime>
#include <time.h>
#include <queue>
using namespace std;

void populateArray(int* array, int size);
void printArray(int* array, int size);
void radixSort(int* array, int MSD, int size, int base);
void sortSequence(int base, int MSD);
unsigned long long int runtime(struct timespec start, struct timespec end);

int main(int argc, char** argv)
{
	srand(time(NULL)); // seed the random function

	if(argc < 2) // check the number of arguments
	{
		cout << "\tUSAGE: ./a.out 2|8|10|16\n";
		exit(0);
	}

	int base = atoi(argv[1]); // change the command line argument to an int

	if(base == 2) sortSequence(2, 14); // if the base is 2, there are 14 digits in a base 10 4 digit number
	else if(base == 8) sortSequence(8, 5); // if the base is 8, there are 5 digits in a base 10 4 digit number
	else if(base == 10) sortSequence(10, 4); // base 10 uses 4 digits
	else if(base == 16) sortSequence(16, 4); // base 16 and base 10 have the same number of digits
	else cout << "The base: " << base << " is not valid\n"; // the base is not valid

	return 0;
}
/*
*Name:		sortSequence
*Parameter:	an int representing the base to be used in the radix sort, and an int representing
*		the number of digits
*Return:
*Description:	This function is used to run and display the results from the radix sort. The function uses
*		two nested for loops to execute the program. The first loop is used to create an array,
*		whose size is a power of power of 10, populate the array, and display the average run time.
*		The inner loop runs the radix sort ten times, displays how long it took in nanoseconds
*/
void sortSequence(int base, int MSD)
{
	unsigned long long int nanosecs; // an int to track the nanoseconds
	long double avg = 0; // a double to hold the average value
	struct timespec begin, end; // timespec structs to determine the time it takes to run the
				    // radix sot

	for(int i = 0, mult = 1; i < 4; i++) // run the radix sort for arrays of size 10, 100, 1000, 10000
	{
		mult = mult * 10; // the size of the array
		int* array = new int[mult]; // create a new array
	
		for(int j = 0; j < 10; j++) // run the radix sort 10 times
		{
			populateArray(array, mult); // populate array with random values
	
			if(i == 0 && j == 0) 
			{
				// display the unsorted array
				cout << "Unsorted:";
			 	printArray(array, mult);
			}

			// use gettime to dtermine how long the radix sort took
			clock_gettime(CLOCK_REALTIME, &begin);
			radixSort(array, MSD, mult, base); // use radix sort on the array
			clock_gettime(CLOCK_REALTIME, &end);
	
			nanosecs = runtime(begin, end); // determine the runtime of the radix sort in nanoseconds
			avg += nanosecs; // increase the average

			if(i == 0 && j == 0) 
			{
				// display the sorted array
				cout << "Sorted  :"; 
			 	printArray(array, mult);
			}
	
			if(j == 0) cout << "Radix Sort: base = " << base << " size = " << mult << endl;

			// display the runtime
			cout << "Pass" << setw(3) << j+1;
			cout << setw(15) << nanosecs << "   nanoseconds." << endl;
		}
		cout << "Average: " << setw(13) << setprecision(10) << avg/10 << endl << endl; // display the average

		delete []array; // delete the array
	}

	return;
}
/*
*Name:		populateArray
*Parameter:	an int* representing an array and an int representing the size
*Return:
*Description:	This function populates the array with random values between 0 and 9999
*/
void populateArray(int* array, int size)
{
	for(int i = 0; i < size; i++) array[i] = rand() % 10000; // populate the array with random values between 0 and 9999

	return;
}
/*
*Name:		printArray
*Parameter:	an int* representing the array and an int representing the size
*Return:	
*Description:	This function is used to print out an array of ints
*/
void printArray(int* array, int size)
{
	for(int i = 0; i < size; i++) cout << setw(6) << array[i];
	cout << endl;

	return;
}
/*
*Name:			radixSort
*Parameter:		an int* representing an array, an int representing the number of digits in
				the number, an int representing the size of the array, and an int representing
				the base used in the sort algorithm
*Return:
*Description:	This function is used to sort an array of integer values using the radix
				sort algorithm. The function creates an base amount of queues and loops
				MSD times. For each pass the most significant digit is increased until the
				sort is complete.
*/
void radixSort(int* array, int MSD, int size, int base)
{
	queue<int> Q[base]; // create a base amount of queues

	int D = 1;

	for(int i = 1, t; i <= MSD; i++) // for the number of digits
	{
		D = D * base; // the current digit 

		for(int k = 0; k < size; k++) // for each random number in the array
		{
			t = (array[k] % D)/(D / base); // determine which queue to put the number in
			Q[t].push(array[k]); // enqueue the number
		}

		for(int p = 0, j = 0; p < base; p++) // for each queue
		{
			while(!Q[p].empty()) // if queue[p] is not empty
			{
				array[j] = Q[p].front(); // dequeue and place the value back in the array
				Q[p].pop(); // dequeue
				j++; // move on to the next index
			}
		}
	}

	return;
}
/*
*Name:		runtime
*Parameter:	a timespec struct that represents the start time and another timespec struct representing
		the end time to measure
*Return:	an unsigned long long int representing the runtime of a function in nanoseconds
*Description:	This function is used to deteremine the nanoseconds between two struct timespecs. The 
		function makes a decision based on whether the difference between nanoseconds is negative
		or not.
*/
unsigned long long int runtime(struct timespec start, struct timespec end)
{
	unsigned long long int nanoseconds = 0;

	if(end.tv_nsec - start.tv_nsec < 0) // if the difference in nanoseconds is negative
	{
		nanoseconds = 1000000000 - start.tv_sec; // account for the difference between 1 sec
		start.tv_sec++; // increase the start's seconds
		nanoseconds = nanoseconds + ((end.tv_sec - start.tv_sec) * 1000000000); // account for seconds
		nanoseconds = nanoseconds + end.tv_nsec; // get the remaining seconds
	} 
	else
	{
		nanoseconds = (end.tv_sec - start.tv_sec) * 1000000000; // acount for the number of seconds
		nanoseconds += (end.tv_nsec - start.tv_nsec); // take the nanosecond difference
	}

	return nanoseconds;
}
