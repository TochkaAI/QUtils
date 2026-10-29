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

#include <QtCore>
#include <QWidget>

class VideoWidget : public QWidget
{
public:
    explicit VideoWidget(QWidget* parent = nullptr);

    // Устанавливает отображаемое изображение/кадр
    void showImage(const QImage&);

    // Возвращает отображаемое изображение/кадр в базовом размере
    QImage image() const {return _baseImage;}

    // Сообщение, отображаемое при отсутствии изображения
    QString noImageMessage() const;
    void setNoImageMessage(const QString&);

    // Определяет метод масштабирования изображения
    Qt::TransformationMode transformMode() const;
    void setTransformMode(Qt::TransformationMode);

    // Растягивать изображение на все пространство виджета с сохранением
    // пропорций
    bool stretch() const;
    void setStretch(bool);

private:
    //Q_OBJECT
    void resizeEvent(QResizeEvent*) override;
    void paintEvent(QPaintEvent*) override;

    QImage scaleImage(const QImage&);

private:
    QImage _baseImage;
    QImage _scaleImage;
    QString _noImageMessage;
    Qt::TransformationMode _transformMode;
    bool _stretch = {false};
};
