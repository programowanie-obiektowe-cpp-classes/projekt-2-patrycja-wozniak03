//tutaj caly mechanizm liczenia kalkulatora

/*
 
  analizuje wyra¿enie matematyczne zapisane jako tekst
  i oblicza jego wartoœæ z uwzglêdnieniem nawiasów oraz
  priorytetów operatorów , czyli mnozenie i dzielenie maja priorytet przed np. dodawaniem.
 
  Do obliczeñ wykorzystywane s¹ dwa stosy (std::stack):
  stos liczb-przechowuje odczytane wartoœci liczbowe
  stos operatorów-przechowuje operatory i nawiasy
 
  Zastosowanie stosów (LIFO) pozwala w prosty sposób:
  - zachowaæ poprawn¹ kolejnoœæ wykonywania dzia³añ,
  - obs³u¿yæ priorytety operatorów
  - poprawnie przetwarzaæ nawiasy
 
 Rozwi¹zanie jest inspirowane klasycznym algorytmem
 obliczania wyra¿eñ arytmetycznych z u¿yciem stosów

*/

#include "Calculator.h"
#include "Operation.h"
#include <stack>  //dla stosów
#include <cctype>  //dla funkcji isspace i isdigit
#include <cmath>
#include <string>
#include <stdexcept>



//deklaracja funkcji tworz¹cej operacjê na podstawie znaku
//zdefiniowana w Operation.cpp, dziedziczy po IOperation
IOperation* createOperation(char);

double Calculator::calculate(const std::string& expr) {

	//stos=lifo (last in, first out), zeby obsluzyc kolejnoœæ dzia³añ, nawiasy itd
	std::stack<double> values; //stos na wartoœci liczbowe
	std::stack<char> ops;   //stos na operatory

	bool expectUnary = true; // flaga oczekiwania na operator jednoargumentowy (do liczb ujemnych)

	//funkcja ktora zdejmuje operator ze stosu i liczy wartosci
   
    auto apply = [&]() {

		//sprawdzenie czy sa dwie wartosci na stosie, ma to na celu unikniecie bledow (proba wykonania operacji bez wystarczajacej liczby zmiennych)
        if (values.size() < 2) {
            throw std::runtime_error("B³¹d: za ma³o wartoœci na stosie do operacji");
        }

		//pobranie dwóch wartoœci i operatora 
        double b = values.top(); values.pop();
        double a = values.top(); values.pop();
        char op = ops.top(); ops.pop();

        //wyjatek
        //zapobieganie dzielenia przez zero przed utworzeniem obiektu operacji
        if (op == '/' && b == 0) {
            throw std::runtime_error("B³¹d: dzielenie przez zero!");
        }


		//polimorfizm, tworzy odpowiedni¹ operacjê 
        IOperation* operation = createOperation(op);

		//wyjatek na wypadek nieznanego operatora 
        if (!operation) throw std::runtime_error("B³¹d: nieznany operator");

		values.push(operation->apply(a, b));  //wykonanie operacji i odlozenie wyniku na stos wartoœci

        };


	//petla i przejscie przez wyra¿enie znak po znaku
    for (size_t i = 0; i < expr.size(); ++i) {
		if (std::isspace(expr[i])) continue;  //pominiecie spacji

        //wersja liczaca tylko na liczbach calkowuitych:
		//jak cyfra to zaczytanie ca³ej liczby
      /*  if (std::isdigit(expr[i])) {
            double val = 0;
			//dziala tak ze zczytuje kolejne cyfry az do napotkania nie-cyfry
            while (i < expr.size() && std::isdigit(expr[i])) {
                val = val * 10 + (expr[i] - '0');
                i++;
            }
            
            
			i--;  //cofniecie o jeden znak bo petla for go zwiekszy

			values.push(val);  //odlozenie wartosci na stos
        }
        */




		//wersja ktora obsluguje liczby zmiennoprzecinkowe:


        // Obs³uga minusa jednoargumentowego
        bool negative = false;

        // jeœli minus i spodziewamy siê liczby/unary minus
        if (expr[i] == '-' && expectUnary) {
            negative = true;
            i++; // przeskocz minus
            while (i < expr.size() && std::isspace(expr[i])) i++; // przeskocz spacje
        }



        // odczytanie liczby ca³kowitej lub zmiennoprzecinkowej
        if (std::isdigit(expr[i]) || expr[i] == '.') {
            std::string number;
            bool dotUsed = false;

			//dziala tak ze zczytuje kolejne cyfry i kropke az do napotkania nie-cyfry lub drugiej kropki
            while (i < expr.size() &&
                (std::isdigit(expr[i]) || expr[i] == '.')) {

                if (expr[i] == '.') {
                    if (dotUsed) break;   // druga kropka koñczy liczbê
                    dotUsed = true;
                }

                number += expr[i];
                i++;
            }

            i--; // cofniêcie indeksu, bo pêtla for te¿ go zwiêkszy

            if (number.empty()) {
                throw std::runtime_error("B³¹d: liczba nie zosta³a odczytana");
            }

			double value = std::stod(number); //zamiana stringa na double
            if (negative) value = -value; // zastosowanie minusa jednoargumentowego

            
            values.push(value);
            expectUnary = false;  // po liczbie nie spodziewamy siê unary minus
        }


		//jak nawias lewy to odlozenie go na stos operatorów
        else if (expr[i] == '(') {
            ops.push(expr[i]);
			expectUnary = true;  // po '(' spodziewamy siê liczby/unary minus
        }

		//jak nawias prawy to liczenie az do lewego nawiasu
        else if (expr[i] == ')') {
            while (ops.top() != '(')
                apply();
			ops.pop();   //usuniecie odlozonego ( ze stosu
			expectUnary = false;  // po ')' nie spodziewamy siê unary minus
        }

        //jesli operator
        else if (std::string("+-*/").find(expr[i]) != std::string::npos) {
            IOperation* current = createOperation(expr[i]);


            //sprawdzenie priorytetu
            while (!ops.empty() && ops.top() != '(') {
                IOperation* prev = createOperation(ops.top());
                if (prev && prev->precedence() >= current->precedence()) {
                    if (values.size() < 2) break; //dodajemy sprawdzenie
                    apply();
                }
                else break;
            }
            ops.push(expr[i]);
			expectUnary = true;  // po operatorze spodziewam siê liczby/unary minus
        }
    }


	//na koniec wykonanie pozostalych operacji, czyli jak cos zostalo na stosie operatorów
    while (!ops.empty()) {
        if (values.size() < 2) break; //dodajemy sprawdzenie
        apply();
    }

	//na stosie wartoœci zostaje wynik koñcowy
    return values.top();
}
