#include "main_window.h"
#include "ui_main_window.h"
#include "auth_window.h"
#include "reg_window.h"
#include <QtDebug>

main_window::main_window(QWidget *parent)
    : QMainWindow(parent)
{
    ui_Main.setupUi(this); // ��������� ������� ��� ����������� GUI ��� ������������� Qt Designer

    ui_Auth.show(); // ����� ���������� ���� ��� �����������

    user_counter = 0;
    m_loginSuccesfull = false;

    authRegButton = ui_Auth.getAuthRegButtonPtr(); // ������������ ����������� ������� ��������� ��������� �� ������ ������ auth.
    connect(authRegButton, &QPushButton::clicked, this, &main_window::registerWindowShow); //��������� ������ �� ��������� ���� � ����� ��������� ������

    regRegButton = ui_Reg.getRegRegButtonPtr(); // ���� �����
    connect(regRegButton, &QPushButton::clicked, this, &main_window::registerUser);

    authAuthButton = ui_Auth.getAuthAuthButtonPtr(); // ���� �����
    connect(authAuthButton, &QPushButton::clicked, this, &main_window::authorizeUser);
 
    connectDB(); // ������ �� � ����������� � ���
    // ������ QSqlQuery query ����� ����� ��� ���� ���������� � ��� ������� � ������� ������ ���������� ���������.

    QSqlQuery query; // ��� ����������� ��������� � ��. ������������� ������ ��������� � ��. � SQlite ����� ���� ������ ���� �������� � �������.

    // ������ �� �������� ������� c ������ userlist
    // ������ ������� � ������ number. PRIMARY KEY (��������� ����) ������ ��������. ������ � ���� ���� �� ������ ���� ������. � ����� ������� ������ ���� ����.
    db_input = "CREATE TABLE userlist ( "               
        "number INTEGER PRIMARY KEY NOT NULL, " // NOT NULL - �������� � ������� �� ����� ���� ������. ���� ��������� NULL �� ����� ����� ��������� ������.    
        "name VARCHAR(20), " // VARCHAR - ���������� ��� ������ � ������ ������ ��������� � ������� � ������.
        "pass VARCHAR(12), "
        "xpos INTEGER, " // INTEGER - ������������� ��� ������
        "ypos INTEGER, "
        "width INTEGER, "
        "length INTEGER );";

    if (!query.exec(db_input)) // exec - ��� ���������� SQL �������. ��������� ������ �������� ������������� ��.
    {
        qDebug() << "Unable to create a table" << query.lastError(); // ���������� ���������� � ��������� ������, ������� ������� ��� ���������� ����� �������.
    }
    else
    {
        qDebug() << "Table was create!";
        qDebug() << "user_counter = " << user_counter;
    }
}

main_window::~main_window()
{
    if (m_loginSuccesfull)
    {
        QString str_t = "UPDATE userlist " // INSERT - ��������� ��������� � �������� �������� ������� 
            "SET xpos = %2, ypos = %3, width = %4, length = %5 " // SET - ����� ������ ����� � �� ����� ��������
            "WHERE name = '%1';"; // WHERE - �������
        db_input = str_t.arg(m_username)
            .arg((this->x())+1) // ���������� ��������� ��������� � ������� ���� � ����. +1 � +30 ���������� �������������� �������� ���� � ������� ����� ��������
            .arg((this->y())+30) // 
            .arg(this->width())
            .arg(this->height());
        QSqlQuery query;
        if (!query.exec(db_input))
        {
            qDebug() << "Unable to insert data" << query.lastError() << " : " << query.lastQuery();
        }
    }
    mw_db.removeDatabase("authorisation"); // ������� ���������� � �� � ������ �� ������.
    qDebug() << "MainWindow Destroyed";
  //  delete ui_Main;
    exit(0);
}

bool main_window::connectDB()
{
    mw_db = QSqlDatabase::addDatabase("QSQLITE"); // ��������� ����� ������������ ������� ��� ����������� � ��
    mw_db.setDatabaseName("authorisation"); // ��������� � ����� �� �����������������. ���� ������ ����� �� ����� �� ���� �� � ��������� ������ ����� ������.

    if (!mw_db.open()) // ��������� ��. ���� �� ��������� �� ����� false
    {
        qDebug() << "Cannot open database: " << mw_db.lastError();
        return false;
    }

    return true;
}




void main_window::test()
{
    qDebug() << "TEST!";
}




