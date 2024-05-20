#pragma once

#include <QMainWindow>
#include "ui_reg_window.h"

class reg_window : public QMainWindow
{
	Q_OBJECT

public:
	reg_window(QWidget *parent = nullptr);
	~reg_window();
	
	QString getName();
	QString getPass();
	bool checkPass();

	void on_nameLineEdit_textEdited();
	void on_passwordLineEdit_textEdited();
	void on_confirmLineEdit_textEdited();

	QPushButton* getRegRegButtonPtr();

private:
	Ui::reg_windowClass ui;

	QString m_userName;
	QString m_userPass;
	QString m_confirmation;
};
