#include <iostream>
#include <time.h>

#define LICZBA_KOLOROW 4
#define LICZBA_WARTOSCI 13
#define LICZBA_TESTOW 10

const char* colors[]{ "SPADES", "HEARTS", "DIAMOND", "CLUBS" };
const char* values[]{ "A", "K", "Q", "J", "10", "9", "8", "7", "6", "5", "4", "3", "2" };

typedef struct {
	const char* color;
	const char* value;
	int wart;
	int punkty1;
	int punkty2;


} karta_t;

void przypisz(karta_t talia[]) {
	static int nr_karty;
	for (int i = 0; i < LICZBA_KOLOROW; i++) {
		for (int j = 0; j < LICZBA_WARTOSCI; j++) {
			if (j >= 0 && j <= 3) {
				talia[nr_karty] = { colors[i], values[j], LICZBA_WARTOSCI - j, 15 - j, 4 - j };
			}
			else {
				talia[nr_karty] = { colors[i], values[j], LICZBA_WARTOSCI - j, 15 - j, 0 };
			}
			nr_karty++;
		}

	}
}

typedef struct element_kolejki {
	karta_t obecna;
	struct element_kolejki* next;
} element_t;

typedef struct {
	element_t* head;
	element_t* tail;

} kolejka_gracz;

void dodaj_karte(karta_t* karta, kolejka_gracz* kolejka) {

	element_t* dodana = (element_t*)malloc(sizeof(element_t));

	if (dodana != NULL)
	{
		dodana->obecna = *karta;
		dodana->next = NULL;
	}


	if (kolejka->head == NULL) {
		kolejka->head = dodana;
		kolejka->tail = dodana;
	}
	else {
		kolejka->tail->next = dodana;
		kolejka->tail = dodana;
	}

}

void randomowe(int tablica[], int nr_losu = 0) {
	int aktualna = rand() % (LICZBA_KOLOROW * LICZBA_WARTOSCI);


	for (int i = 0; i < nr_losu; i++) {
		if (tablica[i] == aktualna) {
			return randomowe(tablica, nr_losu);
		}
	}

	tablica[nr_losu] = aktualna;

	if (nr_losu == (LICZBA_KOLOROW * LICZBA_WARTOSCI) - 1) return;
	return randomowe(tablica, nr_losu + 1);
}

void przydziel_karty(karta_t* talia, int kolejnosc[], kolejka_gracz* g1, kolejka_gracz* g2) {
	int g1_punkty1 = 0, g2_punkty1 = 0, g1_punkty2 = 0, g2_punkty2 = 0;

	for (int i = 0; i < LICZBA_KOLOROW * LICZBA_WARTOSCI; i++) {
		if (i % 2 == 0) {
			dodaj_karte(&talia[kolejnosc[i]], g1);
			g1_punkty1 += talia[kolejnosc[i]].punkty1;
			g1_punkty2 += talia[kolejnosc[i]].punkty2;
		}
		else {
			dodaj_karte(&talia[kolejnosc[i]], g2);
			g2_punkty1 += talia[kolejnosc[i]].punkty1;
			g2_punkty2 += talia[kolejnosc[i]].punkty2;
		}
	}
	//std::cout << g1_punkty1 << "\t" << g2_punkty1 << "\t\t" << g1_punkty2 << "\t" << g2_punkty2 << "\n";
}

void przesun_do_nastepnego(kolejka_gracz* gracz) {
	element_kolejki* nowa = gracz->head->next;
	free(gracz->head);
	gracz->head = nowa;
}

void zwolnij_kolejke(kolejka_gracz* gracz) {
	while (gracz->head != NULL) {
		przesun_do_nastepnego(gracz);
	}
	gracz->tail = NULL;
}

void rzuc_karte(kolejka_gracz* gracz, kolejka_gracz* stol) {

	std::cout << gracz->head->obecna.color << "  " << gracz->head->obecna.value;
	karta_t rzucana = gracz->head->obecna;
	dodaj_karte(&rzucana, stol);

	przesun_do_nastepnego(gracz);
}

void ruch(kolejka_gracz* g1, kolejka_gracz* g2, kolejka_gracz* stol_1, kolejka_gracz* stol_2) {
	rzuc_karte(g1, stol_1);
	std::cout << "\t";
	rzuc_karte(g2, stol_2);
	std::cout << "\n";
}

