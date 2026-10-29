/* clang-format off */
/*****************************************************************************
  The MIT License

  Copyright © 2026 TochkaAI

  Permission is hereby granted, free of charge, to any person obtaining
  a copy of this software and associated documentation files (the
  "Software"), to deal in the Software without restriction, including
  without limitation the rights to use, copy, modify, merge, publish,
  distribute, sublicense, and/or sell copies of the Software, and to
  permit persons to whom the Software is furnished to do so, subject to
  the following conditions:

  The above copyright notice and this permission notice shall be included
  in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*****************************************************************************/

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
