
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
#ifndef WORDLISTGENERALCONTROLSFORM_H
#define WORDLISTGENERALCONTROLSFORM_H

#include <QWidget>

namespace Ui {
class WordListGeneralControlsForm;
}

class WordListGeneralControlsForm : public QWidget
{
    Q_OBJECT

public:
    explicit WordListGeneralControlsForm(QWidget *parent = nullptr, int data_count = 14);
    ~WordListGeneralControlsForm();

    void setLabel(const QString& text);
    void allWidgetsEnabled(bool value);
    void setDataIndex(int index = 0);
    void setup(int min, int data_count);
    void setDeleteEnabled(bool enabled);

public slots:
    void on_savePushButton_clicked();
    void on_saveAllPushButton_clicked();
    void on_loadPushButton_clicked();
    void on_findPushButton_clicked();
    void on_findLineEdit_editingFinished();
    void on_deletePushButton_clicked();
    void on_updateChosenNotChosen(unsigned int chosen_count,unsigned int not_chosen_count);
    void on_filterChanged();
    void on_dataChoiceHorizontalScrollBar_valueChanged();
    void on_fontSizeComboBox_currentIndexChanged(int i);

signals:
    void newDataChosen(int index);
    void newFontSizeChosen(int index);
    void loadData();
    void saveData();
    void saveAllData();
    void deleteData();
    void findData(const QString& find_string);
    void message(const QString& str);
    void changeFilter(int filter_num);

    friend class MainWindow;
    friend class WordTab;
    friend class ChoosePhrase;
    friend class PhraseUniqueWordsForm;
    friend class NGramDataForm;

private:
    Ui::WordListGeneralControlsForm *ui;
    int current_index;
    int data_count;
    int min_index;
};

#endif // 3WORDLISTGENERALCONTROLSFORM_H
