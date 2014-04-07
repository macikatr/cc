#include "Board.h"
#include <iostream>
#include <stack>
#include "ImageProc.h"
int SCREENWIDTH=0;
int SCREENHEIGHT=0;
#define QUARTERCARDWIDTH 6
using namespace std;

struct vec4
{
	vec4(int RR, int GG, int BB, int AA)
	{
		R=RR;
		G=GG;
		B=BB;
		A=AA;
	}
	vec4()
	{}
	int R;
	int G;
	int B;
	int A;
};

bool operator== ( vec4 a, vec4 b)
{
	return a.A==b.A && a.B==b.B && a.G==b.G && a.R==b.R;
}
bool operator == (const Card A, const Card B)
{
	return A.msuit == B.msuit && A.mvalue == B.mvalue;
}
// A-> A+B
void mergeCoordSet(set<COORD> &A, set<COORD> &B)
{
	auto itr = B.begin();
	while (itr != B.end())
	{
		A.insert(*itr);
		itr++;
	}
}

void Board::joinElementSets(set<CardElement> Suits, set<CardElement> Values, bool dark)
{
	/* 
		1.) Find where the Y values are clustered 
		2.) Throw out Elements that are too far away from it to be used.
		3.) Build the Player Hand set out of light colored ones
		4.) Build the last hand out of the dark ones
	*/


	if (!dark)
	{
		//Throw away live cards less than 400Y from anchors
		auto itrS = Suits.begin();
		while (itrS != Suits.end())
		{
			if (abs((itrS->Location.Y - anchY)) < 400)
			{
				Suits.erase(itrS);
				itrS = Suits.begin();
			}
			else
			{
				itrS++;
			}
		}
		//Values
		auto itrV = Values.begin();
		while (itrV != Values.end())
		{
			if (abs((itrV->Location.Y - anchY)) < 400)
			{
				Values.erase(itrV);
				itrV = Values.begin();
			}
			else
			{
				itrV++;
			}
		}

		//Join the reduced sets
		itrS = Suits.begin();
		itrV = Values.begin();

		while (itrS != Suits.end() && itrV != Values.end())
		{
			if (abs(itrS->Location.X - itrV->Location.X) <= QUARTERCARDWIDTH && abs(itrS->Location.Y - itrV->Location.Y)<25)
			{
				PlayerHand.insert(Card(itrV->value, itrS->value));
				Suits.erase(itrS);
				Values.erase(itrV);
				itrS = Suits.begin();
				itrV = Values.begin();
			}
			else
			{
				itrV++;
				if (itrV == Values.end())
				{
					itrS++;
					if (itrS == Suits.end())
					{
						return;
					}
					else
					{
						itrV = Values.begin();
					}
				}
			}
		}

	}
	else // we are filling in used cards
	{
		if (Suits.size() <4)
		{
			for (int k = 0; k < 4; k++)
			{
				LastHand[k].msuit = -1;
				LastHand[k].mvalue = -1;
			}
		}
		else
		{
			int insertIndex = 0;
			auto itrS = Suits.begin();
			auto itrV = Values.begin();
			while (itrS != Suits.end() && itrV != Values.end())
			{
				if (abs(itrS->Location.X - itrV->Location.X) <= QUARTERCARDWIDTH)
				{
					LastHand[insertIndex].msuit = itrS->value;
					LastHand[insertIndex].mvalue = itrV->value;
					insertIndex++;
					Suits.erase(itrS);
					Values.erase(itrV);
					itrS = Suits.begin();
					itrV = Values.begin();
				}
				else
				{
					itrV++;
					if (itrV == Values.end())
					{
						itrS++;
						if (itrS == Suits.end())
						{
							return;
						}
						else
						{
							itrV = Values.begin();
						}
					}
				}
			}
		}
	}
}


