#ifndef HEADERS
#define HEADERS
#include "headers.h"
#endif

#define MAXLENGHT 300

// Zmienne globalne
char * path;
int fd;

void handler () {
	close(fd);
	exit(0);
}

int main (int argc, char* argv[]) {
	
	// Sprawdzamy argumenty

	if (argc != 2) {
		fprintf(stderr, "Nie podano argumentu. Prosze podac nazwe kolejki FIFO!\n");
		return -1;	
	}

	// Ustawiam zmienna globalna
	path = argv[1];

	// Ustawiamy obsluge syngalow

	if (signal(SIGINT, handler) == SIG_ERR) {
		fprintf(stderr, "Blad przy ustawianiu procedury obslugi sygnalu!\n");
		exit(-1);			
	}

	// Alokujemy pamiec

        char *line;

	if ((line = malloc(sizeof(char)*300)) == NULL) {
		fprintf(stderr, "Blad przy alokacji pamieci!\n");
		return -1;
	}

	// Otwieramy potok
	
    	if ((fd = open( path , O_WRONLY)) == -1) {
		fprintf(stderr, "Blad przy otwieraniu kolejki fifo!\n");
		return -1;
	}

	printf("Strumien otwarty. Prosze podawac do wpisania. Aby zakonczyc prosze wcisnac kombinacje CTRL-C\n");


        size_t len = 0;
        ssize_t read;

	char * buffer;	
	if ((buffer = malloc(sizeof(char)*60)) == NULL) {
		fprintf(stderr, "Blad przy alokacji pamieci!\n");
		return -1;
	}

	int pid;

        while (1) {
		// Znak zachety
		fprintf(stderr, "> ");

		// Czyscimy zmienna line
		line[0] = '\0';	

		// PID		
		pid = getpid();
		sprintf(buffer, "%d", pid);
		strcat(line, buffer);
		line[strlen(line)]='\0'; // Obcinamy \n	
		strcat(line," - ");

		// Godzina wyslania
		time_t current_time;
		current_time = time(NULL);	
	   	buffer = ctime(&current_time);
		strcat(line, buffer);
		line[strlen(line)-1]='\0'; // Obcinamy \n
		strcat(line," - ");


		if (((read = getline(&(buffer), &len, stdin)) == -1)) {			
			fprintf(stderr, "Blad przy pobieraniu komunikatu!\n");
		}
		strcat(line, buffer);

		// Wysylanie pelnej wiadomosci
	    	write(fd, line, MAXLENGHT); 

		fprintf(stderr, "Wysylany komunikat: %s", line);
        }

	return 0;
}
