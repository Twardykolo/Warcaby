#include "Pionek.h"
#include "allegro5/allegro5.h"
Pionek::Pionek(int x, int y, bool czyCzerwony)
{
	pozycja_X = x;
	pozycja_Y = y;
	logicznyX = x / 99;
	logicznyY = y / 99;
	this->czyCzerwony = czyCzerwony;
	if (this->czyCzerwony == true)
		obrazekPionka = al_load_bitmap("czerwonyPionek.png");
	else
		obrazekPionka = al_load_bitmap("czarnyPionek.png");
}

bool Pionek::czyAktywny()
{
	return aktywny;
}

bool Pionek::czyGracza()
{
	return czyCzerwony;
}

bool Pionek::czyKrólowa()
{
	return Królowa;
}

void Pionek::jesteœKlikniêty()
{
	klikniêty = true;
}

void Pionek::nieJesteœKlikniêty()
{
	klikniêty = false;
}

void Pionek::rysujPionek()
{
	if (this->aktywny == false)
		return;
	ALLEGRO_BITMAP* obrazekPionkaRysowany = this->obrazekPionka;
	if(this->klikniêty==true && this->czyKrólowa()==false)
		obrazekPionkaRysowany = al_load_bitmap("czerwonyKliknietyPionek.png");
	if (this->klikniêty == true && this->czyKrólowa() == true)
		obrazekPionkaRysowany = al_load_bitmap("czerwonyKliknietyKrolowa.png");
	assert(obrazekPionkaRysowany != NULL);
	al_draw_bitmap(obrazekPionkaRysowany, this->pozycja_X+4, this->pozycja_Y+4, 0);
}

void Pionek::zostajeKrólow¹()
{
	this->Królowa = true;
	if (this->czyCzerwony == true)
		obrazekPionka = al_load_bitmap("czerwonyKrolowa.png");
	else
		obrazekPionka = al_load_bitmap("czarnyKrolowa.png");
}

void Pionek::rysujPionek(int X, int Y)
{
	if (this->aktywny == false)
		return;
	assert(obrazekPionka != NULL);
	al_draw_bitmap(obrazekPionka, X + 4, Y + 4, 0);
}

