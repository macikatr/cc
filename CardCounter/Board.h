#pragma once

#include <fstream>
#include <iostream>
#include <set>
#include <Windows.h>
#include <algorithm>
#include "ImageProc.h"
#define CARDHEIGHT 80
#define CARDWIDTH 60
extern int SCREENWIDTH;
extern int SCREENHEIGHT;
#define HEARTS 0
#define DIAMONDS 1
#define CLUBS 2
#define SPADES 3


struct Card
{
	Card(int value=0, int suit=0)
	{
		msuit = suit;
		mvalue = value;
	}

	int mvalue;
	int msuit;
};
struct CardElement
{
	CardElement(COORD loc, bool suited = false, int val = -1)
	{
		isSuit = suited;
		value = val;
		Location.X = loc.X;
		Location.Y = loc.Y;
	}
	COORD Location;
	int value;
	bool isSuit;	
};

void CoordSetToElementSet(std::set<COORD> &A, std::set<CardElement> &B, int value, bool suited);


extern bool operator < (const Card A, const Card B);
extern bool operator == (const Card A, const Card B);
extern bool operator < (const COORD A, const COORD B);
extern void mergeCoordSet(std::set<COORD> &A, std::set<COORD> &B);


class Board
{
public:
	Board(bool output=true)
	{
		for (int s = 0; s < 4; s++)
		{//suit
			for (int v = 2; v <= 14; v++) //234567890jqka
			{//value
				CardsInPlay.insert(Card(v, s));
			}
		}
		//Now look for a board
		Update(output);
	}

	void joinElementSets(std::set<CardElement> Suits, std::set<CardElement> Values, bool dark);
	//screen Anchor point: Top Left of board
	int anchX;
	int anchY;

	//Actual Board guts
	std::set <Card> PlayerHand; //this gets constructed each time we update the board
	std::set <Card> CardsInPlay; //this gets constructed at the beginning and gradually destructed
	Card LastHand[4];			// this gets rebuilt each update

