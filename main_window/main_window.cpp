#include "main_window.h"
#include "ui_main_window.h"
#include "auth_window.h"
#include "reg_window.h"
#include <QtDebug>

main_window::main_window(QWidget *parent)
    : QMainWindow(parent)
{
    ui_Main.setupUi(this); // дефолтная функция для отображения GUI при использовании Qt Designer

    ui_Auth.show(); // сразу отображаем окно для авторизации

    user_counter = 0;
    m_loginSuccesfull = false;

    authRegButton = ui_Auth.getAuthRegButtonPtr(); // приравниваем присваиваем пустому указателю указатель на кнопку класса auth.
    connect(authRegButton, &QPushButton::clicked, this, &main_window::registerWindowShow); //коннектим кнопку из указателя выше с нашим кастомным слотом

    regRegButton = ui_Reg.getRegRegButtonPtr(); // тоже самое
    connect(regRegButton, &QPushButton::clicked, this, &main_window::registerUser);

    authAuthButton = ui_Auth.getAuthAuthButtonPtr(); // тоже самое
    connect(authAuthButton, &QPushButton::clicked, this, &main_window::authorizeUser);
 
    connectDB(); // создаём БД и подключение к ней
    // Видимо QSqlQuery query сразу знает что надо обращаться к той таблице к которой сейчас подключена программа.

    QSqlQuery query; // для возможности обращения к БД. Инкапсулирует методы обращения к БД. В SQlite может быть только один оператор в запросе.

    // запрос на создание таблицы c именем userlist
    // создаём столбец с именем number. PRIMARY KEY (первичный ключ) всегда уникален. Запись в этом поле не должна быть пустой. В одной таблице только один ключ.
    db_input = "CREATE TABLE userlist ( "               
        "number INTEGER PRIMARY KEY NOT NULL, " // NOT NULL - значение в таблице не может быть пустым. Если поставить NULL то можно будет оставлять пустым.    
        "name VARCHAR(20), " // VARCHAR - символьный тип данных с длиной строки указанной в скобках в байтах.
        "pass VARCHAR(12), "
        "xpos INTEGER, " // INTEGER - целочисленный тип данных
        "ypos INTEGER, "
        "width INTEGER, "
        "length INTEGER );";

    if (!query.exec(db_input)) // exec - для выполнения SQL запроса. Синтаксис должен отвечать запрашиваемой БД.
    {
        qDebug() << "Unable to create a table" << query.lastError(); // Возвращаем информацию о последней ошибке, таковая имеется при выполнении этого запроса.
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
        QString str_t = "UPDATE userlist " // INSERT - обновляем знаачения в заданных столбцах таблицы 
            "SET xpos = %2, ypos = %3, width = %4, length = %5 " // SET - задаём список полей и их новые значения
            "WHERE name = '%1';"; // WHERE - условие
        db_input = str_t.arg(m_username)
            .arg((this->x())+1) // записываем последнее положение и размеры окна в базу. +1 и +30 нивелирует необоснованное смещение окна в сторону после закрытия
            .arg((this->y())+30) // 
            .arg(this->width())
            .arg(this->height());
        QSqlQuery query;
        if (!query.exec(db_input))
        {
            qDebug() << "Unable to insert data" << query.lastError() << " : " << query.lastQuery();
        }
    }
    mw_db.removeDatabase("authorisation"); // удаляет соединение с БД с именем из скобок.
    qDebug() << "MainWindow Destroyed";
  //  delete ui_Main;
    exit(0);
}

bool main_window::connectDB()
{
    mw_db = QSqlDatabase::addDatabase("QSQLITE"); // указываем какой использовать драйвер для подключения к БД
    mw_db.setDatabaseName("authorisation"); // Указываем с какой БД взаимодействовать. Если такого имени не найдёт то файл БД с указанным именем будет создан.

    if (!mw_db.open()) // открываем БД. Если не открывает то вернёт false
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

    if (!query.exec(db_input)) // передаём SQL-запрос в через функцию exec()
    {
        qDebug() << "Unable to execute query - exiting" << query.lastError() << " : " << query.lastQuery();
    }
    rec = query.record(); // передлаём запрос в rec
    query.next(); // передвигает запись на следующую запись
    user_counter = query.value(rec.indexOf("number")).toInt(); // чтобы получить значение из запроса используем value
    username = query.value(rec.indexOf("name")).toString(); // value - возвращщает значение поля в QVariant
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
        this->setGeometry(xPos, yPos, width, length); // задаём положение и размер создаваемого окна.
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
        // SELECT - оператор запроса. Пример запроса определённых столбцов (name, age) из таблицы (employees): "SELECT name, age FROM employees;".
        // Для выбора всех столбцов используется оператор *: "SELECT * FROM employees;"
        // COUNT - подсчитывает количество записей в таблице.
        // WHERE - позволит задать условие если дописать его в конце

        QString str_t = "SELECT COUNT(*) " 
            "FROM userlist;";

        db_input = str_t;

        if (!query.exec(db_input)) // query получает результат запроса и exec возвращает true если запросы был выполнен
        {
            qDebug() << "Unable to get number " << query.lastError() << " : " << query.lastQuery();
            return;
        }
        else
        {

            query.next(); // преходим на следующиую строку (если exec вернул 8 то переходим к 9-ой строке)

            rec = query.record();// record - возвращает QSqlRecord содержащий информацию о поле для текущего запроса

            // поля нумеруются слева направо. Например в "SELECT forename, surname FROM people;" поле 0 это "forename" а поле 1 это "surname".
            user_counter = rec.value(0).toInt(); 
            qDebug() << "user_counter = " << user_counter;
        }


        m_username = ui_Reg.getName();
        m_userpass = ui_Reg.getPass();
        user_counter++;
        qDebug() << "user_counter = " << user_counter << Qt::endl;
        // INSERT + VALUES - определяет одинокую строку со значениями описанными в VALUES. INTO позволяет указать в какую именно таблицу произвести запись
        str_t = "INSERT INTO userlist(number, name, pass, xpos, ypos, width, length)"
            "VALUES(%1, '%2', '%3', %4, %5, %6, %7);"; // VALUES - определяет те значения которые будут записаниы в строку
        db_input = str_t.arg(user_counter) // .arg - заменяет маркер места с наименьшим номером строкой или значением.
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
