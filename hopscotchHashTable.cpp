/*
Author:		Jesus Rodriguez
Name:		pg2.cpp
Date:		11/15/2017
Description:	This program implements the Hopscotch Hash algorithm. The program takes a number from
		the user, determines its hash key and attempts to put it in the bucket. If the value
		is placed within MAX DISTANCE, the function finishes. Otherwise, the program uses 
		recursion to determine a place for the value. Adding a value will fail if there are
		no empty buckets or if there are no values that can hop and stay within MAX DISTANCE.
		The program uses a bitmap to represent the final location of entered values. In
		addition, this program can print the Hashtable and the bitmap, search, and delete values.
*/
#include <iostream>
#include <iomanip>
using namespace std;

#define MAX_DISTANCE 4 // the maximum distance allowed to be placed in the Hashtable
#define TABLE_SIZE 17 

int Hash(int k);
bool Add(int k, int *HashTable, bool bitmap[][MAX_DISTANCE]);
int Search(const int search_value, const int *HashTable);
void Delete(int del_value, int *HashTable, bool bitmap[][MAX_DISTANCE]);
void Initialize(int *HashTable, bool bitmap[][MAX_DISTANCE]);
void Print(const int *HashTable, const bool bitmap[][MAX_DISTANCE]);
bool Hop(int num, int index, int *HashTable, bool bitmap[][MAX_DISTANCE]);