	//Update looks for a screenshot containing a board, and fills in the guts, but doesn't do initialization
	void Update(bool output=true)
	{
		//Destruct last time's board's guts
		PlayerHand.clear();
		for (int k = 0; k < 4; k++)
		{
			LastHand[k].msuit = -1;
			LastHand[k].mvalue = -1;
		}
		//look for a board
		anchX = -1;
		anchY = -1;
		int foundHearts = 0;
		int foundSpades = 0;
		curShot = new ScreenShot;
		while (foundHearts != 1 && foundSpades != 1)
		{

			std::set<COORD> numHFound = curShot->FindBMP(L"images\\logohearts.bmp");
			std::set<COORD> numSpadesFound = curShot->FindBMP(L"images\\logospades.bmp");

			foundSpades = numSpadesFound.size();
			foundHearts = numHFound.size();

			if (numHFound.size() == 1 || numSpadesFound.size() == 1)
			{
				if (numHFound.size() == 1)
				{
					anchX = numHFound.begin()->X;
					anchY = numHFound.begin()->Y;
					if (output){ std::cout << "HEARTS!!!" << std::endl; }
				}
				else
				{
					anchX = numSpadesFound.begin()->X;
					anchY = numSpadesFound.begin()->Y;
					if (output){ std::cout << "SPADES!!!" << std::endl; }
				}

			}
			else
			{
				delete curShot;
				curShot = new ScreenShot;
			}
		}

		// All Light colored Card Elements go in here to be processed
		std::set<CardElement> unusedCardSuits;
		std::set<CardElement> unusedCardValues;

		// p= player h=hearts c=clubs d=diamonds s=spades
		// The hearts suit is a dumb
		auto ph = curShot->FindBMP(L"images\\uhearts.bmp");
		auto ph2 = curShot->FindBMP(L"images\\uhearts2.bmp");
		mergeCoordSet(ph, ph2);
		//diamonds is a dumb too
		auto pd = curShot->FindBMP(L"images\\udiamonds.bmp");
		auto pd2 = curShot->FindBMP(L"images\\udiamonds2.bmp");
		mergeCoordSet(pd, pd2);
		auto pc = curShot->FindBMP(L"images\\uclubs.bmp");
		auto ps = curShot->FindBMP(L"images\\uspades.bmp");

		CoordSetToElementSet(ph, unusedCardSuits, HEARTS, true);
		CoordSetToElementSet(pd, unusedCardSuits, DIAMONDS, true);
		CoordSetToElementSet(pc, unusedCardSuits, CLUBS, true);
		CoordSetToElementSet(ps, unusedCardSuits, SPADES, true);

		//Numbers/Letters - light colored
		//black
		auto pb2 = curShot->FindBMP(L"images\\2b.bmp");
		auto pb3 = curShot->FindBMP(L"images\\3b.bmp");
		auto pb4 = curShot->FindBMP(L"images\\4b.bmp");
		auto pb5 = curShot->FindBMP(L"images\\5b.bmp");
		auto pb6 = curShot->FindBMP(L"images\\6b.bmp");
		auto pb7 = curShot->FindBMP(L"images\\7b.bmp");
		auto pb8 = curShot->FindBMP(L"images\\8b.bmp");
		auto pb9 = curShot->FindBMP(L"images\\9b.bmp");
		auto pb10 = curShot->FindBMP(L"images\\10b.bmp");
		auto pbj = curShot->FindBMP(L"images\\jb.bmp");
		auto pbq = curShot->FindBMP(L"images\\qb.bmp");
		auto pbk = curShot->FindBMP(L"images\\kb.bmp");
		auto pba = curShot->FindBMP(L"images\\ab.bmp");
		//red
		auto pr2 = curShot->FindBMP(L"images\\2r.bmp");
		auto pr3 = curShot->FindBMP(L"images\\3r.bmp");
		auto pr4 = curShot->FindBMP(L"images\\4r.bmp");
		auto pr5 = curShot->FindBMP(L"images\\5r.bmp");
		auto pr6 = curShot->FindBMP(L"images\\6r.bmp");
		auto pr7 = curShot->FindBMP(L"images\\7r.bmp");
		auto pr8 = curShot->FindBMP(L"images\\8r.bmp");
		auto pr9 = curShot->FindBMP(L"images\\9r.bmp");
		auto pr10 = curShot->FindBMP(L"images\\10r.bmp");
		auto prj = curShot->FindBMP(L"images\\jr.bmp");
		//Q is dumb
		auto prq = curShot->FindBMP(L"images\\qr.bmp");
		auto prq2 = curShot->FindBMP(L"images\\qr2.bmp");
		mergeCoordSet(prq, prq2);
		auto prk = curShot->FindBMP(L"images\\kr.bmp");
		auto pra = curShot->FindBMP(L"images\\ar.bmp");

		CoordSetToElementSet(pb2, unusedCardValues, 2, false);
		CoordSetToElementSet(pb3, unusedCardValues, 3, false);
		CoordSetToElementSet(pb4, unusedCardValues, 4, false);
		CoordSetToElementSet(pb5, unusedCardValues, 5, false);
		CoordSetToElementSet(pb6, unusedCardValues, 6, false);
		CoordSetToElementSet(pb7, unusedCardValues, 7, false);
		CoordSetToElementSet(pb8, unusedCardValues, 8, false);
		CoordSetToElementSet(pb9, unusedCardValues, 9, false);
		CoordSetToElementSet(pb10, unusedCardValues, 10, false);
		CoordSetToElementSet(pbj, unusedCardValues, 11, false);
		CoordSetToElementSet(pbq, unusedCardValues, 12, false);
		CoordSetToElementSet(pbk, unusedCardValues, 13, false);
		CoordSetToElementSet(pba, unusedCardValues, 14, false);
		//Reds
		CoordSetToElementSet(pr2, unusedCardValues, 2, false);
		CoordSetToElementSet(pr3, unusedCardValues, 3, false);
		CoordSetToElementSet(pr4, unusedCardValues, 4, false);
		CoordSetToElementSet(pr5, unusedCardValues, 5, false);
		CoordSetToElementSet(pr6, unusedCardValues, 6, false);
		CoordSetToElementSet(pr7, unusedCardValues, 7, false);
		CoordSetToElementSet(pr8, unusedCardValues, 8, false);
		CoordSetToElementSet(pr9, unusedCardValues, 9, false);
		CoordSetToElementSet(pr10, unusedCardValues, 10, false);
		CoordSetToElementSet(prj, unusedCardValues, 11, false);
		CoordSetToElementSet(prq, unusedCardValues, 12, false);
		CoordSetToElementSet(prk, unusedCardValues, 13, false);
		CoordSetToElementSet(pra, unusedCardValues, 14, false);


		// Suits (dark=played last hand)
		// All DARK colored Card Elements go in here to be processed
		std::set<CardElement> usedCardSuits;
		std::set<CardElement> usedCardValues;

		// p= player h=hearts c=clubs d=diamonds s=spades
		// The hearts suit is a dumb
		auto pdh = curShot->FindBMP(L"images\\dhearts.bmp");
		auto pdd = curShot->FindBMP(L"images\\ddiamonds.bmp");
		auto pdc = curShot->FindBMP(L"images\\dclubs.bmp");
		auto pds = curShot->FindBMP(L"images\\dspades.bmp");

		CoordSetToElementSet(pdh, usedCardSuits, HEARTS, true);
		CoordSetToElementSet(pdd, usedCardSuits, DIAMONDS, true);
		CoordSetToElementSet(pdc, usedCardSuits, CLUBS, true);
		CoordSetToElementSet(pds, usedCardSuits, SPADES, true);

		//Numbers and Letters : Dark (already played)
		//black
		auto pdb2 = curShot->FindBMP(L"images\\d2b.bmp");
		auto pdb3 = curShot->FindBMP(L"images\\d3b.bmp");
		auto pdb4 = curShot->FindBMP(L"images\\d4b.bmp");
		auto pdb5 = curShot->FindBMP(L"images\\d5b.bmp");
		auto pdb6 = curShot->FindBMP(L"images\\d6b.bmp");
		auto pdb7 = curShot->FindBMP(L"images\\d7b.bmp");
		auto pdb8 = curShot->FindBMP(L"images\\d8b.bmp");
		auto pdb9 = curShot->FindBMP(L"images\\d9b.bmp");
		auto pdb10 = curShot->FindBMP(L"images\\d10b.bmp");
		auto pdbj = curShot->FindBMP(L"images\\djb.bmp");
		auto pdbq = curShot->FindBMP(L"images\\dqb.bmp");
		auto pdbk = curShot->FindBMP(L"images\\dkb.bmp");
		auto pdba = curShot->FindBMP(L"images\\dab.bmp");
		//red
		auto pdr2 = curShot->FindBMP(L"images\\d2r.bmp");
		auto pdr3 = curShot->FindBMP(L"images\\d3r.bmp");
		auto pdr4 = curShot->FindBMP(L"images\\d4r.bmp");
		auto pdr5 = curShot->FindBMP(L"images\\d5r.bmp");
		auto pdr6 = curShot->FindBMP(L"images\\d6r.bmp");
		auto pdr7 = curShot->FindBMP(L"images\\d7r.bmp");
		auto pdr8 = curShot->FindBMP(L"images\\d8r.bmp");
		auto pdr9 = curShot->FindBMP(L"images\\d9r.bmp");
		auto pdr10 = curShot->FindBMP(L"images\\d10r.bmp");
		auto pdrj = curShot->FindBMP(L"images\\djr.bmp");
		auto pdrq = curShot->FindBMP(L"images\\dqr.bmp");
		auto pdrk = curShot->FindBMP(L"images\\dkr.bmp");
		auto pdra = curShot->FindBMP(L"images\\dar.bmp");

		//put them in the list
		CoordSetToElementSet(pdb2, usedCardValues, 2, false);
		CoordSetToElementSet(pdb3, usedCardValues, 3, false);
		CoordSetToElementSet(pdb4, usedCardValues, 4, false);
		CoordSetToElementSet(pdb5, usedCardValues, 5, false);
		CoordSetToElementSet(pdb6, usedCardValues, 6, false);
		CoordSetToElementSet(pdb7, usedCardValues, 7, false);
		CoordSetToElementSet(pdb8, usedCardValues, 8, false);
		CoordSetToElementSet(pdb9, usedCardValues, 9, false);
		CoordSetToElementSet(pdb10, usedCardValues, 10, false);
		CoordSetToElementSet(pdbj, usedCardValues, 11, false);
		CoordSetToElementSet(pdbq, usedCardValues, 12, false);
		CoordSetToElementSet(pdbk, usedCardValues, 13, false);
		CoordSetToElementSet(pdba, usedCardValues, 14, false);
		//Reds
		CoordSetToElementSet(pdr2, usedCardValues, 2, false);
		CoordSetToElementSet(pdr3, usedCardValues, 3, false);
		CoordSetToElementSet(pdr4, usedCardValues, 4, false);
		CoordSetToElementSet(pdr5, usedCardValues, 5, false);
		CoordSetToElementSet(pdr6, usedCardValues, 6, false);
		CoordSetToElementSet(pdr7, usedCardValues, 7, false);
		CoordSetToElementSet(pdr8, usedCardValues, 8, false);
		CoordSetToElementSet(pdr9, usedCardValues, 9, false);
		CoordSetToElementSet(pdr10, usedCardValues, 10, false);
		CoordSetToElementSet(pdrj, usedCardValues, 11, false);
		CoordSetToElementSet(pdrq, usedCardValues, 12, false);
		CoordSetToElementSet(pdrk, usedCardValues, 13, false);
		CoordSetToElementSet(pdra, usedCardValues, 14, false);

		//Join them together
		joinElementSets(unusedCardSuits, unusedCardValues, false);
		joinElementSets(usedCardSuits, usedCardValues, true);

		//Remove Last Hand from in play cards if not already gone
		for (int k = 0; k < 4; k++)
		{
			if (LastHand[k].msuit != -1)
			{
				auto delThis = std::find(CardsInPlay.begin(), CardsInPlay.end(), LastHand[k]);
				if (delThis != CardsInPlay.end())
				{
					CardsInPlay.erase(delThis);
				}
			}
		}
		for (auto itr = PlayerHand.begin(); itr != PlayerHand.end(); itr++)
		{
			Card toDel;
			toDel.msuit = itr->msuit;
			toDel.mvalue = itr->mvalue;
			auto delThis = std::find(CardsInPlay.begin(), CardsInPlay.end(), toDel);
			if (delThis != CardsInPlay.end())
			{
				CardsInPlay.erase(delThis);
			}
		}

		// Now build something to show
		char CardMatrix[4][13];
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 13; j++)
			{
				CardMatrix[i][j] = ' ';
			}
		}
		for (auto itr = CardsInPlay.begin(); itr != CardsInPlay.end(); itr++)
		{
			CardMatrix[itr->msuit][itr->mvalue-2] = 'X';
		}
		for (auto itr = PlayerHand.begin(); itr != PlayerHand.end(); itr++)
		{
			CardMatrix[itr->msuit][itr->mvalue-2] = 'O';
		}
		if (output)
		{
			std::cout << "Suit:    2   3   4   5   6   7   8   9   10  J   Q   K   A" << std::endl;
			std::cout << "-----------------------------------------------------------" << std::endl;
			std::cout << "\nSpades   ";

			for (int k = 0; k < 13; k++)
			{
				std::cout << CardMatrix[SPADES][k] << "   ";
			}
			std::cout << std::endl;
			std::cout << std::endl << "-----------------------------------------------------------" << std::endl;
			std::cout << std::endl;
			std::cout << "Hearts   ";

			for (int k = 0; k < 13; k++)
			{
				std::cout << CardMatrix[HEARTS][k] << "   ";
			}
			std::cout << std::endl << "-----------------------------------------------------------" << std::endl;
			std::cout << std::endl;
			std::cout << "Clubs    ";
			for (int k = 0; k < 13; k++)
			{
				std::cout << CardMatrix[CLUBS][k] << "   ";
			}
			std::cout << std::endl;
			std::cout << std::endl << "-----------------------------------------------------------" << std::endl;
			std::cout << std::endl;
			std::cout << "Diamonds ";
			for (int k = 0; k < 13; k++)
			{
				std::cout << CardMatrix[DIAMONDS][k] << "   ";
			}
			std::cout << std::endl;
			std::cout << std::endl << "-----------------------------------------------------------" << std::endl;
			std::cout << std::endl;
		}
	}

private:
	ScreenShot * curShot;
};
// Nonparallel functions
Board *  createBoard();
Board * processScreenShot(BYTE* bmpBuffer, int size);

