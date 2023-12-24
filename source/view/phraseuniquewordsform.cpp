
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
#include "phraseuniquewordsform.h"
#include "ui_phraseuniquewordsform.h"
#include <QMenu>
#include <QLabel>
#include <QPushButton>

PhraseUniqueWordsForm::PhraseUniqueWordsForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PhraseUniqueWordsForm)
    , my_selectionmodel(new QItemSelectionModel())
    , my_sortfilterproxymodel(new FilterModel(0))
    , model(PhraseUniqueWordsModel())
    , ngram_phrase_ptr(nullptr)
    , current_puw_ptr(nullptr)
    //, raw_1grams_ptr(data_hub.get1GramSharedPtr())
    , puw_ptr(nullptr)
    , current_word_length(-1)
{
    qDebug() << "PhraseUniqueWordsForm::PhraseUniqueWordsForm";
    ui->setupUi(this);
    ui->wordList_general_controls->setDeleteEnabled(false);
    ui->tableView->verticalHeader()->setVisible(true); // Get Vertical header and hide it

    QObject::connect(ui->wordList_general_controls, &WordListGeneralControlsForm::newDataChosen,this, &PhraseUniqueWordsForm::on_newDataChosen);
    QObject::connect(ui->wordList_general_controls, &WordListGeneralControlsForm::loadData,this, &PhraseUniqueWordsForm::on_loadData);
    QObject::connect(ui->wordList_general_controls, &WordListGeneralControlsForm::saveData,this, &PhraseUniqueWordsForm::on_saveData);
    QObject::connect(ui->wordList_general_controls, &WordListGeneralControlsForm::deleteData,this, &PhraseUniqueWordsForm::on_deleteData);
    QObject::connect(ui->wordList_general_controls, &WordListGeneralControlsForm::findData,this, &PhraseUniqueWordsForm::on_findData);
    QObject::connect(ui->wordList_general_controls, &WordListGeneralControlsForm::saveAllData,this, &PhraseUniqueWordsForm::on_saveAllData);

    QObject::connect(ui->wordList_general_controls,
                     &WordListGeneralControlsForm::changeFilter,
                     this,
                     &PhraseUniqueWordsForm::on_changeFilter);


//    auto* anyLayout = new QVBoxLayout();
//    anyLayout->addWidget(new QLabel("Some Text in Section", ui->spoiler));
//    anyLayout->addWidget(new QPushButton("Button in Section", ui->spoiler));
//    ui->spoiler->setContentLayout(*anyLayout);
    //        // todo, can the following just be in constructor ?

// TODO
// uncomment this and it breaks! on //    ui->tableView->setModel(my_sortfilterproxymodel);
// this
//    my_sortfilterproxymodel->invalidate();
    my_sortfilterproxymodel->setSourceModel(&model);
    my_selectionmodel->setModel(my_sortfilterproxymodel);
    ui->tableView->setModel(my_sortfilterproxymodel);
    ui->tableView->setSelectionModel(my_selectionmodel);
    ui->tableView->show();
    ui->wordList_general_controls->setDataIndex();
}

PhraseUniqueWordsForm::~PhraseUniqueWordsForm()
{
    delete ui;
}

void PhraseUniqueWordsForm::setPUW(QList<PhraseUniqueWords*>& ptr)
{
    qDebug() << "\nPhraseUniqueWordsForm::setPUW";
    puw_ptr = &ptr;
    ui->wordList_general_controls->setup(0, (*puw_ptr).size());
    ui->wordList_general_controls->setDataIndex(0);
}


void PhraseUniqueWordsForm::setPUWChosenCounts(const QList<int>& counts){
    //qDebug() << "PhraseUniqueWordsForm::setPUWChosenCounts" << counts;
    ui->ngram_puw_rationlize->setPUWCountLabel(counts);
}

void PhraseUniqueWordsForm::setNgramChosenCounts(const QList<int>& counts){
    //qDebug() << "PhraseUniqueWordsForm::setNgramChosenCounts";
    ui->ngram_puw_rationlize->setNgramCountLabel(counts);
}


void PhraseUniqueWordsForm::setNewWordData(int word_index_in_phrase){
    qDebug() << "\nPhraseUniqueWordsForm::setNewWordData" << word_index_in_phrase;
    if(!puw_ptr){
        return;
    }
    if( (word_index_in_phrase > -1) && (word_index_in_phrase < puw_ptr->size()) ){
        //current_word_length = word_length;
        current_word_index = word_index_in_phrase;
        current_puw_ptr = (*puw_ptr)[current_word_index];
        qDebug() << "setNewData" ;
        model.setNewData( *current_puw_ptr );
        ui->tableView->resizeColumnsToContents();
        QStringList newlabel;
        newlabel << QString("Phrase Index:");
        newlabel << QString::number(word_index_in_phrase);
        newlabel << QString("Len:");
        newlabel <<QString::number(current_puw_ptr->word_length);
        setWordListGeneralLabel(newlabel.join(" "));
        QObject::connect(current_puw_ptr,
                         &PhraseUniqueWords::updatePUWChosen,
                         ui->wordList_general_controls,
                         &WordListGeneralControlsForm::on_updateChosenNotChosen);
        current_puw_ptr->updateChosenCounts();
    }
    else{
       qDebug() << "ERROR word_index_in_phrase =" << word_index_in_phrase;
    }
}


