#include "message_box.h"
#include <QApplication>

QMessageBox::StandardButton messageBox(QWidget* parent,
                                       QMessageBox::Icon icon,
                                       QString message)
{
    message.replace("  ", "\n").replace(" ", "&nbsp;");
    QMessageBox msgBox {parent};
    //msgBox.setWindowModality(Qt::WindowModal);
    msgBox.setIcon(icon);
    msgBox.setWindowTitle(qApp->applicationName());
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText(message);
    int res = msgBox.exec();
    return static_cast<QMessageBox::StandardButton>(res);
}
