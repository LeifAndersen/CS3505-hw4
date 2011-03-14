#include "settings.h"
#include "ui_settings.h"

Settings::Settings(KeyBindings *currentSettings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);


}

Settings::~Settings()
{
    delete ui;
}