void main_window::authorizeUser()
{
    ui_Auth.on_nameLineEdit_textEditedAuth();
    ui_Auth.on_passwordLineEdit_textEditedAuth();

    m_username = ui_Auth.getLogin();
    m_userpass = ui_Auth.getPass();

    if ((m_username == "") || (m_userpass == ""))
    {
        qDebug() << "Login or Password line is empty";
        return;
    }

    QString str_t = " SELECT * "
        " FROM userlist "
        " WHERE name = '%1'";
    // int db_number = 0;

    QString username = "";
    QString userpass = "";

    int xPos = 0;
    int yPos = 0;

    int width = 0;
    int length = 0;

    db_input = str_t.arg(m_username);

    QSqlQuery query;
    QSqlRecord rec;

    if (!query.exec(db_input)) // ������� SQL-������ � ����� ������� exec()
    {
        qDebug() << "Unable to execute query - exiting" << query.lastError() << " : " << query.lastQuery();
    }
    rec = query.record(); // �������� ������ � rec
    query.next(); // ����������� ������ �� ��������� ������
    user_counter = query.value(rec.indexOf("number")).toInt(); // ����� �������� �������� �� ������� ���������� value
    username = query.value(rec.indexOf("name")).toString(); // value - ����������� �������� ���� � QVariant
    userpass = query.value(rec.indexOf("pass")).toString();
    if (m_username != username || m_userpass != userpass)
    {
        qDebug() << "Password missmatch" << username << " " << userpass;
        m_loginSuccesfull = false;
    }
    else
    {
        m_loginSuccesfull = true;
        xPos = query.value(rec.indexOf("xpos")).toInt();
        yPos = query.value(rec.indexOf("ypos")).toInt();
        width = query.value(rec.indexOf("width")).toInt();
        length = query.value(rec.indexOf("length")).toInt();
        ui_Auth.close();
        ui_Reg.close();
        this->setGeometry(xPos, yPos, width, length); // ����� ��������� � ������ ������������ ����.
        this->show();
    }
}

void main_window::registerWindowShow()
{
    ui_Auth.hide();
    ui_Reg.show();
}

void main_window::registerUser()
{
    ui_Reg.on_nameLineEdit_textEdited();
    ui_Reg.on_passwordLineEdit_textEdited();
    ui_Reg.on_confirmLineEdit_textEdited();

    if ((ui_Reg.getName() == "") || (ui_Reg.getPass() == ""))
    {
        qDebug() << "Login or Password line is empty";
        return;
    }

    if (ui_Reg.checkPass())
    {
        QSqlQuery query;
        QSqlRecord rec;
        // SELECT - �������� �������. ������ ������� ����������� �������� (name, age) �� ������� (employees): "SELECT name, age FROM employees;".
        // ��� ������ ���� �������� ������������ �������� *: "SELECT * FROM employees;"
        // COUNT - ������������ ���������� ������� � �������.
        // WHERE - �������� ������ ������� ���� �������� ��� � �����

        QString str_t = "SELECT COUNT(*) " 
            "FROM userlist;";

        db_input = str_t;

        if (!query.exec(db_input)) // query �������� ��������� ������� � exec ���������� true ���� ������� ��� ��������
        {
            qDebug() << "Unable to get number " << query.lastError() << " : " << query.lastQuery();
            return;
        }
        else
        {

            query.next(); // �������� �� ���������� ������ (���� exec ������ 8 �� ��������� � 9-�� ������)

            rec = query.record();// record - ���������� QSqlRecord ���������� ���������� � ���� ��� �������� �������

            // ���� ���������� ����� �������. �������� � "SELECT forename, surname FROM people;" ���� 0 ��� "forename" � ���� 1 ��� "surname".
            user_counter = rec.value(0).toInt(); 
            qDebug() << "user_counter = " << user_counter;
        }


        m_username = ui_Reg.getName();
        m_userpass = ui_Reg.getPass();
        user_counter++;
        qDebug() << "user_counter = " << user_counter << Qt::endl;
        // INSERT + VALUES - ���������� �������� ������ �� ���������� ���������� � VALUES. INTO ��������� ������� � ����� ������ ������� ���������� ������
        str_t = "INSERT INTO userlist(number, name, pass, xpos, ypos, width, length)"
            "VALUES(%1, '%2', '%3', %4, %5, %6, %7);"; // VALUES - ���������� �� �������� ������� ����� ��������� � ������
        db_input = str_t.arg(user_counter) // .arg - �������� ������ ����� � ���������� ������� ������� ��� ���������.
            .arg(m_username)
            .arg(m_userpass)
            .arg(400)
            .arg(400)
            .arg(400)
            .arg(400);

        if (!query.exec(db_input))
        {
            qDebug() << "Unable to insert data" << query.lastError() << " : " << query.lastQuery();
        }
        else
        {
            ui_Reg.hide();
            ui_Auth.show();
        }
    }
    else
    {
        qDebug() << "Confirm your password again";
    }
}

void main_window::display()
{
    ui_Auth.show();
}
