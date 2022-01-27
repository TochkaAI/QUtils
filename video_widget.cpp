#include "video_widget.h"
#include <QPainter>

VideoWidget::VideoWidget(QWidget* parent) : QWidget(parent)
{
    _noImageMessage = QObject::tr("No Image");
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

QImage VideoWidget::scaleImage(const QImage& img)
{
    if (img.isNull())
        return img;

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
    p.setRenderHint(QPainter::HighQualityAntialiasing, true);
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