void rzuc_zakryta_karte(kolejka_gracz* gracz, kolejka_gracz* stol) {
	std::cout << "(zakryta)";
	karta_t rzucana = gracz->head->obecna;
	dodaj_karte(&rzucana, stol);

	przesun_do_nastepnego(gracz);
}

void ruch_zakryte(kolejka_gracz* g1, kolejka_gracz* g2, kolejka_gracz* stol_1, kolejka_gracz* stol_2) {
	rzuc_zakryta_karte(g1, stol_1);
	std::cout << "\t";
	rzuc_zakryta_karte(g2, stol_2);
	std::cout << "\n";
}

void pozycz(kolejka_gracz* gracz, kolejka_gracz* rywal) {
	karta_t rzucana = gracz->head->obecna;
	dodaj_karte(&rzucana, rywal);

	przesun_do_nastepnego(gracz);
}

void usun_ze_stolu(kolejka_gracz* stol_1, kolejka_gracz* stol_2) {
	zwolnij_kolejke(stol_1);
	zwolnij_kolejke(stol_2);
}

int wygrany(kolejka_gracz* g1, kolejka_gracz* g2) {
	if (g1->head == NULL && g2->head == NULL) {
		return 0;
	}
	else if (g1->head == NULL)
		return 2;
	else if (g2->head == NULL)
		return 1;
	else return 3;

}

int wypisz_zwyciezce(kolejka_gracz* g1, kolejka_gracz* g2) {
	int wynik;
	wynik = wygrany(g1, g2);
	if (wynik == 1 || wynik == 2) {
		std::cout << "wygral gracz " << wynik << "\t";
	}
	else if (wynik == 0)
	{
		std::cout << "remis! ";
	}
	else
	{
		std::cout << "gra zapetlila sie ";
	}

	return wynik;
}

void zbierz_karty_ze_stolu(kolejka_gracz* gracz, kolejka_gracz* stol) {
	while (stol->head != NULL) {
		karta_t dodawana = stol->head->obecna;
		dodaj_karte(&dodawana, gracz);
		przesun_do_nastepnego(stol);
	}
}

int wojna_A(kolejka_gracz* g1, kolejka_gracz* g2, kolejka_gracz* stol_1, kolejka_gracz* stol_2, int ruchy = 0) {
	std::cout << "\t WOJNA!!! \n";

	if (wygrany(g1, g2) == 1 || wygrany(g1, g2) == 2 || wygrany(g1, g2) == 0) {
		std::cout << "brak kart!\n";
		return ruchy;
	}
	else {
		ruch_zakryte(g1, g2, stol_1, stol_2);
		ruchy++;
	}

	if (wygrany(g1, g2) == 1 || wygrany(g1, g2) == 2 || wygrany(g1, g2) == 0) {
		std::cout << "brak kart!\n";
		if (wygrany(g1, g2) == 1)
		{
			zbierz_karty_ze_stolu(g1, stol_1);
			zbierz_karty_ze_stolu(g1, stol_2);
		}
		else if (wygrany(g1, g2) == 2)
		{
			zbierz_karty_ze_stolu(g2, stol_2);
			zbierz_karty_ze_stolu(g2, stol_1);
		}
		return ruchy;
	}
	else {
		ruch(g1, g2, stol_1, stol_2);
		ruchy++;
	}

	if ((stol_1->tail->obecna.wart) == (stol_2->tail->obecna.wart))
		return wojna_A(g1, g2, stol_1, stol_2, ruchy);
	else return ruchy;
}

