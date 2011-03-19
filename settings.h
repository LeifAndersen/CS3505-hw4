/* Leif Andersen, Robert Christensen
 * (c) TEAM: Katastrofie, 2011
 *
 * Released under the terms of the LGPL v2.1
 * as part of a modification of the KAstroids projec
 * (from the qt demo library), which is where most of
 * the code came from.
 */
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

/**
  * Settings object, for changing the key configurations of the user.
  * Currently only for arrow keys and in game actions, but not for launching
  * new ships or making new games.
  */
class Settings : public QDialog
{
    Q_OBJECT

public:
    /**
      * Construct a settings window
      * parameters for QWidget
      */
    explicit Settings(KeySettings *currentSettings, QWidget *parent = 0);

    /**
      * Destruct the settings window.
      */
    ~Settings();

private:

    // Box taking precidence (selected)
    int activebox;

    // The buttons for the box
    std::vector<QPushButton*> buttons;

    // Settings for the buttons
    KeySettings editedSettings;

    // UI settings
    Ui::Settings *ui;

    // String for the buttons
    // returns a string representitive of what key is pressed
    static QString keyString(int);
private slots:

    /**
      * Called once the new binding is accepted. (ok button)
      */
    void on_buttonBox_accepted();

    /**
      * Change p1 thrust
      */
    void setP1_thrust();

    /**
      * Change p1 left
      */
    void setP1_left();

    /**
      * Change p1 right
      */
    void setP1_right();

    /**
      * Change p1 shield
      */
    void setP1_shield();

    /**
      * Change p1 shoot
      */
    void setP1_shoot();

    /**
      * Change p1 brake
      */
    void setP1_brake();

/////
    /**
      * Change p2 thrust
      */
    void setP2_thrust();

    /**
      * Change p2 left
      */
    void setP2_left();

    /**
      * Change p2 right
      */
    void setP2_right();

    /**
      * Change p2 shield
      */
    void setP2_shield();

    /**
      * Change p2 shoot
      */
    void setP2_shoot();

    /**
      * Change p2 brake
      */
    void setP2_brake();

signals:
    /**
      * Signled when a key changes
      */
    void SubmitKeyChange(KeySettings);


protected:
    /**
      * Called when a key is pressed
      */
    virtual void keyPressEvent( QKeyEvent *event );
};

#endif // SETTINGS_H