bool operator < (const CardElement A, const CardElement B)
{
	return A.Location < B.Location;
}
// A->B
void CoordSetToElementSet(std::set<COORD> &A, std::set<CardElement> &B, int value, bool suited)
{
	auto itr = A.begin();
	while (itr != A.end())
	{
		B.insert(CardElement(*itr, suited, value));
		itr++;
	}
}
bool operator < (const Card A, const Card B)
{
	if (A.msuit < B.msuit)
	{
		return true;
	}
	else if (A.msuit > B.msuit)
	{
		return false;
	}
	else
	{
		//same suit, check values
		if (A.mvalue < B.mvalue)
		{
			return true;
		}
		else if (A.mvalue > B.mvalue)
		{
			return false;
		}
		else
		{
			// This should never run
			return false;
		}
	}
}

bool operator < (const COORD A, const COORD B)
{
	if (A.Y != B.Y)
	{
		return A.Y < B.Y;
	}
	return A.X < B.X;
}
// This function does the image processing
Board * processScreenShot(BYTE* bmpBuffer, int size)
{
	Board * ret=NULL;
	int k=0;
	for(; k< size-4; k+=4)
	{
		if(bmpBuffer[k]==0x3d && bmpBuffer[k+1]==0x4a && bmpBuffer[k+2]==0xaa)
		{
			bool foundSF=true;
			for(int j=1; j<5; j++)
			{
				switch( j )
				{
				case 1:
					{
						if(bmpBuffer[k+j*4]!=0x17 || bmpBuffer[k+j*4+1]!=0x20 || bmpBuffer[k+j*4+2]!=0xB0)
						{
							foundSF=false;
						}
						break;
					}
				case 2:
					{
						if(bmpBuffer[k+j*4]!=0x07 || bmpBuffer[k+j*4+1]!=0x07 || bmpBuffer[k+j*4+2]!=0xB8)
						{
							foundSF=false;
						}
						break;
					}
				case 3:
					{
						if(bmpBuffer[k+j*4]!=0x26 || bmpBuffer[k+j*4+1]!=0x32 || bmpBuffer[k+j*4+2]!=0xBf)
						{
							foundSF=false;
						}
						break;
					}
				case 4:
					{
						if(bmpBuffer[k+j*4]!=0x4c || bmpBuffer[k+j*4+1]!=0x5B || bmpBuffer[k+j*4+2]!=0xBc)
						{
							foundSF=false;
						}
						break;
					}
				}
			}
			if(foundSF)
			{
				ret = new Board;
				ret->anchX=(k/4)-((k/4)/SCREENWIDTH)*SCREENWIDTH+2;
				ret->anchY=(k/4)/SCREENWIDTH;
				break;
			}
		}
	}
	if(ret)
	{ //if we found the anchor point, look for the other pieces
		//			Anchor +- offset + pixelSelector
		// b=block, s=sword, f=fused, d=drop(known sprinkle)
	
	}
	
	return ret;
}
// This function takes a screenshot then passes to an image processor
Board *  createBoard()
{
	//get client DC
	HDC desktophandle= GetDC(NULL);
	//create memory DC
	HDC memHDC= CreateCompatibleDC(desktophandle);
	//create new HBITMAP compatible with client dc
	HBITMAP Teemo= CreateCompatibleBitmap(desktophandle,SCREENWIDTH,SCREENHEIGHT);
	//select bitmap into memory DC
	SelectObject(memHDC, Teemo);
	//copy from client DC to memory DC
	BitBlt(memHDC,0,0,SCREENWIDTH,SCREENHEIGHT,desktophandle,0,0,SRCCOPY);
	// GetBitmapBits
	long numBits=SCREENWIDTH*SCREENHEIGHT*4;
	LPVOID lpvBITBUF= new LPVOID;
	BYTE* bmpBuffer=(BYTE*)GlobalAlloc(GPTR, SCREENWIDTH*SCREENHEIGHT*4);
	GetBitmapBits(Teemo,numBits,bmpBuffer);
//	SaveToFile(Teemo, L"images\\Teemo.bmp");
	Board * ret =processScreenShot(bmpBuffer,SCREENWIDTH*SCREENHEIGHT*4);
	ReleaseDC(NULL,desktophandle);
	DeleteDC(memHDC);
	DeleteObject(Teemo);
	delete lpvBITBUF;
	GlobalFree(bmpBuffer);
	return ret;
}



