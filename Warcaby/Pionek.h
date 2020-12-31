#pragma once
#include "allegro5/allegro5.h"
class Pionek
{
	bool aktywny = true;
	bool czyCzerwony = false;
	bool Kr�lowa = false;
	bool klikni�ty = false;
	ALLEGRO_BITMAP* obrazekPionka = NULL;
public:
	struct Ruch {
		int x;
		int y;
		int ileZbija;
		Pionek* zbijany = nullptr;
		Pionek* wykonuj�cyRuch = nullptr;
	};
	int pozycja_X;
	int pozycja_Y;
	int logicznyX;
	int logicznyY;
	int dostepneRuchy = 0;
	Ruch* ruchy[4] = { nullptr };
	Pionek(int x, int y, bool czyCzerwony = false);
	bool czyAktywny();
	void jeste�Klikni�ty();
	void nieJeste�Klikni�ty();
	bool czyGracza();
	bool czyKr�lowa();
	void rysujPionek();
	void rysujPionek(int X,int Y);
	void zostajeKr�low�();
	void zostajeZbity(bool czyMiejscaZaj�te[8][8]);
	void obliczRuchy(bool czyMiejscaZaj�te[8][8], Pionek* pionki[24]);
	void ruch(Ruch *ruch, bool czyMiejscaZaj�te[8][8]);
};