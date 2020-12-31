#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include "Plansza.h"
#include "Pionek.h"
#include <allegro5/allegro_ttf.h>
#include <Windows.h>

void ruchKomputera(Plansza* plansza, int& pionkiGracza, int& liczbaPionkowCPU, int& ruchyDamekBezZbicia) {
	Pionek* pionkiCPU[12] = { nullptr };
	int czyZbija = -1;
	bool combo = false;
	if (liczbaPionkowCPU == 0)
		return;
	Sleep(2000); // czekanie 2 sekundy, ¿eby wydawa³o siê, ¿e myœli nad ruchem
	// PÊTLA SZUKAJ¥CA NAJWIÊKSZEJ WYSTÊPUJ¥CEJ WARTOŒCI RUCHU
	for (int i = 0; i < 12; i++) {
		pionkiCPU[i] = plansza->pionki[i];
		if (pionkiCPU[i]->czyAktywny()) {
			pionkiCPU[i]->obliczRuchy(plansza->czyMiejscaZajête, plansza->pionki);
			for (int j = 0; j < pionkiCPU[i]->dostepneRuchy; j++) {
				if (pionkiCPU[i]->ruchy[j] == nullptr)
					continue;
				if (pionkiCPU[i]->ruchy[j]->ileZbija > czyZbija) {
					czyZbija = pionkiCPU[i]->ruchy[j]->ileZbija;
				}
			}
		}
	}
	// PÊTLA WYBIERAJ¥CA I WYKONUJ¥CA RUCH
	for (int i = 0; i < 12; i++) {
		if (pionkiCPU[i]->dostepneRuchy > 0) {
			for (int j = 0; j < pionkiCPU[i]->dostepneRuchy; j++) {
				if (pionkiCPU[i]->ruchy[j] != nullptr) {
					// WARUNEK ANTY-SAMOBÓJCZY
					if (pionkiCPU[i]->ruchy[j]->ileZbija == -1 && czyZbija > -1)
						continue;
					// JE¯ELI PUNKTACJA TEGO RUCHU JEST RÓWNA PUNKTACJI NAJWY¯EJ PUNKTOWANEGO MO¯LIWEGO RUCHU
					if (pionkiCPU[i]->ruchy[j]->ileZbija == czyZbija) {
						// JE¯ELI JEST TO RUCH NIEZBIJAJ¥CY
						if (czyZbija <= 0) {
							if (pionkiCPU[i]->czyKrólowa())
								ruchyDamekBezZbicia++;
							else
								ruchyDamekBezZbicia = 0;
							pionkiCPU[i]->ruch(pionkiCPU[i]->ruchy[j], plansza->czyMiejscaZajête);
							for (int i = 0; i < 12; i++) {
								for (int j = 0; j < pionkiCPU[i]->dostepneRuchy; j++) {
									delete[] pionkiCPU[i]->ruchy[j];
									pionkiCPU[i]->ruchy[j]=nullptr;
								}
								pionkiCPU[i]->dostepneRuchy = 0;
							}
							return;
						}
						// JE¯ELI JEST TO RUCH ZBIJAJ¥CY
						else {
							if (pionkiCPU[i]->ruchy[j]->ileZbija > 1) {
								combo = true;
							}
							if (pionkiCPU[i]->czyKrólowa())
								ruchyDamekBezZbicia = 0;
							pionkiCPU[i]->ruchy[j]->zbijany->zostajeZbity(plansza->czyMiejscaZajête);
							pionkiGracza--;
							pionkiCPU[i]->ruch(pionkiCPU[i]->ruchy[j], plansza->czyMiejscaZajête);
							for (int i = 0; i < 12; i++) {
								for (int j = 0; j < pionkiCPU[i]->dostepneRuchy; j++) {
									delete[] pionkiCPU[i]->ruchy[j];
									pionkiCPU[i]->ruchy[j] = nullptr;
								}
								pionkiCPU[i]->dostepneRuchy = 0;
							}
							// JE¯ELI MO¯E ZBIÆ WIELOKROTNIE
							if (combo == true) {
								plansza->drukujPlansza();
								al_flip_display();
								Sleep(200);
								pionkiCPU[i]->obliczRuchy(plansza->czyMiejscaZajête,plansza->pionki);
								for (int k = 0; k < pionkiCPU[i]->dostepneRuchy; k++) {
									if (pionkiCPU[i]->ruchy[k]->ileZbija >= 1) {
										pionkiCPU[i]->ruchy[k]->zbijany->zostajeZbity(plansza->czyMiejscaZajête);
										pionkiGracza--;
										pionkiCPU[i]->ruch(pionkiCPU[i]->ruchy[k], plansza->czyMiejscaZajête);
										combo = false;
										break;
									}
								}
								for (int k = 0; k < pionkiCPU[i]->dostepneRuchy; k++){
									delete[] pionkiCPU[i]->ruchy[k];
									pionkiCPU[i]->ruchy[k] = nullptr;
								}
							}
							return;
						}
					}
				}
			}
		}
	}
	// GDYBY ODMÓWI£ WYKONANIA RUCHU
	for (int i = 0; i < 12; i++) {
		if (pionkiCPU[i]->dostepneRuchy > 0) {
			pionkiCPU[i]->ruch(pionkiCPU[i]->ruchy[0], plansza->czyMiejscaZajête);
		}
	}
}

