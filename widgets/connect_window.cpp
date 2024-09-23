#include "connect_window.h"
#include "ui_connect_window.h"

#include "connect_item.h"
#include "connect_editor.h"

#include "shared/defmac.h"
#include "shared/spin_locker.h"
#include "shared/logger/logger.h"
#include "shared/logger/format.h"
#include "shared/config/appl_conf.h"
#include "shared/qt/logger_operators.h"
#include "shared/qt/network/interfaces.h"

#include "pproto/commands/pool.h"
#include "pproto/transport/udp.h"

#ifndef ONLY_STATIC_CONNECTION
#include "commands/discovery.h"
#endif

#include "message_box.h"

#include <QCloseEvent>
#include <QInputDialog>
#include <unistd.h>

#define KILL_TIMER(TIMER) {if (TIMER != -1) {killTimer(TIMER); TIMER = -1;}}

ConnectWindow::ConnectWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectWindow)
{
    Q_INIT_RESOURCE(qutils);

    ui->setupUi(this);
    ui->btnConnect->setEnabled(false);

    //setWindowTitle(qApp->applicationName());
    //setFixedSize(size());
    //setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    //setWindowState(windowState() & ~(Qt::WindowMaximized | Qt::WindowFullScreen));
    //setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint|Qt::MSWindowsFixedSizeDialogHint);

#ifndef ONLY_STATIC_CONNECTION
    chk_connect_q(&udp::socket(), &udp::Socket::message,
                  this, &ConnectWindow::message)

    #define FUNC_REGISTRATION(COMMAND) \
        _funcInvoker.registration(command:: COMMAND, &ConnectWindow::command_##COMMAND, this);

    FUNC_REGISTRATION(CtlDiscovery)

    #undef FUNC_REGISTRATION

#else
    ui->tabWidget->setTabEnabled(0, false);
    ui->tabWidget->setCurrentIndex(1);
#endif
}

ConnectWindow::~ConnectWindow()
{
    delete ui;
}

bool ConnectWindow::init(tcp::Socket::Ptr socket, int port, const QString& prefix,
                         const QString caption)
{
    _socket = socket;
    if (!caption.isEmpty())
        setWindowTitle(caption);

#ifndef ONLY_STATIC_CONNECTION
    network::Interface::List nl = network::getInterfaces();
    _netInterfaces.swap(nl);

    _port = port;
    _prefix = prefix;
    //config::base().getValue("connection.port", _port);

    int attempts = 1;
    while (attempts <= 5)
    {
        if (!udp::socket().init({QHostAddress::AnyIPv4, _port + attempts}))
            return false;

        udp::socket().start();

        int cnt = 3;
        while (cnt--)
        {
            QThread::msleep(50);
            if (udp::socket().isBound())
                break;
        }
        if (udp::socket().isBound())
            break;

        ++attempts;
        udp::socket().stop();
    }
    if (attempts > 5)
    {
        log_error << "The number attempts of initialization UDP is exhausted";
        return false;
    }

    requestList();
    _timerId = startTimer(2*1000);
#endif

    loadGeometry();
    loadStaticList();

    return true;
}

void ConnectWindow::deinit()
{
#ifndef ONLY_STATIC_CONNECTION
    KILL_TIMER(_timerId)
    udp::socket().stop();
#endif
    saveGeometry();
}

void ConnectWindow::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == _timerId)
    {
        requestList();
        updateList();
    }
}

void ConnectWindow::loadGeometry()
{
    QVector<int> v {10, 10, 280, 350};
    if (!config::base().getValue("windows.connect_window.geometry", v))
    {
        QRect parentGeometry = static_cast<QWidget*>(parent())->geometry();
        move(parentGeometry.center() - geometry().center());
    }
    else
        setGeometry(v[0], v[1], v[2], v[3]);

#ifndef ONLY_STATIC_CONNECTION
    QString tabIndexStr = "windows.connect_window.";
    if (!_prefix.isEmpty())
        tabIndexStr += _prefix + ".tab_index";
    else
        tabIndexStr += "tab_index";

    int tabIndex = 0;
    config::base().getValue(tabIndexStr.toStdString(), tabIndex);
    ui->tabWidget->setCurrentIndex(tabIndex);
#endif
}

