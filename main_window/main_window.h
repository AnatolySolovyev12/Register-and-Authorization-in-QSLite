#pragma once

#include <QtWidgets/QMainWindow>

#include "ui_main_window.h"
#include "auth_window.h"
#include "reg_window.h"
#include <QString>

//#include <QSqlDatabase>
#include <QtSql/QtSql>

class main_window : public QMainWindow
{
    Q_OBJECT

public:
    main_window(QWidget *parent = nullptr);
    ~main_window();
    
    void display(); // �������� ���������������� ������� �����������
    bool connectDB(); // �������� ������ ����������� � ��

    auth_window ui_Auth; // ���������� ���� ����������� � ���� �����������
    QPushButton* authRegButton = nullptr; // ��� ������� � ������ ����������� ���� �����������
    QPushButton* authAuthButton = nullptr; // ��� ������� � ������ ����������� ���� �����������

    reg_window ui_Reg; // ����������� �������� ����
    QPushButton* regRegButton = nullptr; // ��� ������� � ������ ����������� ���� �����������

    QString m_username; // ������ ��� ��������� 
    QString m_userpass; // ����������������� �����

    QString db_input; // ������ ��� �������� ������� � ��

    QSqlDatabase mw_db; // ��������� ����������� � ��

    int user_counter = 0; // ������� �������������
    bool m_loginSuccesfull = false; // ���� �������� �����������

    int myTest = 0;

private:
    Ui_BarMessege ui_Main;
    
private:


    void authorizeUser(); // ���������������� �����
    void registerWindowShow();
    void registerUser();

};
