
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
#ifndef WORDTAB_H
#define WORDTAB_H

#include <QWidget>
#include <QItemSelectionModel>
#include "raw1grammodel.h"
#include "raw1grams.h"
#include "ngramdatahub.h"
#include "filtermodel.h"

namespace Ui {
class WordTab;
}

class WordTab : public QWidget
{
    Q_OBJECT

public:
    explicit WordTab(QWidget *parent = nullptr);
    ~WordTab();

    void setWordListGeneralLabel(const QString& text);
    inline void clear(){ model.clear();}

signals:
    void message(const QString& str);

public slots:
    void on_tableView_customContextMenuRequested();
    void on_tableView_left_doubleclick_sig();
    void on_tableView_space_bar_sig();
    void on_tableView_delete_sig();
    void on_tableView_t_sig(); // set true
    void on_tableView_f_sig(); // set false
    void on_tableView_alt_a_sig(); // set filter - all
    void on_tableView_alt_c_sig(); // set filter - chosen
    void on_tableView_alt_n_sig(); // set filter - not chosen



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
    void on_changeFontSize(int fs);

    friend class MainWindow;
    friend class WordListGeneralControlsForm;
    friend class ChoosePhrase;

private:
    // set word data
    void setNewWordData(int word_length);
    void applyFilter(int a);
    Ui::WordTab *ui;
    // eventuially will need a version of all of these
    QItemSelectionModel* onegram_selectionmodel;
    FilterModel* my_sortfilterproxymodel;
    Raw1GramModel model;
    NgramDataHub data_hub;
    const Raw1Grams * const raw_1grams_ptr;
    RawWordData* current_raw_1grams_ptr;
    int current_word_length;
    int current_word_index;
    enum Constants
    {
        all_filter = 0,
        chosen_filter = 1,
        not_chosen_filter = 2
    };

};

#endif // WORDTAB_H
