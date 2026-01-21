/*
operation.cpp tu sa klasy dla kazdej operacji matematycznej
kazda klasa ma swoj priorytet i implementacje metody apply

*/

#include "Operation.h"



//dodawanie
class Add : public IOperation {
public:
	double apply(double a, double b) const override { return a + b; } //u¿ycie metody z interfejsu, nadpisanie metody apply przy uzyciu override
	int precedence() const override { return 1; } //priorytet operacji dodawania
};

//odejmowanie
class Sub : public IOperation {
public:
	double apply(double a, double b) const override { return a - b; } //tak jak przy dodawaniu
	int precedence() const override { return 1; } //priorytet operacji odejmowania
};


//mnozenie
class Mul : public IOperation {
public:
	double apply(double a, double b) const override { return a * b; }  //tak jak w poprzednich
	int precedence() const override { return 2; } //priorytet operacji mnozenia
};

class Div : public IOperation {
public:
	double apply(double a, double b) const override { return a / b; }  //tak jak w poprzednich
	int precedence() const override { return 2; }  //priorytet dzielenia
};

//funkcja tworz¹ca odpowiedni¹ operacjê na podstawie znaku
//zwraca wskaŸnik na odpowiedni¹ klasê dziedzicz¹c¹ po IOperation

IOperation* createOperation(char c) {

	//obiekty statyczne dla ka¿dej operacji
	//tworzone raz i wspó³dzielone z innymi wywo³aniami funkcji-brak potrzeby wielokrotnego tworzenia obiektów, oszczêdnoœæ pamiêci
    static Add add;
    static Sub sub;
    static Mul mul;
    static Div div;

	//sprawdzenie znaku i zwrócenie odpowiedniego obiektu
    if (c == '+') return &add;
    if (c == '-') return &sub;
    if (c == '*') return &mul;
    if (c == '/') return &div;
    return nullptr;
}
