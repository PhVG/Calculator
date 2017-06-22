#include "calculator.h"
#include "ui_calculator.h"
#include <QDebug>


Calculator::Calculator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Calculator)
{
    ui->setupUi(this);

    m_numbers << ui->tb0
            << ui->tb1
            << ui->tb2
            << ui->tb3
            << ui->tb4
            << ui->tb5
            << ui->tb6
            << ui->tb7
            << ui->tb8
            << ui->tb9;

    m_operators << ui->tbPlus
              << ui->tbMinus
              << ui->tbMult
              << ui->tbDiv;

    for(auto it: m_numbers)
    {
        connect(it, SIGNAL(clicked(bool)), SLOT(slotNumberClicked()));
    }

    for(auto it: m_operators)
    {
        connect(it, SIGNAL(clicked(bool)), SLOT(slotOperator()));
    }

    connect(ui->tbC, SIGNAL(clicked(bool)), SLOT(slotClear()));
    connect(ui->tbErase, SIGNAL(clicked(bool)), SLOT(slotErase()));
    connect(ui->tbPoint, SIGNAL(clicked(bool)), SLOT(slotPoint()));
    connect(ui->tbRes, SIGNAL(clicked(bool)), SLOT(slotRes()));

    connect(&m_newNumber, SIGNAL(numberChanged(QString)), SLOT(slotNumberChanged(QString)));
}


Calculator::~Calculator()
{
    delete ui;
}


void Calculator::slotOperator()
{
    calculate();

    QToolButton *op = qobject_cast<QToolButton*>(sender());
    m_currentOperator = (CurrentOperator)m_operators.indexOf(op);
}


void Calculator::slotNumberClicked()
{
    QToolButton *num = qobject_cast<QToolButton*>(sender());

    QChar c = QString::number(m_numbers.indexOf(num))[0];
    m_newNumber.append(c);
}


void Calculator::slotErase()
{
    m_newNumber.erase();
}


void Calculator::slotClear()
{
    m_newNumber.clear();
}


void Calculator::slotPoint()
{
    m_newNumber.append('.');
}


void Calculator::slotRes()
{
    calculate();
}

void Calculator::slotNumberChanged(QString num)
{
    ui->lblNumber->setText(num);
}

void Calculator::calculate()
{
    if(m_currentOperator == None)
    {
        m_prevNumber = m_newNumber;
        m_newNumber.clear();
        return;
    }

    double res = 0;

    if(m_currentOperator == Plus)
    {
        res = m_prevNumber.getProcessingNumber() + m_newNumber.getProcessingNumber();
    }
    else if(m_currentOperator == Minus)
    {
        res = m_prevNumber.getProcessingNumber() - m_newNumber.getProcessingNumber();
    }
    else if(m_currentOperator == Mult)
    {
        res = m_prevNumber.getProcessingNumber() * m_newNumber.getProcessingNumber();
    }
    else if(m_currentOperator == Div)
    {
        res = m_prevNumber.getProcessingNumber() / m_newNumber.getProcessingNumber();
    }

    m_currentOperator = None;

    m_newNumber.setNumber(res);
}


void ScreenNumber::append(QChar c)
{
    if ((c == '.' && m_number.contains(c)) || m_number.length() > 11)
    {
        return;
    }

    if(m_number.length() == 1 && m_number == "0")
    {
        m_number = "";
    }

    m_number += c;

    emit numberChanged(m_number);
}


void ScreenNumber::erase()
{
    if(m_number.length() == 0)
    {
        return;
    }

    m_number.chop(1);

    if(m_number.length() > 0 &&
            (m_number.at(m_number.length() - 1) == '.'
            || m_number.at(m_number.length() - 1) == '-')
            )
    {
        m_number.chop(1);
    }

    if(m_number.length() == 0)
    {
        m_number = "0";
    }

    emit numberChanged(m_number);
}


void ScreenNumber::clear()
{
    m_number = "0";
    emit numberChanged(m_number);
}


double ScreenNumber::getProcessingNumber()
{
    return m_number.toDouble();
}


void ScreenNumber::setNumber(double n)
{
    m_number = QString::number(n, 'g', 12);

    emit numberChanged(m_number);
}
