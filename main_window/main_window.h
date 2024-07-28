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
    
    void display(); // прототип пользовательской функции отображения
    bool connectDB(); // прототип метода подключения к БД

    auth_window ui_Auth; // экземпляры окна авторизации и окна регистрации
    QPushButton* authRegButton = nullptr; // для доступа к кнопке регистрации окна авторизации
    QPushButton* authAuthButton = nullptr; // для доступа к кнопке регистрации окна авторизации

    reg_window ui_Reg; // принадлежат главному окну
    QPushButton* regRegButton = nullptr; // для доступа к кнопке регистрации окна авторизации

    QString m_username; // строки для обработки 
    QString m_userpass; // пользовательского ввода

    QString db_input; // строка для отправки запроса к БД

    QSqlDatabase mw_db; // экземпляр подключения к БД

    int user_counter = 0; // счетчик пользователей
    bool m_loginSuccesfull = false; // флаг успешной авторизации

    int myTest = 0;

private:
    Ui_BarMessege ui_Main;
    
private:


    void authorizeUser(); // пользовательские слоты
    void registerWindowShow();
    void registerUser();

};
