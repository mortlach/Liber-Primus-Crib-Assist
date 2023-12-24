
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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
//#include <QFutureWatcher>
#include <QTextEdit>
#include "wordtab.h"
#include "ngramphraseworker.h"
#include <QThread>
/*!
 * \brief The WorkerThread class
 * to display qDebug() calls from threads in GUI use this thread
 */
class WorkerThread : public QThread
{
    Q_OBJECT
public:
    WorkerThread(QObject *parent){
        Q_UNUSED(parent);
    };
    ~WorkerThread(){
        if(isRunning())
        {
            //requestInterruption(); // Stop loop
            wait(); // Wait for return from run()
        }
    }
signals:
    void debug(QString);
protected:
    //void run() override;
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QSettings& settings_in, QWidget *parent = nullptr);
    ~MainWindow();
    // for prtinting to GUI, not used atm due to thread safety
    static QTextEdit * s_textEdit;

public slots:
    /*!
     * \brief on_newPhraseChosen
     * \param data
     */
    void on_newPhraseChosen(const NgramPhraseMetaData& data);
    /*!
     * \brief on_saveNgramPhrases
     */
    void on_saveNgramPhrases();
    /*!
     * \brief on_rationalizeNgramData
     */
    void on_rationalizeNgramData();
    /*!
     * \brief on_deleteNotChosenNgrams
     * \param state
     */
    void on_deleteNotChosenNgrams(Qt::CheckState state);
    /*!
     * \brief on_ShowAllPhrases
     */
    void on_ShowAllPhrases();
    /*!
     * \brief openPhrase
     */
    void openPhrase();
    /*!
     * \brief clearPhrase
     */
    void clearPhrase();
    /*!
     * \brief closeEvent
     * \param event
     */
    void closeEvent (QCloseEvent *event);
    /*!
     * \brief on_puwHasPriority
     * \param puw_has_priority
     */
    void on_puwHasPriority(bool puw_has_priority);

    friend class WordListGeneralControlsForm;
    friend class WordTab;
    friend class ChoosePhrase;
    friend class PhraseUniqueWordsForm;
    friend class NgramPUWRationlizeForm;

private:
    Ui::MainWindow *ui;
    /*!
     * \brief main_settings
     */
    QSettings& main_settings;
    /*!
     * \brief setGUIEnabled
     * \param enable
     */
    void setGUIEnabled(bool enable);
    /*!
     * \brief updateChosenPUWRunningTotals
     */
    void updateChosenPUWRunningTotals();
    /*!
     * \brief updateChosenNgramRunningTotals
     */
    void updateChosenNgramRunningTotals();
    /*!
     * \brief updateGuiAfterLoadRawPhrase
     */
    void updateGuiAfterLoadRawPhrase();
    /*!
     * \brief connectPhrase
     */
    void connectPhrase();
    /*!
     * \brief updatePUWNgramCOuntsEnable
     */
    void updatePUWNgramCOuntsEnable();
    /*!
     * \brief showLPSentences
     */
    void showLPSentences();
    /*!
     * \brief busy
     * true while a theead is working
     */
    bool busy;

};
#endif // MAINWINDOW_H
