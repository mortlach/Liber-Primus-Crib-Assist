
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
#ifndef NGRAMDATAFORM_H
#define NGRAMDATAFORM_H

#include <QWidget>
#include <QItemSelectionModel>
//#include "ngramdatafiltermodel.h"
#include "filtermodel.h"
#include "ngramdatahub.h"
#include "ngramdatamodel.h"
#include "baseform.h"


namespace Ui {
class NGramDataForm;
}

class NGramDataForm : public BaseForm
{
    Q_OBJECT

public:
    explicit NGramDataForm(QWidget *parent = nullptr);
    ~NGramDataForm();
    void setWordListGeneralLabel(const QString& text);
    void setNgramData(QList<NgramData*>& ptr);
    void setNgramChosenCounts(const QList<int>& counts);
    void setPUWChosenCounts(const QList<int>& counts);
    inline void clear(){ model.clear();}


public slots:
    void on_tableView_left_doubleclick_sig();
    void on_tableView_space_bar_sig();
    void update(); // main update function
    void updateLabel(); // main update function
    void updateTable(); // main update function
    void on_newDataChosen(int index);
    void on_loadData();
    void on_saveData()const;
    void on_saveAllData()const;
    void on_deleteData();
    void on_findData(const QString& find_string);
    void on_changeFilter(int filter_num);
    void on_changeFontSize(int filter_num);


    friend class MainWindow;
    friend class WordListGeneralControlsForm;
    friend class WordTab;
    friend class ChoosePhrase;

protected:
    NGramDataModel model;


private:
    Ui::NGramDataForm *ui;

    void setNewNgramDataInGUI(int ngram_index);
    void applyFilter(int a);
    // eventuially will need a version of all of these
    QItemSelectionModel* ngram_selectionmodel;
    int current_ngram_index;

    enum Constants
    {
        all_filter = 0,
        chosen_filter = 1,
        not_chosen_filter = 2
    };
};
#endif // PHRASEUNIQUEWORDSFORM_H
