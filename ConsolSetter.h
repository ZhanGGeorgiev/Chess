#pragma once
class Kletka;

class ConsolSetter
{
public:
	static void SetConsole(const Kletka& k);
	static void setConsoleColor(int text, int bgColor);
	static void resetSettings();
};

