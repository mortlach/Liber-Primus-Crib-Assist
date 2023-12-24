
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
#ifndef NGRAMPUWRATIONLIZEFORM_H
#define NGRAMPUWRATIONLIZEFORM_H

#include <QWidget>
#include <QCheckBox>

namespace Ui {
class NgramPUWRationlizeForm;
}
/*!
 * \brief The NgramPUWRationlizeForm class
 */
class NgramPUWRationlizeForm : public QWidget
{
    Q_OBJECT

public:
    explicit NgramPUWRationlizeForm(QWidget *parent = nullptr);
    ~NgramPUWRationlizeForm();

    /*!
     * \brief setPUWCountLabel
     * \param counts
     */
    void setPUWCountLabel(const QList<int>& counts);
    /*!
     * \brief setNgramCountLabel
     * \param counts
     */
    void setNgramCountLabel(const QList<int>& counts);
    /*!
     * \brief setTotalPossiblePhrasesLabel
     * \param puw_counts
     */
    void setTotalPossiblePhrasesLabel(const QList<int>& puw_counts);
    /*!
     * \brief setTotalPossiblePhrasesLabel
     * \param count
     */
    void setTotalPossiblePhrasesLabel(unsigned long long count);

    bool isPUWPriority()const;
    bool isNgramPriority()const;


signals:
    void saveNgramPhrases();
    void viewNgramPhrases();
    void rationalizeNgramData();
    void deleteNotChosenNgrams(Qt::CheckState state);
    void puwHasPriority(bool value);


public slots:
    void on_savePossiblePhrasesPushButton_clicked();
    void on_viewPossiblePhrasesPushButton_clicked();
    void on_rationlizePushButton_clicked();
    void on_puwPriorityRadioButton_toggled();
    void on_ngramPriorityRadioButton_toggled();
    void on_deleteNotChosenNgramsCheckBox_stateChanged(int state);


    friend class MainWindow;
    friend class WordListGeneralControlsForm;
    friend class WordTab;
    friend class ChoosePhrase;

private:
    Ui::NgramPUWRationlizeForm *ui;
};

#endif // NGRAMPUWRATIONLIZEFORM_H
