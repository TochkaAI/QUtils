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
