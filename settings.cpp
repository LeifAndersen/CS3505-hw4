#include <QString>
#include <QKeyEvent>

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
    connect(ui->Player1_left,   SIGNAL(pressed()), this, SLOT(setP1_left()));
    connect(ui->Player1_right,  SIGNAL(pressed()), this, SLOT(setP1_right()));
    connect(ui->Player1_shield, SIGNAL(pressed()), this, SLOT(setP1_shield()));
    connect(ui->Player1_shoot,  SIGNAL(pressed()), this, SLOT(setP1_shoot()));
    connect(ui->Player1_brake,  SIGNAL(pressed()), this, SLOT(setP1_brake()));

    connect(ui->Player2_thrust, SIGNAL(pressed()), this, SLOT(setP2_thrust()));
    connect(ui->Player2_left,   SIGNAL(pressed()), this, SLOT(setP2_left()));
    connect(ui->Player2_right,  SIGNAL(pressed()), this, SLOT(setP2_right()));
    connect(ui->Player2_shield, SIGNAL(pressed()), this, SLOT(setP2_shield()));
    connect(ui->Player2_shoot,  SIGNAL(pressed()), this, SLOT(setP2_shoot()));
    connect(ui->Player2_brake,  SIGNAL(pressed()), this, SLOT(setP2_brake()));
}

Settings::~Settings()
{
    delete ui;
}

// converts the key to a human readable string for the buttons
QString Settings::keyString(int value)
{
    // for standard alpha keys
    if(value > 33 && value < 126)
        return QString(value);

 return "UNKNOWN";
}

void Settings::setP1_thrust()
{
    ui->Player1_thrust->setText("");
    activebox = 0;
}

void Settings::setP1_left()
{
    ui->Player1_left->setText("");
    activebox = 1;
}

void Settings::setP1_right()
{
    ui->Player1_right->setText("");
    activebox = 2;
}

void Settings::setP1_shield()
{
    ui->Player1_shield->setText("");
    activebox = 3;
}

void Settings::setP1_shoot()
{
    ui->Player1_shoot->setText("");
    activebox = 4;
}

void Settings::setP1_brake()
{
    ui->Player1_brake->setText("");
    activebox = 5;
}
////
void Settings::setP2_thrust()
{
    ui->Player2_thrust->setText("");
    activebox = 6;
}

void Settings::setP2_left()
{
    ui->Player2_left->setText("");
    activebox = 7;
}

void Settings::setP2_right()
{
    ui->Player2_right->setText("");
    activebox = 8;
}

void Settings::setP2_shield()
{
    ui->Player2_shield->setText("");
    activebox = 9;
}

void Settings::setP2_shoot()
{
    ui->Player2_shoot->setText("");
    activebox = 10;
}

void Settings::setP2_brake()
{
    ui->Player2_brake->setText("");
    activebox = 11;
}
////
void Settings::keyPressEvent( QKeyEvent *event )
{
    if(activebox>=0)
    {
        QString text(keyString(event->key()));
        switch(activebox)
        {
        case 0:
            ui->Player1_thrust->setText(text);
            editedSettings.player1_thrust = event->key();
            break;
        case 1:
            ui->Player1_left->setText(text);
            editedSettings.player1_left = event->key();
            break;
        case 2:
            ui->Player1_right->setText(text);
            editedSettings.player1_right = event->key();
            break;
        case 3:
            ui->Player1_shield->setText(text);
            editedSettings.player1_shield = event->key();
            break;
        case 4:
            ui->Player1_shoot->setText(text);
            editedSettings.player1_shoot = event->key();
            break;
        case 5:
            ui->Player1_brake->setText(text);
            editedSettings.player1_brake = event->key();
            break;

        case 6:
            ui->Player2_thrust->setText(text);
            editedSettings.player2_thrust = event->key();
            break;
        case 7:
            ui->Player2_left->setText(text);
            editedSettings.player2_left = event->key();
            break;
        case 8:
            ui->Player2_right->setText(text);
            editedSettings.player2_right = event->key();
            break;
        case 9:
            ui->Player2_shield->setText(text);
            editedSettings.player2_shield = event->key();
            break;
        case 10:
            ui->Player2_shoot->setText(text);
            editedSettings.player2_shoot = event->key();
            break;
        case 11:
            ui->Player2_brake->setText(text);
            editedSettings.player2_brake = event->key();
            break;
        }
    }

    activebox = -1;
}