int main()
{
	bool bitmap[TABLE_SIZE][MAX_DISTANCE]; // the bit map for each location in the Hashtable
	int HashTable[TABLE_SIZE]; // the hash table
	int choice; // the number used for the menu choice and the inserted value
	bool done = false; // a bool to determine when to end the do while loop

	Initialize(HashTable, bitmap); // call a function to initialize the positions in both arrays

	do
	{
		// display the menu
		cout << "HOPSCOTCH HASHING MENU" << endl;
		cout << " 1 - Insert Value" << endl;
		cout << " 2 - Delete Value" << endl;
		cout << " 3 - Search Value" << endl;
		cout << " 4 - Output Table" << endl;
		cout << " 5 - Exit Program" << endl;

		// ask for input from the user
		cout << "Select an item from the menu: ";
		cin  >> choice;
	
		// use a switch case to determine which action to take
		switch(choice)
		{
			// case 1 add a value
			case 1: cout << "Enter a positive number to add to the Hopscotch Hash Table: ";
				cin  >> choice;
				if(choice < 0) cout << choice << " is not a valid value.\n";
				else Add(choice, HashTable, bitmap);
				break;
			// case 2 delete a value
			case 2: cout << "Enter a positive number to delete from the Hopscotch Hash Table: ";
				cin  >> choice;
				if(choice < 0) cout << choice << " is not a valid value.\n";
				else Delete(choice, HashTable, bitmap);
				break;
			// case 3 search for a value
			case 3: cout << "Enter a positive number to search for in the Hopscotch Hash Table: ";
				cin  >> choice;
				if(choice < 0) cout << choice << " is not a valid value.\n";
				else
				{
					int i = Search(choice, HashTable);
					if(i < 0) cout << "Failed to find " << choice << endl;
					else cout << choice << " found in index: " << i << endl;
				}
				break;
			// case 4 print the tables
			case 4: Print(HashTable, bitmap);
				break;
			// case 5 quit
			case 5: cout << "Terminating program...\n";
				done = true;
				break;
			default: cout << "Invalid selection. Enter a number between 1 and 5 inclusively\n"; // the input is invalid
		}
	}while(!done);

	return 0;
}
/*
Name:		Hash
Parameter:	an int representing the value that must be Hashed
Return:		an int representing the key of the input number
Description	this function is used to determine the Hash key of a value passed to the function
*/
int Hash(int k)
{
	return k % TABLE_SIZE; // return the Hashed value
}
/*
Name:		Initialize
Parameter:	an int pointer representing a HashTable and a 2D array represeting the bitmap of each Hash
Return:		
Description	This function is used to initialize all the positions of the Hashtable and the bitmap
*/
void Initialize(int *HashTable, bool bitmap[][MAX_DISTANCE])
{
	for(int i = 0; i < TABLE_SIZE; i++)
	{
		HashTable[i] = -1;
		for(int j = 0; j < MAX_DISTANCE; j++) bitmap[i][j] = 0;
	}

	return;
}
/*
Name:		Add
Parameter:	a int representing the value that is to be added, an int* representing the Hashtable, and 
		a bool 2D array representing the bitmap
Return:		a bool representing if the value was added successfully or not
Description	This function attempts to add a value to the Hashtable. If the value is placed within a bucket
		that is less than the max distace, the function return true and changes the table and bitmap. If
		the table reaches the end of the table, the function returns false as there is no place for it on 
		the table. If the value is placed at a location further than the MAX DISTANCE, the function calls the
		hopping function.
*/
bool Add(int k, int *HashTable, bool bitmap[][MAX_DISTANCE])
{
	int i, j; // looping variables

	for(i = Hash(k), j = 0; HashTable[i] > 0  && i < TABLE_SIZE; i++, j++); // determine the location where k will be added

	if(i == TABLE_SIZE)
	{
		cout << "Could not add: " << k << ". Linear probing failed.\n";
		return false; // if there are no locations on the table
	}
	else if(j < MAX_DISTANCE) // if the index is within the allowed MAX DISTANCE
	{
		HashTable[i] = k; // place k on the table
		bitmap[Hash(k)][j] = true; // indicate the location on the bitmap
		cout << k << " inserted at position " << i << endl;
		return true; // addded successfully
	}
	else return Hop(k, i, HashTable, bitmap); // k needs to hop
}
/*
Name:		print
Parameter:	an int* representing the Hashtable and a 2D bool array representing the bitmap
Return:
Description	this function prints the values of the Hashtable and the bitmap
*/
void Print(const int *HashTable, const bool bitmap[][MAX_DISTANCE])
{
	cout << "HOPSCOTCH HASHING TABLE\n+-----------------------+\n";
	cout << "| " << setw(2) << '#' << " | " << setw(9) << "item" <<" | " << setw(4) << "hop" << " |\n";
	cout << "+-----------------------+\n";
	for(int i = 0; i < TABLE_SIZE; i++)
	{
		cout << "| " << setw(2) << i << " | ";
		if(HashTable[i] >= 0 ) cout << setw(9) << HashTable[i] << " | "; // print only valid values
		else cout << setw(9) << "" << " | "; // print a blank
		for(int j = 0; j < MAX_DISTANCE; j++) // print the bitmap
		{
			cout << bitmap[i][j];
		}
		cout << " | " << endl;
	}
	cout << "+-----------------------+\n";

	return;
}
/*
Name:		Hop
Parameter:	an int representing the value that needs to hop, an int representing the current index of num, an int
		pointer representing the Hashtable, and a 2D bool array representing the bitmap
Return:		a boolean representing if the value was successfully added or not
Description	The function is used to hop a value to within MAX DISTANCE of its hash key. If no values can be moved,
		the function returns false. Otherwise, num changes places with a calculated value and the bitmap values
		are arranged to represent that. If num is within MAX DISTANCE after changing places, the function
		return true, other wise it calls hop again with the new index.
*/
bool Hop(int num, int index, int *HashTable, bool bitmap[][MAX_DISTANCE])
{
	int i; // the index of the value that will hop with num
	for(i = index - MAX_DISTANCE + 1; (Hash(HashTable[i]) + MAX_DISTANCE) <= index && i < index ; i++); // determine the index of the value that can hop with num

	if(i == index)
	{
		cout << "Could not add: " << num << ". Hopscotch Hashing failed.\n";
		HashTable[i] = -1;
		return false; // can't move any values in the hash table
	}
	else // else we moved something so check if we need to move again
	{
		bitmap[Hash(HashTable[i])][i - Hash(HashTable[i])] =  false; // reset the bitmap position before jumping
		bitmap[Hash(HashTable[i])][index - i] =  true; // indicate the new bitmap location

		// change places
		HashTable[index] = HashTable[i];
		HashTable[i] = num;
		
		if(i < Hash(num) + MAX_DISTANCE) // if num is done hopping 
		{
			cout << num << " inserted at position " << i << endl;
			bitmap[Hash(num)][i - Hash(num)] = true; // indicate so in the bitmap
			return true; 
		}
		else return Hop(num, i, HashTable, bitmap); // else need to hop again
	}
}	
/*
Name:		Search
Parameter:	an int representing the value to be searched for and an int* representing the Hashtable
Return:		an int representing the index of the search value or a -1 representing an unsuccessful search
Description	this function loops through the Hashtable and searches for search value but only within MAX DISTANCE
		of search value's hash key
*/
int Search(const int search_value, const int *HashTable)
{
	int i; // the index of the search value

	for(i = Hash(search_value); HashTable[i] != search_value && i < Hash(search_value) + MAX_DISTANCE; i++); // search for the value

	if(i == Hash(search_value) + MAX_DISTANCE) return -1; // could not find the number
	else return i; // return the index
}
/*
Name:		Delete
Parameter:	an int representing the value to be deleted, an int* representing the Hashtable, and a bool 2D array
		representing the bitmap
Return:		
Description	The function determines the location of del_value and removes it from the Hashtable and changes the
		location of del value in the bitmap
*/
void Delete(int del_value, int *HashTable, bool bitmap[][MAX_DISTANCE])
{
	int i = Search(del_value, HashTable); // determine the index of the deleted value
	
	if(i < 0) cout << "Failed to delete " << del_value << endl; // the value does not exist in the table
	else
	{
		HashTable[i] = -1; // remove del value from the Hashtable
		bitmap[Hash(del_value)][i-Hash(del_value)] = false; // change the bit map
		cout << del_value << " deleted from position " << i << endl; // show a succesful search
	}

	return;	
}
