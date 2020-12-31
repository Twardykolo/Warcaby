#pragma once
#include "Pionek.h"
#include "allegro5/allegro5.h"
class Plansza
{
	ALLEGRO_BITMAP* obrazekPlanszy = NULL;
public:
	Pionek* pionki[24];
	Pionek* klikni�tyPionek=nullptr;
	Plansza();
	~Plansza();
	void drukujPlansza();
	int plansza_X[8]={ 4,103,202,301,400,499,598,697 };
	int plansza_Y[8]= { 4,103,202,301,400,499,598,697 };
	bool czyMiejscaZaj�te[8][8] = { false };
	void initPlansza();
	Pionek* wyb�rPionka(int klikX,int klikY);
};