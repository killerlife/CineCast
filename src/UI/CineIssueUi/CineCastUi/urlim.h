#ifndef URLIM_H
#define URLIM_H

#include <QDialog>
#include "ui_urlim.h"

class UrlIM : public QDialog
{
	Q_OBJECT

public:
	UrlIM(QWidget *parent = 0);
	~UrlIM();

	void SetTitle(QString s);
	QString GetUrl();
	void SetUrl(QString s);

private:
	void AddChar(char *s);

private:
	Ui::UrlIMClass ui;

private slots:
	void on_pushButton_22_clicked();
	void on_pushButton_21_clicked();
	void on_pushButton_20_clicked();
	void on_pushButton_19_clicked();
	void on_pushButton_18_clicked();
	void on_pushButton_17_clicked();
	void on_pushButton_16_clicked();
	void on_pushButton_15_clicked();
	void on_pushButton_14_clicked();
	void on_pushButton_13_clicked();
	void on_pushButton_12_clicked();
	void on_pushButton_11_clicked();
	void on_pushButton_10_clicked();
	void on_pushButton_9_clicked();
	void on_pushButton_8_clicked();
	void on_pushButton_7_clicked();
	void on_pushButton_6_clicked();
	void on_pushButton_5_clicked();
	void on_pushButton_4_clicked();
	void on_pushButton_3_clicked();
	void on_pushButton_2_clicked();
	void on_pushButtonDot_clicked();
	void on_pushButtonM_clicked();
	void on_pushButtonN_clicked();
	void on_pushButtonB_clicked();
	void on_pushButtonV_clicked();
	void on_pushButtonC_clicked();
	void on_pushButtonX_clicked();
	void on_pushButtonZ_clicked();
	void on_pushButton_1_clicked();
	void on_pushButtonL_clicked();
	void on_pushButtonK_clicked();
	void on_pushButtonJ_clicked();
	void on_pushButtonH_clicked();
	void on_pushButtonG_clicked();
	void on_pushButtonF_clicked();
	void on_pushButtonD_clicked();
	void on_pushButtonS_clicked();
	void on_pushButtonA_clicked();
	void on_pushButtonP_clicked();
	void on_pushButtonO_clicked();
	void on_pushButtonI_clicked();
	void on_pushButtonU_clicked();
	void on_pushButtonY_clicked();
	void on_pushButtonT_clicked();
	void on_pushButtonR_clicked();
	void on_pushButtonE_clicked();
	void on_pushButtonW_clicked();
	void on_pushButtonQ_clicked();
	void on_pushButton0_clicked();
	void on_pushButton9_clicked();
	void on_pushButton8_clicked();
	void on_pushButton7_clicked();
	void on_pushButton6_clicked();
	void on_pushButton5_clicked();
	void on_pushButton4_clicked();
	void on_pushButton3_clicked();
	void on_pushButton2_clicked();
	void on_pushButton1_clicked();
	void on_pushButtonOK_clicked();
	void on_pushButtonClose_clicked();
	void on_pushButtonBack_clicked();
};

#endif // URLIM_H
