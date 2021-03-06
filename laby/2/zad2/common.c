// W tym pliku zawarte sa te z funkcje, ktore sa wspoledzione przez obydwa warianty

#ifndef COMMON 
	#define COMMON
	#include "common.h"
#endif

// Parsuje nasz string, ktory okresla poszukiwane prawa do dostepu
int parse_access(char* accessMode) {
	// Sprawdzamy czy nasz argument ma poprawna dlugosc
	if(strlen(accessMode) != 9) {
		return -1;
	}
	// Sprawdzamy wszystkie pozycje lanchuca dostepu
	if (!(accessMode[0] == '-' || accessMode[0] == 'r')) return -1;
	if (!(accessMode[1] == '-' || accessMode[1] == 'w')) return -1;
	if (!(accessMode[2] == '-' || accessMode[2] == 'x')) return -1;
	if (!(accessMode[3] == '-' || accessMode[3] == 'r')) return -1;
	if (!(accessMode[4] == '-' || accessMode[4] == 'w')) return -1;
	if (!(accessMode[5] == '-' || accessMode[5] == 'x')) return -1;
	if (!(accessMode[6] == '-' || accessMode[6] == 'r')) return -1;
	if (!(accessMode[7] == '-' || accessMode[7] == 'w')) return -1;
	if (!(accessMode[8] == '-' || accessMode[8] == 'x')) return -1;
	// Prawa dostepu sa poprawne
	return 0;
}

// Sprawdza czy dany plik pasuje do naszych uprawnien
int check(char* ourAccessMode, struct stat st) {
	mode_t thisFileMode = 0;
	if(ourAccessMode[0] == 'r') thisFileMode+= S_IRUSR;
	if(ourAccessMode[1] == 'w') thisFileMode+= S_IWUSR;
	if(ourAccessMode[2] == 'x') thisFileMode+= S_IXUSR;
	if(ourAccessMode[3] == 'r') thisFileMode+= S_IRGRP;
	if(ourAccessMode[4] == 'w') thisFileMode+= S_IWGRP;
	if(ourAccessMode[5] == 'x') thisFileMode+= S_IXGRP;
	if(ourAccessMode[6] == 'r') thisFileMode+= S_IROTH;
	if(ourAccessMode[7] == 'w') thisFileMode+= S_IWOTH;
	if(ourAccessMode[8] == 'x') thisFileMode+= S_IXOTH;
 
	// St_mode jest zapisana w systemie osemkowym 
	// Na poszczegolnych miejscach oktalnych znajduja sie uprawnienia
	// Chcemy sprawdzic tylko ostatnie 3 pozycje
	if((st.st_mode % 512) == thisFileMode) {
		 return 1;
	} else {
		return 0;
	}	
}
