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
    int player1_thrust;
    int player1_left;
    int player1_right;
    int player1_shoot;
    int player1_shield;
    int player1_brake;

    int player2_thrust;
    int player2_left;
    int player2_right;
    int player2_shoot;
    int player2_shield;
    int player2_brake;
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
    static QString keyString(int); // returns a string representitive of what key is pressed
private slots:
    void on_buttonBox_accepted();
    void setP1_thrust();
    void setP1_left();
    void setP1_right();
    void setP1_shield();
    void setP1_shoot();
    void setP1_brake();

    void setP2_thrust();
    void setP2_left();
    void setP2_right();
    void setP2_shield();
    void setP2_shoot();
    void setP2_brake();

signals:
    void SubmitKeyChange(KeySettings);


protected:
        virtual void keyPressEvent( QKeyEvent *event );
};

#endif // SETTINGS_H
