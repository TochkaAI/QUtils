#include "connect_item.h"
#include "ui_connect_item.h"

ConnectItem::ConnectItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnectItem)
{
    ui->setupUi(this);
    ui->labelConnectCount->clear();
}

ConnectItem::~ConnectItem()
{
    delete ui;
}

QString ConnectItem::info() const
{
    return ui->labelInfo->text();
}

void ConnectItem::setInfo(const QString& info)
{
    if (info != ui->labelInfo->text())
        ui->labelInfo->setText(info);
}

void ConnectItem::setHostPoint(const pproto::HostPoint& val)
{
    _hostPoint = val;
    QString s = "%1 : %2";
    ui->labelHostPoint->setText(s.arg(_hostPoint.address().toString()).arg(_hostPoint.port()));
}

//void ConnectItem::setConfigConnectCount(int val)
//{
//    _configConnectCount = val;
//    if (_configConnectCount != 0)
//    {
//        QPixmap pixmap = QPixmap("://resources/connection_lock.png");
//        int size = 20;
//        if (pixmap.height() > size || pixmap.width() > size)
//            pixmap = pixmap.scaled(size, size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
//        ui->labelConnectCount->setPixmap(pixmap);
//    }
//    else
//        ui->labelConnectCount->clear();
//}

void ConnectItem::resetLifeTimer()
{
    _lifeTimer.reset();
}

bool ConnectItem::lifeTimeExpired() const
{
    return (_lifeTimer.elapsed() > 5*1000 /*5 сек*/);
}

//bool ConnectItem::lessThan(Comparator* c) const
//{
//    if (ConnectItem* cw = dynamic_cast<ConnectItem*>(c))
//        return (QString::compare(info(), cw->info(), Qt::CaseInsensitive) < 0);
//    else
//        return true;
//}
