#ifndef GAMEMAPWINDOW_H
#define GAMEMAPWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QJsonArray>
#include "BackgroundHelper.h"
#include "MapWidget.h"
class GameMapWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GameMapWindow(QWidget* parent = nullptr);
    ~GameMapWindow() override;

public slots:
    void onSettingsClicked();
    void displayMap(const QJsonArray& mapData); 

signals:
void SettingsClicked();
void KeyPressed(int key);

protected:
    void resizeEvent(QResizeEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    void adjustMapWidgetSize(QResizeEvent* event);
    QPushButton* settingsButton;
    QLabel* titleLabel;
    MapWidget* mapWidget;     
    QJsonArray mapData;
};

#endif // GAMEMAPWINDOW_H
