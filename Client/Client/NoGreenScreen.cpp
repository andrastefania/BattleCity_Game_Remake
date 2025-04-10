#include <QImage>
#include <QColor>
#include<QDebug>


QImage RemoveGreenScreen(const QString& imagePath, int threshold = 30)
{
    QImage inputImage(imagePath);
    if (inputImage.isNull())
    {
        qDebug() << "Input image is null! Path:" << imagePath;
        return QImage();
    }
    QImage resultImage = inputImage;
    int width = resultImage.width();
    int height = resultImage.height();

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            QColor pixelColor = resultImage.pixelColor(x, y);
            int red = pixelColor.red();
            int green = pixelColor.green();
            int blue = pixelColor.blue();
            if (green > red + threshold && green > blue + threshold)
            {
                resultImage.setPixelColor(x, y, QColor(0, 0, 0, 0));
            }
        }
    }
    return resultImage;
}