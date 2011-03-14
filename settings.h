#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>

namespace Ui {
    class Settings;
}

// used to comunicate to the settings class what the settings are
struct KeySettings
{
public:
    char player1_thrust;
    char player1_left;
    char player1_right;
    char player1_shoot;
    char player1_shield;
    char player1_brake;

    char player2_thrust;
    char player2_left;
    char player2_right;
    char player2_shoot;
    char player2_shield;
    char player2_brake;
};

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(KeySettings *currentSettings, QWidget *parent = 0);
    ~Settings();

private:
    Ui::Settings *ui;

protected:
};

#endif // SETTINGS_H
