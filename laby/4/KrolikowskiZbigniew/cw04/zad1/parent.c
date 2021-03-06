#ifndef HEADERS
#define HEADERS
#include "headers.h"
#endif

int counter = 0;

int argument;

void r_usr1 () {
	counter++;
}

void r_usr2 () {
	printf("Koncowa wartosc licznika to: %d\n", counter);
	printf("Oczekiwana wartosc licznka to: %d\n", argument);
	exit(0);
}

int main (int argc, char* argv[]) {

	// Sprawdzamy argumenty
	if (argc != 2) {
		fprintf(stderr, "Podano bledna liczbe arguentow! Prosze podac <liczba sygnalow>!\n");	
		return (-1);
	}

	// Ustawiamy zmienna argument z linii polecen w postaci inta
	argument = atoi(argv[1]);

	if (argument <= 0 ) {
		fprintf(stderr, "Podano bledny argument. Prosze podac liczbe naturalna!\n");
		return (-1);		
	}

	// Najpierw zablokujmy usr1 i usr2
	sigset_t set;
	sigemptyset (&set);
	sigaddset(&set, SIGUSR1);
	sigaddset(&set, SIGUSR2);
	sigprocmask(SIG_BLOCK, &set, NULL);	

	// Ustawienie obslugi sygnalow

	struct sigaction usr1Action, usr2Action;
	usr1Action.sa_handler = r_usr1;
	usr1Action.sa_mask = set;
	usr2Action.sa_handler = r_usr2;
	usr2Action.sa_mask = set;
	
	if (sigaction(SIGUSR1, &usr1Action, NULL) < 0) {
		fprintf(stderr, "Blad przy ustawianiu procedury obslugi sygnalu!\n");
		exit(-1);			
	}
	if (sigaction(SIGUSR2, &usr2Action, NULL) < 0 ) {
		fprintf(stderr, "Blad przy ustawianiu procedury obslugi sygnalu!\n");	
		exit(-1);		
	}

	// Bedziemy forkowac
	
	int PID;
	if((PID = fork()) < 0) {		
		fprintf(stderr, "Blad funkcji fork!\n");	
		exit(-1);
	} else if (PID == 0) {
		// Potomek
		execl("./child", "child", NULL);
	}
	// Rodzic teraz bedzie wysylal sygnaly
	for (int i = 0; i < atoi(argv[1]); ++i) {
		if (kill(PID, SIGUSR1) < 0) {
			fprintf(stderr, "Blad przy wyslaniu syngalu SIGUSR1!\n");
		}

	}

	// Odblokujmy wszystkie sygnaly
	sigemptyset (&set);

	// Teraz usr 2	
	if (kill(PID, SIGUSR2) < 0) {		
		fprintf(stderr, "Blad przy wyslaniu syngalu SIGUSR2!\n");

	}
	// Teraz bedziemy odbierac

	// Czekania na sygnaly
	while (1) {
		 sigsuspend(&set);
	}
}
