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

#include "connect_editor.h"
#include "ui_connect_editor.h"

#include "shared/config/appl_conf.h"

ConnectEditor::ConnectEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectEditor)
{
    ui->setupUi(this);
}

ConnectEditor::~ConnectEditor()
{
    delete ui;
}

bool ConnectEditor::init(const QString& info, const pproto::HostPoint& hostPoint)
{
    ui->lineInfo->setText(info);
    ui->lineAddress->setText(hostPoint.address().toString());
    ui->sboxPort->setValue(hostPoint.port());

    loadGeometry();
    return true;
}

void ConnectEditor::deinit()
{
    saveGeometry();
}

QString ConnectEditor::info() const
{
    return ui->lineInfo->text();
}

pproto::HostPoint ConnectEditor::hostPoint() const
{
    return {QHostAddress(ui->lineAddress->text()), ui->sboxPort->value()};
}

void ConnectEditor::done(int r)
{
    QDialog::done(r);
}

void ConnectEditor::loadGeometry()
{
    QVector<int> v {10, 10, 350, 160};
    if (!config::base().getValue("windows.connect_editor.geometry", v))
    {
        setGeometry(0, 0, v[2], v[3]);
        QRect parentGeometry = static_cast<QWidget*>(parent())->geometry();
        move(parentGeometry.center() - geometry().center());
    }
    else
        setGeometry(v[0], v[1], v[2], v[3]);
}

void ConnectEditor::saveGeometry()
{
    QRect g = geometry();
    QVector<int> v {g.x(), g.y(), g.width(), g.height()};
    config::base().setValue("windows.connect_editor.geometry", v);
}
