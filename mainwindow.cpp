#include "mainwindow.h"
#include "./ui_mainwindow.h"

long currentDisplay = 0;
long previousNumber = 0;
long factor = 0;
int currentOperation = 0;
bool eqPressed = false;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->Display->setText(QString::number(currentDisplay));
    ui->Display->setReadOnly(true);
    QPushButton *numButtons[10];
    for (int i = 0; i < 10; i++)
    {
        QString buttonName = "Button" + QString::number(i);
        numButtons[i] = MainWindow::findChild<QPushButton *>(buttonName);
        connect(numButtons[i], SIGNAL(released()), this, SLOT(NumPressed()));
    }

    QPushButton *operationButtons[4];

    operationButtons[0] = MainWindow::findChild<QPushButton *>("ButtonAdd");
    operationButtons[1] = MainWindow::findChild<QPushButton *>("ButtonSub");
    operationButtons[2] = MainWindow::findChild<QPushButton *>("ButtonX");
    operationButtons[3] = MainWindow::findChild<QPushButton *>("ButtonDiv");

    for (int i = 0; i < 4; i++)
    {
        connect(operationButtons[i], SIGNAL(released()), this, SLOT(OperationPressed()));
    }

    QPushButton *equalsButton = MainWindow::findChild<QPushButton *>("ButtonEquals");
    connect(equalsButton, SIGNAL(released()), this, SLOT(EqualsPressed()));

    QPushButton *clearButton = MainWindow::findChild<QPushButton *>("ButtonC");
    connect(clearButton, SIGNAL(released()), this, SLOT(ClearPressed()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::NumPressed()
{
    // In case factor had any value, it's reset because a new number is being entered
    factor = 0;
    QPushButton *button = (QPushButton *)sender();
    QString butVal = button->text();

    // The number in the Display gets updated with the new character
    currentDisplay = currentDisplay * 10 + butVal.toLong();

    ui->Display->setText(QString::number(currentDisplay));
}

void MainWindow::OperationPressed()
{
    QPushButton *button = (QPushButton *)sender();
    QChar butVal = button->text().at(0);
    // If there was an operation pending, it must be resolved before the next one
    if (currentOperation != 0 && !eqPressed)
    {
        EqualsPressed();
    }
    eqPressed = false;
    // The currentOperation flag keeps track of the last operation button pressed
    switch (static_cast<char>(butVal.toLatin1())) {
    case '+':
        previousNumber = currentDisplay;
        currentDisplay = 0;
        currentOperation = 1;
        break;
    case '-':
        previousNumber = currentDisplay;
        currentDisplay = 0;
        currentOperation = 2;
        break;
    case 'x':
        previousNumber = currentDisplay;
        currentDisplay = 0;
        currentOperation = 3;
        break;
    case '/':
        // TODO
        break;
    default:
        break;
    }
}

void MainWindow::EqualsPressed()
{
    eqPressed = true;

    // If there's not a previously stored factor, it's stored (to allow keep adding/substracting by only pressing = again)
    if (factor == 0)
    {
        factor = currentDisplay;
        currentDisplay = previousNumber;
    }
    // Value previously stored when pressing one of the operation buttons
    switch(currentOperation)
    {
    case 1:
        currentDisplay = currentDisplay + factor;
        break;
    case 2:
        currentDisplay = currentDisplay - factor;
        break;
    case 3:
        currentDisplay = currentDisplay * factor;
        break;
        // TODO
    default:
        break;
    }
    // Display is updated
    ui->Display->setText(QString::number(currentDisplay));
}

void MainWindow::ClearPressed()
{
    // Reset all values to 0
    currentDisplay = 0;
    previousNumber = 0;
    factor = 0;
    currentOperation = 0;
    eqPressed = false;
    ui->Display->setText(QString::number(currentDisplay));
}


