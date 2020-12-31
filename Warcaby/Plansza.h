#pragma once
#include "Pionek.h"
#include "allegro5/allegro5.h"
class Plansza
{
	ALLEGRO_BITMAP* obrazekPlanszy = NULL;
public:
	Pionek* pionki[24];
	Pionek* klikniêtyPionek=nullptr;
	Plansza();
	~Plansza();
	void drukujPlansza();
	int plansza_X[8]={ 4,103,202,301,400,499,598,697 };
	int plansza_Y[8]= { 4,103,202,301,400,499,598,697 };
	bool czyMiejscaZajête[8][8] = { false };
	void initPlansza();
	Pionek* wybórPionka(int klikX,int klikY);
};