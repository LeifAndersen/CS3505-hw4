#include "settings.h"
#include "ui_settings.h"

Settings::Settings(KeySettings *currentSettings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    // set active settings for player 1
    ui->Player1_thrust->setText(&currentSettings->player1_thrust);
    ui->Player1_left->setText(  &currentSettings->player1_left  );
    ui->Player1_right->setText( &currentSettings->player1_right );
    ui->Player1_break->setText( &currentSettings->player1_brake );
    ui->Player1_shield->setText(&currentSettings->player1_shield);
    ui->Player1_shoot->setText( &currentSettings->player1_shoot);
}

Settings::~Settings()
{
    delete ui;
}
