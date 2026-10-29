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
