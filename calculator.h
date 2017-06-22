#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QWidget>


namespace Ui {
class Calculator;
}

class QToolButton;

class ScreenNumber : public QObject
{
    Q_OBJECT
public:
    enum Part {IntPart = 0, FractPart};

    ScreenNumber(QObject *parent = nullptr)
        : QObject(parent), m_number("0")
    {
        emit numberChanged(m_number);
    }

    ScreenNumber& operator=(const ScreenNumber& n)
    {
        this->m_number = n.m_number;
    }

    void append(QChar c);
    void erase();
    void clear();

    double getProcessingNumber();
    void setNumber(double n);

private:
    QString m_number;

signals:
    void numberChanged(QString);
};


class Calculator : public QWidget
{
    Q_OBJECT
public:
    explicit Calculator(QWidget *parent = nullptr);
    ~Calculator();

private slots:
    void slotOperator();
    void slotNumberClicked();
    void slotErase();
    void slotClear();
    void slotPoint();
    void slotRes();
    void slotNumberChanged(QString);

private:
    enum CurrentOperator { Plus = 0, Minus, Mult, Div, None = 42 };

    void calculate();

    Ui::Calculator *ui;

    ScreenNumber m_newNumber;
    ScreenNumber m_prevNumber;

    CurrentOperator m_currentOperator = None;

    QList<QToolButton*> m_numbers;
    QList<QToolButton*> m_operators;
};

#endif // CALCULATOR_H
