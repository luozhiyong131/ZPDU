#include "zebrdatasql.h"

#include <QDir>
#include <QDebug>

#include "zebra/common_debug.h"

ZebraChannelDb* ZebraChannelDb::m_zebra_channel_db = nullptr;
ZebraKeyDataDb* ZebraKeyDataDb::m_zebra_key_data = nullptr;

ZebrDataSqlBasic::ZebrDataSqlBasic()
{
    create_sql_database();
}

ZebrDataSqlBasic::~ZebrDataSqlBasic()
{
    m_database.close();
}

int ZebrDataSqlBasic::create_sql_database()
{
    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        m_database = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        QString file_name = "ZebraData.db";
        QDir dataDir = QDir::home();
        QString dirName = ".CleverDIPS";
        if(!dataDir.exists(dirName))
        {
            dataDir.mkdir(dirName);
        }
        dataDir.cd(dirName);

        m_database = QSqlDatabase::addDatabase("QSQLITE");
        m_database.setDatabaseName(dataDir.absoluteFilePath(file_name));
        m_database.setUserName("peercome");
        m_database.setPassword("123456");
    }

    if (!m_database.open())
    {
        MY_DEBUG(3, RED) << "Error: Failed to connect database." << DEBUG_END;
        qDebug() << m_database.lastError();

        return -1;
    }

    return 0;
}



ZebraChannelDb::ZebraChannelDb()
{
    QString sql_cmd =
            "create table if not exists zebra_channels("
            "id                        INTEGER primary key autoincrement not null,"
            "channel_state             INTEGER,"
            "channel_id                BLOB not null,"
            "channel_name              TEXT);";

    QSqlQuery sql_query(m_database);
    sql_query.prepare(sql_cmd);
    if(!sql_query.exec())
    {
        qDebug() << "Fail to create zebra channel table : " << sql_query.lastError();
    }
}

ZebraChannelDb::~ZebraChannelDb()
{

}

ZebraChannelDb* ZebraChannelDb::get_instance()
{
    if(nullptr == m_zebra_channel_db)
    {
        m_zebra_channel_db = new ZebraChannelDb;
    }

    return m_zebra_channel_db;
}

int ZebraChannelDb::insert_to_zebra_channel(const DbZebraChannelItem item)
{
    QSqlQuery sql_query(m_database);
    QString sql_cmd = "insert into zebra_channels values "
                      "(null, ?, ?, ?)";
    sql_query.prepare(sql_cmd);
    sql_query.addBindValue(item.channel_state);
    sql_query.addBindValue(item.channel_id);
    sql_query.addBindValue(item.channel_name);
    if(!sql_query.exec())
    {
        qDebug() << "insert_to_zebra_channel() - " << sql_query.lastError();
    }

    return 0;
}

int ZebraChannelDb::remove_from_zebra_channel(const DbZebraChannelItem item)
{
    QSqlQuery sql_query(m_database);
    QString delete_sql = "delete from zebra_channels where id = :id";
    sql_query.prepare(delete_sql);
    sql_query.bindValue(":id", item.id);
    if(!sql_query.exec())
    {
        qDebug() << "remove_from_zebra_channel() - " << sql_query.lastError();
    }

    return 0;
}

int ZebraChannelDb::update_to_zebra_channel(const DbZebraChannelItem item)
{
    QSqlQuery sql_query(m_database);
    QString update_sql = "update zebra_channels set "
                         "channel_id = :channel_id,"
                         "channel_name = :channel_name,"
                         "channel_state=:channel_state "
                         "where id = :id";
    sql_query.prepare(update_sql);
    sql_query.bindValue(":channel_id", item.channel_id);
    sql_query.bindValue(":channel_name", item.channel_name);
    sql_query.bindValue(":channel_state", item.channel_state);
    sql_query.bindValue(":id", item.id);
    if(!sql_query.exec())
    {
        qDebug() << "update_to_zebra_channel() - " << sql_query.lastError();
    }

    return 0;
}

QVector<DbZebraChannelItem> ZebraChannelDb::select_zebra_channel_by_chennel_id(const QByteArray channel_id)
{
    QVector<DbZebraChannelItem> items;
    QSqlQuery sql_query(m_database);
    QString select_sql = "SELECT * from zebra_channels where channel_id = :channel_id";
    sql_query.prepare(select_sql);
    sql_query.bindValue(":channel_id", channel_id);
    if(sql_query.exec())
    {
        while(sql_query.next())
        {
            DbZebraChannelItem item;
            item.id = sql_query.value("id").toInt();
            item.channel_id = sql_query.value("channel_id").toByteArray();
            item.channel_name = sql_query.value("channel_name").toString();
            item.channel_state = sql_query.value("channel_state").toInt();
            items.append(item);
        }
    }
    else
    {
        qDebug() << "select_zebra_channel_by_chennel_id() - " << sql_query.lastError();
    }

    return items;
}