void PhraseUniqueWordsForm::on_loadData(){
    //qDebug() << "on_loadData";
    if(current_puw_ptr){
        if(current_puw_ptr->loadData()){
           setNewWordData(current_puw_ptr->word_index_in_phrase);
        }
    }
}
void PhraseUniqueWordsForm::on_saveData()const{
    //qDebug() << "PhraseUniqueWordsForm::on_saveData";
    if(current_puw_ptr){
        current_puw_ptr->saveData();
    }
}
void PhraseUniqueWordsForm::on_saveAllData()const{
    qDebug() << "PhraseUniqueWordsForm::on_saveAllData";
    for(const auto& item: *puw_ptr){
        item->saveData();
    }
}

void PhraseUniqueWordsForm::on_deleteData(){
    //qDebug() << "PhraseUniqueWordsForm::on_deleteData";
    if(ngram_phrase_ptr){
        current_puw_ptr->deleteNotChosen2();
        setNewWordData(current_word_length);
    }
}
void PhraseUniqueWordsForm::on_newDataChosen(int index){
    //qDebug() << "PhraseUniqueWordsForm::on_newDataChosen" << index;
    setNewWordData(index);
}
void PhraseUniqueWordsForm::setWordListGeneralLabel(const QString& text){
    ui->wordList_general_controls->setLabel(text);
}
void PhraseUniqueWordsForm::update(){
    updateLabel();
    updateTable();
}
void PhraseUniqueWordsForm::updateLabel(){
    //    setLabel();
}
void PhraseUniqueWordsForm::updateTable(){
    ui->tableView->update();
}

void PhraseUniqueWordsForm::on_changeFilter(int filter_num){
    applyFilter(filter_num);
}


void PhraseUniqueWordsForm::on_tableView_space_bar_sig(){
    QModelIndexList to_toggle;
    QModelIndex new_selection;
    bool add_row = true;
    int new_selected_row = 0;
    for(const QModelIndex& index: ui->tableView->selectionModel()->selectedRows()){
        //qDebug() << "row/col" << index.row() << "/" << index.column();
        to_toggle.push_back(my_sortfilterproxymodel->mapToSource(index));
        if(add_row){
            new_selection = index;
            qDebug() << new_selection.row();
            if(new_selection.row() > 0){
                new_selected_row = new_selection.row()-1;
            }
            add_row = false;
        }
    }
    model.toggleChosen(to_toggle);
    if(my_sortfilterproxymodel->filter_mode == QString("all") ){
    }
    else{
        ui->tableView->selectRow(new_selected_row);
    }
    update();
}

void PhraseUniqueWordsForm::on_findData(const QString& find_string){
    qDebug() << "PhraseUniqueWordsForm::on_findData" << find_string;
    //void PhraseUniqueWordsForm::on_findPushButton_clicked(){
    //    QVariant s = QVariant (ui->findLineEdit->text());
    QVariant s = QVariant (find_string);
    //startrow =
    QList<QModelIndex> list = model.match(
        model.index(0,1),
        Qt::DisplayRole,
        s,
        Qt::MatchContains | Qt::MatchRecursive
        );
    if(list.size() > 0){
        my_selectionmodel->select(list[0], QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
        ui->tableView->scrollTo(my_sortfilterproxymodel->mapFromSource(list[0]));
        qDebug() << "meh" << list;
    }
}
//void PhraseUniqueWordsForm::on_delPushButton_clicked(){
//    emit model.beginResetModel();
//    gng_ngram->deleteNotChosen2();
//    update();
//}
void PhraseUniqueWordsForm::on_tableView_left_doubleclick_sig(){
    QModelIndexList to_toggle;
    for(const QModelIndex& index: ui->tableView->selectionModel()->selectedRows()){
        //qDebug() << "row/col" << index.row() << "/" << index.column();
        to_toggle.push_back(my_sortfilterproxymodel->mapToSource(index));
    }
    model.toggleChosen(to_toggle);
    update();
}

void PhraseUniqueWordsForm::applyFilter(int a){
    //    check filters for phrase unqiue words
    qDebug() << "ngramApplyFilter" << a;
    switch(a){
    case not_chosen_filter: my_sortfilterproxymodel->setFilter("not_chosen"); break;
    case chosen_filter:     my_sortfilterproxymodel->setFilter("chosen");     break;
    case all_filter:        my_sortfilterproxymodel->setFilter("");           break;
    default:;
    }
}
void PhraseUniqueWordsForm::on_tableView_customContextMenuRequested(){
    //qDebug() << pt.x() << pt.y();
    //qDebug() << "showContextMenu passed" << pt.x() << pt.y();
    //QPoint globalPos = ui->tableView->mapToGlobal(pt);
    //qDebug() << globalPos.x() << globalPos.y();
    qDebug() << QCursor().pos().x() << QCursor().pos().y();
    QPoint pt2 = QPoint(QCursor().pos().x() - 100 , QCursor().pos().y()  );
    QMenu contextMenu(tr("NGRAM Filter Menu"), this);
    QAction* all = contextMenu.addAction("Filer: All");
    QAction* chosen = contextMenu.addAction("Filer: Chosen");
    QAction* not_chosen = contextMenu.addAction("Filer: Not Chosen");
    //QAction* reload = contextMenu.addAction("Reload Data");
    QAction* selectedItem = contextMenu.exec(pt2);
    if (selectedItem == all){ applyFilter(all_filter); }
    if (selectedItem == chosen){ applyFilter(chosen_filter); }
    if (selectedItem == not_chosen){ applyFilter(not_chosen_filter); }
    //if (selectedItem == reload){ applyFilter(3); }
}
