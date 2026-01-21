/*
operation.h opisuje interfejs IOperation dla operacji matematycznych

*/

#pragma once


#ifndef OPERATION_H
#define OPERATION_H

//interfejs dla operacji matematycznych

class IOperation {
public:

	virtual double apply(double a, double b) const = 0;  //wirtualna metoda do wykonania operacji
	virtual int precedence() const = 0;  //priorytet operacji (czyli np. najpierw mnozenie i dzielenie, potem dodawanie i odejmowanie)
	virtual ~IOperation() = default;  //wirtualny destruktor
};

#endif
