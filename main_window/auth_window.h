#pragma once

#include <QMainWindow>
#include "ui_auth_window.h"

class auth_window : public QMainWindow
{
	Q_OBJECT

public:
	auth_window(QWidget *parent = nullptr);
	~auth_window();

	QString getLogin();
	QString getPass();

	QPushButton* getAuthRegButtonPtr();
	QPushButton* getAuthAuthButtonPtr();

	void on_nameLineEdit_textEditedAuth();
	void on_passwordLineEdit_textEditedAuth();


	bool pushRegButton = false;

private:
	Ui::auth_windowClass ui;

	QString m_username;
	QString m_userpass;

};