void ConnectWindow::saveGeometry()
{
    QRect g = geometry();
    QVector<int> v {g.x(), g.y(), g.width(), g.height()};
    config::base().setValue("windows.connect_window.geometry", v);


    QString tabIndexStr = "windows.connect_window.";
    if (!_prefix.isEmpty())
        tabIndexStr += _prefix + ".tab_index";
    else
        tabIndexStr += "tab_index";

    int tabIndex = ui->tabWidget->currentIndex();
    config::base().setValue(tabIndexStr.toStdString(), tabIndex);
}

void ConnectWindow::loadStaticList()
{
    ui->listStatic->clear();

    YamlConfig::Func loadFunc = [this](YamlConfig* conf, YAML::Node& nodes, bool logWarn)
    {
        for (const YAML::Node& node : nodes)
        {
            QString addrStr;
            conf->getValue(node, "address", addrStr, logWarn);
            if (addrStr.isEmpty())
                continue;

            QHostAddress netAddr = QHostAddress(addrStr);
            if (netAddr.isNull())
            {
                log_error << "Failed to create QHostAddress from " << addrStr;
                continue;
            }

            QString portStr;
            conf->getValue(node, "port", portStr, logWarn);
            if (portStr.isEmpty())
                continue;

            bool ok;
            int port = portStr.toInt(&ok);
            if (!ok)
            {
                log_error << "Port incorrect";
                continue;
            }

            QString info {u8"Пусто"};
            conf->getValue(node, "info", info, logWarn);

            ConnectItem* ci = new ConnectItem();
            ci->setInfo(info);
            ci->setHostPoint({netAddr, port});
            QListWidgetItem* lwi = new QListWidgetItem(/*ci*/);
            lwi->setSizeHint(ci->sizeHint());
            ui->listStatic->addItem(lwi);
            ui->listStatic->setItemWidget(lwi, ci);
        }
        return true;
    };

    QString connectStr = "static_connections";
    if (!_prefix.isEmpty())
        connectStr = _prefix + ".static_connections";

    config::base().getValue(connectStr.toStdString(), loadFunc);

    //ui->listCtl->sortItems();
    on_tabWidget_currentChanged(ui->tabWidget->currentIndex());
}

void ConnectWindow::saveStaticList()
{
    YamlConfig::Func saveFunc = [this](YamlConfig* conf, YAML::Node& node, bool)
    {
        for (int i = 0; i < ui->listStatic->count(); ++i)
        {
            QListWidgetItem* lwi = ui->listStatic->item(i);
            if (ConnectItem* ci =
                  qobject_cast<ConnectItem*>(ui->listStatic->itemWidget(lwi)))
            {
                YAML::Node n;
                conf->setValue(n, "info",    ci->info());
                conf->setValue(n, "address", ci->hostPoint().address().toString());
                conf->setValue(n, "port",    ci->hostPoint().port());
                node.push_back(n);
            }
        }
        return true;
    };

    QString connectStr = "static_connections";
    if (!_prefix.isEmpty())
        connectStr = _prefix + ".static_connections";

    config::base().remove(connectStr.toStdString());
    config::base().setValue(connectStr.toStdString(), saveFunc);
    config::base().saveFile();
}

void ConnectWindow::message(const pproto::Message::Ptr& message)
{
#ifndef ONLY_STATIC_CONNECTION
    if (message->processed())
        return;

    if (lst::FindResult fr = _funcInvoker.findCommand(message->command()))
    {
        if (command::pool().commandIsSinglproc(message->command()))
            message->markAsProcessed();
        _funcInvoker.call(message, fr);
    }
#endif
}

