
#include "generator.h"
#include "functions.h"

#define FILENAMEORG "rekordy/original"

int main() {

	int sizes[4] = {4,512,4096,8192};
	int counts[4] = {4000,4000,4000,4000};
	char* filenamesLib[4] = {"rekordy/file1Lib","rekordy/file2Lib","rekordy/file3Lib","rekordy/file4Lib"};
	char* filenamesSys[4] = {"rekordy/file1Sys","rekordy/file2Sys","rekordy/file3Sys","rekordy/file4Sys"};

	// Struktura dla times
	struct tms startStruct;
	struct tms endStruct; // Musimy zapamietywac roznice

	// Czas dla time
	clock_t timeStart;
	clock_t timeEnd;	


	for (int i = 0; i < 4; ++i) {
		printf( "---Test bedzie przeprowadzony rekordow wielkosci %d, oraz ilosci %d---\n", sizes[i], counts[i]);

		// Generujemy plik testowy
		generate(FILENAMEORG,sizes[i],counts[i]);

		// Robimy kopie plikow

		copy_file(FILENAMEORG, filenamesLib[i]);
		copy_file(FILENAMEORG, filenamesSys[i]);

		// Generuje czasy poczatkowe
		timeStart = clock();
		times(&startStruct);

		// Pierwszy test 
		sortLib(filenamesLib[i], sizes[i]);

		//Wypisuje czasy
		printf( "---Czasy dla fwrite i fread---\n");
		times(&endStruct);
		timeEnd = clock();
		printf( "Czas zegara scienego: %f\n", ((double)(timeEnd - timeStart) / (double)(CLOCKS_PER_SEC)));
		printf("Czas uzytkownika: %f\n", ((double)(endStruct.tms_utime - startStruct.tms_utime) / (double)sysconf(_SC_CLK_TCK)));
		printf("Czas systemu: %f\n", ((double)(endStruct.tms_stime - startStruct.tms_stime) / (double)sysconf(_SC_CLK_TCK)));

		// Generuje czasy poczatkowe
		timeStart = clock();
		times(&startStruct);

		// Drugi test
		sortSys(filenamesSys[i], sizes[i]);

		//Wypisuje czasy
		printf( "---Czasy dla write i read---\n");
		times(&endStruct);
		timeEnd = clock();
		printf( "Czas zegara scienego: %f\n", ((double)(timeEnd - timeStart) / (double)(CLOCKS_PER_SEC)));
		printf("Czas uzytkownika: %f\n", ((double)(endStruct.tms_utime - startStruct.tms_utime) / (double)sysconf(_SC_CLK_TCK)));
		printf("Czas systemu: %f\n", ((double)(endStruct.tms_stime - startStruct.tms_stime) / (double)sysconf(_SC_CLK_TCK)));

	}
}
