#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

using namespace std;

int main (int argc, char* argv[])
{	
	// Sprawdzamy czy endl flushuje
	cout << "baba idzie spac" << endl;
	cout << "chlop juz poszedl";
	sleep(2);
	cout << " ale za to dwie sekundy temu \n";
	return 0;
}
