#include "calculation.h"

Calculation::Calculation(QVector<QString> vector)
{
    infix=vector;
}

int Calculation::precedence(QString str)
{
    if(str=="*" || str=="/")
        return 2;
    else if(str=="+" || str=="-")
        return 1;
    else
        return 0;
}

bool Calculation::isDouble(QString str)
{
    bool success;
    double num=str.toDouble(&success);
    return success;
}

double Calculation::evaluate(double num1, double num2, QString operation)
{
    if(operation=="+")
        return num1+num2;
    else if(operation=="-")
        return num1-num2;
    else if(operation=="*")
        return num1*num2;
    else if(operation=="/")
        return num1/num2;
    else
        return 0;
}


void Calculation::infixToPostfix()
{
    QVector<QString> op; //fronta operatoru

    for(int i=0;i<infix.length();i++){

        if(isDouble(infix[i]))  // pokud je prvek infix[i] cislo, pridej ho do postfix
            postfix.push_back(infix[i]);
        else if(infix[i]=="(") // pokud je prvek infix[i] zavorka "(", prida se do fronty operatoru na vrchni pozici (nejvetsi precedence)
            op.push_front(infix[i]);
        else if(infix[i]==")"){ // pokud je prvek infix[i] ukoncujici zavorka, pridaji se vsechny operatory mezi zavorkami do postfix a odstrani se z fronty operatoru
            while(op.front()!="("){
                postfix.push_back(op.front());
                op.pop_front();
            }
            op.pop_front(); // odstraneni zavorky "(" z fronty
        }
        else{   // prvek je nektery z operatoru + - * /
            // pokud jde do fronty operatoru operator s mensi nebo stejnou hodnotou precedence (prednosti),
            // operatory s vyssi prednosti se z fronty ulozi do postfix a pote se do fronty operatoru ulozi prichozi operator
            // pokud ma operator vyssi prednost, ulozi se do fronty
            while(!op.empty() && precedence(infix[i])<=precedence(op.front())){
                postfix.push_back(op.front());
                op.pop_front();
            }
            op.push_front(infix[i]);
        }
    }

    while(!op.empty()){  // ulozeni zbyvajicich operatoru z fronty do postfix
        postfix.push_back(op.front());
        op.pop_front();
    }

}

void Calculation::evaluatePostfix()
{
    QVector<double> nums; // vektor, do ktereho jsou ukladana cisla nasledovne:
    // dokud neni prvek operator, jsou ukladana cisla
    // kdyz se narazi na operator, 2 cisla pred nim se z vektoru odstrani a provede se operace, vysledek se pak ulozi do vektoru
    // nakonec ve vektoru zbyde jedno cislo, ktere je vysledkem

    for(int i=0;i<postfix.length();i++){

        if(postfix[i]=="+" || postfix[i]=="-" || postfix[i]=="*" || postfix[i]=="/"){
            // cisla pred operaci se odstrani z vektoru nums a vyhodnoti se jejich vysledek, ten se pak ulozi zpet do nums
            double number2=nums.back();
            nums.pop_back();
            double number1=nums.back();
            nums.pop_back();
            nums.push_back(evaluate(number1, number2, postfix[i]));
        }else{ // jedna se o cislo, to se ulozi do nums
            nums.push_back(postfix[i].toDouble());
        }
    }
    // nakonec zbyde v nums jen vysledek
    result=nums[0];
}

double Calculation::getAns()
{
    return result;
}


