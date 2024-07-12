#pragma once

#ifndef ONLY_STATIC_CONNECTION
#include "shared/qt/network/interfaces.h"
#include "pproto/func_invoker.h"
#endif

#include "pproto/transport/tcp.h"

#include <QDialog>
#include <QCloseEvent>
#include <functional>

using namespace std;
using namespace pproto;
using namespace pproto::transport;

class ConnectionWidget;

namespace Ui {
class ConnectWindow;
}

struct ConnectItemData
{
    QString info;
    QHostAddress addr = {QHostAddress::LocalHost};
    int port = {0};
};

class ConnectWindow : public QDialog
{
public:
    explicit ConnectWindow(QWidget *parent = 0);
    ~ConnectWindow();

    bool init(tcp::Socket::Ptr socket, int port, const QString& prefix = {},
              const QString caption = {});
    void deinit();

private slots:
    void message(const pproto::Message::Ptr&);
    //void socketConnected(pproto::SocketDescriptor);
    //void socketDisconnected(pproto::SocketDescriptor);

    void on_btnConnect_clicked(bool checked);
    void on_tabWidget_currentChanged(int index);

    void on_btnAddConn_clicked(bool);
    void on_btnEditConn_clicked(bool);
    void on_btnDeleteConn_clicked(bool);

private:
    Q_OBJECT
    void timerEvent(QTimerEvent* event) override;

    void loadGeometry();
    void saveGeometry();

    void loadStaticList();
    void saveStaticList();

    //--- Обработчики команд ---
    void command_CtlDiscovery(const Message::Ptr&);

    void requestList();
    void updateList();

private:
    Ui::ConnectWindow *ui;

    int _timerId = {-1};
    int _port = {0};
    QString _prefix;

    tcp::Socket::Ptr _socket;

#ifndef ONLY_STATIC_CONNECTION
    FunctionInvoker  _funcInvoker;
    network::Interface::List _netInterfaces;
#endif
};
