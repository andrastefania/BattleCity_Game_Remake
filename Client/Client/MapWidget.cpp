#include "MapWidget.h"
#include <QPainter>
#include <QResizeEvent>
#include <QDebug>
#include <cstdlib>
#include <ctime>
#include "NoGreenScreen.h"
MapWidget::MapWidget(QWidget* parent) : QWidget(parent) {
	qDebug() << "MapWidget created:" << this;
}

void MapWidget::setMapData(const QJsonArray& data)
{
    mapData = data;
	update();  
}

void MapWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (mapData.isEmpty()) {
        qDebug() << "Map data is empty!";
        return;
    }

    int rows = mapData.size();
    int cols = mapData[0].toArray().size();

    const double scaleFactor = 0.9;

    int cellWidth = width() / cols * scaleFactor;
    int cellHeight = height() / rows * scaleFactor;

    int squareSize = qMin(cellWidth, cellHeight);

    int offsetX = (width() - squareSize * cols) / 2;
    int offsetY = (height() - squareSize * rows) / 2;


    for (int row = 0; row < rows; ++row) {
        QJsonArray rowArray = mapData[row].toArray();
        for (int col = 0; col < rowArray.size(); ++col) {
            int value = rowArray[col].toInt();

            QImage img;


            if (value == 0) { 
                img = QImage();
            }
            else if (value == 1) {
                img = RemoveGreenScreen(":/Wall/resources/Asteroid.png");
            }
            else if (value == 2)
            {
                img = RemoveGreenScreen(":/Wall/resources/Asteroid.png");
            }
            else if (value == 3)
            {
                img = RemoveGreenScreen(":/Wall/resources/Asteroid.png");
            }
            else if (value == 4)
            {
                img = RemoveGreenScreen(":/Wall/resources/Asteroid.png");
            }
            else if (value == 5)
            {
                std::srand(static_cast<unsigned>(std::time(nullptr)));
                int randomNumber = 11 + std::rand() % 4;
                switch (randomNumber)
                {
                case 11:
                {
                    img = RemoveGreenScreen(":/PlayersImages/resources/Astronaut_1.png");
                    break;
                };
                case 12:
                {
                    img = RemoveGreenScreen(":/PlayersImages/resources/Astronaut_2.png");
                    break;
                }
                case 13:
                {
                    img = RemoveGreenScreen(":/PlayersImages/resources/Astronaut_3.png");
                    break;
                } case 14:
                {
                    img = RemoveGreenScreen(":/PlayersImages/resources/Astronaut_4.png");
                    break;
                }
                }
            }
            else if (value == 7)
            {
                img = RemoveGreenScreen(":/Border/resources/Border.png");
            }
            else {
                img = QImage();
            }
            QRect cellRect(offsetX + col * squareSize, offsetY + row * squareSize, squareSize, squareSize);
            QImage scaledImg = img.scaled(squareSize, squareSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

            painter.drawImage(cellRect, scaledImg);
        }

    }

}