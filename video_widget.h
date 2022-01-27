#pragma once

#include <QtCore>
#include <QWidget>

class VideoWidget : public QWidget
{
public:
    explicit VideoWidget(QWidget* parent = nullptr);

    // Отображает изображение/кадр
    void showImage(const QImage&);

    // Определяет сообщение, которое будет отображаться, когда изображение
    // отсутствует
    QString noImageMessage() const;
    void setNoImageMessage(const QString&);

    // Определяет метод масштабирования изображения
    Qt::TransformationMode transformMode() const;
    void setTransformMode(Qt::TransformationMode);

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
};
