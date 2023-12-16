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
