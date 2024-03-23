#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_digitButton_clicked();
    void on_operatorButton_clicked();
    void on_clearButton_clicked();
    void on_equalsButton_clicked();
    void on_backspaceButton_clicked();

private:
    QLineEdit *resultLineEdit;
    QPushButton *digitButtons[10];
    QPushButton *operatorButtons[4];
    QPushButton *clearButton;
    QPushButton *equalsButton;

    void createUI();
};

#endif


