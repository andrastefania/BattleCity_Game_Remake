#ifndef CONTROLCHOICEWINDOW_H
#define CONTROLCHOICEWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QMap>
#include <QString>
#include <QKeyEvent>
#include "BackgroundHelper.h";
class CustomLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    explicit CustomLineEdit(QWidget* parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent* event) override;

signals:
    void ForwardKeyEvent(QKeyEvent* event);
};


class ControlChoiceWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ControlChoiceWindow(const QString& clientId, QWidget* parent = nullptr);
    ~ControlChoiceWindow();

    static int GetKeyEventFromChar(char letter);

signals:
    void ControlsSet(const QMap<QString, int>& controls);

private slots:
    void OnSaveControls();
    void OnResetControls();
    void HandleForwardedKeyEvent(QKeyEvent* event);

protected:
    void resizeEvent(QResizeEvent* event)override;

private:
    void HandleKeyEvent(const QString& keyText);

    CustomLineEdit* upInput;
    CustomLineEdit* downInput;
    CustomLineEdit* leftInput;
    CustomLineEdit* rightInput;
    CustomLineEdit* shootInput;
    QPushButton* saveButton;
    QPushButton* resetButton;
    QMap<QString, int> controls;
};

#endif // CONTROLCHOICEWINDOW_H
