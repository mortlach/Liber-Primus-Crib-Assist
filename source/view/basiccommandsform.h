#ifndef BASICCOMMANDSFORM_H
#define BASICCOMMANDSFORM_H

#include <QWidget>

namespace Ui {
class BasicCcommandsForm;
}

class BasicCommandsForm : public QWidget
{
    Q_OBJECT

public:
    explicit BasicCommandsForm(QWidget *parent = nullptr);
    ~BasicCommandsForm();

private:
    Ui::BasicCcommandsForm *ui;
};

#endif // BASICCOMMANDSFORM_H
