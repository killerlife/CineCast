#include "number.h"

Number::Number(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

Number::~Number()
{

}

void Number::SetNumber(QString s)
{
	ui.lineEdit->setText(s);
}

QString Number::GetNumber()
{
	return ui.lineEdit->text();
}

void Number::SetTitle(QString s)
{
	setWindowTitle(s);
}