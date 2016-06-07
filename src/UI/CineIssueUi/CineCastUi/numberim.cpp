#include "numberim.h"

NumberIM::NumberIM(QWidget *parent)
	: QDialog(parent)
{
// 	setWindowFlags(/*Qt::WindowStaysOnTopHint | */Qt::X11BypassWindowManagerHint);
	ui.setupUi(this);
	ui.lineEdit->setReadOnly(true);
}

NumberIM::~NumberIM()
{

}

void NumberIM::SetNumber(QString s)
{
	ui.lineEdit->setText(s);
}

QString NumberIM::GetNumber()
{
	return ui.lineEdit->text();
}

void NumberIM::SetTitle(QString s, bool bIp)
{
	setWindowTitle(s);
	ui.pushButtonDot->setEnabled(bIp);
// 	QWidget::activateWindow();
}

void NumberIM::AddNumber(char *s)
{
	QString ss = ui.lineEdit->text();
	ss += s;
	ui.lineEdit->setText(ss);
}

void NumberIM::on_pushButton0_clicked()
{
	AddNumber("0");
}

void NumberIM::on_pushButton1_clicked()
{
	AddNumber("1");
}

void NumberIM::on_pushButton2_clicked()
{
	AddNumber("2");
}

void NumberIM::on_pushButton3_clicked()
{
	AddNumber("3");
}

void NumberIM::on_pushButton4_clicked()
{
	AddNumber("4");
}

void NumberIM::on_pushButton5_clicked()
{
	AddNumber("5");
}

void NumberIM::on_pushButton6_clicked()
{
	AddNumber("6");
}

void NumberIM::on_pushButton7_clicked()
{
	AddNumber("7");
}

void NumberIM::on_pushButton8_clicked()
{
	AddNumber("8");
}

void NumberIM::on_pushButton9_clicked()
{
	AddNumber("9");
}

void NumberIM::on_pushButtonBack_clicked()
{
	QString ss = ui.lineEdit->text();
	ss.resize(ss.size() - 1);
	ui.lineEdit->setText(ss);
}

void NumberIM::on_pushButtonOK_clicked()
{
	accept();
}

void NumberIM::on_pushButtonClose_clicked()
{
	reject();
}

void NumberIM::on_pushButtonDot_clicked()
{
	AddNumber(".");
}