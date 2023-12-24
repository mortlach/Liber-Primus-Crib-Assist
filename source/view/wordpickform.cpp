
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
#include <QComboBox>
#include "wordpickform.h"
#include "utilities.h"
#include "ui_wordpickform.h"

WordPickFORM::WordPickFORM(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WordPickFORM)
    , puw_ptr(nullptr)
    , gridLayout(nullptr)
    , words_per_row(5)
{
    ui->setupUi(this);
}

WordPickFORM::~WordPickFORM(){
    delete ui;
}

void WordPickFORM::setNewPUW(QList<PhraseUniqueWords*>& ptr){
    puw_ptr = &ptr;
    if(gridLayout){
        // delete existing widgets and layout, then make and setup new one
        QLayoutItem *child;
        while ((child = gridLayout->takeAt(0)) != nullptr) {
            delete child->widget(); // delete the widget
            delete child;   // delete the layout item
        }
        ui->baseGridLayout->removeItem(gridLayout);
        delete gridLayout;
        gridLayout = nullptr;
    }
    gridLayout = new QGridLayout();
    ui->baseGridLayout->addLayout(gridLayout, 0, 0, 1, 1, Qt::AlignCenter);
    ui->baseGridLayout->update();
    int row_num = 0;
    int col_num= 0;
    for(const auto& item: *puw_ptr){
        QComboBox* next_combo = new QComboBox();
        QFont font("Courier New",14);
        next_combo->setFont(font);
        next_combo->addItems(item->getChosenWordsInCountOrder());
        gridLayout->addWidget(next_combo, row_num, col_num, 1, 1, Qt::AlignCenter);
        if(col_num == words_per_row){
            row_num +=1 ;
            col_num = 0;
        }else{
            col_num += 1;
        }
        qDebug() << "WordPickFORM added item";
    }
}
