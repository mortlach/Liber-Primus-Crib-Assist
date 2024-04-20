
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
#include "mainwindow.h"
#include "ngramphraseworker.h"
#include "qstyleoption.h"
#include "ui_choosephrase.h"
#include "ui_mainwindow.h"
#include "ui_ngramdataform.h"
#include "ui_ngrampuwrationlizeform.h"
#include "ngrampuwrationlizeform.h"
#include "ui_phraseuniquewordsform.h"
#include "utilities.h"
#include "definitions.h"
#include <QProxyStyle>
#include <QTabBar>
#include <QMessageBox>
#include <QtConcurrent>
#include <QImage>

class CustomTabStyle : public QProxyStyle {
public:
    QSize sizeFromContents(ContentsType type, const QStyleOption* option,
                           const QSize& size, const QWidget* widget) const {
        QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);
        if (type == QStyle::CT_TabBarTab) {
            s.transpose();
        }
        return s;
    }
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const {
        if (element == CE_TabBarTabLabel) {
            if (const QStyleOptionTab* tab = qstyleoption_cast<const QStyleOptionTab*>(option)) {
                QStyleOptionTab opt(*tab);
                opt.shape = QTabBar::RoundedNorth;
                QProxyStyle::drawControl(element, &opt, painter, widget);
                return;
            }
        }
        QProxyStyle::drawControl(element, option, painter, widget);
    }
};

QTextEdit * MainWindow::s_textEdit = nullptr;
MainWindow::MainWindow(QSettings& settings_in, QWidget *parent)
    : ui(new Ui::MainWindow)
    , main_settings(settings_in)
    , busy(false)
{
    Q_UNUSED(parent);
    utilities::printSettings(main_settings);
    ui->setupUi(this);
    showLPSentences();
    s_textEdit = ui->textEdit;
    QString title = QString("LP Crib Assist: A Game of Guess the Words");
    setWindowTitle(title);
    //QSettings settings(definitions::CONFIG_PATH, QSettings::IniFormat);
    // setNgramPathRoot
    ui->choosePhrase->setNgramPathRoot( QDir(main_settings.value(definitions::PATH_TO_RAW_NGRAMS).toString()));
    ui->choosePhrase->setPhrasePathSaveRoot( QDir(main_settings.value(definitions::PATH_TO_PHRASE_SAVE_ROOT).toString()));
    ui->tabWidget->tabBar()->setStyle(new CustomTabStyle);
    QObject::connect(ui->choosePhrase, &ChoosePhrase::newPhraseChosen, this, &MainWindow::on_newPhraseChosen);
    // testing
    bool auto_set_phrase = false;
    if(auto_set_phrase){
        auto t = ui->choosePhrase;
        t->ui->wordLengthLineEdit->setText("s,3,12,4");
        t->on_wordLengthLineEdit_editingFinished();
        t->ui->ngramFileLoadPushButton->click();
    }
    // menu actions
    QObject::connect(ui->actionLoad_Phrase, &QAction::triggered,this, &MainWindow::openPhrase);
    QObject::connect(ui->actionClear_Phrase, &QAction::triggered,this, &MainWindow::clearPhrase);
    // handy ref
    auto& pt = ui->puw_tab->ui->ngram_puw_rationlize;
    auto& nt = ui->ngram_tab->ui->ngram_puw_rationlize;
    // rationalize PUW and ngram choices
    QObject::connect(pt,&NgramPUWRationlizeForm::rationalizeNgramData,this,&MainWindow::on_rationalizeNgramData);
    QObject::connect(nt,&NgramPUWRationlizeForm::rationalizeNgramData,this,&MainWindow::on_rationalizeNgramData);
    // save data
    QObject::connect(pt,&NgramPUWRationlizeForm::saveNgramPhrases,this,&MainWindow::on_saveNgramPhrases);
    QObject::connect(nt,&NgramPUWRationlizeForm::saveNgramPhrases,this,&MainWindow::on_saveNgramPhrases);
    // delete data that is not chosen
    QObject::connect(pt,&NgramPUWRationlizeForm::deleteNotChosenNgrams,this,&MainWindow::on_deleteNotChosenNgrams);
    QObject::connect(nt,&NgramPUWRationlizeForm::deleteNotChosenNgrams,this,&MainWindow::on_deleteNotChosenNgrams);
    // view all possible phrase combos (limited)
    QObject::connect(pt,&NgramPUWRationlizeForm::viewNgramPhrases,this,&MainWindow::on_ShowAllPhrases);
    QObject::connect(nt,&NgramPUWRationlizeForm::viewNgramPhrases,this,&MainWindow::on_ShowAllPhrases);
    //
    QObject::connect(pt,&NgramPUWRationlizeForm::puwHasPriority,this,&MainWindow::on_puwHasPriority);
    QObject::connect(nt,&NgramPUWRationlizeForm::puwHasPriority,this,&MainWindow::on_puwHasPriority);
    //
    ui->mainWords->setNewWordData(0);
    // progress bar
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(0);
    ui->progressBar->hide();
    //
    NgramDataHub ngram_data_hub = NgramDataHub();
    if(main_settings.value(definitions::LOAD_LAST_PHRASE_ON_STARTUP).toBool()){
        QString chosen_dir = main_settings.value(definitions::PATH_TO_LAST_PHRASE).toString();
        QString chosen_fn ="phrase_config.ini";
        ngram_data_hub.setPath(chosen_dir,chosen_fn);
        ui->progressBar->show();
        //ui->tabWidget->setCurrentIndex(7);
        // thread idiom, worker class passed to new thread and set up to run specific functions that take time
        if(!busy){
            busy = true;
            WorkerThread* worker_thread = new WorkerThread(this);
            NgramPhraseWorker* phrase_worker = new NgramPhraseWorker();
            phrase_worker->setPath(chosen_dir,chosen_fn);
            phrase_worker->moveToThread(worker_thread);
            connect(worker_thread, &WorkerThread::started, phrase_worker, &NgramPhraseWorker::processLoadPhrase);
            connect(phrase_worker, &NgramPhraseWorker::finished, worker_thread, &WorkerThread::quit);
            connect(worker_thread, &WorkerThread::finished, worker_thread, &WorkerThread::deleteLater);
            connect(phrase_worker, &NgramPhraseWorker::finished, this, &MainWindow::updateGuiAfterLoadRawPhrase);
            // append to text TODO not thread safe
            // connect(worker_thread, &WorkerThread::debug,ui->textEdit, &QTextEdit::append);
            worker_thread->start();
        }
        setGUIEnabled(false);
    }
    ui->tabWidget->setCurrentIndex(1);
}
MainWindow::~MainWindow(){
    s_textEdit = nullptr;
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event){
    Q_UNUSED(event);
    clearPhrase();
}
void MainWindow::on_puwHasPriority(bool puw_has_priority){
    qDebug() << "MainWindow::on_puwHasPriority" << puw_has_priority;
    ui->puw_tab->ui->ngram_puw_rationlize->ui->puwPriorityRadioButton->setChecked(puw_has_priority);
    ui->puw_tab->ui->ngram_puw_rationlize->ui->ngramPriorityRadioButton->setChecked(!puw_has_priority);
    ui->ngram_tab->ui->ngram_puw_rationlize->ui->puwPriorityRadioButton->setChecked(puw_has_priority);
    ui->ngram_tab->ui->ngram_puw_rationlize->ui->ngramPriorityRadioButton->setChecked(!puw_has_priority);
}

