#ifndef CREATEORJOINGAMEWINDOW_H
#define CREATEORJOINGAMEWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "BackgroundHelper.h"
class CreateOrJoinGameWindow : public QWidget {
    Q_OBJECT

public:
    explicit CreateOrJoinGameWindow(QWidget* parent = nullptr);
    ~CreateOrJoinGameWindow();

signals:
    void Generate(const QString& username);
    void CheckCode(const QString& code, const QString& username);
    void GeneralSettings();

protected:
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void OnSetting();
private:

    QLabel* titleLabel;
    QPushButton* generateCodeButton;
    QPushButton* checkCodeButton;
    QLineEdit* typeCode;
    QPushButton* generalSettingsButton;

    QString m_username;
    QString m_code;
};

#endif // CREATEORJOINGAMEWINDOW_H