void Pionek::obliczRuchy(bool czyMiejscaZajête[8][8],Pionek* pionki[24])
{
	// Funkcja sprawiaj¹ca, ¿e programista rozwa¿a zostanie akrobat¹
	if (!this->czyAktywny())
		return;
	Pionek* zbijany = nullptr;
	logicznyX = this->pozycja_X / 99;
	logicznyY = this->pozycja_Y / 99;
	// RUCHY GRACZA LUB DAMKI
	if (this->czyGracza() || this->czyKrólowa()) {
		// RUCH W LEWO DO GÓRY
		if(logicznyX>0 && czyMiejscaZajête[logicznyX-1][logicznyY-1]==false && logicznyY>0){
			Ruch* nowyRuch = new Ruch({ pozycja_X - 99,pozycja_Y - 99,0 });
			for (int i = 0; i < 24; i++) {
				if (!pionki[i]->czyAktywny())
					continue;
				if (pionki[i]->pozycja_X == nowyRuch->x - 99 && pionki[i]->pozycja_Y == nowyRuch->y - 99 || pionki[i]->pozycja_X == nowyRuch->x + 99 && pionki[i]->pozycja_Y == nowyRuch->y - 99) {
					if (pionki[i]->czyGracza() != this->czyGracza() && pionki[i]->czyKrólowa()) {
						nowyRuch->ileZbija = -1;
						break;
					}
				}
			}
			nowyRuch->wykonuj¹cyRuch = this;
			this->ruchy[dostepneRuchy++] = nowyRuch;
		}
		// RUCH W PRAWO DO GÓRY
		if(logicznyX<7 && czyMiejscaZajête[logicznyX+1][logicznyY-1]==false && logicznyY>0){
			Ruch* nowyRuch = new Ruch({ pozycja_X + 99,pozycja_Y - 99,0 });
			for (int i = 0; i < 24; i++) {
				if (!pionki[i]->czyAktywny())
					continue;
				if (pionki[i]->pozycja_X == nowyRuch->x - 99 && pionki[i]->pozycja_Y == nowyRuch->y - 99 || pionki[i]->pozycja_X == nowyRuch->x + 99 && pionki[i]->pozycja_Y == nowyRuch->y - 99) {
					if (pionki[i]->czyGracza() != this->czyGracza() && pionki[i]->czyKrólowa()) {
						nowyRuch->ileZbija = -1;
						break;
					}
				}
			}
			nowyRuch->wykonuj¹cyRuch = this;
			this->ruchy[dostepneRuchy++] = nowyRuch;
		}
		// RUCH ZBIJAJ¥CY W LEWO DO GÓRY
		if (logicznyX > 1 && czyMiejscaZajête[logicznyX - 1][logicznyY - 1] == true && czyMiejscaZajête[logicznyX - 2][logicznyY - 2] == false && logicznyY > 1) {
			for (int i = 0; i < 24; i++) {
				if (pionki[i]->pozycja_X == pozycja_X - 99 && pionki[i]->pozycja_Y == pozycja_Y - 99) {
					zbijany = pionki[i];
					break;
				}
			}
			if (this->czyGracza() != zbijany->czyGracza()) {
				Ruch* nowyRuch = new Ruch({ pozycja_X - 2 * 99,pozycja_Y - 2 * 99,1 ,zbijany });
				nowyRuch->wykonuj¹cyRuch = this;
				// SPRAWD CZY MO¯NA ZBIÆ WIELOKROTNIE
				if (zbijany->logicznyY >= 3) {
					//ZDALA OD LEWEJ KRAWÊDZI PLANSZY
					if (zbijany->logicznyX >= 3) {
						if (czyMiejscaZajête[zbijany->logicznyX - 2][zbijany->logicznyY - 2] == true && czyMiejscaZajête[zbijany->logicznyX - 3][zbijany->logicznyY - 3] == false || czyMiejscaZajête[zbijany->logicznyX][zbijany->logicznyY - 2] == true && czyMiejscaZajête[zbijany->logicznyX + 1][zbijany->logicznyY - 3] == false) {
							for (int i = 0; i < 24; i++) {
								if (pionki[i]->logicznyX == zbijany->logicznyX - 2 && pionki[i]->logicznyY == zbijany->logicznyY - 2 || pionki[i]->logicznyX == zbijany->logicznyX && pionki[i]->logicznyY == zbijany->logicznyY - 2) {
									if (pionki[i]->czyGracza() != this->czyGracza()) {
										nowyRuch->ileZbija = 2;
										break;
									}
								}
							}
						}
					}
					// BLISKO LEWEJ KRAWÊDZI PLANSZY
					if (zbijany->logicznyX > 0 && zbijany->logicznyX < 3) {
						if (czyMiejscaZajête[zbijany->logicznyX][zbijany->logicznyY - 2] == true && czyMiejscaZajête[zbijany->logicznyX + 1][zbijany->logicznyY - 3] == false) {
							for (int i = 0; i < 24; i++) {
								if (pionki[i]->logicznyX == zbijany->logicznyX && pionki[i]->logicznyY == zbijany->logicznyY - 2) {
									if (pionki[i]->czyGracza() != this->czyGracza()) {
										nowyRuch->ileZbija = 2;
										break;
									}
								}
							}
						}
					}
				}
				this->ruchy[dostepneRuchy++] = nowyRuch;
			}
		}
		// RUCH ZBIJAJ¥CY W PRAWO DO GÓRY
		if (logicznyX < 6 && czyMiejscaZajête[logicznyX + 1][logicznyY - 1] == true && czyMiejscaZajête[logicznyX+2][logicznyY-2] == false && logicznyY > 1) {
			for (int i = 0; i < 24; i++) {
				if (pionki[i]->pozycja_X == pozycja_X + 99 && pionki[i]->pozycja_Y == pozycja_Y - 99) {
					zbijany = pionki[i];
					break;
				}
			}
			if (this->czyGracza() != zbijany->czyGracza()) {
				Ruch *nowyRuch = new Ruch( { pozycja_X + 2 * 99,pozycja_Y - 2 * 99,1 ,zbijany });
				nowyRuch->wykonuj¹cyRuch = this;
				// SPRAWD CZY MO¯NA ZBIÆ WIELOKROTNIE
				if (zbijany->logicznyY >= 3) {
					//ZDALA OD PRAWEJ KRAWÊDZI PLANSZY
					if (zbijany->logicznyX <= 4) {
						if (czyMiejscaZajête[zbijany->logicznyX + 2][zbijany->logicznyY - 2] == true && czyMiejscaZajête[zbijany->logicznyX + 3][zbijany->logicznyY - 3] == false || czyMiejscaZajête[zbijany->logicznyX][zbijany->logicznyY - 2] == true && czyMiejscaZajête[zbijany->logicznyX - 1][zbijany->logicznyY - 3] == false) {
							for (int i = 0; i < 24; i++) {
								if (pionki[i]->logicznyX == zbijany->logicznyX + 2 && pionki[i]->logicznyY == zbijany->logicznyY - 2 || pionki[i]->logicznyX == zbijany->logicznyX && pionki[i]->logicznyY == zbijany->logicznyY - 2) {
									if (pionki[i]->czyGracza() != this->czyGracza()) {
										nowyRuch->ileZbija = 2;
										break;
									}
								}
							}
						}
					}
					// BLISKO PRAWEJ KRAWÊDZI PLANSZY
					if (zbijany->logicznyX < 7 && zbijany->logicznyX > 4) {
						if (czyMiejscaZajête[zbijany->logicznyX][zbijany->logicznyY - 2] == true && czyMiejscaZajête[zbijany->logicznyX - 1][zbijany->logicznyY - 3] == false) {
							for (int i = 0; i < 24; i++) {
								if (pionki[i]->logicznyX == zbijany->logicznyX && pionki[i]->logicznyY == zbijany->logicznyY - 2) {
									if (pionki[i]->czyGracza() != this->czyGracza()) {
										nowyRuch->ileZbija = 2;
										break;
									}
								}
							}
						}
					}
				}
				
				this->ruchy[dostepneRuchy++] = nowyRuch;
			}
		}
	}
	// RUCHY KOMPUTERA LUB DAMKI
	if (!this->czyGracza() || this->czyKrólowa()) {
		// RUCH W LEWO DO DO£U
		if (logicznyX > 0 && czyMiejscaZajête[logicznyX - 1][logicznyY + 1] == false && logicznyY < 7) {
			Ruch* nowyRuch = new Ruch({pozycja_X-99,pozycja_Y+99,0});
			for (int i = 0; i < 24; i++) {
				if (!pionki[i]->czyAktywny())
					continue;
				if (pionki[i]->pozycja_X == nowyRuch->x - 99 && pionki[i]->pozycja_Y == nowyRuch->y - 99 || pionki[i]->pozycja_X == nowyRuch->x + 99 && pionki[i]->pozycja_Y == nowyRuch->y - 99 || pionki[i]->czyKrólowa() && pionki[i]->pozycja_X == nowyRuch->x - 99 && pionki[i]->pozycja_Y == nowyRuch->y + 99 || pionki[i]->czyKrólowa() && pionki[i]->pozycja_X == nowyRuch->x + 99 && pionki[i]->pozycja_Y == nowyRuch->y + 99) {
					if (pionki[i]->czyGracza() != this->czyGracza()) {
						nowyRuch->ileZbija = -1;
						break;
					}
				}
			}
			nowyRuch->wykonuj¹cyRuch = this;
			this->ruchy[dostepneRuchy++] = nowyRuch;
		}
		// RUCH W PRAWO DO DO£U
		if (logicznyX < 7 && czyMiejscaZajête[logicznyX + 1][logicznyY + 1] == false && logicznyY<7) {
			Ruch* nowyRuch = new Ruch({ pozycja_X + 99,pozycja_Y + 99,0 });
			for (int i = 0; i < 24; i++) {
				if (!pionki[i]->czyAktywny())
					continue;
				if (pionki[i]->pozycja_X == nowyRuch->x - 99 && pionki[i]->pozycja_Y == nowyRuch->y - 99 || pionki[i]->pozycja_X == nowyRuch->x + 99 && pionki[i]->pozycja_Y == nowyRuch->y - 99 || pionki[i]->czyKrólowa() && pionki[i]->pozycja_X == nowyRuch->x - 99 && pionki[i]->pozycja_Y == nowyRuch->y + 99 || pionki[i]->czyKrólowa() && pionki[i]->pozycja_X == nowyRuch->x + 99 && pionki[i]->pozycja_Y == nowyRuch->y + 99) {
					if (pionki[i]->czyGracza() != this->czyGracza()) {
						nowyRuch->ileZbija = -1;
						break;
					}
				}
			}
			nowyRuch->wykonuj¹cyRuch = this;
			this->ruchy[dostepneRuchy++] = nowyRuch;
		}
		// RUCH ZBIJAJ¥CY W LEWO DO DO£U
		if (logicznyX > 1 && logicznyX<=8 && czyMiejscaZajête[logicznyX - 1][logicznyY + 1] == true && czyMiejscaZajête[logicznyX - 2][logicznyY + 2] == false && logicznyY < 6) {
			for (int i = 0; i < 24; i++) {
				if (pionki[i]->pozycja_X == pozycja_X - 99 && pionki[i]->pozycja_Y == pozycja_Y + 99) {
					zbijany = pionki[i];
					break;
				}
			}
			if (this->czyGracza() != zbijany->czyGracza()) {
				Ruch* nowyRuch = new Ruch({ pozycja_X - 2 * 99,pozycja_Y + 2 * 99,1,zbijany});
				nowyRuch->wykonuj¹cyRuch = this;
				// SPRAWD CZY MO¯NA ZBIÆ WIELOKROTNIE
				if (zbijany->logicznyY <= 4) {
					//ZDALA OD LEWEJ KRAWÊDZI PLANSZY
					if (zbijany->logicznyX >= 3) {
						if (czyMiejscaZajête[zbijany->logicznyX - 2][zbijany->logicznyY + 2] == true && czyMiejscaZajête[zbijany->logicznyX - 3][zbijany->logicznyY + 3] == false || czyMiejscaZajête[zbijany->logicznyX][zbijany->logicznyY + 2] == true && czyMiejscaZajête[zbijany->logicznyX + 1][zbijany->logicznyY + 3] == false) {
							for (int i = 0; i < 24; i++) {
								if (pionki[i]->logicznyX == zbijany->logicznyX - 2 && pionki[i]->logicznyY == zbijany->logicznyY + 2 || pionki[i]->logicznyX == zbijany->logicznyX && pionki[i]->logicznyY == zbijany->logicznyY + 2) {
									if (pionki[i]->czyGracza() != this->czyGracza()) {
										nowyRuch->ileZbija = 2;
										break;
									}
								}
							}
						}
					}
					// BLISKO LEWEJ KRAWÊDZI PLANSZY
					if (zbijany->logicznyX > 0 && zbijany->logicznyX < 3) {
						if (czyMiejscaZajête[zbijany->logicznyX][zbijany->logicznyY + 2] == true && czyMiejscaZajête[zbijany->logicznyX + 1][zbijany->logicznyY + 3] == false) {
							for (int i = 0; i < 24; i++) {
								if (pionki[i]->logicznyX == zbijany->logicznyX && pionki[i]->logicznyY == zbijany->logicznyY + 2) {
									if (pionki[i]->czyGracza() != this->czyGracza()) {
										nowyRuch->ileZbija = 2;
										break;
									}
								}
							}
						}
					}
				}
				this->ruchy[dostepneRuchy++] = nowyRuch;
			}
		}
		// RUCH ZBIJAJ¥CY W PRAWO DO DO£U
		if (logicznyX < 6 && logicznyX >= 0 && czyMiejscaZajête[logicznyX + 1][logicznyY + 1] == true && czyMiejscaZajête[logicznyX + 2][logicznyY + 2] == false && logicznyY < 6) {
			for (int i = 0; i < 24; i++) {
				if (pionki[i]->pozycja_X == pozycja_X + 99 && pionki[i]->pozycja_Y == pozycja_Y + 99) {
					zbijany = pionki[i];
					break;
				}
			}
			if(zbijany!=nullptr)
			if (this->czyGracza() != zbijany->czyGracza()) {
				Ruch* nowyRuch = new Ruch({ pozycja_X + 2 * 99,pozycja_Y + 2 * 99, 1 ,zbijany });
				nowyRuch->wykonuj¹cyRuch = this;
				// SPRAWD CZY MO¯NA ZBIÆ WIELOKROTNIE
				if (zbijany->logicznyY <= 4) {
					//ZDALA OD PRAWEJ KRAWÊDZI PLANSZY
					if (zbijany->logicznyX <= 4) {
						if (czyMiejscaZajête[zbijany->logicznyX + 2][zbijany->logicznyY + 2] == true && czyMiejscaZajête[zbijany->logicznyX + 3][zbijany->logicznyY + 3] == false || czyMiejscaZajête[zbijany->logicznyX][zbijany->logicznyY + 2] == true && czyMiejscaZajête[zbijany->logicznyX - 1][zbijany->logicznyY + 3] == false) {
							for (int i = 0; i < 24; i++) {
								if (pionki[i]->logicznyX == zbijany->logicznyX + 2 && pionki[i]->logicznyY == zbijany->logicznyY + 2 || pionki[i]->logicznyX == zbijany->logicznyX && pionki[i]->logicznyY == zbijany->logicznyY + 2) {
									if (pionki[i]->czyGracza() != this->czyGracza()) {
										nowyRuch->ileZbija = 2;
										break;
									}
								}
							}
						}
					}
					// BLISKO PRAWEJ KRAWÊDZI PLANSZY
					if (zbijany->logicznyX < 7 && zbijany->logicznyX > 4) {
						if (czyMiejscaZajête[zbijany->logicznyX][zbijany->logicznyY + 2] == true && czyMiejscaZajête[zbijany->logicznyX - 1][zbijany->logicznyY + 3] == false) {
							for (int i = 0; i < 24; i++) {
								if (pionki[i]->logicznyX == zbijany->logicznyX && pionki[i]->logicznyY == zbijany->logicznyY + 2) {
									if (pionki[i]->czyGracza() != this->czyGracza()) {
										nowyRuch->ileZbija = 2;
										break;
									}
								}
							}
						}
					}
				}
				this->ruchy[dostepneRuchy++] = nowyRuch;
			}
		}
	}
}

void Pionek::zostajeZbity(bool czyMiejscaZajête[8][8])
{
	if (this == nullptr)
		return;
	this->aktywny = false;
	czyMiejscaZajête[this->logicznyX][this->logicznyY] = false;
	this->pozycja_X = -200;
	this->pozycja_Y = -200;
	this->logicznyX = -15;
	this->logicznyY = -15;
}

void Pionek::ruch(Ruch *ruch,bool czyMiejscaZajête[8][8])
{
	czyMiejscaZajête[logicznyX][logicznyY] = false;
	this->pozycja_X = ruch->x;
	this->pozycja_Y = ruch->y;
	this->logicznyX = this->pozycja_X / 99;
	this->logicznyY = this->pozycja_Y / 99;
	czyMiejscaZajête[logicznyX][logicznyY] = true;
	this->klikniêty = false;
	if (this->czyGracza() && this->logicznyY == 0)
		this->zostajeKrólow¹();
	if (!this->czyGracza() && this->logicznyY == 7)
		this->zostajeKrólow¹();
	for (int i = 0; i < dostepneRuchy; i++) {
		delete[] this->ruchy[i];
		ruchy[i] = nullptr;
	}
	this->dostepneRuchy = 0;
}