QVector<DbZebraChannelItem> ZebraChannelDb::select_zebra_channel_by_chennel_name(const QString channel_name)
{
    QVector<DbZebraChannelItem> items;
    QSqlQuery sql_query(m_database);
    QString select_sql = "SELECT * from zebra_channels where channel_name = :channel_name";
    sql_query.prepare(select_sql);
    sql_query.bindValue(":channel_name", channel_name);
    if(sql_query.exec())
    {
        while(sql_query.next())
        {
            DbZebraChannelItem item;
            item.id = sql_query.value("id").toInt();
            item.channel_id = sql_query.value("channel_id").toByteArray();
            item.channel_name = sql_query.value("channel_name").toString();
            item.channel_state = sql_query.value("channel_state").toInt();
            items.append(item);
        }
    }
    else
    {
        qDebug() << "select_zebra_channel_by_chennel_name() - " << sql_query.lastError();
    }

    return items;
}

QVector<DbZebraChannelItem> ZebraChannelDb::get_all_zerba_chennel()
{
    QVector<DbZebraChannelItem> items;
    QSqlQuery sql_query(m_database);
    QString select_sql = "SELECT * from zebra_channels";
    sql_query.prepare(select_sql);
    if(sql_query.exec())
    {
        while(sql_query.next())
        {
            DbZebraChannelItem item;
            item.id = sql_query.value("id").toInt();
            item.channel_id = sql_query.value("channel_id").toByteArray();
            item.channel_name = sql_query.value("channel_name").toString();
            item.channel_state = sql_query.value("channel_state").toInt();
            items.append(item);
        }
    }
    else
    {
        qDebug() << "get_all_zerba_chennel() - " << sql_query.lastError();
    }

    return items;
}

ZebraKeyDataDb::ZebraKeyDataDb()
{
    QString sql_cmd = "create table if not exists zebra_key_data("
                      "id                INTEGER primary key autoincrement not null,"
                      "key                BLOB not null,"
                      "data               BLOB);";

    QSqlQuery sql_query(m_database);
    sql_query.prepare(sql_cmd);
    if(!sql_query.exec())
    {
        qDebug() << "Fail to create zebra key data table : " << sql_query.lastError();
    }
}

ZebraKeyDataDb::~ZebraKeyDataDb()
{

}

ZebraKeyDataDb* ZebraKeyDataDb::get_instance()
{
    if(nullptr == m_zebra_key_data)
    {
        m_zebra_key_data = new ZebraKeyDataDb;
    }

    return m_zebra_key_data;
}

int ZebraKeyDataDb::insert_to_zebra_key_data(const ZebraKeyItem item)
{
    QSqlQuery sql_query(m_database);
    QString sql_cmd = "insert into zebra_key_data values "
                      "(null, ?, ?)";
    sql_query.prepare(sql_cmd);
    sql_query.addBindValue(item.key);
    sql_query.addBindValue(item.data);
    if(!sql_query.exec())
    {
        qDebug() << "insert_to_zebra_key_data() - " << sql_query.lastError();
    }

    return 0;
}

int ZebraKeyDataDb::delete_from_zebra_key_data(const ZebraKeyItem item)
{
    QSqlQuery sql_query(m_database);
    QString delete_sql = "delete from zebra_key_data where id = :id";
    sql_query.prepare(delete_sql);
    sql_query.bindValue(":id", item.id);
    if(!sql_query.exec())
    {
        qDebug() << "delete_from_zebra_key_data() - " << sql_query.lastError();
    }

    return 0;
}

QVector<ZebraKeyItem> ZebraKeyDataDb::select_zebra_key_data_by_key(const QByteArray &key)
{
    QVector<ZebraKeyItem> items;
    QSqlQuery sql_query(m_database);
    QString select_sql = "SELECT * from zebra_key_data where key = :key";
    sql_query.prepare(select_sql);
    sql_query.bindValue(":key", key);
    if(sql_query.exec())
    {
        while(sql_query.next())
        {
            ZebraKeyItem item;
            item.id = sql_query.value("id").toInt();
            item.key = sql_query.value("key").toByteArray();
            item.data = sql_query.value("data").toByteArray();
            items.append(item);
        }
    }
    else
    {
        qDebug() << "select_zebra_key_data_by_key() - " << sql_query.lastError();
    }

    return items;
}

