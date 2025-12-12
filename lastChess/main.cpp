#include <iostream>
#include "ChessGame.h"

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#else
#include <clocale>
#endif

void setupConsole() {
#ifdef _WIN32
    (void)_setmode(_fileno(stdout), _O_U16TEXT);
    (void)_setmode(_fileno(stdin), _O_U16TEXT);
#else
    std::setlocale(LC_ALL, "");
#endif
}

int main() {
    setupConsole();
    Chess::ChessGame game;
    game.run();
    return 0;
}