#include <QtSql>
#include <QJsonValue>

int main()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");     // Грузим драйвер для работы с SqLite
    db.setDatabaseName("test_bd.sqlite");                       // Открываем файл с бд под названием "test_bd.sqlite"

    if (!db.open()) {
        qDebug() << "Не удалось открыть базу данных";
        return -1;
    }

    QSqlQuery query;            // Объект для отправки запросов в базу

    if(db.tables().isEmpty()){  // Если в базе нет таблицы, то создаём её
        QString str = "CREATE TABLE test_table ("
                "id             INTEGER PRIMARY KEY NOT NULL, "
                "description    TEXT, "
                "answer         TEXT, "
                "data           JSON);";

        if (!query.exec(str)) {     // Отправляем запрос на создание таблицы
            qDebug() << "Не удалось создать таблицу";
        }
    }


    // Подготавливаем данные для вставки
    QString str_insert = "INSERT INTO test_table(id, description, answer, data) "
            "VALUES (%1, '%2', '%3', '%4');";
    // В строку "str_insert" вставляем вместо (%1, '%2', '%3', '%4') след. значения:
    QString str = str_insert.arg("1")
                            .arg("Типо описание задачи")
                            .arg("102021")
                            .arg("{}");

    if (!query.exec(str)) { // Отправляем данный запрос в бд
        qDebug() << "Не удалось вставить данные";
    }


    query.exec("SELECT * FROM test_table");     // Отправляем запрос на просмотр таблицы
    QSqlRecord rec = query.record();            // Объект который получает данные о таблице

    // Переменные в которые будем сохранять ответы
    int id;
    QString description;
    QString answer;
    QJsonValue data;

    while (query.next()) {      // Идём по строкам таблицы и получаем значения
        id = query.value(rec.indexOf("id")).toInt();
        description = query.value(rec.indexOf("description")).toString();
        answer = query.value(rec.indexOf("answer")).toString();
        data = query.value(rec.indexOf("data")).toJsonValue();

        qDebug() << "id:" << id;
        qDebug() << "description: " << description;
        qDebug() << "answer" << answer;
        qDebug() << "data" << data.toString();
    }

    return 0;
}
