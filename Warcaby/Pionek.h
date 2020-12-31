#pragma once
#include "allegro5/allegro5.h"
class Pionek
{
	bool aktywny = true;
	bool czyCzerwony = false;
	bool Królowa = false;
	bool klikniêty = false;
	ALLEGRO_BITMAP* obrazekPionka = NULL;
public:
	struct Ruch {
		int x;
		int y;
		int ileZbija;
		Pionek* zbijany = nullptr;
		Pionek* wykonuj¹cyRuch = nullptr;
	};
	int pozycja_X;
	int pozycja_Y;
	int logicznyX;
	int logicznyY;
	int dostepneRuchy = 0;
	Ruch* ruchy[4] = { nullptr };
	Pionek(int x, int y, bool czyCzerwony = false);
	bool czyAktywny();
	void jesteœKlikniêty();
	void nieJesteœKlikniêty();
	bool czyGracza();
	bool czyKrólowa();
	void rysujPionek();
	void rysujPionek(int X,int Y);
	void zostajeKrólow¹();
	void zostajeZbity(bool czyMiejscaZajête[8][8]);
	void obliczRuchy(bool czyMiejscaZajête[8][8], Pionek* pionki[24]);
	void ruch(Ruch *ruch, bool czyMiejscaZajête[8][8]);
};