void ConnectWindow::on_btnConnect_clicked(bool /*checked*/)
{
    QListWidgetItem* lwi;
    ConnectItem* ci = nullptr;
    if (ui->tabWidget->currentIndex() == 0)
    {
        lwi = ui->listDynamic->currentItem();
        ci = qobject_cast<ConnectItem*>(ui->listDynamic->itemWidget(lwi));
    }
    if (ui->tabWidget->currentIndex() == 1)
    {
        lwi = ui->listStatic->currentItem();
        ci = qobject_cast<ConnectItem*>(ui->listStatic->itemWidget(lwi));
    }
    if (!ci) return;

    if (_socket->isConnected())
        _socket->disconnect();

    QString msg;
    if (!_socket->init(ci->hostPoint()))
    {
        msg = u8"Не удалось инициализировать сокет для подключения.  "
              u8"Подробную информацию смотри в лог-файле.";
        messageBox(this, QMessageBox::Icon::Warning, msg);
        return;
    }

    ui->btnConnect->setEnabled(false);
    setCursor(Qt::WaitCursor);

    _socket->connect();

    int attempts = 0;
    while (attempts++ < 60 /*ждем 6 сек*/)
    {
        qApp->processEvents();
        QThread::msleep(100 /*0.1 сек*/);
        if (_socket->isConnected())
            break;
    }
    setCursor(Qt::ArrowCursor);
    on_tabWidget_currentChanged(ui->tabWidget->currentIndex());

    if (!_socket->isConnected())
    {
        _socket->stop();

        msg = u8"Не удалось подключиться к хосту %1:%2";
        msg = msg.arg(ci->hostPoint().address().toString())
                 .arg(ci->hostPoint().port());
        messageBox(this, QMessageBox::Icon::Warning, msg);
        return;
    }
    accept();
}

void ConnectWindow::on_tabWidget_currentChanged(int index)
{
    if (index == 0)
        ui->btnConnect->setEnabled(ui->listDynamic->count());

    if (index == 1)
        ui->btnConnect->setEnabled(ui->listStatic->count());

    if (!ui->listDynamic->currentItem() && ui->listDynamic->count())
        ui->listDynamic->setCurrentRow(0);

    if (!ui->listStatic->currentItem() && ui->listStatic->count())
        ui->listStatic->setCurrentRow(0);
}

void ConnectWindow::requestList()
{
#ifndef ONLY_STATIC_CONNECTION
    const int attempts = 5;

    // Отправка на localhost
    for (int a = 0; a < attempts; ++a)
    {
        Message::Ptr message = createMessage(command::CtlDiscovery);
        message->appendDestinationPoint({QHostAddress::LocalHost, _port + a});
        udp::socket().send(message);
    }

    for (network::Interface* intf : _netInterfaces)
        for (int a = 0; a < attempts; ++a)
        {
            if (intf->canBroadcast() && !intf->isPointToPoint())
            {
                Message::Ptr message = createMessage(command::CtlDiscovery);
                message->appendDestinationPoint({intf->broadcast(), _port + a});
                udp::socket().send(message);
            }
            else if (intf->isPointToPoint() && (intf->subnetPrefixLength() == 24))
            {
                Message::Ptr message = createMessage(command::CtlDiscovery);
                union {
                    quint8  ip4[4];
                    quint32 ip4_val;
                };
                ip4_val = intf->subnet().toIPv4Address();
                for (quint8 i = 1; i < 255; ++i)
                {
                    ip4[0] = i;
                    QHostAddress addr {ip4_val};
                    message->appendDestinationPoint({addr, _port + a});
                    if (message->destinationPoints().count() > 20)
                    {
                        udp::socket().send(message);
                        message = createMessage(command::CtlDiscovery);
                        usleep(10);
                    }
                }
                if (!message->destinationPoints().isEmpty())
                    udp::socket().send(message);
            }
        }

//    for (const QHostAddress& route : _routes)
//    {
//        for (int k = 0; k < 5; ++k)
//        {
//            Message::Ptr message = createMessage(command::CtlDiscovery);
//            union {
//                quint8  ip4[4];
//                quint32 ip4_val;
//            };
//            ip4_val = route.toIPv4Address();
//            for (quint8 i = 1; i < 255; ++i)
//            {
//                ip4[0] = i;
//                QHostAddress addr {ip4_val};
//                message->destinationPoints().insert({addr, _port + k});
//                if (message->destinationPoints().count() > 20)
//                {
//                    udp::socket().send(message);
//                    message = createMessage(command::CtlDiscovery);
//                    usleep(10);
//                }
//            }
//            if (!message->destinationPoints().isEmpty())
//                udp::socket().send(message);
//        }
//    }
#endif
}

