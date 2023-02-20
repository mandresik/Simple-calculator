#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "about.h"
#include "calculation.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // prirazeni akci a udalosti
    // stisk cislic a desetinne carky
    connect(ui->btn0, SIGNAL(released()), this, SLOT(digitPressed()));
    connect(ui->btn1, SIGNAL(released()), this, SLOT(digitPressed()));
    connect(ui->btn2, SIGNAL(released()), this, SLOT(digitPressed()));
    connect(ui->btn3, SIGNAL(released()), this, SLOT(digitPressed()));
    connect(ui->btn4, SIGNAL(released()), this, SLOT(digitPressed()));
    connect(ui->btn5, SIGNAL(released()), this, SLOT(digitPressed()));
    connect(ui->btn6, SIGNAL(released()), this, SLOT(digitPressed()));
    connect(ui->btn7, SIGNAL(released()), this, SLOT(digitPressed()));
    connect(ui->btn8, SIGNAL(released()), this, SLOT(digitPressed()));
    connect(ui->btn9, SIGNAL(released()), this, SLOT(digitPressed()));
    connect(ui->btnDecimal, SIGNAL(released()), this, SLOT(digitPressed()));

    // stisk operatoru + - * / ( )
    connect(ui->btnAdd, SIGNAL(released()), this, SLOT(operatorPressed()));
    connect(ui->btnSubtract, SIGNAL(released()), this, SLOT(operatorPressed()));
    connect(ui->btnMultiply, SIGNAL(released()), this, SLOT(operatorPressed()));
    connect(ui->btnDivide, SIGNAL(released()), this, SLOT(operatorPressed()));
    connect(ui->btnLeftBracket, SIGNAL(released()), this, SLOT(operatorPressed()));
    connect(ui->btnRightBracket, SIGNAL(released()), this, SLOT(operatorPressed()));

    // stisk tlacitek BACK a CLEAR
    connect(ui->btnBack, SIGNAL(released()), this, SLOT(backPressed()));
    connect(ui->btnClear, SIGNAL(released()), this, SLOT(clearPressed()));

    // stisk tlacitek +/- a =
    connect(ui->btnPlusMinus, SIGNAL(released()) ,this, SLOT(changeSignPressed()));
    connect(ui->btnEqual, SIGNAL(released()), this, SLOT(equalPressed()));
}

void MainWindow::digitPressed(){

    // pri stisku se cislice prida na display
    QPushButton *button=(QPushButton *)sender();
    QString btnValue=button->text();
    QString displayValue=ui->display->text();
    ui->display->setText(displayValue+btnValue);

    // pokud se jiz cislo uklada, ukladane cislo number se zmeni podle stisknute klavesy a toto zmenene cislo se prepise ve vektoru
    if(savingNumber){
        number=number+btnValue;
        vec.back()=number;
    }else{ // pokud se cislo neuklada (minula stisknuta klavesa byla operace), zacne se ukladat a prida se do vektoru
        number=btnValue;
        vec.push_back(number);
        savingNumber=true;
    }
}

void MainWindow::operatorPressed(){

    // pri stisku se operace prida na display
    QPushButton *button=(QPushButton *)sender();
    QString btnValue=button->text();
    QString displayValue=ui->display->text();
    ui->display->setText(displayValue+btnValue);

    // operace se ulozi do vektoru a promenne pro ukladani cisla se nastavi na default
    savingNumber=false;
    number="";
    vec.push_back(btnValue);
}


void MainWindow::backPressed()
{
    QString displayValue=ui->display->text();

    // pokud je na displeji nejaky vyraz, je mazani umozneno
    if(!displayValue.isEmpty()){

        if(vec.back().length()==1){ // pokud ma posledni hodnota ve vectoru delku 1, jedna se o cislici nebo operaci
            // prvek se odstrani z vektoru, number se nastavi na prazdny string a posledni prvek displayValue se odstrani
            vec.pop_back();
            number="";
            displayValue.chop(1);
            if(displayValue.length()!=0){ // pokud na displeji zbyva nejaky vyraz, vysetri se, jestli bylo pred odstranenym prvkem cislo
                // pokud ano, zjisti se jeho hodnota a ukladani cisla je nastaveno na true
                // pokud ne, je tam operator a ukladani cisla je nastaveno na false
                if((displayValue.right(1)>='0') && (displayValue.right(1)<='9')){
                    savingNumber=true;
                    number=vec.back();
                }else
                    savingNumber=false;
            }
        }else if(vec.back().length()==2){ // pokud je delka 2, muze se jednat o zaporne cislo a znamenko musi byt odstraneno spolecne s cislem
            double num=vec.back().toDouble(); // delku 2 a vice muze mit jen cislo
            if(num<0){ // pokud je zaporne, odstrani se z vektoru
                // pred zapornym cislem muze byt jen operator, savingNumber je nastaveno na false a number na prazdny string
                // z displeje se odstrani posledni dva prvky
                vec.pop_back();
                savingNumber=false;
                number="";
                displayValue.chop(2);
            }else{ // pokud je kladne, odstrani se posledni cislice a a cislo ve vektoru se prepise
                number=number.left(1);
                vec.back()=number;
                displayValue.chop(1);
            }
        }else{ // jakoukoliv vetsi delku ma jen cislo, odstrani se posledni cislice a hodnota se prepise ve vektoru
            number=number.left(number.length()-1);
            vec.back()=number;
            displayValue.chop(1);
        }

        // vyraz na displeji se nastavi na upravenou hdonotu
        ui->display->setText(displayValue);
    }
}

void MainWindow::clearPressed()
{
    //vse je nastaveno na default
    ui->display->setText("");
    savingNumber=false;
    number="";
    vec={};
}

void MainWindow::changeSignPressed()
{
    // zmena znamenka nastava pouze pokud se uklada cislo
    if(savingNumber){

        // vyraz na displeji se rozdeli na cislo, kteremu se bude menit znamenko a na zbyvajici vyraz (vlevo od meneneho cisla)
        QString displayValue=ui->display->text();
        QString leftSide=displayValue.left(displayValue.length()-number.length());

        // menene cislo se prevede na double, zmeni se mu znamenko, pak se prevede zpet na string a zmeni ve vektoru
        double dblNumber=number.toDouble();
        dblNumber*=-1;
        number=QString::number(dblNumber);
        vec.back()=number;

        // sestaveni vyrazu na displeji se zmenenym cislem
        ui->display->setText(leftSide+number);
    }
}

void MainWindow::equalPressed()
{

    // pri stisku = se vytvori instance tridy calculation, do ktere se odesle vektor,
    // ten je pak zpracovan pomoci metod infixToPostfix a evaluatePostfix pro vypocet vysledku
    Calculation *calc=new Calculation(vec);
    calc->infixToPostfix();
    calc->evaluatePostfix();

    // obdrzeni vysledku a vypis na displeji
    double ans=calc->getAns();
    ui->display->setText(QString::number(ans));

    // k vysledku se pristupuje, jako by ho zadal uzivatel ... jedna se o jedno "zadane" cislo
    savingNumber=true;
    number=QString::number(ans);
    vec={};
    vec.push_back(number);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionO_aplikaci_triggered()
{
    About *about=new About(this);
    about->show();
}

