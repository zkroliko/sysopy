#include "functions.h"

// Porownujemy dwa rekordy
unsigned char compare(char* first, char* second) {
	if ((unsigned int)first[0] > (unsigned int)second[0]) {
		return 1;	
	}
	return 0;
}

//Wypisuje klucze
void printKeys(char* filename, int size) {

	FILE* file = fopen(filename,"rb");
	if (!file) {
		fprintf(stderr, "Blad przy otwarciu pliku!\n");
		exit(2);
	}

	// Determinujemy ilosc rekordow w pliku
	fseek(file, 0L, SEEK_END);
	int count = ftell(file)/size;
	fseek(file, 0L, SEEK_SET);

	char unsigned* key = malloc(sizeof(char));

	size_t read;

	for (int i = 0; i < count; i++) {
		read = fread(key,sizeof(char),1,file);
		if (read != 1) {
			fprintf(stderr, "Zly odczyt z pliku w wypisywaniu kluczy!\n");
			exit(2);
		}
		printf("%u ",*key);
		fseek(file,size-1,SEEK_CUR); // -1 poniewaz funkcja fread juz nas przesunela
	}
	printf("\n");
	fclose(file);
}

// Do kopiowania pliku
int copy_file(char *old_filename, char  *new_filename)	{
	FILE  *ptr_old, *ptr_new;
	char  character;

	ptr_old = fopen(old_filename, "rb");
	ptr_new = fopen(new_filename, "wb");

	if(!ptr_old) {
		fprintf(stderr, "Zle otwarcie pliku podczas kopiowania!\n");
		return  -1;
	}
	if(!ptr_new) {
		fprintf(stderr, "Zle otwarcie pliku podczas kopiowania!\n");
		fclose(ptr_old);
		return  -1;
	}
	while(1) {
		character  =  fgetc(ptr_old);

		if(!feof(ptr_old))
			fputc(character, ptr_new);
		else
			break;
	}

	fclose(ptr_new);
	fclose(ptr_old);
	return  0;
}

// Test fwirte i fread
void sortLib(char* filename, int size) {

	FILE* file = fopen(filename,"rw+");

	if (!file) {
		fprintf(stderr, "Blad przy otwarciu pliku.\n");
		exit(2);
	}

	// Determinujemy ilosc rekordow w pliku
	fseek(file, 0L, SEEK_END);
	int count = ftell(file)/size;
	fseek(file, 0L, SEEK_SET);

	// Alokujmeny pamiec dla buforow
	char* first = malloc(size*sizeof(char));
	char* second = malloc(size*sizeof(char));

	if (first == NULL || second == NULL) {
		fprintf(stderr, "Zla alokacja w tescie.\n");
		exit(2);
	}

	size_t readFirst;
	size_t readSecond;
	size_t written;

	for (int i = 0; i < count -1; i++) {
		// Przesuwamy sie na odpowiednia pozycje w pliku
		fseek(file,0,SEEK_SET);
		for( int j = 0 ; j < count - i -1; j++) {
			readFirst  = fread(first,sizeof(char),size,file);
			readSecond = fread(second,sizeof(char),size,file);
			// Sprawdzamy poprawnosc odczytu
			if (readFirst != size || readSecond != size) {
				fprintf(stderr, "Zly odczyt z pliku w tescie z stdio.\n");
				exit(2);
			}
			if (compare(first,second) != 0) {
				// Jezeli sa w odwrotnej kolejnosci to zapisujemy je w pliku w prawidlowej
				fseek(file,-2*size,SEEK_CUR);
				written = fwrite(second,sizeof(char),size,file);
				written += fwrite(first,sizeof(char),size,file);
				if (written != size*2) {
					fprintf(stderr, "Blas zapisu stdio w sortowaniu!\n");
					exit(2);
				}
			}
			// Przesuwamy sie o jedna pozycje do tylu w pliku
			fseek(file,-size,SEEK_CUR);

			// DEBUGprintKeys(size,count);
		}
	}	

	fclose(file);
	free(first);
	free(second);
}

void sortSys(char* filename, int size) {

	int file = open(filename,O_RDWR);

	if (file < 0) {
		fprintf(stderr, "Blad przy otwarciu pliku.\n");
		exit(2);
	}

	// Determinujemy ilosc rekordow w pliku
	struct stat st; // To jest z system/stat
	stat(filename, &st);
	int count = st.st_size /size;

	// Alokujmeny pamiec dla buforow
	void* first = malloc(size*sizeof(char));
	void* second = malloc(size*sizeof(char));

	if (first == NULL || second == NULL) {
		fprintf(stderr, "Zla alokacja w tescie.\n");
		exit(2);
	}


	size_t readFirst;
	size_t readSecond;
	size_t written;

	for (int i = 0; i < count -1; i++) {
		// Przesuwamy sie na odpowiednia pozycje w pliku
		lseek(file,0,SEEK_SET);
		
		for( int j = 0 ; j < count - i -1; j++) {
			readFirst  = read(file, first,size);
			readSecond = read(file, second,size);
			
			if (readFirst != size || readSecond != size) {
				fprintf(stderr, "Zly odczyt z pliku w tescie z unistd!\n");
				exit(2);
			}
			
			if (compare(first,second) != 0) {
				// Jezeli sa w odwrotnej kolejnosci to zapisujemy je w pliku w prawidlowej
				lseek(file,-2*size,SEEK_CUR);
				written = write(file,second,size);
				written += write(file,first,size);
				if (written != size*2) {
					fprintf(stderr, "Blas zapisu stdio w sortowaniu!\n");
					exit(2);
				}
			}
			
			// Przesuwamy sie o jedna pozycje do tylu w pliku
			lseek(file,-size,SEEK_CUR);	
		}


	}

	close(file);
	free(first);
	free(second);
}

