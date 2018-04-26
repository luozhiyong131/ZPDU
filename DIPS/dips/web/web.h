#ifndef WEB_H
#define WEB_H

#include <QObject>

#ifdef ZEBRA_MODULE
#include "zebra/signaldispatch.h"
#endif

class WEB : public QObject
{
    Q_OBJECT
public:
    explicit WEB(QObject *parent = nullptr);
    static WEB *getInstance();

signals:

#ifdef ZEBRA_MODULE
public slots:
    void onUpdateZebraSecurityList(std::map<QString, zebra_security_data> mapData);
#endif

private:
    static WEB *mInstance;
};

#endif // WEB_H
