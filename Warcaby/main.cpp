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
	Sleep(2000); // czekanie 2 sekundy, �eby wydawa�o si�, �e my�li nad ruchem
	// P�TLA SZUKAJ�CA NAJWI�KSZEJ WYST�PUJ�CEJ WARTO�CI RUCHU
	for (int i = 0; i < 12; i++) {
		pionkiCPU[i] = plansza->pionki[i];
		if (pionkiCPU[i]->czyAktywny()) {
			pionkiCPU[i]->obliczRuchy(plansza->czyMiejscaZaj�te, plansza->pionki);
			for (int j = 0; j < pionkiCPU[i]->dostepneRuchy; j++) {
				if (pionkiCPU[i]->ruchy[j] == nullptr)
					continue;
				if (pionkiCPU[i]->ruchy[j]->ileZbija > czyZbija) {
					czyZbija = pionkiCPU[i]->ruchy[j]->ileZbija;
				}
			}
		}
	}
	// P�TLA WYBIERAJ�CA I WYKONUJ�CA RUCH
	for (int i = 0; i < 12; i++) {
		if (pionkiCPU[i]->dostepneRuchy > 0) {
			for (int j = 0; j < pionkiCPU[i]->dostepneRuchy; j++) {
				if (pionkiCPU[i]->ruchy[j] != nullptr) {
					// WARUNEK ANTY-SAMOB�JCZY
					if (pionkiCPU[i]->ruchy[j]->ileZbija == -1 && czyZbija > -1)
						continue;
					// JE�ELI PUNKTACJA TEGO RUCHU JEST R�WNA PUNKTACJI NAJWY�EJ PUNKTOWANEGO MO�LIWEGO RUCHU
					if (pionkiCPU[i]->ruchy[j]->ileZbija == czyZbija) {
						// JE�ELI JEST TO RUCH NIEZBIJAJ�CY
						if (czyZbija <= 0) {
							if (pionkiCPU[i]->czyKr�lowa())
								ruchyDamekBezZbicia++;
							else
								ruchyDamekBezZbicia = 0;
							pionkiCPU[i]->ruch(pionkiCPU[i]->ruchy[j], plansza->czyMiejscaZaj�te);
							for (int i = 0; i < 12; i++) {
								for (int j = 0; j < pionkiCPU[i]->dostepneRuchy; j++) {
									delete[] pionkiCPU[i]->ruchy[j];
									pionkiCPU[i]->ruchy[j]=nullptr;
								}
								pionkiCPU[i]->dostepneRuchy = 0;
							}
							return;
						}
						// JE�ELI JEST TO RUCH ZBIJAJ�CY
						else {
							if (pionkiCPU[i]->ruchy[j]->ileZbija > 1) {
								combo = true;
							}
							if (pionkiCPU[i]->czyKr�lowa())
								ruchyDamekBezZbicia = 0;
							pionkiCPU[i]->ruchy[j]->zbijany->zostajeZbity(plansza->czyMiejscaZaj�te);
							pionkiGracza--;
							pionkiCPU[i]->ruch(pionkiCPU[i]->ruchy[j], plansza->czyMiejscaZaj�te);
							for (int i = 0; i < 12; i++) {
								for (int j = 0; j < pionkiCPU[i]->dostepneRuchy; j++) {
									delete[] pionkiCPU[i]->ruchy[j];
									pionkiCPU[i]->ruchy[j] = nullptr;
								}
								pionkiCPU[i]->dostepneRuchy = 0;
							}
							// JE�ELI MO�E ZBI� WIELOKROTNIE
							if (combo == true) {
								plansza->drukujPlansza();
								al_flip_display();
								Sleep(200);
								pionkiCPU[i]->obliczRuchy(plansza->czyMiejscaZaj�te,plansza->pionki);
								for (int k = 0; k < pionkiCPU[i]->dostepneRuchy; k++) {
									if (pionkiCPU[i]->ruchy[k]->ileZbija >= 1) {
										pionkiCPU[i]->ruchy[k]->zbijany->zostajeZbity(plansza->czyMiejscaZaj�te);
										pionkiGracza--;
										pionkiCPU[i]->ruch(pionkiCPU[i]->ruchy[k], plansza->czyMiejscaZaj�te);
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
	// GDYBY ODM�WI� WYKONANIA RUCHU
	for (int i = 0; i < 12; i++) {
		if (pionkiCPU[i]->dostepneRuchy > 0) {
			pionkiCPU[i]->ruch(pionkiCPU[i]->ruchy[0], plansza->czyMiejscaZaj�te);
		}
	}
}

void reset(Plansza *plansza, int &pionkiGracza, int &pionkiCPU, int &ruchyDamekBezZbicia) {
	for (int i = 0; i < 24; i++)
		delete[] plansza->pionki[i];
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			plansza->czyMiejscaZaj�te[i][j] = false;
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
	Pionek* klikni�tyPionek = nullptr, * zbijanyPionek = nullptr;
	Pionek::Ruch* wykonywanyRuch = nullptr;
	ALLEGRO_EVENT zdarzenie;
	bool dzia�a=true,combo=false;
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
	// G��WNA P�TLA GRY
	for (;dzia�a==true;) {
		plansza->drukujPlansza();
		al_flip_display();
		al_wait_for_event(kolejka, &zdarzenie);
		if (pionkiGracza == 0 || pionkiKomputera == 0 || ruchyDamekBezZbicia==15) {
			koniecGry(plansza,pionkiGracza,pionkiKomputera,ruchyDamekBezZbicia,klikX,klikY,kolor,kolejka,zdarzenie,czcionka);
			continue;
		}
		// OBS�UGA KLIKNI�� MYSZY
		if (zdarzenie.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			klikX = zdarzenie.mouse.x;
			klikY = zdarzenie.mouse.y;
			if (klikni�tyPionek == nullptr) {
				klikni�tyPionek = plansza->wyb�rPionka(klikX, klikY);
				if (klikni�tyPionek == nullptr)
					continue;
				if (klikni�tyPionek->czyGracza()) {
					klikni�tyPionek->jeste�Klikni�ty();
					klikni�tyPionek->obliczRuchy(plansza->czyMiejscaZaj�te,plansza->pionki);
				}
				else {
					klikni�tyPionek = nullptr;
				}
			}
		}
		// P�TLA KIEDY WYBRALI�MY PIONEK
		while (klikni�tyPionek != nullptr) {
			al_wait_for_event(kolejka, &zdarzenie);
			if (zdarzenie.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
				break;
			klikX = zdarzenie.mouse.x;
			klikY = zdarzenie.mouse.y;
			if (zdarzenie.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && pionkiGracza!=0 && pionkiKomputera!=0) {
				// OD�O�ENIE PIONKA
				if (klikni�tyPionek->pozycja_X<klikX && klikni�tyPionek->pozycja_X + 99>klikX && klikni�tyPionek->pozycja_Y<klikY && klikni�tyPionek->pozycja_Y + 99>klikY) {
					klikni�tyPionek->nieJeste�Klikni�ty();
					klikni�tyPionek = nullptr;
					break;
				}
				// ODNALEZIENIE RUCHU, KT�RY CHCEMY WYKONA�
				for (int i = 0; i < klikni�tyPionek->dostepneRuchy; i++) {
					if (klikni�tyPionek->ruchy[i]->x<klikX && klikni�tyPionek->ruchy[i]->x + 99>klikX && klikni�tyPionek->ruchy[i]->y<klikY && klikni�tyPionek->ruchy[i]->y + 99>klikY) {
						wykonywanyRuch = klikni�tyPionek->ruchy[i];
						break;
					}
				}
				// WYKONANIE RUCHU
				if (wykonywanyRuch != nullptr) {
					if (wykonywanyRuch->ileZbija <= 0) {
						// PRZYPADEK KIEDY RUCH NIE ZBIJA �ADNEGO PIONKA
						if (klikni�tyPionek->czyKr�lowa())
							ruchyDamekBezZbicia++;
						else
							ruchyDamekBezZbicia = 0;
						klikni�tyPionek->ruch(wykonywanyRuch, plansza->czyMiejscaZaj�te);
						wykonywanyRuch = nullptr;
						klikni�tyPionek->nieJeste�Klikni�ty();
						klikni�tyPionek = nullptr;
						plansza->drukujPlansza();
						al_flip_display();
						al_unregister_event_source(kolejka, al_get_mouse_event_source());
						ruchKomputera(plansza, pionkiGracza, pionkiKomputera,ruchyDamekBezZbicia);
						al_register_event_source(kolejka, al_get_mouse_event_source());
						break;
					}
					else {
						// PRZYPADEK KIEDY RUCH ZBIJA PIONEK
						if (klikni�tyPionek->czyKr�lowa())
							ruchyDamekBezZbicia=0;
						if (wykonywanyRuch->ileZbija > 1)
							combo = true;
						wykonywanyRuch->zbijany->zostajeZbity(plansza->czyMiejscaZaj�te);
						klikni�tyPionek->ruch(wykonywanyRuch, plansza->czyMiejscaZaj�te);
						wykonywanyRuch = nullptr;
						if (!combo) {
							klikni�tyPionek->nieJeste�Klikni�ty();
							klikni�tyPionek = nullptr;
						}
						pionkiKomputera--;
						plansza->drukujPlansza();
						if (combo) {
							tekst = (char*)"Musisz wykonac";
							tekst2 = (char*)"wielokrotne bicie";
							kolor = al_map_rgb(200, 200, 20);
							al_draw_text(czcionka, kolor, 100, 250, 0, tekst);
							al_draw_text(czcionka, kolor, 100, 350, 0, tekst2);
							klikni�tyPionek->obliczRuchy(plansza->czyMiejscaZaj�te,plansza->pionki);
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
									dzia�a = false;
									break;
								}
								if (zdarzenie.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
									// ODNALEZIENIE RUCHU, KT�RY CHCEMY WYKONA�
									for (int i = 0; i < klikni�tyPionek->dostepneRuchy; i++) {
										if (klikni�tyPionek->ruchy[i]->x<klikX && klikni�tyPionek->ruchy[i]->x + 99>klikX && klikni�tyPionek->ruchy[i]->y<klikY && klikni�tyPionek->ruchy[i]->y + 99>klikY) {
											wykonywanyRuch = klikni�tyPionek->ruchy[i];
											break;
										}
									}
									if (wykonywanyRuch == nullptr) {
										continue;
									}
									if (wykonywanyRuch->ileZbija >= 1) {
										if (wykonywanyRuch->ileZbija > 1)
											combo = true;
										wykonywanyRuch->zbijany->zostajeZbity(plansza->czyMiejscaZaj�te);
										klikni�tyPionek->ruch(wykonywanyRuch, plansza->czyMiejscaZaj�te);
										wykonywanyRuch = nullptr;
										pionkiKomputera--;
										plansza->drukujPlansza();
										if (combo) {
											klikni�tyPionek->obliczRuchy(plansza->czyMiejscaZaj�te, plansza->pionki);
											if (klikni�tyPionek->ruchy[0] == nullptr) {
												combo = false;
												break;
											}
										}
									}
									if (!combo) {
										klikni�tyPionek->nieJeste�Klikni�ty();
										klikni�tyPionek = nullptr;
										break;
									}
								}
								plansza->drukujPlansza();
								al_draw_text(czcionka, kolor, 100, 250, 0, tekst);
								al_draw_text(czcionka, kolor, 100, 350, 0, tekst2);
								klikni�tyPionek->rysujPionek(klikX - 44, klikY - 44);
								al_flip_display();
							}
						}
						if (!dzia�a)
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
			klikni�tyPionek->rysujPionek(klikX-44,klikY-44);
			al_flip_display();
		}
		if (!dzia�a)
			break;
		if (zdarzenie.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			dzia�a = false;
	}
	delete plansza;
	al_destroy_font(czcionka);
	al_destroy_display(ekran);
	al_destroy_event_queue(kolejka);
	al_uninstall_mouse();
	return 0;
}