int wojna_B(kolejka_gracz* g1, kolejka_gracz* g2, kolejka_gracz* stol_1, kolejka_gracz* stol_2, int ruchy = 0, bool wspomagana = false) {
	std::cout << "\t WOJNA!!! \n";

	if (g1->head == NULL) {
		if (g2->head->next->next == NULL) {
			std::cout << "za malo kart pozostalo, by mozliwe bylo przeprowadzenie wojny \n";
			return ruchy;
		}
		else {
			wspomagana = true;
			pozycz(g2, g1);
			pozycz(g2, g1);
			ruch_zakryte(g1, g2, stol_1, stol_2);
			ruchy++;
			ruch(g1, g2, stol_1, stol_2);
			ruchy++;
		}
	}
	else if (g2->head == NULL) {
		if (g1->head->next->next == NULL) {
			std::cout << "za malo kart pozostalo, by mozliwe bylo przeprowadzenie wojny \n";
			return ruchy;
		}
		else {
			wspomagana = true;
			pozycz(g1, g2);
			pozycz(g1, g2);
			ruch_zakryte(g1, g2, stol_1, stol_2);
			ruchy++;
			ruch(g1, g2, stol_1, stol_2);
			ruchy++;
		}
	}
	else if (g1->head->next == NULL) {
		wspomagana = true;
		ruch_zakryte(g1, g2, stol_1, stol_2);
		ruchy++;
		if (wygrany(g1, g2) == 0) {
			return ruchy;
		}
		else {
			pozycz(g2, g1);
			ruch(g1, g2, stol_1, stol_2);
			ruchy++;
		}

	}
	else if (g2->head->next == NULL) {
		wspomagana = true;
		ruch_zakryte(g1, g2, stol_1, stol_2);
		ruchy++;
		if (wygrany(g1, g2) == 0) {
			return ruchy;
		}
		else {
			pozycz(g1, g2);
			ruch(g1, g2, stol_1, stol_2);
			ruchy++;
		}
	}
	else {
		ruch_zakryte(g1, g2, stol_1, stol_2);
		ruchy++;
		wygrany(g1, g2);
		ruch(g1, g2, stol_1, stol_2);
		ruchy++;
	}
	if ((stol_1->tail->obecna.wart) == (stol_2->tail->obecna.wart) && wspomagana == false)
		return wojna_B(g1, g2, stol_1, stol_2, ruchy, wspomagana);
	else if ((stol_1->tail->obecna.wart) == (stol_2->tail->obecna.wart) && wspomagana == true) {
		std::cout << "brak kart !";
		wygrany(g1, g2);
		return ruchy;
	}
	else return ruchy;
}



int rozgrywka_A(kolejka_gracz gracz1, kolejka_gracz gracz2, kolejka_gracz stol_gracz1, kolejka_gracz stol_gracz2, karta_t* card) {
	int kolejnosc_kart[LICZBA_KOLOROW * LICZBA_WARTOSCI];
	randomowe(kolejnosc_kart);
	przydziel_karty(card, kolejnosc_kart, &gracz1, &gracz2);

	int liczba_ruchow = 0;

	while (gracz1.head != NULL && gracz2.head != NULL && liczba_ruchow < 10000) {
		ruch(&gracz1, &gracz2, &stol_gracz1, &stol_gracz2);
		liczba_ruchow++;

		if ((stol_gracz1.tail->obecna.wart) == (stol_gracz2.tail->obecna.wart)) {
			liczba_ruchow += wojna_A(&gracz1, &gracz2, &stol_gracz1, &stol_gracz2);
		}

		if ((stol_gracz1.tail->obecna.wart) > (stol_gracz2.tail->obecna.wart)) {
			zbierz_karty_ze_stolu(&gracz1, &stol_gracz1);
			zbierz_karty_ze_stolu(&gracz1, &stol_gracz2);
		}
		else if ((stol_gracz2.tail->obecna.wart) > (stol_gracz1.tail->obecna.wart)) {
			zbierz_karty_ze_stolu(&gracz2, &stol_gracz2);
			zbierz_karty_ze_stolu(&gracz2, &stol_gracz1);
		}
		std::cout << "_______________________________\n";
		usun_ze_stolu(&stol_gracz1, &stol_gracz2);
	}

	int winner = wypisz_zwyciezce(&gracz1, &gracz2);
	zwolnij_kolejke(&gracz1);
	zwolnij_kolejke(&gracz2);

	std::cout << "liczba ruchow: " << liczba_ruchow << "\n";

	return liczba_ruchow;
}

void gra_A() {
	karta_t card[LICZBA_KOLOROW * LICZBA_WARTOSCI];

	przypisz(card);

	kolejka_gracz gracz1 = { NULL, NULL };
	kolejka_gracz gracz2 = { NULL, NULL };
	kolejka_gracz stol_gracz1 = { NULL, NULL };
	kolejka_gracz stol_gracz2 = { NULL, NULL };


	rozgrywka_A(gracz1, gracz2, stol_gracz1, stol_gracz2, card);

}

