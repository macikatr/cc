#include <iostream>
#include <time.h>
//#include "MouseMovement.h"
#include <Windows.h>
#include "wtypes.h"
#include <vector>
#include <fstream>
#include <string>
#include <cassert>
#include "ImageProc.h"
#include "Board.h"
using namespace std;

void colorPickerLoop()
{
	POINT * curserpos= new POINT();
	while(true)
	{
		HDC myHDC= GetDC(NULL);
		GetCursorPos(curserpos);
		COLORREF TURTLE = GetPixel( myHDC, curserpos->x,curserpos->y);
		cout<<hex<< TURTLE<<endl;
	}
}
void getpixelLots()
{
	cout<<"starting"<<endl;
	clock_t tStart = clock();
	for(int k=0; k<700; k++)
	{
		HDC myHDC= GetDC(NULL);
		COLORREF TURTLE = GetPixel( myHDC, 0,0);
	}
	printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
	cout<<"ending"<<endl;
}
bool operator!=(Card A, Card B)
{
	return !(A == B);
}
bool operator==(Board A, Board B)
{ // Don't need to check if all the cards are the same, only the player's hand and lasthand
	//bool sameHand = (A.PlayerHand == B.PlayerHand);
	bool sameHand = true;
	if (A.PlayerHand.size() != B.PlayerHand.size())
	{
		sameHand = false;
	}
	else
	{
		sameHand = (equal(A.PlayerHand.begin(), A.PlayerHand.end(), B.PlayerHand.begin()));
	}
//	bool sameTrick = (A.LastHand == B.LastHand);
	bool sameTrick = true;
	for (int k = 0; k < 4; k++)
	{
		if (A.LastHand[k] != B.LastHand[k])
		{
			sameTrick = false;
		}
	}
	return (sameHand && sameTrick);
}

void GetDesktopResolution(int& horizontal, int& vertical)
{
   RECT desktop;
   // Get a handle to the desktop window
   const HWND hDesktop = GetDesktopWindow();
   // Get the size of screen to the variable desktop
   GetWindowRect(hDesktop, &desktop);
   // The top left corner will have coordinates (0,0)
   // and the bottom right corner will have coordinates
   // (horizontal, vertical)
   horizontal = desktop.right;
   vertical = desktop.bottom;
}
void BOT()
{
	cout << "Press Enter to start" << endl;
	if (!SCREENHEIGHT || !SCREENWIDTH)
	{
		GetDesktopResolution(SCREENHEIGHT, SCREENWIDTH);
	}
	string g;
	getline(cin, g);
	cout << "Searching for Board" << endl;
	auto gameStatus = Board();
	while (true)
	{
		if (gameStatus.PlayerHand.size() == 0 && gameStatus.CardsInPlay.size() <= 4)
		{
			cout << "Hand might be over. Reset (press r and ENTER)?" << endl;
			getline(cin, g);
			if (g.size()!=0 && tolower(g[0]) == 'r')
			{ // reset
				cout << "Resetting Board...";
				gameStatus = Board();
				Sleep(10);
			}
		}
		auto nextStatus = Board(false);
		if (nextStatus == gameStatus)
		{
			Sleep(100);
		}
		else
		{
			system("cls");
			gameStatus.Update();
		}
	}
}
int main()
{
	BOT();
}