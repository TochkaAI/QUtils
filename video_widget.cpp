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

#include "video_widget.h"
#include <QPainter>

VideoWidget::VideoWidget(QWidget* parent) : QWidget(parent)
{
    _noImageMessage = QObject::tr("No image");
    _transformMode = Qt::SmoothTransformation;
}

void VideoWidget::showImage(const QImage& image)
{
    _baseImage = image;
    _scaleImage = scaleImage(_baseImage);
    update();
}

QString VideoWidget::noImageMessage() const
{
    return _noImageMessage;
}

void VideoWidget::setNoImageMessage(const QString& val)
{
    _noImageMessage = val;
}

Qt::TransformationMode VideoWidget::transformMode() const
{
    return _transformMode;
}

void VideoWidget::setTransformMode(Qt::TransformationMode val)
{
    _transformMode = val;
}

bool VideoWidget::stretch() const
{
    return _stretch;
}

void VideoWidget::setStretch(bool val)
{
    _stretch = val;
}

QImage VideoWidget::scaleImage(const QImage& img)
{
    if (img.isNull())
        return img;

    if (!_stretch)
    {
        if (width() > img.width() && (height() > img.height()))
            return img;
    }

    return img.scaled(size(),
                      Qt::KeepAspectRatio /*Qt::KeepAspectRatioByExpanding*/,
                      _transformMode);
}

void VideoWidget::resizeEvent(QResizeEvent* event)
{
    _scaleImage = scaleImage(_baseImage);
}

void VideoWidget::paintEvent(QPaintEvent* event)
{
    QPainter p {this};
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setRenderHint(QPainter::TextAntialiasing, true);
    p.setRenderHint(QPainter::SmoothPixmapTransform, true);
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
    // Note: Since HighQualityAntialiasing was actually deprecated in Qt 5.14,
    p.setRenderHint(QPainter::HighQualityAntialiasing, true);
#endif
    p.setCompositionMode(QPainter::CompositionMode_SourceOver);

    if (_scaleImage.isNull())
    {
        if (!_noImageMessage.isEmpty())
        {
            QFont font = p.font();
            font.setPointSize(16);
            p.setFont(font);

            QSize sz = size();
            QFontMetrics fm {font};
            QRect rect = fm.boundingRect(_noImageMessage);
            rect.moveTop(sz.height() / 2 - rect.height() / 2);
            rect.moveLeft(sz.width() / 2 - rect.width()  / 2);

            //QPen pen = p.pen();
            //pen.setColor(_printPersonId.fontColor);
            //p.setPen(pen);
            p.drawText(rect, Qt::AlignCenter, _noImageMessage);
        }
    }
    else
    {
        //p.setOpacity(_blendFactor);
        int x = width()  / 2 - _scaleImage.width()  / 2;
        int y = height() / 2 - _scaleImage.height() / 2;
        p.drawImage(QPoint(x, y), _scaleImage);
    }
}
