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

#include "pproto/host_point.h"

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class ConnectEditor;
}

class ConnectEditor : public QDialog
{

public:
    explicit ConnectEditor(QWidget *parent = nullptr);
    ~ConnectEditor();

    //bool init(const ConnectItemData&, bool isEdit);
    bool init(const QString& info, const pproto::HostPoint&);
    void deinit();

    QString info() const;
    //void setInfo(const QString& info);

    pproto::HostPoint hostPoint() const;
    //void setHostPoint(const pproto::HostPoint&);

//signals:
//    void connectionSaved(const ConnectItemData&);

//private slots:
//    void on_buttonBox_accepted();
//    void on_buttonBox_rejected();

//    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Q_OBJECT
    void done(int r) override;

    void loadGeometry();
    void saveGeometry();

private:
    Ui::ConnectEditor *ui;

//    bool _isEdit = {false};
//    ConnectItemData _ciData;
};