void MainWindow::clearPhrase(){
    qDebug() << "MainWindow::clearPhrase";
    ui->puw_tab->clear();
    ui->ngram_tab->clear();
    NgramDataHub ngram_data_hub = NgramDataHub();
    ngram_data_hub.clearPhraseData();
}

void MainWindow::openPhrase(){
    qDebug() << "MainWindow::openPhrase";
    QString start_dir  = QDir(main_settings.value(definitions::PATH_TO_PHRASE_SAVE_ROOT).toString()).path();
    QString chosen_dir = QString("");
    QString chosen_fn = QString("");
    QString message = QString("Chose folder, data in existing folders will be overwritten");
    utilities::getFileNameAndDir(start_dir, chosen_dir, chosen_fn);
    qDebug() << "chosen_dir:" << chosen_dir << "chosen_fn" << chosen_fn;
    NgramDataHub ngram_data_hub = NgramDataHub();
    ngram_data_hub.setPath(chosen_dir,chosen_fn);
    // thread idiom, worker class passed to new thread and set up to run specific functions that take time
    if(!busy){
        ui->progressBar->show();
        //ui->tabWidget->setCurrentIndex(7);
        busy = true;
        WorkerThread* worker_thread = new WorkerThread(this);
        NgramPhraseWorker* phrase_worker = new NgramPhraseWorker();
        phrase_worker->setPath(chosen_dir,chosen_fn);
        phrase_worker->moveToThread(worker_thread);
        connect(worker_thread, &WorkerThread::started, phrase_worker, &NgramPhraseWorker::processLoadPhrase);
        connect(phrase_worker, &NgramPhraseWorker::finished, worker_thread, &WorkerThread::quit);
        connect(worker_thread, &WorkerThread::finished, worker_thread, &WorkerThread::deleteLater);
        connect(phrase_worker, &NgramPhraseWorker::finished, this, &MainWindow::updateGuiAfterLoadRawPhrase);
        // append to text
        // connect(worker_thread, &WorkerThread::debug,ui->textEdit, &QTextEdit::append);
        worker_thread->start();
        setGUIEnabled(false);
    }
}

