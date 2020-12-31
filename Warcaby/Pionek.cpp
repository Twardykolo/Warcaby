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

bool Pionek::czyKr�lowa()
{
	return Kr�lowa;
}

void Pionek::jeste�Klikni�ty()
{
	klikni�ty = true;
}

void Pionek::nieJeste�Klikni�ty()
{
	klikni�ty = false;
}

void Pionek::rysujPionek()
{
	if (this->aktywny == false)
		return;
	ALLEGRO_BITMAP* obrazekPionkaRysowany = this->obrazekPionka;
	if(this->klikni�ty==true && this->czyKr�lowa()==false)
		obrazekPionkaRysowany = al_load_bitmap("czerwonyKliknietyPionek.png");
	if (this->klikni�ty == true && this->czyKr�lowa() == true)
		obrazekPionkaRysowany = al_load_bitmap("czerwonyKliknietyKrolowa.png");
	assert(obrazekPionkaRysowany != NULL);
	al_draw_bitmap(obrazekPionkaRysowany, this->pozycja_X+4, this->pozycja_Y+4, 0);
}

void Pionek::zostajeKr�low�()
{
	this->Kr�lowa = true;
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

void Pionek::obliczRuchy(bool czyMiejscaZaj�te[8][8],Pionek* pionki[24])
{
	// Funkcja sprawiaj�ca, �e programista rozwa�a zostanie akrobat�
	if (!this->czyAktywny())
		return;
	Pionek* zbijany = nullptr;
	logicznyX = this->pozycja_X / 99;
	logicznyY = this->pozycja_Y / 99;
	// RUCHY GRACZA LUB DAMKI
	if (this->czyGracza() || this->czyKr�lowa()) {
		// RUCH W LEWO DO G�RY
		if(logicznyX>0 && czyMiejscaZaj�te[logicznyX-1][logicznyY-1]==false && logicznyY>0){
			Ruch* nowyRuch = new Ruch({ pozycja_X - 99,pozycja_Y - 99,0 });
			for (int i = 0; i < 24; i++) {
				if (!pionki[i]->czyAktywny())
					continue;
				if (pionki[i]->pozycja_X == nowyRuch->x - 99 && pionki[i]->pozycja_Y == nowyRuch->y - 99 || pionki[i]->pozycja_X == nowyRuch->x + 99 && pionki[i]->pozycja_Y == nowyRuch->y - 99) {
					if (pionki[i]->czyGracza() != this->czyGracza() && pionki[i]->czyKr�lowa()) {
						nowyRuch->ileZbija = -1;
						break;
					}
				}
			}
			nowyRuch->wykonuj�cyRuch = this;
			this->ruchy[dostepneRuchy++] = nowyRuch;
		}
		// RUCH W PRAWO DO G�RY
		if(logicznyX<7 && czyMiejscaZaj�te[logicznyX+1][logicznyY-1]==false && logicznyY>0){
			Ruch* nowyRuch = new Ruch({ pozycja_X + 99,pozycja_Y - 99,0 });
			for (int i = 0; i < 24; i++) {
				if (!pionki[i]->czyAktywny())
					continue;
				if (pionki[i]->pozycja_X == nowyRuch->x - 99 && pionki[i]->pozycja_Y == nowyRuch->y - 99 || pionki[i]->pozycja_X == nowyRuch->x + 99 && pionki[i]->pozycja_Y == nowyRuch->y - 99) {
					if (pionki[i]->czyGracza() != this->czyGracza() && pionki[i]->czyKr�lowa()) {
						nowyRuch->ileZbija = -1;
						break;
					}
				}
			}
			nowyRuch->wykonuj�cyRuch = this;
			this->ruchy[dostepneRuchy++] = nowyRuch;
		}
		// RUCH ZBIJAJ�CY W LEWO DO G�RY
		if (logicznyX > 1 && czyMiejscaZaj�te[logicznyX - 1][logicznyY - 1] == true && czyMiejscaZaj�te[logicznyX - 2][logicznyY - 2] == false && logicznyY > 1) {
			for (int i = 0; i < 24; i++) {
				if (pionki[i]->pozycja_X == pozycja_X - 99 && pionki[i]->pozycja_Y == pozycja_Y - 99) {
					zbijany = pionki[i];
					break;
				}
			}
			if (this->czyGracza() != zbijany->czyGracza()) {
				Ruch* nowyRuch = new Ruch({ pozycja_X - 2 * 99,pozycja_Y - 2 * 99,1 ,zbijany });
				nowyRuch->wykonuj�cyRuch = this;
				// SPRAWD� CZY MO�NA ZBI� WIELOKROTNIE
				if (zbijany->logicznyY >= 3) {
					//ZDALA OD LEWEJ KRAW�DZI PLANSZY
					if (zbijany->logicznyX >= 3) {
						if (czyMiejscaZaj�te[zbijany->logicznyX - 2][zbijany->logicznyY - 2] == true && czyMiejscaZaj�te[zbijany->logicznyX - 3][zbijany->logicznyY - 3] == false || czyMiejscaZaj�te[zbijany->logicznyX][zbijany->logicznyY - 2] == true && czyMiejscaZaj�te[zbijany->logicznyX + 1][zbijany->logicznyY - 3] == false) {
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
					// BLISKO LEWEJ KRAW�DZI PLANSZY
					if (zbijany->logicznyX > 0 && zbijany->logicznyX < 3) {
						if (czyMiejscaZaj�te[zbijany->logicznyX][zbijany->logicznyY - 2] == true && czyMiejscaZaj�te[zbijany->logicznyX + 1][zbijany->logicznyY - 3] == false) {
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
		// RUCH ZBIJAJ�CY W PRAWO DO G�RY
		if (logicznyX < 6 && czyMiejscaZaj�te[logicznyX + 1][logicznyY - 1] == true && czyMiejscaZaj�te[logicznyX+2][logicznyY-2] == false && logicznyY > 1) {
			for (int i = 0; i < 24; i++) {
				if (pionki[i]->pozycja_X == pozycja_X + 99 && pionki[i]->pozycja_Y == pozycja_Y - 99) {
					zbijany = pionki[i];
					break;
				}
			}
			if (this->czyGracza() != zbijany->czyGracza()) {
				Ruch *nowyRuch = new Ruch( { pozycja_X + 2 * 99,pozycja_Y - 2 * 99,1 ,zbijany });
				nowyRuch->wykonuj�cyRuch = this;
				// SPRAWD� CZY MO�NA ZBI� WIELOKROTNIE
				if (zbijany->logicznyY >= 3) {
					//ZDALA OD PRAWEJ KRAW�DZI PLANSZY
					if (zbijany->logicznyX <= 4) {
						if (czyMiejscaZaj�te[zbijany->logicznyX + 2][zbijany->logicznyY - 2] == true && czyMiejscaZaj�te[zbijany->logicznyX + 3][zbijany->logicznyY - 3] == false || czyMiejscaZaj�te[zbijany->logicznyX][zbijany->logicznyY - 2] == true && czyMiejscaZaj�te[zbijany->logicznyX - 1][zbijany->logicznyY - 3] == false) {
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
					// BLISKO PRAWEJ KRAW�DZI PLANSZY
					if (zbijany->logicznyX < 7 && zbijany->logicznyX > 4) {
						if (czyMiejscaZaj�te[zbijany->logicznyX][zbijany->logicznyY - 2] == true && czyMiejscaZaj�te[zbijany->logicznyX - 1][zbijany->logicznyY - 3] == false) {
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
	if (!this->czyGracza() || this->czyKr�lowa()) {
		// RUCH W LEWO DO DO�U
		if (logicznyX > 0 && czyMiejscaZaj�te[logicznyX - 1][logicznyY + 1] == false && logicznyY < 7) {
			Ruch* nowyRuch = new Ruch({pozycja_X-99,pozycja_Y+99,0});
			for (int i = 0; i < 24; i++) {
				if (!pionki[i]->czyAktywny())
					continue;
				if (pionki[i]->pozycja_X == nowyRuch->x - 99 && pionki[i]->pozycja_Y == nowyRuch->y - 99 || pionki[i]->pozycja_X == nowyRuch->x + 99 && pionki[i]->pozycja_Y == nowyRuch->y - 99 || pionki[i]->czyKr�lowa() && pionki[i]->pozycja_X == nowyRuch->x - 99 && pionki[i]->pozycja_Y == nowyRuch->y + 99 || pionki[i]->czyKr�lowa() && pionki[i]->pozycja_X == nowyRuch->x + 99 && pionki[i]->pozycja_Y == nowyRuch->y + 99) {
					if (pionki[i]->czyGracza() != this->czyGracza()) {
						nowyRuch->ileZbija = -1;
						break;
					}
				}
			}
			nowyRuch->wykonuj�cyRuch = this;
			this->ruchy[dostepneRuchy++] = nowyRuch;
		}
		// RUCH W PRAWO DO DO�U
		if (logicznyX < 7 && czyMiejscaZaj�te[logicznyX + 1][logicznyY + 1] == false && logicznyY<7) {
			Ruch* nowyRuch = new Ruch({ pozycja_X + 99,pozycja_Y + 99,0 });
			for (int i = 0; i < 24; i++) {
				if (!pionki[i]->czyAktywny())
					continue;
				if (pionki[i]->pozycja_X == nowyRuch->x - 99 && pionki[i]->pozycja_Y == nowyRuch->y - 99 || pionki[i]->pozycja_X == nowyRuch->x + 99 && pionki[i]->pozycja_Y == nowyRuch->y - 99 || pionki[i]->czyKr�lowa() && pionki[i]->pozycja_X == nowyRuch->x - 99 && pionki[i]->pozycja_Y == nowyRuch->y + 99 || pionki[i]->czyKr�lowa() && pionki[i]->pozycja_X == nowyRuch->x + 99 && pionki[i]->pozycja_Y == nowyRuch->y + 99) {
					if (pionki[i]->czyGracza() != this->czyGracza()) {
						nowyRuch->ileZbija = -1;
						break;
					}
				}
			}
			nowyRuch->wykonuj�cyRuch = this;
			this->ruchy[dostepneRuchy++] = nowyRuch;
		}
		// RUCH ZBIJAJ�CY W LEWO DO DO�U
		if (logicznyX > 1 && logicznyX<=8 && czyMiejscaZaj�te[logicznyX - 1][logicznyY + 1] == true && czyMiejscaZaj�te[logicznyX - 2][logicznyY + 2] == false && logicznyY < 6) {
			for (int i = 0; i < 24; i++) {
				if (pionki[i]->pozycja_X == pozycja_X - 99 && pionki[i]->pozycja_Y == pozycja_Y + 99) {
					zbijany = pionki[i];
					break;
				}
			}
			if (this->czyGracza() != zbijany->czyGracza()) {
				Ruch* nowyRuch = new Ruch({ pozycja_X - 2 * 99,pozycja_Y + 2 * 99,1,zbijany});
				nowyRuch->wykonuj�cyRuch = this;
				// SPRAWD� CZY MO�NA ZBI� WIELOKROTNIE
				if (zbijany->logicznyY <= 4) {
					//ZDALA OD LEWEJ KRAW�DZI PLANSZY
					if (zbijany->logicznyX >= 3) {
						if (czyMiejscaZaj�te[zbijany->logicznyX - 2][zbijany->logicznyY + 2] == true && czyMiejscaZaj�te[zbijany->logicznyX - 3][zbijany->logicznyY + 3] == false || czyMiejscaZaj�te[zbijany->logicznyX][zbijany->logicznyY + 2] == true && czyMiejscaZaj�te[zbijany->logicznyX + 1][zbijany->logicznyY + 3] == false) {
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
					// BLISKO LEWEJ KRAW�DZI PLANSZY
					if (zbijany->logicznyX > 0 && zbijany->logicznyX < 3) {
						if (czyMiejscaZaj�te[zbijany->logicznyX][zbijany->logicznyY + 2] == true && czyMiejscaZaj�te[zbijany->logicznyX + 1][zbijany->logicznyY + 3] == false) {
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
		// RUCH ZBIJAJ�CY W PRAWO DO DO�U
		if (logicznyX < 6 && logicznyX >= 0 && czyMiejscaZaj�te[logicznyX + 1][logicznyY + 1] == true && czyMiejscaZaj�te[logicznyX + 2][logicznyY + 2] == false && logicznyY < 6) {
			for (int i = 0; i < 24; i++) {
				if (pionki[i]->pozycja_X == pozycja_X + 99 && pionki[i]->pozycja_Y == pozycja_Y + 99) {
					zbijany = pionki[i];
					break;
				}
			}
			if(zbijany!=nullptr)
			if (this->czyGracza() != zbijany->czyGracza()) {
				Ruch* nowyRuch = new Ruch({ pozycja_X + 2 * 99,pozycja_Y + 2 * 99, 1 ,zbijany });
				nowyRuch->wykonuj�cyRuch = this;
				// SPRAWD� CZY MO�NA ZBI� WIELOKROTNIE
				if (zbijany->logicznyY <= 4) {
					//ZDALA OD PRAWEJ KRAW�DZI PLANSZY
					if (zbijany->logicznyX <= 4) {
						if (czyMiejscaZaj�te[zbijany->logicznyX + 2][zbijany->logicznyY + 2] == true && czyMiejscaZaj�te[zbijany->logicznyX + 3][zbijany->logicznyY + 3] == false || czyMiejscaZaj�te[zbijany->logicznyX][zbijany->logicznyY + 2] == true && czyMiejscaZaj�te[zbijany->logicznyX - 1][zbijany->logicznyY + 3] == false) {
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
					// BLISKO PRAWEJ KRAW�DZI PLANSZY
					if (zbijany->logicznyX < 7 && zbijany->logicznyX > 4) {
						if (czyMiejscaZaj�te[zbijany->logicznyX][zbijany->logicznyY + 2] == true && czyMiejscaZaj�te[zbijany->logicznyX - 1][zbijany->logicznyY + 3] == false) {
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

void Pionek::zostajeZbity(bool czyMiejscaZaj�te[8][8])
{
	if (this == nullptr)
		return;
	this->aktywny = false;
	czyMiejscaZaj�te[this->logicznyX][this->logicznyY] = false;
	this->pozycja_X = -200;
	this->pozycja_Y = -200;
	this->logicznyX = -15;
	this->logicznyY = -15;
}

void Pionek::ruch(Ruch *ruch,bool czyMiejscaZaj�te[8][8])
{
	czyMiejscaZaj�te[logicznyX][logicznyY] = false;
	this->pozycja_X = ruch->x;
	this->pozycja_Y = ruch->y;
	this->logicznyX = this->pozycja_X / 99;
	this->logicznyY = this->pozycja_Y / 99;
	czyMiejscaZaj�te[logicznyX][logicznyY] = true;
	this->klikni�ty = false;
	if (this->czyGracza() && this->logicznyY == 0)
		this->zostajeKr�low�();
	if (!this->czyGracza() && this->logicznyY == 7)
		this->zostajeKr�low�();
	for (int i = 0; i < dostepneRuchy; i++) {
		delete[] this->ruchy[i];
		ruchy[i] = nullptr;
	}
	this->dostepneRuchy = 0;
}