void reset(Plansza *plansza, int &pionkiGracza, int &pionkiCPU, int &ruchyDamekBezZbicia) {
	for (int i = 0; i < 24; i++)
		delete[] plansza->pionki[i];
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			plansza->czyMiejscaZajête[i][j] = false;
	plansza->initPlansza();
	pionkiGracza = 12;
	pionkiCPU = 12;
	ruchyDamekBezZbicia = 0;
}

void koniecGry(Plansza* plansza, int& pionkiGracza, int& pionkiKomputera,int& ruchyDamekBezZbicia, int& klikX, int& klikY, ALLEGRO_COLOR& kolor, ALLEGRO_EVENT_QUEUE* kolejka, ALLEGRO_EVENT& zdarzenie, ALLEGRO_FONT* czcionka) {
	char* tekst = new char[12];
	if (pionkiGracza == 0) {
		tekst = (char*)"Przegrana...";
		kolor = al_map_rgb(200, 20, 20);
	}
	if (ruchyDamekBezZbicia == 15) {
		tekst = (char*)"Remis";
		kolor = al_map_rgb(200, 200, 20);
	}
	if (pionkiKomputera == 0) {
		tekst = (char*)"Wygrna!";
		kolor = al_map_rgb(20, 200, 20);
	}
	for (;;) {
		al_wait_for_event(kolejka, &zdarzenie);
		plansza->drukujPlansza();
		al_draw_text(czcionka, kolor, 250, 320, 0, tekst);
		al_flip_display();
		if (zdarzenie.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN || zdarzenie.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}
	}
	reset(plansza, pionkiGracza, pionkiKomputera,ruchyDamekBezZbicia);
	klikX = 0;
	klikY = 0;
}

