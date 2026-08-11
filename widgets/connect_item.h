#pragma once

//#include "comparator.h"
#include "shared/steady_timer.h"
#include "shared/qt/quuidex.h"
#include "pproto/host_point.h"

#include <QWidget>

namespace Ui {
class ConnectItem;
}

class ConnectItem : public QWidget /*Comparator*/
{
public:
    explicit ConnectItem(QWidget *parent = 0);
    ~ConnectItem();

    QString info() const;
    void setInfo(const QString& info);

    QUuidEx applId() const {return _applId;}
    void setApplId(const QUuidEx& val) {_applId = val;}

    QUuidEx procId() const {return _procId;}
    void setProcId(const QUuidEx& val) {_procId = val;}

    pproto::HostPoint hostPoint() const {return _hostPoint;}
    void setHostPoint(const pproto::HostPoint&);

    void resetLifeTimer();
    bool lifeTimeExpired() const;

    //bool lessThan(Comparator*) const override;

private:
    Q_OBJECT
    Ui::ConnectItem *ui;

    QUuidEx _applId;
    QUuidEx _procId;
    steady_timer _lifeTimer;
    pproto::HostPoint _hostPoint;
};
