#include "reg_window.h"

reg_window::reg_window(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

reg_window::~reg_window(){}

void reg_window::on_nameLineEdit_textEdited()
{
	reg_window::m_userName = ui.lineEditName->text();
}

void reg_window::on_passwordLineEdit_textEdited()
{
	reg_window::m_userPass = ui.lineEditPassword->text();
}

void reg_window::on_confirmLineEdit_textEdited()
{
	reg_window::m_confirmation = ui.lineEditConfirm->text();
}

QString reg_window::getName()
{
	return m_userName;
}

QString reg_window::getPass()
{
	return m_userPass;
}

bool reg_window::checkPass()
{
	return (m_confirmation == m_userPass);
}

QPushButton* reg_window::getRegRegButtonPtr()
{
	return ui.RegistButton;
}
