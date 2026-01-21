/*
calculator.h deklaruje klasê Calculator ktora z kolei udostepnia metodê calculate do obliczania wyra¿eñ matematycznych zapisanych jako ³añcuchy znaków
*/

#pragma once
#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <string>


//klasa kalkulatora do obliczania wyra¿eñ matematycznych
//udostepnia metodê calculate
class Calculator {
public:
    double calculate(const std::string& expr);
};

#endif
