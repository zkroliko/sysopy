#ifndef HEADERS
#define HEADERS
#include "headers.h"
#endif

int counter = 0;

int PID;

int received_conf = 0;

void r_conf () {
	printf("Dostalem potwierdzenie, wysylam nastepny!\n");
	received_conf = 1;
}

void r_usr1 () {
	printf("Parent tutaj! Odebralem, wysylam potwierdzenie!\n");
	counter++;
	if (kill(PID, SIGUSR1) < 0) {
		fprintf(stderr, "Blad przy wyslaniu syngalu SIGUSR1!\n");
	}
}

void r_usr2 () {
	printf("Tu parent! Koncowa wartosc licznika to: %d\n", counter);
	exit(0);
}

int main (int argc, char* argv[]) {

	// Najpierw zablokujmy usr1 i usr2
	sigset_t set;
	sigemptyset (&set);
	sigaddset(&set, SIGUSR1);
	sigaddset(&set, SIGUSR2);
	sigprocmask(SIG_BLOCK, &set, NULL);	

	// Ustawienie obslugi sygnalow
	if (signal(SIGUSR1, r_usr1) == SIG_ERR) {
		fprintf(stderr, "Blad przy ustawianiu procedury obslugi sygnalu!\n");
		exit(-1);			
	}
	if (signal(SIGUSR2, r_usr2) == SIG_ERR) {
		fprintf(stderr, "Blad przy ustawianiu procedury obslugi sygnalu!\n");	
		exit(-1);		
	}

	// Sprawdzamy argumenty
	if (argc != 2) {
		fprintf(stderr, "Podano bledna liczbe arguentow! Prosze podac <liczba sygnalow>!\n");	
		return (-1);
	}
	if (atoi(argv[1]) <= 0 ) {
		fprintf(stderr, "Podano bledny argument. Prosze podac liczbe naturalna!\n");
		return (-1);		
	}
	// Wszystko w porzadku
	// Bedziemy forkowac

	if((PID = fork()) < 0) {		
		fprintf(stderr, "Blad funkcji fork!\n");	
	} else if (PID == 0) {
		// Potomek
		execl("./child", "child", NULL);
	}

	// Rodzic teraz bedzie wysylal sygnaly
	// Ustawienie obslugi sygnalow na confirmation
	if (signal(SIGUSR1, r_conf) == SIG_ERR) {
		fprintf(stderr, "Blad przy ustawianiu procedury obslugi sygnalu!\n");
		exit(-1);			
	}

	// Odblokujmy usr1 i usr2
	sigemptyset (&set);

	for (int i = 0; i < atoi(argv[1]); ++i) {
		if (kill(PID, SIGUSR1) < 0) {
			fprintf(stderr, "Blad przy wyslaniu syngalu SIGUSR1!\n");
		}
		
		received_conf = 0;

		// Czekanie na potwierdzenie
		while (!received_conf) {
			sigsuspend(&set);
		}		
	}

	// Teraz ostatni
	if (kill(PID, SIGUSR2) < 0) {
		fprintf(stderr, "Blad przy wyslaniu syngalu SIGUSR2!\n");

	}
	// Teraz bedziemy odbierac

	// Ustawienie obslugi sygnalow na zliczanie
	if (signal(SIGUSR1, r_usr1) == SIG_ERR) {
		fprintf(stderr, "Blad przy ustawianiu procedury obslugi sygnalu!\n");
		exit(-1);			
	}

	// Czekania na sygnaly
	while (1) {
		 sigsuspend(&set);
	}
}