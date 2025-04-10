#ifndef BACKGROUNDHELPER_H
#define BACKGROUNDHELPER_H

#include <QWidget>
#include <QPalette>
#include <QImage>
#include <QResizeEvent>
#include <QDebug>

namespace BackgroundHelper
{
    inline void InitializeBackground(QWidget* widget)
    {
        const QString imagePath = ":/StartImage/resources/StartGame.jpg"; 
        QImage image(imagePath);

        if (image.isNull())
        {
            qDebug() << "Failed to load background image from:" << imagePath;
            return;
        }

        QPalette palette;
        palette.setBrush(QPalette::Window, QBrush(image.scaled(widget->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation)));
        widget->setPalette(palette);
        widget->setAutoFillBackground(true);
    }
}

#endif // BACKGROUNDHELPER_H