void zapetlona_gra_A() {
	karta_t card[LICZBA_KOLOROW * LICZBA_WARTOSCI];

	przypisz(card);

	kolejka_gracz gracz1 = { NULL, NULL };
	kolejka_gracz gracz2 = { NULL, NULL };
	kolejka_gracz stol_gracz1 = { NULL, NULL };
	kolejka_gracz stol_gracz2 = { NULL, NULL };

	int a = 0, b = 0, remis = 0, zapetlenie = 0;
	int y = 0;
	std::cout << "start \n";
	for (int i = 0; i < LICZBA_TESTOW; i++)
	{
		/*int x = rozgrywka_A(gracz1, gracz2, stol_gracz1, stol_gracz2, card);
		if (x == 1) a++;
		else if (x == 2) b++;
		else if (x == 0) remis++;
		else zapetlenie++;*/
		y += rozgrywka_A(gracz1, gracz2, stol_gracz1, stol_gracz2, card);
	}
	//std::cout << "G1 : " << a << " remis: " << remis << " G2 : " << b << " zapetlenia: " << zapetlenie << "\n";
	std::cout << y;

}

int rozgrywka_B(kolejka_gracz gracz1, kolejka_gracz gracz2, kolejka_gracz stol_gracz1, kolejka_gracz stol_gracz2, karta_t* card) {
	int kolejnosc_kart[LICZBA_KOLOROW * LICZBA_WARTOSCI];
	randomowe(kolejnosc_kart);
	przydziel_karty(card, kolejnosc_kart, &gracz1, &gracz2);

	int liczba_ruchow = 0;

	while (gracz1.head != NULL && gracz2.head != NULL && liczba_ruchow < 10000) {
		ruch(&gracz1, &gracz2, &stol_gracz1, &stol_gracz2);
		liczba_ruchow++;

		if ((stol_gracz1.tail->obecna.wart) == (stol_gracz2.tail->obecna.wart)) {
			liczba_ruchow += wojna_B(&gracz1, &gracz2, &stol_gracz1, &stol_gracz2);
		}
		if ((stol_gracz1.tail->obecna.wart) > (stol_gracz2.tail->obecna.wart)) {
			zbierz_karty_ze_stolu(&gracz1, &stol_gracz1);
			zbierz_karty_ze_stolu(&gracz1, &stol_gracz2);
		}
		else if ((stol_gracz2.tail->obecna.wart) > (stol_gracz1.tail->obecna.wart)) {
			zbierz_karty_ze_stolu(&gracz2, &stol_gracz2);
			zbierz_karty_ze_stolu(&gracz2, &stol_gracz1);
		}
		std::cout << "_______________________________\n";
		usun_ze_stolu(&stol_gracz1, &stol_gracz2);
	}


	int winner = wypisz_zwyciezce(&gracz1, &gracz2);
	zwolnij_kolejke(&gracz1);
	zwolnij_kolejke(&gracz2);

	std::cout << "liczba ruchow: " << liczba_ruchow << "\n";

	return winner;
}

void gra_B() {
	karta_t card[LICZBA_KOLOROW * LICZBA_WARTOSCI];

	przypisz(card);

	kolejka_gracz gracz1 = { NULL, NULL };
	kolejka_gracz gracz2 = { NULL, NULL };
	kolejka_gracz stol_gracz1 = { NULL, NULL };
	kolejka_gracz stol_gracz2 = { NULL, NULL };


	rozgrywka_B(gracz1, gracz2, stol_gracz1, stol_gracz2, card);
}

void zapetlona_gra_B() {
	karta_t card[LICZBA_KOLOROW * LICZBA_WARTOSCI];

	przypisz(card);

	kolejka_gracz gracz1 = { NULL, NULL };
	kolejka_gracz gracz2 = { NULL, NULL };
	kolejka_gracz stol_gracz1 = { NULL, NULL };
	kolejka_gracz stol_gracz2 = { NULL, NULL };

	int a = 0, b = 0, remis = 0, zapetlenie = 0;

	std::cout << "start \n";
	for (int i = 0; i < LICZBA_TESTOW; i++)
	{
		int x = rozgrywka_B(gracz1, gracz2, stol_gracz1, stol_gracz2, card);
		if (x == 1) a++;
		else if (x == 2) b++;
		else if (x == 0) remis++;
		else zapetlenie++;
	}
	std::cout << "G1 : " << a << " remis: " << remis << " G2 : " << b << " zapetlenia: " << zapetlenie;
}

//dodatkowe opcje do madrej wojny

