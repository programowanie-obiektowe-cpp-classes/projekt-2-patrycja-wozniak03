/*
main odpowiada za interakcjê z u¿ytkownikiem
pobiera wyrazenie jakie kalkulator ma obliczyæ i przekazuje je do klasy Calculator

*/


#include <iostream>
#include "Calculator.h"

int main() {
	Calculator calc; //utworzenie obiektu kalkulatora
	std::string input; //zmienna na wyra¿enie wejœciowe, rodzaj string

    std::cout << "Podaj wyrazenie: ";
    std::getline(std::cin, input);

	//obliczenie i wyœwietlenie wyniku
	//calculate to metoda klasy Calculator zdefiniowana w Calculator.cpp
    std::cout << "Wynik: " << calc.calculate(input) << "\n";
}
