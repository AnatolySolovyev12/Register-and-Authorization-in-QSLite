#include "auth_window.h"

auth_window::auth_window(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

auth_window::~auth_window()
{}


QPushButton* auth_window::getAuthRegButtonPtr()
{
	return auth_window::ui.RegisterButton;
}

QPushButton* auth_window::getAuthAuthButtonPtr()
{
	return auth_window::ui.LogInButton;
}


QString auth_window::getLogin()
{
	return auth_window::m_username;
}

QString auth_window::getPass()
{
	return auth_window::m_userpass;
}

void auth_window::on_nameLineEdit_textEditedAuth()
{
	auth_window::m_username = ui.lineEditName->text();
}

void auth_window::on_passwordLineEdit_textEditedAuth()
{
	auth_window::m_userpass = ui.lineEditPassword->text();
}

