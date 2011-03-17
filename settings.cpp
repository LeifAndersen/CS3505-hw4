#include <QString>

#include "settings.h"
#include "ui_settings.h"

Settings::Settings(KeySettings *currentSettings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    // no box is active
    activebox = -1;

    editedSettings = *currentSettings;

    // set all the buttons to display the current settings
    ui->Player1_thrust->setText(keyString(editedSettings.player1_thrust));
    ui->Player1_left->setText(keyString(editedSettings.player1_left));
    ui->Player1_right->setText(keyString(editedSettings.player1_right));
    ui->Player1_shield->setText(keyString(editedSettings.player1_shield));
    ui->Player1_shoot->setText(keyString(editedSettings.player1_shoot));
    ui->Player1_brake->setText(keyString(editedSettings.player1_brake));

    ui->Player2_thrust->setText(keyString(editedSettings.player2_thrust));
    ui->Player2_left->setText(keyString(editedSettings.player2_left));
    ui->Player2_right->setText(keyString(editedSettings.player2_right));
    ui->Player2_shield->setText(keyString(editedSettings.player2_shield));
    ui->Player2_shoot->setText(keyString(editedSettings.player2_shoot));
    ui->Player2_brake->setText(keyString(editedSettings.player2_brake));

    // map the buttons to an vector to use when indexing things durring remaping
    // for now they are hard coded in. . .eventually something that should be fixed.
    buttons.resize(12);
    buttons[0] = ui->Player1_thrust;
    buttons[1] = ui->Player1_left;
    buttons[2] = ui->Player1_right;
    buttons[3] = ui->Player1_shield;
    buttons[4] = ui->Player1_shoot;
    buttons[5] = ui->Player1_brake;

    buttons[6] = ui->Player2_thrust;
    buttons[7] = ui->Player2_left;
    buttons[8] = ui->Player2_right;
    buttons[9] = ui->Player2_shield;
    buttons[10]= ui->Player2_shoot;
    buttons[11]= ui->Player2_brake;
    // set active settings for player 1

    connect(ui->Player1_thrust, SIGNAL(pressed()), this, SLOT(setP1_thrust()));
}

Settings::~Settings()
{
    delete ui;
}

// converts the key to a human readable string for the buttons
QString Settings::keyString(Qt::Key value)
{
    // for standard alpha keys
    if(value > 33 && value < 126)
        return QString(value);

 return "UNKNOWN";
}

void Settings::setP1_thrust()
{
    ui->Player1_thrust->setText("Hello");
    activebox = 0;
}
