#include "CreateOrJoinGameWindow.h"

CreateOrJoinGameWindow::CreateOrJoinGameWindow(QWidget* parent)
    : QWidget(parent)
    , titleLabel(new QLabel("Create Game or Join Game", this))
    , generateCodeButton(new QPushButton("Generate Code", this))
    , checkCodeButton(new QPushButton("Check Code", this))
    , generalSettingsButton(new QPushButton("Settings", this))
    , typeCode(new QLineEdit(this))
{
    setWindowTitle("Create Game or Join Game");
    resize(600, 400);

    setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);
    activateWindow();
    raise();

    BackgroundHelper::InitializeBackground(this);

    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "font-size: 24px; "
        "color: white; "
        "font-weight: bold; "
        "margin-bottom: 20px;"
    );

    QString buttonStyle =
        "font-size: 16px; "
        "color: black; "
        "background-color: white; "
        "border: 2px solid black; "
        "border-radius: 5px; "
        "padding: 5px;";
    generateCodeButton->setStyleSheet(buttonStyle);
    checkCodeButton->setStyleSheet(buttonStyle);
    generalSettingsButton->setStyleSheet(buttonStyle);



    typeCode->setStyleSheet(
        "font-size: 16px; "
        "color: black; "
        "background-color: white; "
        "border: 2px solid black; "
        "border-radius: 5px; "
        "padding: 5px;"
    );

    generateCodeButton->setFixedSize(150, 50);
    typeCode->setFixedSize(200, 40);
    checkCodeButton->setFixedSize(100, 40);
    generalSettingsButton->setFixedSize(80, 30);

    typeCode->setPlaceholderText("Type code ...");

    QHBoxLayout* settingsLayout = new QHBoxLayout();
    settingsLayout->addStretch();
    settingsLayout->addWidget(generalSettingsButton, 0, Qt::AlignRight);

    QHBoxLayout* generateLayout = new QHBoxLayout();
    generateLayout->addStretch();
    generateLayout->addWidget(generateCodeButton);
    generateLayout->addStretch();


    QHBoxLayout* inputLayout = new QHBoxLayout();
    inputLayout->addStretch();
    inputLayout->addWidget(typeCode);
    inputLayout->addWidget(checkCodeButton);
    inputLayout->addStretch();

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(settingsLayout);
    mainLayout->addStretch();
    mainLayout->addWidget(titleLabel);
    mainLayout->addLayout(generateLayout);
    mainLayout->addSpacing(20);
    mainLayout->addLayout(inputLayout);
    mainLayout->addStretch();

    setLayout(mainLayout);


    connect(generateCodeButton, &QPushButton::clicked, this, [this]() {
        emit Generate(m_username);
        this->close();
        });

    connect(checkCodeButton, &QPushButton::clicked, this, [this]() {
        emit CheckCode(typeCode->text(), m_username);
        this->close();
        });
    connect(generalSettingsButton, &QPushButton::clicked, this, &CreateOrJoinGameWindow::OnSetting);
}

void CreateOrJoinGameWindow::OnSetting()
{
    emit GeneralSettings();

}
void CreateOrJoinGameWindow::resizeEvent(QResizeEvent* event)
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
CreateOrJoinGameWindow::~CreateOrJoinGameWindow() = default;
