#include "ControlChoiceWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QDebug>
#include <QLabel>
#include <QKeyEvent>
#include <QSet>


CustomLineEdit::CustomLineEdit(QWidget* parent) : QLineEdit(parent) 
{}

void CustomLineEdit::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Up ||
        event->key() == Qt::Key_Down ||
        event->key() == Qt::Key_Left ||
        event->key() == Qt::Key_Right ||
        event->key() == Qt::Key_Space)
    {
        emit ForwardKeyEvent(event);
        return; 
    }
    QLineEdit::keyPressEvent(event); 
}


ControlChoiceWindow::ControlChoiceWindow(const QString& clientId, QWidget* parent)
    : QWidget(parent)
    , upInput(new CustomLineEdit(this))
    , downInput(new CustomLineEdit(this))
    , leftInput(new CustomLineEdit(this))
    , rightInput(new CustomLineEdit(this))
    , shootInput(new CustomLineEdit(this))
    , saveButton(new QPushButton("Save Controls", this))
    , resetButton(new QPushButton("Reset Controls", this))
{
    setWindowTitle("Set Controls");
    resize(600, 400);

    BackgroundHelper::InitializeBackground(this);

    QString inputStyle =
        "font-size: 20px; "
        "color: black; "
        "border: 2px solid gray; "
        "border-radius: 5px; "
        "padding: 5px;";
    upInput->setStyleSheet(inputStyle);
    downInput->setStyleSheet(inputStyle);
    leftInput->setStyleSheet(inputStyle);
    rightInput->setStyleSheet(inputStyle);
    shootInput->setStyleSheet(inputStyle);

    upInput->setPlaceholderText("Up");
    downInput->setPlaceholderText("Down");
    leftInput->setPlaceholderText("Left");
    rightInput->setPlaceholderText("Right");
    shootInput->setPlaceholderText("Shoot");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QHBoxLayout* inputsLayout = new QHBoxLayout();

    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftLayout->addStretch();
    leftLayout->addWidget(leftInput);
    leftLayout->addStretch();

    QVBoxLayout* rightLayout = new QVBoxLayout();
    rightLayout->addStretch();
    rightLayout->addWidget(rightInput);
    rightLayout->addStretch();

    QVBoxLayout* centerLayout = new QVBoxLayout();
    centerLayout->addWidget(upInput, 0, Qt::AlignCenter);
    centerLayout->addWidget(new QLabel("Set Your Controls", this), 0, Qt::AlignCenter);
    centerLayout->addWidget(downInput, 0, Qt::AlignCenter);

    QVBoxLayout* shootLayout = new QVBoxLayout();
    shootLayout->addStretch();
    shootLayout->addWidget(shootInput);
    shootLayout->addStretch();

    inputsLayout->addLayout(leftLayout);
    inputsLayout->addLayout(centerLayout);
    inputsLayout->addLayout(rightLayout);
    inputsLayout->addLayout(shootLayout);

    mainLayout->addLayout(inputsLayout);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(resetButton);
    buttonLayout->addWidget(saveButton);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    connect(upInput, &CustomLineEdit::ForwardKeyEvent, this, &ControlChoiceWindow::HandleForwardedKeyEvent);
    connect(downInput, &CustomLineEdit::ForwardKeyEvent, this, &ControlChoiceWindow::HandleForwardedKeyEvent);
    connect(leftInput, &CustomLineEdit::ForwardKeyEvent, this, &ControlChoiceWindow::HandleForwardedKeyEvent);
    connect(rightInput, &CustomLineEdit::ForwardKeyEvent, this, &ControlChoiceWindow::HandleForwardedKeyEvent);
    connect(shootInput, &CustomLineEdit::ForwardKeyEvent, this, &ControlChoiceWindow::HandleForwardedKeyEvent);

    connect(saveButton, &QPushButton::clicked, this, &ControlChoiceWindow::OnSaveControls);
    connect(resetButton, &QPushButton::clicked, this, &ControlChoiceWindow::OnResetControls);
}

ControlChoiceWindow::~ControlChoiceWindow()
{
    disconnect(upInput, nullptr, this, nullptr);
    disconnect(downInput, nullptr, this, nullptr);
    disconnect(leftInput, nullptr, this, nullptr);
    disconnect(rightInput, nullptr, this, nullptr);
    disconnect(shootInput, nullptr, this, nullptr);
}

