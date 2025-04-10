#pragma once
#ifndef DISPLAYCODEWINDOW_H
#define DISPLAYCODEWINDOW_H

#include <QWidget>
#include <QLabel>;
#include<QVBoxLayout>;

class DisplayCodeWindow : public QWidget {
    Q_OBJECT

public:
    explicit DisplayCodeWindow(QWidget* parent = nullptr);
    ~DisplayCodeWindow();
    void setMessage(const QString& message);


private:
    QVBoxLayout* layout;
    QLabel* messageLabel;

};
#endif //DISPLAYCODEWINDOW_H