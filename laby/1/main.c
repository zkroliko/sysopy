#include <stdio.h>
#include "list.h"

int main () {

	printf("Alokujemy pamiec dla 6 generycznych kontaktow oraz listy.\n");
	List* list = List_new();

	Data* data1 = Data_new("Andrzej", "Wolbrom", "1994-05-03", "Warszawa, Konopnicka 3", "4566123-34", "Andrzej@Wolbrom.com");
	Data* data2 = Data_new("Jacek", "Placek", "1983-09-03", "Łodz, Rycerska 5", "9563213-34", "halastra@gumail.com");
	Data* data3 = Data_new("Anna", "Anninska", "1991-01-08", "Szczecin, Morska 6", "4544221-44", "anusia@xumail.com");
	Data* data4 = Data_new("Teodor", "Tadorski", "1989-05-03", "Koszalow", "11329217-34", "lodzie@podwodne.com");
	Data* data5 = Data_new("Konrad", "Bartus", "1964-11-03", "Tczyny, Wiejska 12", "465653-14", "tczyny@wujtix.com");
	Data* data6 = Data_new("Czeslaw", "Czeplinski", "1971-01-23", "Krakow, Czarnowiejska 24", "4612323-555", "czarnowies@krakow.com");

	if (list == NULL || data1 == NULL || data2 == NULL || data3 == NULL || data4 == NULL || data5 == NULL || data6 == NULL) {
		fprintf(stderr, "Bad allocation!\n");
		return 1;
	}
	printf("Alokacja powiodla sie.\n");

	printf("Kazdy kontakt dodajemy do listy 1000 razy.\n");

	for (int i = 0; i < 1000; i++) {
		List_push_back(list, data1);
		List_push_back(list, data2);
		List_push_back(list, data3);
		List_push_back(list, data4);
		List_push_back(list, data5);
		List_push_back(list, data6);
	}

	printf("Sortujemy liste 6000 elementowa sortowaniem przez wstawianie.\n");
	List_sort(list);

	printf("Wykonujemy 1000 wyszukiwan liniowych w liscie.\n");
	for (int i = 0; i < 1000; i++) {
		List_find(list, "Koszalow");
		List_find(list, "czarnowies@krakow.com");
	}

	printf("Dealokujemy pamiec w tym liste. Kazdy node posiada osobna kopie generycznego kontaktu.\n");

	Data_del(data1);
	Data_del(data2);
	Data_del(data3);
	Data_del(data4);
	Data_del(data5);
	Data_del(data6);

	List_del(list);
	return 0;
}