void ConnectWindow::updateList()
{
#ifndef ONLY_STATIC_CONNECTION
    for (int i = 0; i < ui->listDynamic->count(); ++i)
    {
        QListWidgetItem* lwi = ui->listDynamic->item(i);
        ConnectItem* ci =
            qobject_cast<ConnectItem*>(ui->listDynamic->itemWidget(lwi));
        if (ci && ci->lifeTimeExpired())
        {
            --i;
            ui->listDynamic->removeItemWidget(lwi);
            delete lwi;
        }
    }
    on_tabWidget_currentChanged(ui->tabWidget->currentIndex());
#endif
}

void ConnectWindow::command_CtlDiscovery(const Message::Ptr& message)
{
#ifndef ONLY_STATIC_CONNECTION
    if (message->socketType() != SocketType::Udp
        || message->type() == Message::Type::Command)
        return;

    data::CtlDiscovery ctlDiscovery;
    readFromMessage(message, ctlDiscovery);

    bool found = false;
    for (int i = 0; i < ui->listDynamic->count(); ++i)
    {
        QListWidgetItem* lwi = ui->listDynamic->item(i);
        ConnectItem* ci =
            qobject_cast<ConnectItem*>(ui->listDynamic->itemWidget(lwi));
        if (ci && (ci->applId() == ctlDiscovery.applId))
        {
            ci->resetLifeTimer();
            ci->setInfo(ctlDiscovery.info);
            //ui->listCtl->sortItems();
            found = true;
            break;
        }
    }
    if (!found)
    {
        ConnectItem* ci = new ConnectItem();
        ci->setInfo(ctlDiscovery.info);
        ci->setApplId(ctlDiscovery.applId);
        ci->setHostPoint(ctlDiscovery.hostPoint);
        ci->resetLifeTimer();
        QListWidgetItem* lwi = new QListWidgetItem(/*ci*/);
        //QSize sz2 = cw->minimumSize();
        //lwi->setSizeHint(cw->minimumSize());
        //QSize sz = cw->sizeHint();
        lwi->setSizeHint(ci->sizeHint());
        ui->listDynamic->addItem(lwi);
        ui->listDynamic->setItemWidget(lwi, ci);
        //ui->listCtl->sortItems();
    }
    on_tabWidget_currentChanged(ui->tabWidget->currentIndex());
#endif
}

void ConnectWindow::on_btnAddConn_clicked(bool /*checked*/)
{
    ConnectEditor ce {this};
    ce.init("", {});

    if (ce.exec() == QDialog::Rejected)
    {
        ce.deinit();
        return;
    }

    ConnectItem* ci = new ConnectItem();
    ci->setInfo(ce.info());
    ci->setHostPoint(ce.hostPoint());
    ci->resetLifeTimer();
    QListWidgetItem* lwi = new QListWidgetItem(/*ci*/);
    lwi->setSizeHint(ci->sizeHint());
    ui->listStatic->addItem(lwi);
    ui->listStatic->setItemWidget(lwi, ci);
    ce.deinit();

    on_tabWidget_currentChanged(ui->tabWidget->currentIndex());
    saveStaticList();
}

void ConnectWindow::on_btnEditConn_clicked(bool /*checked*/)
{
    QListWidgetItem* lwi = ui->listStatic->currentItem();
    if (ConnectItem* ci =
          qobject_cast<ConnectItem*>(ui->listStatic->itemWidget(lwi)))
    {
        ConnectEditor ce {this};
        ce.init(ci->info(), ci->hostPoint());
        if (ce.exec() == QDialog::Accepted)
        {
            ci->setInfo(ce.info());
            ci->setHostPoint(ce.hostPoint());
            saveStaticList();
        }
        ce.deinit();
    }
}

void ConnectWindow::on_btnDeleteConn_clicked(bool /*checked*/)
{
    // TODO Сделать сообщение о подтверждении удаления

    if (QListWidgetItem* lwi = ui->listStatic->currentItem())
    {
        ui->listStatic->removeItemWidget(lwi);
        delete lwi;

        on_tabWidget_currentChanged(ui->tabWidget->currentIndex());
        saveStaticList();
    }
}
