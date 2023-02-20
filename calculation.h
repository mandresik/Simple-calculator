#ifndef CALCULATION_H
#define CALCULATION_H

#include <QString>
#include <QVector>

class Calculation
{
public:
    Calculation(QVector<QString> vec);
    void infixToPostfix(); // metoda prevadejici vektor z infixove do postfixove notace
    void evaluatePostfix(); // metoda, ktera vyhodnoti vektor v postfixove notaci
    double getAns(); // metoda vracejici vysledek

private:
    QVector<QString> infix; // vektor cisel a operaci v infixove notaci
    QVector<QString> postfix; // vektor cisel a operaci v postfixove notaci
    // metoda vracejici int, ktere znaci prednost na operace, * / maji vetsi prioritu nez + -
    int precedence(QString str);
    // metoda, ktera vraci true, pokud je prichozi string typu double
    bool isDouble(QString str);
    // metoda, ktera vraci vysledek dvou cisel a prislusne operace + - * /
    double evaluate(double num1, double num2, QString operation);
    double result;
};

#endif // CALCULATION_H
