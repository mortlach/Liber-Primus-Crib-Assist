
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
#ifndef WORDPICKFORM_H
#define WORDPICKFORM_H

#include <QWidget>
#include <QGridLayout>
#include "ngramdatahub.h"


namespace Ui {
class WordPickFORM;
}

class WordPickFORM : public QWidget
{
    Q_OBJECT

public:
    explicit WordPickFORM(QWidget *parent = nullptr);
    ~WordPickFORM();

    void setNewPUW(QList<PhraseUniqueWords*>& ptr);

private:
    Ui::WordPickFORM *ui;
    NgramDataHub data_hub;
    QList<PhraseUniqueWords*>* puw_ptr;
    QGridLayout* gridLayout;
    int words_per_row;
};

#endif // WORDPICKFORM_H
