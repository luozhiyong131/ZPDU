/*
 * PduUsrHash.cpp
 * PDU设备用户Hash表
 *  其中使用Hash表，以用户名为Key,用户类为value 进行管理
 *
 *   注意：当组有变动时，请调用del()方法重新加载数据
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pduusrhash.h"

PduUsrHash::PduUsrHash()
{
    mLock = new QReadWriteLock();
}

PduUsrHash::~PduUsrHash()
{
    del();
    delete mLock;
}


/**
 * @brief 增加用户
 * @param name
 */
void PduUsrHash::addUsr(const QString &name)
{
    bool ret = mHash.contains(name);
    if(ret == false)
    {
        PduDevUsr *usr = new PduDevUsr();
        mHash.insert(name, usr);
    }
}

void PduUsrHash::del(void)
{
    QWriteLocker locker(mLock);
    mHash.clear();
}


/**
 * @brief 删除用户
 * @param name
 */
void PduUsrHash::del(const QString &name)
{
    QWriteLocker locker(mLock);
    bool ret = mHash.contains(name);
    if(ret)
        mHash.remove(name);
}

/**
 * @brief 修改用户信息
 * @param usr
 * @param from
 */
void PduUsrHash::setUsr(PduDevUsr *usr,  PduDevUsr *from)
{
    QString str;
    from->usr->get(str);
    usr->usr->set(str); // 修改用户名

    from->pwd->get(str);
    usr->pwd->set(str); // 修改密码

    from->emil->get(str);
    usr->emil->set(str);

    from->emil2->get(str);
    usr->emil2->set(str);

    from->emil3->get(str);
    usr->emil3->set(str);

    from->phone->get(str);
    usr->phone->set(str);

    from->group->get(str);
    usr->group->set(str);
}

void PduUsrHash::setPwd(const QString &name, const QString &pwd)
{
    QWriteLocker locker(mLock);
    addUsr(name);
    mHash[name]->pwd->set(pwd);
}

/**
 * @brief 邮件设置
 * @param name
 * @param id 0 表示第一个邮件地址， 1表示第二个邮件地址
 * @param emil
 */
void PduUsrHash::setEmil(const QString &name, int id, const QString &emil)
{
    QWriteLocker locker(mLock);
    addUsr(name);

    switch (id) {
    case 0:
        mHash[name]->emil->set(emil);
        break;

    case 1:
        mHash[name]->emil2->set(emil);
        break;

    case 2:
        mHash[name]->emil3->set(emil);
        break;
    default:
        break;
    }

}

/**
 * @brief 设置手机号
 * @param name
 * @param phone
 */
void PduUsrHash::setPhone(const QString &name, const QString &phone)
{
    QWriteLocker locker(mLock);
    addUsr(name);
    mHash[name]->phone->set(phone);
}

/**
 * @brief 设置组名
 * @param name
 * @param group
 */
void PduUsrHash::setGroup(const QString &name, const QString &group)
{
    QWriteLocker locker(mLock);
    addUsr(name);
    mHash[name]->group->set(group);
}

/**
 * @brief 用户数量
 * @return
 */
int PduUsrHash::size(void)
{
    QReadLocker locker(mLock);
    return mHash.size();
}

/**
 * @brief 获取用户列表
 * @param usr
 * @return
 */
int PduUsrHash::getUsr(QStringList &usr)
{
    QReadLocker locker(mLock);
    QHash<QString, PduDevUsr*>::const_iterator i;
    for(i=mHash.constBegin(); i!= mHash.constEnd(); ++i)
        usr << i.key();

    return mHash.size();
}

/**
 * @brief 获取用户信息
 * @param name
 * @param usr
 * @return
 */
PduDevUsr *PduUsrHash::get(const QString &name)
{
    PduDevUsr *usr = NULL;

    QReadLocker locker(mLock);
    QHash<QString, PduDevUsr*>::const_iterator it;
    it = mHash.constFind(name);
    if(it != mHash.constEnd())
        usr = it.value();

    return usr;
}
