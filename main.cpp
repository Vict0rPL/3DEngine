/**
 * @file main.cpp
 * @brief Punkt startowy aplikacji uruchamiającej silnik 3D.
 *
 * Plik zawiera funkcję main, która tworzy instancję klasy Engine,
 * konfiguruje jej podstawowe parametry, inicjalizuje i uruchamia główną pętlę aplikacji.
 */

#include "Engine.h"

 /**
  * @brief Główna funkcja programu.
  *
  * Tworzy obiekt klasy Engine, ustawia kolor czyszczenia ekranu, inicjalizuje
  * silnik, uruchamia pętlę zdarzeń oraz sprząta zasoby po zakończeniu działania.
  *
  * @param argc Liczba argumentów przekazanych do programu.
  * @param argv Tablica argumentów przekazanych do programu.
  * @return int Kod zakończenia programu (0 oznacza sukces).
  */
int main(int argc, char** argv) {
    Engine engine(argc, argv);
    engine.SetClearColor(0.1f, 0.1f, 0.15f);
    engine.Init();
    engine.Run();
    engine.Cleanup();
    return 0;
}