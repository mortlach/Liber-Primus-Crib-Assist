
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
#include "ngrampuwrationlizeform.h"
#include "utilities.h"
#include "ui_ngrampuwrationlizeform.h"

NgramPUWRationlizeForm::NgramPUWRationlizeForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NgramPUWRationlizeForm)
{
    ui->setupUi(this);
    ui->groupBox->hide();
}

NgramPUWRationlizeForm::~NgramPUWRationlizeForm()
{
    delete ui;
}

void NgramPUWRationlizeForm::setPUWCountLabel(const QList<int>& counts){
    QString s = utilities::toString(counts);
    ui->puwCountLabel->setText(s);
    setTotalPossiblePhrasesLabel(counts);
}

void NgramPUWRationlizeForm::setNgramCountLabel(const QList<int>& counts){
    QString s = utilities::toString(counts);
    ui->ngramCountsLabel->setText(s);
}

void NgramPUWRationlizeForm::setTotalPossiblePhrasesLabel(const QList<int>& puw_counts){
    unsigned long long total = 1;
    for(const auto& item: puw_counts){
        total *= (unsigned long  long)item;
        //qDebug() << "setTotalPossiblePhrasesLabel total=" << total;
    }
    if(total < 1000000){     // number
        ui->viewPossiblePhrasesPushButton->setEnabled(true);
    }else{
        ui->viewPossiblePhrasesPushButton->setEnabled(false);
    }

    setTotalPossiblePhrasesLabel(total);
}


void NgramPUWRationlizeForm::setTotalPossiblePhrasesLabel(unsigned long long counts){
    ui->totalPossiblePhrasesLabel->setText(QString::number(counts));
}

void NgramPUWRationlizeForm::on_deleteNotChosenNgramsCheckBox_stateChanged(int state){
    if(state == Qt::CheckState::Checked){
        emit deleteNotChosenNgrams(Qt::CheckState::Checked);
    }
    else{
        emit deleteNotChosenNgrams(Qt::CheckState::Unchecked);
    }
}

void NgramPUWRationlizeForm::on_savePossiblePhrasesPushButton_clicked(){
    qDebug() << "NgramPUWRationlizeForm::on_savePossiblePhrasesPushButton_clicked";
    emit saveNgramPhrases();
}
void NgramPUWRationlizeForm::on_viewPossiblePhrasesPushButton_clicked(){
    qDebug() << "NgramPUWRationlizeForm::on_viewPossiblePhrasesPushButton_clicked";
    emit viewNgramPhrases();
}
void NgramPUWRationlizeForm::on_rationlizePushButton_clicked(){
    qDebug() << "NgramPUWRationlizeForm::on_rationlizePushButton_clicked";
    emit rationalizeNgramData();
}
void NgramPUWRationlizeForm::on_puwPriorityRadioButton_toggled(){
    qDebug() << "NgramPUWRationlizeForm::on_puwPriorityRadioButton_toggled";
    emit puwHasPriority(isPUWPriority());
}

void NgramPUWRationlizeForm::on_ngramPriorityRadioButton_toggled(){

}

bool NgramPUWRationlizeForm::isPUWPriority()const{
    return ui->puwPriorityRadioButton->isChecked();
}
bool NgramPUWRationlizeForm::isNgramPriority()const{
    return ui->ngramPriorityRadioButton->isChecked();
}


