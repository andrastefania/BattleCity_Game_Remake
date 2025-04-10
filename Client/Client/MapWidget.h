#pragma once

#include <QWidget>
#include <QJsonArray>
#include <QImage>


class MapWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MapWidget(QWidget* parent = nullptr);
    void setMapData(const QJsonArray& data);
  
protected:
    void paintEvent(QPaintEvent* event) override;

private:
     QJsonArray mapData; 
};