void MainWindow::setGUIEnabled(bool enable){
    setEnabled(enable);
}

void MainWindow::updateGuiAfterLoadRawPhrase(){
    NgramDataHub ngram_data_hub = NgramDataHub();
    NgramPhrase& ngram_phrase_ptr = ngram_data_hub.getNgramPhraseRef();
    busy = false;
    ui->puw_tab->setPUW(ngram_phrase_ptr.getPhraseUniqueWordsRef());
    qDebug() << "puw_tab setNewWordData 0";
    ui->puw_tab->setNewWordData(0);
    // set up new tabs and pass data in
    ui->wordpick_tab->setNewPUW(ngram_phrase_ptr.getPhraseUniqueWordsRef());
    //ui->ngram_tab->setNgramData(ngram_phrase_ptr->getNgramDataPtr());
    ui->ngram_tab->setNgramData(ngram_phrase_ptr.getNgramDataRef());
    //qDebug() << "ngram_tab setNgramData 0";
    ui->ngram_tab->setNewNgramDataInGUI(0);
    connectPhrase();
    // update gui
    updatePUWNgramCOuntsEnable();
}

void MainWindow::updatePUWNgramCOuntsEnable(){
    updateChosenPUWRunningTotals();
    updateChosenNgramRunningTotals();
    ui->progressBar->hide();
    setGUIEnabled(true);
}

