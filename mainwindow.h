#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "about.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    // vector, do ktereho se ulozi cisla a operace ve formatu QString v poradi, jako jsou na displeji (infixova notace)
    QVector<QString> vec;
    // bool promenna ktera je true, prave kdyz se uklada cislo
    bool savingNumber;
    // cislo ve formatu QString, ktere je ukladano do vektoru vec
    QString number;


private slots:
    // udalosti stisku jednotlivych tlacitek
    void digitPressed();
    void operatorPressed();
    void changeSignPressed();
    void equalPressed();
    void backPressed();
    void clearPressed();
    // udalost stistknuti "O aplikaci" v menu
    void on_actionO_aplikaci_triggered();
};
#endif // MAINWINDOW_H