void rzuc_pierwsza(kolejka_gracz* gracz, kolejka_gracz* stol) {

	std::cout << gracz->head->obecna.color << "  " << gracz->head->obecna.value;
	karta_t rzucana = gracz->head->obecna;
	dodaj_karte(&rzucana, stol);

	gracz->head = gracz->tail;
}

void rzuc_druga(kolejka_gracz* gracz, kolejka_gracz* stol) {

	std::cout << gracz->tail->obecna.color << "  " << gracz->tail->obecna.value;
	karta_t rzucana = gracz->tail->obecna;
	dodaj_karte(&rzucana, stol);

	gracz->tail = gracz->head;
}

int wygrany_w_madrej(kolejka_gracz* g1, kolejka_gracz* g2, kolejka_gracz* reka_g1, kolejka_gracz* reka_g2) {
	if (g1->head == NULL && g2->head == NULL && reka_g1->head == NULL && reka_g2->head == NULL) {
		return 0;
	}
	else if (g1->head == NULL && reka_g1->head == NULL)
		return 2;
	else if (g2->head == NULL && reka_g2->head == NULL)
		return 1;
	else return 3;
}

void wojna_w_madrej_wojnie(kolejka_gracz* g1, kolejka_gracz* g2, kolejka_gracz* stol_1, kolejka_gracz* stol_2, kolejka_gracz* reka1, kolejka_gracz* reka2, int ruchy = 0) {

	std::cout << "\t WOJNA!!! \n";

	if (wygrany_w_madrej(g1, g2, reka1, reka2) == 1 || wygrany_w_madrej(g1, g2, reka1, reka2) == 2 || wygrany_w_madrej(g1, g2, reka1, reka2) == 0) {
		std::cout << "brak kart!\n";
		return;
	}
	else
	{
		rzuc_zakryta_karte(g2, stol_2);
		std::cout << "\n";
		rzuc_zakryta_karte(reka1, stol_1);
		zwolnij_kolejke(reka1);
		std::cout << "\n";
	}
	if (wygrany_w_madrej(g1, g2, reka1, reka2) == 1 || wygrany_w_madrej(g1, g2, reka1, reka2) == 2 || wygrany_w_madrej(g1, g2, reka1, reka2) == 0) {
		std::cout << "brak kart!\n";
		return;
	}
	else
	{
		rzuc_karte(g2, stol_2);
		std::cout << "\n";
		rzuc_karte(g1, stol_1);
		std::cout << "\n";

	}

	if ((stol_1->tail->obecna.wart) == (stol_2->tail->obecna.wart))
	{
		pozycz(g1, reka1);
		return wojna_w_madrej_wojnie(g1, g2, stol_1, stol_2, reka1, reka2, ruchy);
	}
	else
	{
		if (g1->head->next == NULL)
		{
			pozycz(g1, reka1);
		}
		else
		{
			pozycz(g1, reka1);
			pozycz(g1, reka1);
		}
	}
}

void wybor_karty(kolejka_gracz* gracz, kolejka_gracz* reka, kolejka_gracz* stol, int kto)
{
	char choose;

	if (gracz->head->next == NULL)
	{
		pozycz(gracz, reka);
	}
	else
	{
		pozycz(gracz, reka);
		pozycz(gracz, reka);
	}
	std::cout << "\t Gracz " << kto << " wybiera: \t";

	std::cout << "A:" << reka->head->obecna.color << " " << reka->head->obecna.value << "\t";
	std::cout << "B: " << reka->tail->obecna.color << " " << reka->tail->obecna.value << "\t";
	std::cin >> choose;
	if (kto == 2)
		std::cout << "G2: ";
	else if (kto == 1)
		std::cout << "G1: ";
	switch (choose) {
	case 'A':
		rzuc_pierwsza(reka, stol);
		break;
	case 'B':
		rzuc_druga(reka, stol);
		break;
	default:
		std::cout << "invalid";
	}
	std::cout << "\n";
}

void rezultat_rundy(kolejka_gracz* g1, kolejka_gracz* g2, kolejka_gracz* stol_g1, kolejka_gracz* stol_g2)
{
	if ((stol_g1->tail->obecna.wart) > (stol_g2->tail->obecna.wart)) {
		zbierz_karty_ze_stolu(g1, stol_g1);
		zbierz_karty_ze_stolu(g1, stol_g2);
	}
	else if ((stol_g2->tail->obecna.wart) > (stol_g1->tail->obecna.wart)) {
		zbierz_karty_ze_stolu(g2, stol_g2);
		zbierz_karty_ze_stolu(g2, stol_g1);
	}
	std::cout << "_______________________________\n";
	usun_ze_stolu(stol_g1, stol_g2);

}