void MainWindow::on_newPhraseChosen(const NgramPhraseMetaData& data){
    if(!busy){
        setGUIEnabled(false);
        ui->progressBar->show();
        busy = true;
        //allWidgetsEnabled(false);
        WorkerThread* worker_thread = new WorkerThread(this);
        NgramPhraseWorker* phrase_worker = new NgramPhraseWorker();
        phrase_worker->setNgramPhraseDataStruct(data);
        phrase_worker->moveToThread(worker_thread);
        connect(worker_thread, &WorkerThread::started, phrase_worker, &NgramPhraseWorker::processNewPhrase);
        connect(phrase_worker, &NgramPhraseWorker::finished, worker_thread, &WorkerThread::quit);
        connect(worker_thread, &WorkerThread::finished, worker_thread, &WorkerThread::deleteLater);
        connect(phrase_worker, &NgramPhraseWorker::finished, this, &MainWindow::updateGuiAfterLoadRawPhrase);
        connect(worker_thread, &WorkerThread::debug,ui->textEdit, &QTextEdit::append);
        worker_thread->start();
    }
}
void MainWindow::connectPhrase()
{   // for each puw data set connect the updatePUWChosen
    NgramDataHub ngram_data_hub = NgramDataHub();
    NgramPhrase& ngram_phrase_ptr = ngram_data_hub.getNgramPhraseRef();
    for(auto& item: ngram_phrase_ptr.getPhraseUniqueWordsRef()){
        QObject::connect(item,
                         &PhraseUniqueWords::updatePUWChosen,this,
                         &MainWindow::updateChosenPUWRunningTotals);
    }
    for(auto& item: ngram_phrase_ptr.getNgramDataRef()){
        QObject::connect(item,
                         &NgramData::updateNgramChosen, this,
                         &MainWindow::updateChosenNgramRunningTotals);
    }
}
//
void MainWindow::on_ShowAllPhrases(){
    NgramDataHub ngram_data_hub = NgramDataHub();
    NgramPhrase& ngram_phrase_ptr = ngram_data_hub.getNgramPhraseRef();
    auto puw_ptr = ngram_phrase_ptr.getPhraseUniqueWordsRef();
    QList<QStringList> main_phrase_list;
    QList<QString> this_puw_chosen_words;
    puw_ptr.at(0)->getChosenWords(this_puw_chosen_words);
    qDebug() << "this_puw_chosen_words.size()" << this_puw_chosen_words.size();
    for(const auto& word : this_puw_chosen_words){
        QStringList nl;
        nl.push_back(word);
        main_phrase_list.push_back(nl);
    }
    qDebug() << "main_phrase_list.size()" << main_phrase_list.size();
    for(int i = 1; i < puw_ptr.size(); ++i){
        puw_ptr.at(i)->getChosenWords(this_puw_chosen_words);
        qDebug() << "this_puw_chosen_words.size()" << this_puw_chosen_words.size();
        utilities::extendPhrases(main_phrase_list,this_puw_chosen_words);
        qDebug() << "main_phrase_list.size()" << main_phrase_list.size();
    }
    qDebug() << "main_phrase_list.size()" << main_phrase_list.size();
    QList<QString> join_phrases;
    for(const auto& item: main_phrase_list ){
        join_phrases << item.join(" ");
    }
    QString text = join_phrases.join("\n");
    // loop over every puw and generate phrases
    ui->allPhrasesTextEdit->clear();
    ui->allPhrasesTextEdit->setText(text);
}
//
void MainWindow::on_deleteNotChosenNgrams(Qt::CheckState state){
    ui->ngram_tab->ui->ngram_puw_rationlize->ui->deleteNotChosenNgramsCheckBox->setCheckState(state);
    ui->puw_tab->ui->ngram_puw_rationlize->ui->deleteNotChosenNgramsCheckBox->setCheckState(state);
}
//
void MainWindow::updateChosenPUWRunningTotals(){
    NgramDataHub ngram_data_hub = NgramDataHub();
    NgramPhrase& ngram_phrase_ptr = ngram_data_hub.getNgramPhraseRef();
    ui->puw_tab->setPUWChosenCounts(ngram_phrase_ptr.getPUWChosenCounts());
    ui->ngram_tab->setPUWChosenCounts(ngram_phrase_ptr.getPUWChosenCounts());
}
void MainWindow::updateChosenNgramRunningTotals(){
    NgramDataHub ngram_data_hub = NgramDataHub();
    NgramPhrase& ngram_phrase_ptr = ngram_data_hub.getNgramPhraseRef();
    ui->ngram_tab->setNgramChosenCounts(ngram_phrase_ptr.getNgramChosenCounts());
    ui->puw_tab->setNgramChosenCounts(ngram_phrase_ptr.getNgramChosenCounts());
}
//
void MainWindow::on_saveNgramPhrases(){
    qDebug() << "MainWindow::on_saveNgramPhrases";
    //ngram_data_hub.savePhrase();
}
//
void MainWindow::on_rationalizeNgramData(){
    qDebug() << "MainWindow::on_rationalizeNgramData";
    if(!busy){
        busy = true;
        setGUIEnabled(false);
        ui->progressBar->show();
        ui->puw_tab->model.beginResetThisModel();
        ui->ngram_tab->model.beginResetThisModel();
        WorkerThread* worker_thread = new WorkerThread(this);
        NgramPhraseWorker* phrase_worker = new NgramPhraseWorker();
        bool do_delete = ui->ngram_tab->ui->ngram_puw_rationlize->ui->deleteNotChosenNgramsCheckBox->isChecked();
        //bool puw_has_priority = ui->puw_tab->ui->ngram_puw_rationlize->isPUWPriority();
        phrase_worker->setDoDelete(do_delete);
        phrase_worker->moveToThread(worker_thread);
        connect(worker_thread, &WorkerThread::started, phrase_worker, &NgramPhraseWorker::processRationalizePhrase);
        connect(phrase_worker, &NgramPhraseWorker::finished, worker_thread, &WorkerThread::quit);
        connect(worker_thread, &WorkerThread::finished, worker_thread, &WorkerThread::deleteLater);
        connect(phrase_worker, &NgramPhraseWorker::finished, this, &MainWindow::updateGuiAfterLoadRawPhrase);
        connect(worker_thread, &WorkerThread::debug,ui->textEdit, &QTextEdit::append);
        worker_thread->start();
    }
    else{
        qDebug() << "ERROR Threads are busy";
    }
}
void MainWindow::showLPSentences(){
    std::vector<std::vector<std::string>> file_data;
    QString fp  = QDir(main_settings.value(definitions::PATH_TO_LP_SENTENCES).toString()).path();
    utilities::readTrueCSVFile(fp.toStdString(),file_data);
    // first element of lines 0,2,4,6 ... are word lengths
    // first element of lines 1,3,5,7 ... are rune words
    int counter = 0;
    QStringList qsl = QStringList();
    for(const auto& item: file_data){
        if(counter % 2 == 0){
            qsl << QString(item.at(0).c_str());
        }
        else{
            qsl << QString(item.at(0).c_str());
        }
    }
    QString data = qsl.join("\n");
    ui->lpSentsPlainTextEdit->setPlainText(data);
}