int main() {
	// INICJALIZACJA BIBLIOTEKI GRAFICZNEJ I POTRZEBNYCH ZMIENNYCH
	ALLEGRO_DISPLAY* ekran;
	ALLEGRO_EVENT_QUEUE* kolejka;
	ALLEGRO_FONT* czcionka;
	ALLEGRO_COLOR kolor;
	Pionek* klikniêtyPionek = nullptr, * zbijanyPionek = nullptr;
	Pionek::Ruch* wykonywanyRuch = nullptr;
	ALLEGRO_EVENT zdarzenie;
	bool dzia³a=true,combo=false;
	int klikX, klikY, pionkiGracza = 12, pionkiKomputera = 12, ruchyDamekBezZbicia=0;
	char* tekst = new char[34];
	char* tekst2 = new char[34];
	al_init();
	al_set_app_name("Kaczka");
	al_init_ttf_addon();
	al_init_image_addon();
	al_install_mouse();
	ekran = al_create_display(800, 800);
	kolejka = al_create_event_queue();
	czcionka = al_load_ttf_font("ComicSans.ttf", 64, 0);
	Plansza* plansza = new Plansza();
	al_register_event_source(kolejka, al_get_display_event_source(ekran));
	al_register_event_source(kolejka, al_get_mouse_event_source());
	// G£ÓWNA PÊTLA GRY
	for (;dzia³a==true;) {
		plansza->drukujPlansza();
		al_flip_display();
		al_wait_for_event(kolejka, &zdarzenie);
		if (pionkiGracza == 0 || pionkiKomputera == 0 || ruchyDamekBezZbicia==15) {
			koniecGry(plansza,pionkiGracza,pionkiKomputera,ruchyDamekBezZbicia,klikX,klikY,kolor,kolejka,zdarzenie,czcionka);
			continue;
		}
		// OBS£UGA KLIKNIÊÆ MYSZY
		if (zdarzenie.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			klikX = zdarzenie.mouse.x;
			klikY = zdarzenie.mouse.y;
			if (klikniêtyPionek == nullptr) {
				klikniêtyPionek = plansza->wybórPionka(klikX, klikY);
				if (klikniêtyPionek == nullptr)
					continue;
				if (klikniêtyPionek->czyGracza()) {
					klikniêtyPionek->jesteœKlikniêty();
					klikniêtyPionek->obliczRuchy(plansza->czyMiejscaZajête,plansza->pionki);
				}
				else {
					klikniêtyPionek = nullptr;
				}
			}
		}
		// PÊTLA KIEDY WYBRALIŒMY PIONEK
		while (klikniêtyPionek != nullptr) {
			al_wait_for_event(kolejka, &zdarzenie);
			if (zdarzenie.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
				break;
			klikX = zdarzenie.mouse.x;
			klikY = zdarzenie.mouse.y;
			if (zdarzenie.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && pionkiGracza!=0 && pionkiKomputera!=0) {
				// OD£O¯ENIE PIONKA
				if (klikniêtyPionek->pozycja_X<klikX && klikniêtyPionek->pozycja_X + 99>klikX && klikniêtyPionek->pozycja_Y<klikY && klikniêtyPionek->pozycja_Y + 99>klikY) {
					klikniêtyPionek->nieJesteœKlikniêty();
					klikniêtyPionek = nullptr;
					break;
				}
				// ODNALEZIENIE RUCHU, KTÓRY CHCEMY WYKONAÆ
				for (int i = 0; i < klikniêtyPionek->dostepneRuchy; i++) {
					if (klikniêtyPionek->ruchy[i]->x<klikX && klikniêtyPionek->ruchy[i]->x + 99>klikX && klikniêtyPionek->ruchy[i]->y<klikY && klikniêtyPionek->ruchy[i]->y + 99>klikY) {
						wykonywanyRuch = klikniêtyPionek->ruchy[i];
						break;
					}
				}
				// WYKONANIE RUCHU
				if (wykonywanyRuch != nullptr) {
					if (wykonywanyRuch->ileZbija <= 0) {
						// PRZYPADEK KIEDY RUCH NIE ZBIJA ¯ADNEGO PIONKA
						if (klikniêtyPionek->czyKrólowa())
							ruchyDamekBezZbicia++;
						else
							ruchyDamekBezZbicia = 0;
						klikniêtyPionek->ruch(wykonywanyRuch, plansza->czyMiejscaZajête);
						wykonywanyRuch = nullptr;
						klikniêtyPionek->nieJesteœKlikniêty();
						klikniêtyPionek = nullptr;
						plansza->drukujPlansza();
						al_flip_display();
						al_unregister_event_source(kolejka, al_get_mouse_event_source());
						ruchKomputera(plansza, pionkiGracza, pionkiKomputera,ruchyDamekBezZbicia);
						al_register_event_source(kolejka, al_get_mouse_event_source());
						break;
					}
					else {
						// PRZYPADEK KIEDY RUCH ZBIJA PIONEK
						if (klikniêtyPionek->czyKrólowa())
							ruchyDamekBezZbicia=0;
						if (wykonywanyRuch->ileZbija > 1)
							combo = true;
						wykonywanyRuch->zbijany->zostajeZbity(plansza->czyMiejscaZajête);
						klikniêtyPionek->ruch(wykonywanyRuch, plansza->czyMiejscaZajête);
						wykonywanyRuch = nullptr;
						if (!combo) {
							klikniêtyPionek->nieJesteœKlikniêty();
							klikniêtyPionek = nullptr;
						}
						pionkiKomputera--;
						plansza->drukujPlansza();
						if (combo) {
							tekst = (char*)"Musisz wykonac";
							tekst2 = (char*)"wielokrotne bicie";
							kolor = al_map_rgb(200, 200, 20);
							al_draw_text(czcionka, kolor, 100, 250, 0, tekst);
							al_draw_text(czcionka, kolor, 100, 350, 0, tekst2);
							klikniêtyPionek->obliczRuchy(plansza->czyMiejscaZajête,plansza->pionki);
						}
						al_flip_display();
						// WIELOKROTNE BICIE
						if (combo) {
							combo = false;
							for (;;) {
								al_wait_for_event(kolejka, &zdarzenie);
								klikX = zdarzenie.mouse.x;
								klikY = zdarzenie.mouse.y;
								if (zdarzenie.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
									dzia³a = false;
									break;
								}
								if (zdarzenie.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
									// ODNALEZIENIE RUCHU, KTÓRY CHCEMY WYKONAÆ
									for (int i = 0; i < klikniêtyPionek->dostepneRuchy; i++) {
										if (klikniêtyPionek->ruchy[i]->x<klikX && klikniêtyPionek->ruchy[i]->x + 99>klikX && klikniêtyPionek->ruchy[i]->y<klikY && klikniêtyPionek->ruchy[i]->y + 99>klikY) {
											wykonywanyRuch = klikniêtyPionek->ruchy[i];
											break;
										}
									}
									if (wykonywanyRuch == nullptr) {
										continue;
									}
									if (wykonywanyRuch->ileZbija >= 1) {
										if (wykonywanyRuch->ileZbija > 1)
											combo = true;
										wykonywanyRuch->zbijany->zostajeZbity(plansza->czyMiejscaZajête);
										klikniêtyPionek->ruch(wykonywanyRuch, plansza->czyMiejscaZajête);
										wykonywanyRuch = nullptr;
										pionkiKomputera--;
										plansza->drukujPlansza();
										if (combo) {
											klikniêtyPionek->obliczRuchy(plansza->czyMiejscaZajête, plansza->pionki);
											if (klikniêtyPionek->ruchy[0] == nullptr) {
												combo = false;
												break;
											}
										}
									}
									if (!combo) {
										klikniêtyPionek->nieJesteœKlikniêty();
										klikniêtyPionek = nullptr;
										break;
									}
								}
								plansza->drukujPlansza();
								al_draw_text(czcionka, kolor, 100, 250, 0, tekst);
								al_draw_text(czcionka, kolor, 100, 350, 0, tekst2);
								klikniêtyPionek->rysujPionek(klikX - 44, klikY - 44);
								al_flip_display();
							}
						}
						if (!dzia³a)
							break;
						plansza->drukujPlansza();
						al_flip_display();
						al_unregister_event_source(kolejka, al_get_mouse_event_source());
						ruchKomputera(plansza, pionkiGracza, pionkiKomputera, ruchyDamekBezZbicia);
						al_register_event_source(kolejka, al_get_mouse_event_source());
						break;
					}
				}
			}
			// WYRYSOWANIE SYTUACJI NA EKRANIE
			plansza->drukujPlansza();
			klikniêtyPionek->rysujPionek(klikX-44,klikY-44);
			al_flip_display();
		}
		if (!dzia³a)
			break;
		if (zdarzenie.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			dzia³a = false;
	}
	delete plansza;
	al_destroy_font(czcionka);
	al_destroy_display(ekran);
	al_destroy_event_queue(kolejka);
	al_uninstall_mouse();
	return 0;
}