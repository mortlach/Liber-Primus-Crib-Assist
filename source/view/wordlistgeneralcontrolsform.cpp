
/////////////////////////////////////////////////////////////////////////////
//    LPCribAssist v0.1: to help with cribbing the Liber Primus
//    mortlach
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.
//
/////////////////////////////////////////////////////////////////////////////
#include "wordlistgeneralcontrolsform.h"
#include "ui_Wordlistgeneralcontrolsform.h"

WordListGeneralControlsForm::WordListGeneralControlsForm(QWidget *parent, int data_count)
    : QWidget(parent), ui(new Ui::WordListGeneralControlsForm)
    , current_index(0)
    , data_count(data_count)
    , min_index(0)
{
    qDebug() << "WordListGeneralControlsForm::WordListGeneralControlsForm";
    ui->setupUi(this);
    setup(min_index, data_count);

    for(int i = 8; i < 21; ++i){
        QString t = QString::number(i);
        QVariant v = QVariant(i);
        ui->fontSizeComboBox->addItem(t, v);
    }


    QObject::connect(ui->allRadioButton,
                     &QRadioButton::toggled,
                     this,
                     &WordListGeneralControlsForm::on_filterChanged);
    QObject::connect(ui->chosenRadioButton,
                     &QRadioButton::toggled,
                     this,
                     &WordListGeneralControlsForm::on_filterChanged);
    QObject::connect(ui->notChosenRadioButton,
                     &QRadioButton::toggled,
                     this,
                     &WordListGeneralControlsForm::on_filterChanged);


}
WordListGeneralControlsForm::~WordListGeneralControlsForm(){
    delete ui;
}
void WordListGeneralControlsForm::setup(int min, int data_count_in){
    data_count = data_count_in;
    min_index = min;
    ui->dataChoiceHorizontalScrollBar->setMaximum(data_count-1);
    qDebug() << "WordListGeneralControlsForm::setup min/data_count_in=" << min << data_count_in;
}
void WordListGeneralControlsForm::setLabel(const QString& text){
    ui->wordListGenerallabel->setText(text);
}

void WordListGeneralControlsForm::on_savePushButton_clicked() {
    //qDebug() << "on_savePushButton_clicked";
    emit saveData();
}
void WordListGeneralControlsForm::on_saveAllPushButton_clicked() {
    //qDebug() << "on_saveAllPushButton_clicked";
    emit saveAllData();
}
void WordListGeneralControlsForm::on_filterChanged(){

    if(ui->allRadioButton->isChecked()){
        qDebug() << "on_filterChanged 0";
        emit changeFilter(0);
    }
    else if(ui->chosenRadioButton->isChecked()){
        qDebug() << "on_filterChanged 1";
        emit changeFilter(1);
    }
    else if(ui->notChosenRadioButton->isChecked()){
        qDebug() << "on_filterChanged 2";
        emit changeFilter(2);
    }
}
void WordListGeneralControlsForm::setAllFilter(){
    ui->allRadioButton->setChecked(true);
}

void WordListGeneralControlsForm::setChosenFilter(){
    ui->chosenRadioButton->setChecked(true);
}

void WordListGeneralControlsForm::setNotChosenFilter(){
    ui->notChosenRadioButton->setChecked(true);
}

void WordListGeneralControlsForm::on_loadPushButton_clicked() {
    qDebug() << "on_loadPushButton_clicked";
    emit loadData();
}

void WordListGeneralControlsForm::on_findPushButton_clicked() {
    qDebug() << "on_findPushButton_clicked";
    emit findData(ui->findLineEdit->text());
}

void WordListGeneralControlsForm::on_findLineEdit_editingFinished() {
    qDebug() << "on_findLineEdit_on_findLineEdit_editingFinished";
    emit findData( ui->findLineEdit->text());
}
void WordListGeneralControlsForm::on_deletePushButton_clicked() {
    //qDebug() << "on_deletePushButton_clicked";
    emit deleteData();
}
void WordListGeneralControlsForm::on_dataChoiceHorizontalScrollBar_valueChanged(){
    int next_index = ui->dataChoiceHorizontalScrollBar->value();
    //ui->dataComboBox->setCurrentIndex(next_index);
    emit newDataChosen(next_index);
}
void WordListGeneralControlsForm::on_fontSizeComboBox_currentIndexChanged(int i){
    qDebug() << "on_fontSizeComboBox_currentTextChanged" << ui->fontSizeComboBox->currentData();
    QVariant nf = ui->fontSizeComboBox->currentData();
    emit newFontSizeChosen(nf.toInt());
}
void WordListGeneralControlsForm::on_updateChosenNotChosen(unsigned int chosen_count,unsigned int not_chosen_count){
    //qDebug() << "on_updateChosenNotChosen" << chosen_count << "/" << not_chosen_count;
    QString newtext = QString::number(chosen_count);
    QString not_chosen_str = QString::number(not_chosen_count);
    newtext.append("/");
    newtext.append(not_chosen_str);
    ui->chosenNotChosenLabel->setText(newtext);
}
void WordListGeneralControlsForm::setDeleteEnabled(bool enabled){
    ui->deletePushButton->setEnabled(enabled);
}

void WordListGeneralControlsForm::allWidgetsEnabled(bool value){
    ui->dataChoiceHorizontalScrollBar->setEnabled(value);
    ui->loadPushButton->setEnabled(value);
    ui->findPushButton->setEnabled(value);
    ui->deletePushButton->setEnabled(value);
}
void WordListGeneralControlsForm::setDataIndex(int index){
    ui->dataChoiceHorizontalScrollBar->setValue(index);
}


