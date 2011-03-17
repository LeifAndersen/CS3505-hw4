#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <qnamespace.h>
#include <QString>
#include <vector>

namespace Ui {
    class Settings;
}

// used to comunicate to the settings class what the settings are
struct KeySettings
{
public:
    Qt::Key player1_thrust;
    Qt::Key player1_left;
    Qt::Key player1_right;
    Qt::Key player1_shoot;
    Qt::Key player1_shield;
    Qt::Key player1_brake;

    Qt::Key player2_thrust;
    Qt::Key player2_left;
    Qt::Key player2_right;
    Qt::Key player2_shoot;
    Qt::Key player2_shield;
    Qt::Key player2_brake;
};

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(KeySettings *currentSettings, QWidget *parent = 0);
    ~Settings();

private:
    int activebox;
    std::vector<QPushButton*> buttons;
    KeySettings editedSettings;
    Ui::Settings *ui;
    static QString keyString(Qt::Key); // returns a string representitive of what key is pressed

    void setP1_thrust();

protected:
};

#endif // SETTINGS_H
