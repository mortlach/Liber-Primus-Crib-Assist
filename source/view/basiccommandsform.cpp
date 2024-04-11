#include "basiccommandsform.h"
#include "ui_basiccommandsform.h"

BasicCommandsForm::BasicCommandsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BasicCcommandsForm)
{
    ui->setupUi(this);
}

BasicCommandsForm::~BasicCommandsForm()
{
    delete ui;
}
