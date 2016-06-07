#include "urlim.h"

UrlIM::UrlIM(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

UrlIM::~UrlIM()
{

}

void UrlIM::SetUrl(QString s)
{
	ui.lineEdit->setText(s);
}

QString UrlIM::GetUrl()
{
	return ui.lineEdit->text();
}

void UrlIM::SetTitle(QString s)
{
	setWindowTitle(s);
}

void UrlIM::AddChar(char *s)
{
	QString ss = ui.lineEdit->text();
	ss += s;
	ui.lineEdit->setText(ss);
}

void UrlIM::on_pushButtonBack_clicked()
{
	QString ss = ui.lineEdit->text();
	ss.resize(ss.size() - 1);
	ui.lineEdit->setText(ss);
}

void UrlIM::on_pushButtonClose_clicked()
{
	reject();
}

void UrlIM::on_pushButtonOK_clicked()
{
	accept();
}

void UrlIM::on_pushButton1_clicked()
{
	AddChar("1");
}

void UrlIM::on_pushButton2_clicked()
{
	AddChar("2");
}

void UrlIM::on_pushButton3_clicked()
{
	AddChar("3");
}

void UrlIM::on_pushButton4_clicked()
{
	AddChar("4");
}

void UrlIM::on_pushButton5_clicked()
{
	AddChar("5");
}

void UrlIM::on_pushButton6_clicked()
{
	AddChar("6");
}

void UrlIM::on_pushButton7_clicked()
{
	AddChar("7");
}

void UrlIM::on_pushButton8_clicked()
{
	AddChar("8");
}

void UrlIM::on_pushButton9_clicked()
{
	AddChar("9");
}

void UrlIM::on_pushButton0_clicked()
{
	AddChar("0");
}

void UrlIM::on_pushButtonQ_clicked()
{
	AddChar("Q");
}

void UrlIM::on_pushButtonW_clicked()
{
	AddChar("W");
}

void UrlIM::on_pushButtonE_clicked()
{
	AddChar("E");
}

void UrlIM::on_pushButtonR_clicked()
{
	AddChar("R");
}

void UrlIM::on_pushButtonT_clicked()
{
	AddChar("T");
}

void UrlIM::on_pushButtonY_clicked()
{
	AddChar("Y");
}

void UrlIM::on_pushButtonU_clicked()
{
	AddChar("U");
}

void UrlIM::on_pushButtonI_clicked()
{
	AddChar("I");
}

void UrlIM::on_pushButtonO_clicked()
{
	AddChar("O");
}

void UrlIM::on_pushButtonP_clicked()
{
	AddChar("P");
}

void UrlIM::on_pushButtonA_clicked()
{
	AddChar("A");
}

void UrlIM::on_pushButtonS_clicked()
{
	AddChar("S");
}

void UrlIM::on_pushButtonD_clicked()
{
	AddChar("D");
}

void UrlIM::on_pushButtonF_clicked()
{
	AddChar("F");
}

void UrlIM::on_pushButtonG_clicked()
{
	AddChar("G");
}

void UrlIM::on_pushButtonH_clicked()
{
	AddChar("H");
}

void UrlIM::on_pushButtonJ_clicked()
{
	AddChar("J");
}

void UrlIM::on_pushButtonK_clicked()
{
	AddChar("K");
}

void UrlIM::on_pushButtonL_clicked()
{
	AddChar("L");
}

void UrlIM::on_pushButton_1_clicked()
{
	AddChar(":");
}

void UrlIM::on_pushButtonZ_clicked()
{
	AddChar("Z");
}

void UrlIM::on_pushButtonX_clicked()
{
	AddChar("X");
}

void UrlIM::on_pushButtonC_clicked()
{
	AddChar("C");
}

void UrlIM::on_pushButtonV_clicked()
{
	AddChar("V");
}

void UrlIM::on_pushButtonB_clicked()
{
	AddChar("B");
}

void UrlIM::on_pushButtonN_clicked()
{
	AddChar("N");
}

void UrlIM::on_pushButtonM_clicked()
{
	AddChar("M");
}

void UrlIM::on_pushButtonDot_clicked()
{
	AddChar(".");
}

void UrlIM::on_pushButton_2_clicked()
{
	AddChar("/");
}

void UrlIM::on_pushButton_3_clicked()
{
	AddChar("~");
}

void UrlIM::on_pushButton_4_clicked()
{
	AddChar("!");
}

void UrlIM::on_pushButton_5_clicked()
{
	AddChar("@");
}


void UrlIM::on_pushButton_6_clicked()
{
	AddChar("#");
}

void UrlIM::on_pushButton_7_clicked()
{
	AddChar("$");
}

void UrlIM::on_pushButton_8_clicked()
{
	AddChar("%");
}

void UrlIM::on_pushButton_9_clicked()
{
	AddChar("^");
}

void UrlIM::on_pushButton_10_clicked()
{
	AddChar("&");
}

void UrlIM::on_pushButton_11_clicked()
{
	AddChar("*");
}

void UrlIM::on_pushButton_12_clicked()
{
	AddChar("?");
}

void UrlIM::on_pushButton_13_clicked()
{
	AddChar("(");
}

void UrlIM::on_pushButton_14_clicked()
{
	AddChar(")");
}

void UrlIM::on_pushButton_15_clicked()
{
	AddChar("-");
}

void UrlIM::on_pushButton_16_clicked()
{
	AddChar("_");
}

void UrlIM::on_pushButton_17_clicked()
{
	AddChar("+");
}

void UrlIM::on_pushButton_18_clicked()
{
	AddChar("=");
}

void UrlIM::on_pushButton_19_clicked()
{
	AddChar("|");
}

void UrlIM::on_pushButton_20_clicked()
{
	AddChar("<");
}

void UrlIM::on_pushButton_21_clicked()
{
	AddChar(">");
}

void UrlIM::on_pushButton_22_clicked()
{
	AddChar("`");
}