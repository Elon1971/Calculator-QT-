#include "mainwindow.h"
#include <QPushButton>
#include <QGridLayout>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QLineEdit>
#include <QMessageBox>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createUI();
}

MainWindow::~MainWindow()
{
}

void MainWindow::createUI()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    resultLineEdit = new QLineEdit(centralWidget);
    resultLineEdit->setAlignment(Qt::AlignRight);
    resultLineEdit->setReadOnly(true);

    QGridLayout *gridLayout = new QGridLayout(centralWidget);

    QString numberResources[10] = {
            ":/image/n0.jpg",
            ":/image/n1.png",
            ":/image/n2.png",
            ":/image/n3.png",
            ":/image/n4.png",
            ":/image/n5.png",
            ":/image/n6.png",
            ":/image/n7.png",
            ":/image/n8.png",
            ":/image/n9.png"
        };

    for (int i = 0; i < 10; ++i) {
        digitButtons[i] = new QPushButton("", centralWidget);
        QPixmap pixmap(numberResources[i]);
        if (pixmap.isNull()) {
            qDebug() << "Error loading image:" << numberResources[i];
        }
        digitButtons[i]->setIcon(QIcon(pixmap));
        digitButtons[i]->setIconSize(QSize(64, 64));
        connect(digitButtons[i], &QPushButton::clicked, this, &MainWindow::on_digitButton_clicked);

        int row = (9 - i) / 3 + 1;
        int col = (i - 1) % 3;
        gridLayout->addWidget(digitButtons[i], row, col);
    }

    operatorButtons[0] = new QPushButton("+", centralWidget);
    operatorButtons[1] = new QPushButton("-", centralWidget);
    operatorButtons[2] = new QPushButton("*", centralWidget);
    operatorButtons[3] = new QPushButton("/", centralWidget);

    clearButton = new QPushButton("C", centralWidget);
    equalsButton = new QPushButton("=", centralWidget);
    QPushButton *backspaceButton = new QPushButton("<-", centralWidget);

    gridLayout->addWidget(resultLineEdit, 0, 0, 1, 4);
    gridLayout->addWidget(digitButtons[0], 4, 0, 1, 2);
    gridLayout->addWidget(clearButton, 4, 2);
    gridLayout->addWidget(equalsButton, 1, 4);

    for (int i = 0; i < 4; ++i) {
        gridLayout->addWidget(operatorButtons[i], i + 1, 3);
        connect(operatorButtons[i], &QPushButton::clicked, this, &MainWindow::on_operatorButton_clicked);
    }
    gridLayout->addWidget(backspaceButton, 0, 4);

    QString operatorButtonStyle = "QPushButton {"
                                  "    background-color: %1;"
                                  "    border: 1px solid black;"
                                  "    padding: 28px 28px 28px 28px;"
                                  "}"
                                  "QPushButton:hover {"
                                  "    background-color: %2;"
                                  "}";

    operatorButtons[0]->setStyleSheet(operatorButtonStyle.arg("red", "blue"));
    operatorButtons[1]->setStyleSheet(operatorButtonStyle.arg("grey", "darkgrey"));
    operatorButtons[2]->setStyleSheet(operatorButtonStyle.arg("yellow", "red"));
    operatorButtons[3]->setStyleSheet(operatorButtonStyle.arg("brown", "orange"));
    equalsButton->setStyleSheet("padding: 28px 28px 28px 28px;");

    clearButton->setStyleSheet("background-color: green;");
    clearButton->setStyleSheet("QPushButton:hover { background-color: pink; }");
    equalsButton->setStyleSheet("background-color: red;");
    equalsButton->setStyleSheet("QPushButton:hover { background-color: purple; }");
    equalsButton->setStyleSheet("padding: 28px 28px 28px 28px");
    clearButton->setStyleSheet("padding: 28px 28px 28px 28px;");

    operatorButtons[0]->setToolTip("Addition");
    operatorButtons[1]->setToolTip("Subtraction");
    operatorButtons[2]->setToolTip("Multiplication");
    operatorButtons[3]->setToolTip("Division");
    clearButton->setToolTip("Clear");


    connect(backspaceButton, &QPushButton::clicked, this, &MainWindow::on_backspaceButton_clicked);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::on_clearButton_clicked);
    connect(equalsButton, &QPushButton::clicked, this, &MainWindow::on_equalsButton_clicked);
}

void MainWindow::on_digitButton_clicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        int digitIndex = -1;
        for (int i = 0; i < 10; ++i) {
            if (digitButtons[i] == button) {
                digitIndex = i;
                break;
            }
        }
        if (digitIndex != -1) {
            QString digit = QString::number(digitIndex);
            QString currentText = resultLineEdit->text();
            resultLineEdit->setText(currentText + digit);
        }
    }
}

void MainWindow::on_operatorButton_clicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    QString op = button->text();
    QString currentText = resultLineEdit->text();
    resultLineEdit->setText(currentText + " " + op + " ");
}

void MainWindow::on_backspaceButton_clicked()
{
    QString currentText = resultLineEdit->text();
    if (!currentText.isEmpty()) {
        currentText.chop(1);
        resultLineEdit->setText(currentText);
    }
}

void MainWindow::on_clearButton_clicked()
{
    resultLineEdit->clear();
}

void MainWindow::on_equalsButton_clicked()
{
    QString expression = resultLineEdit->text().trimmed();

    QRegExp rx("(\\+|\\-|\\*|\\/)");
    QStringList tokens = expression.split(rx);

    if (tokens.size() != 2) {
        resultLineEdit->setText("Error: Invalid expression");
        return;
    }

    double operand1 = tokens[0].toDouble();
    QString op = expression.mid(tokens[0].size(), 1);
    double operand2 = tokens[1].toDouble();

    double result = 0;
    if (op == "+")
        result = operand1 + operand2;
    else if (op == "-")
        result = operand1 - operand2;
    else if (op == "*")
        result = operand1 * operand2;
    else if (op == "/") {
        // Check for division by zero
        if (operand2 == 0) {
            resultLineEdit->setText("Error: Division by zero");
            return;
        }
        result = operand1 / operand2;
    }

    resultLineEdit->setText(QString::number(result));
}

