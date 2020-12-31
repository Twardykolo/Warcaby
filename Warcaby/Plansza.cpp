#include "Plansza.h"
Plansza::Plansza()
{
	obrazekPlanszy = al_load_bitmap("plansza.png");
	initPlansza();
}
Plansza::~Plansza()
{
	for (int i = 0; i < 24; i++)
		delete pionki[i];
	al_destroy_bitmap(obrazekPlanszy);
}
void Plansza::drukujPlansza()
{
	al_clear_to_color(al_map_rgb(0, 0, 0));
	assert(obrazekPlanszy != NULL);
	al_draw_bitmap(obrazekPlanszy, 4, 4, 0);
	for (int i = 0; i < 24; i++)
		pionki[i]->rysujPionek();
}
void Plansza::initPlansza()
{
	pionki[0] = new Pionek(plansza_X[0], plansza_Y[1]);
	pionki[1] = new Pionek(plansza_X[1], plansza_Y[0]);
	pionki[2] = new Pionek(plansza_X[1], plansza_Y[2]);
	pionki[3] = new Pionek(plansza_X[2], plansza_Y[1]);
	pionki[4] = new Pionek(plansza_X[3], plansza_Y[0]);
	pionki[5] = new Pionek(plansza_X[3], plansza_Y[2]);
	pionki[6] = new Pionek(plansza_X[4], plansza_Y[1]);
	pionki[7] = new Pionek(plansza_X[5], plansza_Y[0]);
	pionki[8] = new Pionek(plansza_X[5], plansza_Y[2]);
	pionki[9] = new Pionek(plansza_X[6], plansza_Y[1]);
	pionki[10] = new Pionek(plansza_X[7], plansza_Y[0]);
	pionki[11] = new Pionek(plansza_X[7], plansza_Y[2]);
	pionki[12] = new Pionek(plansza_X[0], plansza_Y[5], true);
	pionki[13] = new Pionek(plansza_X[0], plansza_Y[7], true);
	pionki[14] = new Pionek(plansza_X[1], plansza_Y[6], true);
	pionki[15] = new Pionek(plansza_X[2], plansza_Y[5], true);
	pionki[16] = new Pionek(plansza_X[2], plansza_Y[7], true);
	pionki[17] = new Pionek(plansza_X[3], plansza_Y[6], true);
	pionki[18] = new Pionek(plansza_X[4], plansza_Y[5], true);
	pionki[19] = new Pionek(plansza_X[4], plansza_Y[7], true);
	pionki[20] = new Pionek(plansza_X[5], plansza_Y[6], true);
	pionki[21] = new Pionek(plansza_X[6], plansza_Y[5], true);
	pionki[22] = new Pionek(plansza_X[6], plansza_Y[7], true);
	pionki[23] = new Pionek(plansza_X[7], plansza_Y[6], true);
	czyMiejscaZaj�te[0][1] = true;
	czyMiejscaZaj�te[1][0] = true;
	czyMiejscaZaj�te[1][2] = true;
	czyMiejscaZaj�te[2][1] = true;
	czyMiejscaZaj�te[3][0] = true;
	czyMiejscaZaj�te[3][2] = true;
	czyMiejscaZaj�te[4][1] = true;
	czyMiejscaZaj�te[5][0] = true;
	czyMiejscaZaj�te[5][2] = true;
	czyMiejscaZaj�te[6][1] = true;
	czyMiejscaZaj�te[7][0] = true;
	czyMiejscaZaj�te[7][2] = true;
	czyMiejscaZaj�te[0][5] = true;
	czyMiejscaZaj�te[0][7] = true;
	czyMiejscaZaj�te[1][6] = true;
	czyMiejscaZaj�te[2][5] = true;
	czyMiejscaZaj�te[2][7] = true;
	czyMiejscaZaj�te[3][6] = true;
	czyMiejscaZaj�te[4][5] = true;
	czyMiejscaZaj�te[4][7] = true;
	czyMiejscaZaj�te[5][6] = true;
	czyMiejscaZaj�te[6][5] = true;
	czyMiejscaZaj�te[6][7] = true;
	czyMiejscaZaj�te[7][6] = true;
}

Pionek* Plansza::wyb�rPionka(int klikX, int klikY)
{
	for (int i = 0; i < 24; i++) {
		if (pionki[i]->pozycja_X<klikX && pionki[i]->pozycja_X + 99>klikX && pionki[i]->pozycja_Y<klikY && pionki[i]->pozycja_Y + 99>klikY) {
			klikni�tyPionek = pionki[i];
			break;
		}
	}
	return klikni�tyPionek;
}
