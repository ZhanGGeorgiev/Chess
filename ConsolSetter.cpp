#include "ConsolSetter.h"
#include "Kletka.h"

void readColors(int& text, int& bgColor, const Kletka& k, const PieceColor& color)
{
	if (k.getColor() == KletkaColor::white)
	{
		bgColor = 7;
	}

	if (k.getColor() == KletkaColor::black)
	{
		bgColor = 8;
	}

	if (color == PieceColor::white)
	{
		text = 15;
	}

	if (color == PieceColor::black)
	{
		text = 0;
	}
}

void ConsolSetter::setConsoleColor(int text, int bgColor)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, text + (bgColor << 4));
}

 void ConsolSetter::SetConsole(const Kletka& k)
{
	 wchar_t symbol = k.getSymbol();
	 PieceColor color = k.getPieceColor();

	 int text = 0;
	 int bgColor = 0;

	 readColors(text, bgColor, k, color);
	 setConsoleColor(text, bgColor);
}

void ConsolSetter::resetSettings()
{
	setConsoleColor(7, 0);
}