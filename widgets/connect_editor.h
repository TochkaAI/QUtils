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