int madraWojna(kolejka_gracz gracz1, kolejka_gracz gracz2, kolejka_gracz stol_gracz1, kolejka_gracz stol_gracz2, karta_t* card, kolejka_gracz reka_wybor1, kolejka_gracz reka_wybor2)
{
	int kolejnosc_kart[LICZBA_KOLOROW * LICZBA_WARTOSCI];
	randomowe(kolejnosc_kart);
	przydziel_karty(card, kolejnosc_kart, &gracz1, &gracz2);


	std::cout << "G1: ";
	rzuc_karte(&gracz1, &stol_gracz1);

	while (gracz1.head != NULL && gracz2.head != NULL)
	{


		wybor_karty(&gracz2, &reka_wybor2, &stol_gracz2, 2);

		if ((stol_gracz1.tail->obecna.wart) == (stol_gracz2.tail->obecna.wart)) {
			wojna_w_madrej_wojnie(&gracz2, &gracz1, &stol_gracz2, &stol_gracz1, &reka_wybor2, &reka_wybor1);
		}

		rezultat_rundy(&gracz1, &gracz2, &stol_gracz1, &stol_gracz2);

		if (wygrany_w_madrej(&gracz1, &gracz2, &reka_wybor1, &reka_wybor2) == 1 || wygrany_w_madrej(&gracz1, &gracz2, &reka_wybor1, &reka_wybor2) == 2 || wygrany_w_madrej(&gracz1, &gracz2, &reka_wybor1, &reka_wybor2) == 0)
		{
			break;
		}

		else
		{
			std::cout << "G2: ";
			rzuc_karte(&reka_wybor2, &stol_gracz2);
			zwolnij_kolejke(&reka_wybor2);
			wybor_karty(&gracz1, &reka_wybor1, &stol_gracz1, 1);

			if ((stol_gracz1.tail->obecna.wart) == (stol_gracz2.tail->obecna.wart)) {
				wojna_w_madrej_wojnie(&gracz1, &gracz2, &stol_gracz1, &stol_gracz2, &reka_wybor1, &reka_wybor2);
			}

			rezultat_rundy(&gracz1, &gracz2, &stol_gracz1, &stol_gracz2);


			std::cout << "G1: ";
			rzuc_karte(&reka_wybor1, &stol_gracz1);
			zwolnij_kolejke(&reka_wybor1);
		}
	}

	std::cout << "\n";
	int winner = wypisz_zwyciezce(&gracz1, &gracz2);
	zwolnij_kolejke(&gracz1);
	zwolnij_kolejke(&gracz2);



	return winner;

}

void gra_madra()
{
	karta_t card[LICZBA_KOLOROW * LICZBA_WARTOSCI];

	przypisz(card);

	kolejka_gracz gracz1 = { NULL, NULL };
	kolejka_gracz gracz2 = { NULL, NULL };
	kolejka_gracz stol_gracz1 = { NULL, NULL };
	kolejka_gracz stol_gracz2 = { NULL, NULL };
	kolejka_gracz reka_gracz1 = { NULL, NULL };
	kolejka_gracz reka_gracz2 = { NULL, NULL };


	madraWojna(gracz1, gracz2, stol_gracz1, stol_gracz2, card, reka_gracz1, reka_gracz2);
}

int main() {

	srand((unsigned int)time(NULL));
	char wybor;

	std::cout << "Wybierz wersje gry: \n";
	std::cout << "A - wojna, wersja A\n";
	std::cout << "a - wywolanie wersji A zdefiniowana ilosc razy \n";
	std::cout << "B - wojna, wersja B\n";
	std::cout << "b - wywolanie wersji B zdefiniowana ilosc razy \n";
	std::cout << "X - MADRA WOJNA \n";
	std::cin >> wybor;

	switch (wybor) {
	case 'A':
		gra_A();
		break;

	case 'a':
		zapetlona_gra_A();
		break;

	case 'B':
		gra_B();
		break;

	case 'b':
		zapetlona_gra_B();
		break;

	case 'X':
		gra_madra();
		break;

	default:
		std::cout << "invalid";
	}

	return 0;
}