int ControlChoiceWindow::GetKeyEventFromChar(char letter)
{
    QMap<char, int> keyMap = {
             {'a', Qt::Key_A},
             {'b', Qt::Key_B},
             {'c', Qt::Key_C},
             {'d', Qt::Key_D},
             {'e', Qt::Key_E},
             {'f', Qt::Key_F},
             {'g', Qt::Key_G},
             {'h', Qt::Key_H},
             {'i', Qt::Key_I},
             {'j', Qt::Key_J},
             {'k', Qt::Key_K},
             {'l', Qt::Key_L},
             {'m', Qt::Key_M},
             {'n', Qt::Key_N},
             {'o', Qt::Key_O},
             {'p', Qt::Key_P},
             {'q', Qt::Key_Q},
             {'r', Qt::Key_R},
             {'s', Qt::Key_S},
             {'t', Qt::Key_T},
             {'u', Qt::Key_U},
             {'v', Qt::Key_V},
             {'w', Qt::Key_W},
             {'x', Qt::Key_X},
             {'y', Qt::Key_Y},
             {'z', Qt::Key_Z},
             {'A', Qt::Key_A},
             {'B', Qt::Key_B},
             {'C', Qt::Key_C},
             {'D', Qt::Key_D},
             {'E', Qt::Key_E},
             {'F', Qt::Key_F},
             {'G', Qt::Key_G},
             {'H', Qt::Key_H},
             {'I', Qt::Key_I},
             {'J', Qt::Key_J},
             {'K', Qt::Key_K},
             {'L', Qt::Key_L},
             {'M', Qt::Key_M},
             {'N', Qt::Key_N},
             {'O', Qt::Key_O},
             {'P', Qt::Key_P},
             {'Q', Qt::Key_Q},
             {'R', Qt::Key_R},
             {'S', Qt::Key_S},
             {'T', Qt::Key_T},
             {'U', Qt::Key_U},
             {'V', Qt::Key_V},
             {'W', Qt::Key_W},
             {'X', Qt::Key_X},
             {'Y', Qt::Key_Y},
             {'Z', Qt::Key_Z},
             {'1', Qt::Key_1},
             {'2', Qt::Key_2},
             {'3', Qt::Key_3},
             {'4', Qt::Key_4},
             {'5', Qt::Key_5},
             {'6', Qt::Key_6},
             {'7', Qt::Key_7},
             {'8', Qt::Key_8},
             {'9', Qt::Key_9},
             {'0', Qt::Key_0},
             { ' ', Qt::Key_Space },
             {'\n', Qt::Key_Return},  // Enter key
             {'\t', Qt::Key_Tab},    // Tab key
             {'\b', Qt::Key_Backspace},  // Backspace
             { '\u2190', Qt::Key_Left },   // Left Arrow
             {'\u2191', Qt::Key_Up},     // Up Arrow
             {'\u2192', Qt::Key_Right},  // Right Arrow
             {'\u2193', Qt::Key_Down},   // Down Arrow
    };

    return keyMap.value(letter, -1);
}

void ControlChoiceWindow::HandleForwardedKeyEvent(QKeyEvent* event)
{
    QString keyText;
    switch (event->key())
    {
        case Qt::Key_Up: keyText = "ArrowUp"; break;
        case Qt::Key_Down: keyText = "ArrowDown"; break;
        case Qt::Key_Left: keyText = "ArrowLeft"; break;
        case Qt::Key_Right: keyText = "ArrowRight"; break;
        case Qt::Key_Space: keyText = "Space"; break;
        default: keyText = QKeySequence(event->key()).toString(); break;
    }
    HandleKeyEvent(keyText);
}

void ControlChoiceWindow::HandleKeyEvent(const QString& keyText)
{
    if (!keyText.isEmpty()) 
    {
        if (upInput->hasFocus() && upInput->text().isEmpty())
        {
            upInput->setText(keyText);
            upInput->setEnabled(false);
        }
        else if (downInput->hasFocus() && downInput->text().isEmpty()) 
        {
            downInput->setText(keyText);
            downInput->setEnabled(false);
        }
        else if (leftInput->hasFocus() && leftInput->text().isEmpty()) 
        {
            leftInput->setText(keyText);
            leftInput->setEnabled(false);
        }
        else if (rightInput->hasFocus() && rightInput->text().isEmpty()) 
        {
            rightInput->setText(keyText);
            rightInput->setEnabled(false);
        }
        else if (shootInput->hasFocus() && shootInput->text().isEmpty())
        {
            shootInput->setText(keyText);
            shootInput->setEnabled(false);
        }
    }
}

void ControlChoiceWindow::OnSaveControls()
{
    if (upInput->text().trimmed() == "ArrowUp" && downInput->text().trimmed() == "ArrowDown"
        && leftInput->text().trimmed() == "ArrowLeft" && rightInput->text().trimmed() == "ArrowRight"
        && shootInput->text().trimmed() == "Space")
    {
        controls["Up"] = 16777235;
        controls["Down"] = 16777237;
        controls["Left"] = 16777234;
        controls["Right"] = 16777236;
        controls["Shoot"] = 32;
    }
    else
    {
        controls["Up"] = GetKeyEventFromChar(upInput->text().toLower().at(0).toLatin1());
        controls["Down"] = GetKeyEventFromChar(downInput->text().toLower().at(0).toLatin1());
        controls["Left"] = GetKeyEventFromChar(leftInput->text().toLower().at(0).toLatin1());
        controls["Right"] = GetKeyEventFromChar(rightInput->text().toLower().at(0).toLatin1());
        controls["Shoot"] = GetKeyEventFromChar(shootInput->text().toLower().at(0).toLatin1());
    }
    emit ControlsSet(controls);
     this->close();
}


void ControlChoiceWindow::OnResetControls()
{
    upInput->clear();
    downInput->clear();
    leftInput->clear();
    rightInput->clear();
    shootInput->clear();

    upInput->setEnabled(true);
    downInput->setEnabled(true);
    leftInput->setEnabled(true);
    rightInput->setEnabled(true);
    shootInput->setEnabled(true);
}

void ControlChoiceWindow::resizeEvent(QResizeEvent* event)
{
    QImage image(":/StartImage/resources/StartGame.jpg");

    if (!image.isNull())
    {
        QPalette palette;
        palette.setBrush(QPalette::Window, QBrush(image.scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation)));
        setPalette(palette);
    }
    QWidget::resizeEvent(event);

}
