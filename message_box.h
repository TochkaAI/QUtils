#pragma once
#include <QMessageBox>

QMessageBox::StandardButton messageBox(QWidget* parent,
                                       QMessageBox::Icon icon,
                                       const QString& message);
