#ifndef ZEBRDATASQL_H
#define ZEBRDATASQL_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class ZebrDataSqlBasic : public QObject
{
    Q_OBJECT

public:
    QSqlDatabase m_database;

    ZebrDataSqlBasic();
    ~ZebrDataSqlBasic();

private:
    int create_sql_database();

};

typedef struct zebra_channel_item
{
    zebra_channel_item():id(-1),channel_state(0){}
    int id,channel_state;
    QByteArray channel_id;
    QString  channel_name;
}DbZebraChannelItem;
Q_DECLARE_METATYPE(DbZebraChannelItem)

class ZebraChannelDb : public ZebrDataSqlBasic
{
public:
    ~ZebraChannelDb();
    static ZebraChannelDb* get_instance();
    int insert_to_zebra_channel(const DbZebraChannelItem item);
    int remove_from_zebra_channel(const DbZebraChannelItem item);
    int update_to_zebra_channel(const DbZebraChannelItem item);
    QVector<DbZebraChannelItem> select_zebra_channel_by_chennel_id(const QByteArray channel_id);
    QVector<DbZebraChannelItem> select_zebra_channel_by_chennel_name(const QString channel_name);
    QVector<DbZebraChannelItem> get_all_zerba_chennel();

private:
    static ZebraChannelDb* m_zebra_channel_db;

    ZebraChannelDb();

};

typedef struct zebra_key_data
{
    zebra_key_data():id(-1){}
    int id;
    QByteArray key;
    QByteArray data;
}ZebraKeyItem;
Q_DECLARE_METATYPE(ZebraKeyItem)

class ZebraKeyDataDb : public ZebrDataSqlBasic
{
public:
    static ZebraKeyDataDb* get_instance();
    ~ZebraKeyDataDb();
    int insert_to_zebra_key_data(const ZebraKeyItem item);
    int delete_from_zebra_key_data(const ZebraKeyItem item);
    QVector<ZebraKeyItem> select_zebra_key_data_by_key(const QByteArray &key);

private:
    static ZebraKeyDataDb* m_zebra_key_data;

    ZebraKeyDataDb();

};

#endif // ZEBRDATASQL_H
