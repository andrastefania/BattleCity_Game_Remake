#ifndef INGAMESETTINGSWINDOW_H
#define INGAMESETTINGSWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include "BackgroundHelper.h"
class InGameSettingsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit InGameSettingsWindow(QWidget* parent = nullptr);
    ~InGameSettingsWindow();
    void positionInTopRight(QWidget* parent);

signals:
    void backToGame();   
    void exitGame(); 
    void SaveSettings(int volume);    

private slots:
    void onBackToGame();
    void onExitGame(); 
    void onSaveSettings();
private:

    QSlider* musicVolumeSlider;
    QPushButton* backToGameButton;
    QPushButton* exitGameButton;
    QPushButton* saveSettingsButton;
};

#endif // INGAMESETTINGSWINDOW_H
