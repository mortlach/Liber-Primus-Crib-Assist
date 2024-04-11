
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
#include "wordtab.h"
#include "ui_wordtab.h"
#include <QLabel>
#include <QMenu>
#include <QPushButton>

WordTab::WordTab(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WordTab)
    , onegram_selectionmodel(new QItemSelectionModel())
    , my_sortfilterproxymodel(new FilterModel(0))
    , model(Raw1GramModel())
    // todo move this
    , data_hub(NgramDataHub())
    , raw_1grams_ptr(&data_hub.get1GramRef())
    , current_raw_1grams_ptr(nullptr)
    , current_word_length(-1)
    , current_word_index(-1)
{
    qDebug() << "WordTab::WordTab";
    ui->setupUi(this);
    ui->spoiler->hide();
    if(!raw_1grams_ptr){
        qDebug() << "WordTab::WordTab ERROR ? no raw 1 gram data!";
    }
    else{
        QObject::connect(ui->wordList_general_controls, &WordListGeneralControlsForm::newDataChosen,this, &WordTab::on_newDataChosen);
        QObject::connect(ui->wordList_general_controls, &WordListGeneralControlsForm::loadData,this, &WordTab::on_loadData);
        QObject::connect(ui->wordList_general_controls, &WordListGeneralControlsForm::saveData,this, &WordTab::on_saveData);
        QObject::connect(ui->wordList_general_controls, &WordListGeneralControlsForm::deleteData,this, &WordTab::on_deleteData);
        QObject::connect(ui->wordList_general_controls, &WordListGeneralControlsForm::findData,this, &WordTab::on_findData);
        QObject::connect(ui->wordList_general_controls, &WordListGeneralControlsForm::saveAllData,this, &WordTab::on_saveAllData);
        QObject::connect(ui->wordList_general_controls, &WordListGeneralControlsForm::changeFilter,this,&WordTab::on_changeFilter);
        QObject::connect(ui->wordList_general_controls, &WordListGeneralControlsForm::newFontSizeChosen,this, &WordTab::on_changeFontSize);
        //QObject::connect(ui->wordList_general_controls, &WordListGeneralControlsForm::newFontSizeChosen,this, &WordTab::on_changeFontSize);


        auto* anyLayout = new QVBoxLayout();
        anyLayout->addWidget(new QLabel("Some Text in Section", ui->spoiler));
        anyLayout->addWidget(new QPushButton("Button in Section", ui->spoiler));
        ui->spoiler->setContentLayout(*anyLayout);
//        // todo, can the following just be in constructor ?
        my_sortfilterproxymodel->setSourceModel(&model);
        onegram_selectionmodel->setModel(my_sortfilterproxymodel);
        ui->tableView->setModel(my_sortfilterproxymodel);
        ui->tableView->setSelectionModel(onegram_selectionmodel);
        ui->tableView->show();
        ui->wordList_general_controls->setDataIndex();
    }
}
//
WordTab::~WordTab()
{
    delete ui;
}

void WordTab::on_changeFontSize(int fs){
    ui->tableView->changeFontSize(fs);
    ui->tableView->resizeColumnsToContents();
}

//
void WordTab::setNewWordData(int word_length_minus_one){
    qDebug() << "\nWordTab::setNewWordData" << word_length_minus_one;
    if( word_length_minus_one < raw_1grams_ptr->raw_word_data.size() ){
        current_word_length = word_length_minus_one+1;
        current_word_index = word_length_minus_one;
        current_raw_1grams_ptr = raw_1grams_ptr->raw_word_data[current_word_index];
        model.setNewData( *current_raw_1grams_ptr );
        ui->tableView->resizeColumnsToContents();
        QString newlabel1 = QString("Rune Length ");
        QString newlabel2 = QString::number(current_word_length);
        setWordListGeneralLabel(newlabel1.append(newlabel2));
        QObject::connect(current_raw_1grams_ptr,
                         &RawWordData::updateRawWordDataChosen,
                         ui->wordList_general_controls,
                         &WordListGeneralControlsForm::on_updateChosenNotChosen);
        current_raw_1grams_ptr->updateChosenCounts();
    }
}
//
void WordTab::on_changeFilter(int filter_num){
    applyFilter(filter_num);
}
//
void WordTab::on_loadData(){
    qDebug() << "on_loadData";
    if(current_raw_1grams_ptr){
        if(current_raw_1grams_ptr->loadData()){
            setNewWordData(current_word_length);
        }
    }
}
//
void WordTab::on_saveData()const{
    qDebug() << "WordTab::on_saveData";
    if(current_raw_1grams_ptr){
        current_raw_1grams_ptr->saveData();
    }
}
//
void WordTab::on_saveAllData()const{
    qDebug() << "WordTab::on_saveAllData";
    if(raw_1grams_ptr){
        raw_1grams_ptr->saveRaw1grams();
    }
}
//
void WordTab::on_deleteData(){
    qDebug() << "WordTab::on_deleteData";
    if(current_raw_1grams_ptr){
        current_raw_1grams_ptr->deleteNotChosen2();
        setNewWordData(current_word_length);
    }
}
//
void WordTab::on_newDataChosen(int index){
    qDebug() << "WordTab::on_newDataChosen" << index;
    setNewWordData(index);
}
//
void WordTab::setWordListGeneralLabel(const QString& text){
    ui->wordList_general_controls->setLabel(text);
}
//
void WordTab::update(){
    updateLabel();
    updateTable();
}
//
void WordTab::updateLabel(){
}
//
void WordTab::updateTable(){
    ui->tableView->update();
}


void WordTab::on_tableView_t_sig(){
    QModelIndexList to_set_t;
    for(const QModelIndex& index: ui->tableView->selectionModel()->selectedRows()){
        //qDebug() << "row/col" << index.row() << "/" << index.column();
        to_set_t.push_back(my_sortfilterproxymodel->mapToSource(index));
    }
    model.setChosen(to_set_t);
    update();
}
void WordTab::on_tableView_f_sig(){
    QModelIndexList to_set_t;
    for(const QModelIndex& index: ui->tableView->selectionModel()->selectedRows()){
        //qDebug() << "row/col" << index.row() << "/" << index.column();
        to_set_t.push_back(my_sortfilterproxymodel->mapToSource(index));
    }
    model.setNotChosen(to_set_t);
    update();
}


void WordTab::on_tableView_alt_f_sig(){
    on_tableView_customContextMenuRequested();
}


void WordTab::on_tableView_alt_a_sig(){
    ui->wordList_general_controls->setAllFilter();
}
void WordTab::on_tableView_alt_c_sig(){
    ui->wordList_general_controls->setChosenFilter();
}
void WordTab::on_tableView_alt_n_sig(){
    ui->wordList_general_controls->setNotChosenFilter();
}


//
void WordTab::on_tableView_delete_sig(){
    qDebug() << "WordTab::on_tableView_delete_sig";
    QModelIndexList to_delete;
    int new_selected_row = 9999999;
    for(const QModelIndex& index: ui->tableView->selectionModel()->selectedRows()){
        to_delete.push_back(my_sortfilterproxymodel->mapToSource(index));
        if(index.row() < new_selected_row){
            new_selected_row = index.row() - 1;
        }
    }
    //new_selected_row = model.deleteSelected(to_delete);
    model.deleteSelected(to_delete);
    update();
    // set the new selected row to be one above the
    new_selected_row -= 1;
    if(new_selected_row < 0)
        new_selected_row = 0;
    //qDebug() << new_selected_row;
    ui->tableView->selectRow(new_selected_row);
}
//
void WordTab::on_tableView_space_bar_sig(){
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
    if(my_sortfilterproxymodel->filter_mode == QString("all") ){
    }
    else{
        ui->tableView->selectRow(new_selected_row);
    }
    model.toggleChosen(to_toggle);
    update();
}



//
void WordTab::on_findData(const QString& find_string){
    qDebug() << "WordTab::on_findData" << find_string;
    QVariant s = QVariant (find_string);
    //startrow =
    QList<QModelIndex> list = model.match(
        model.index(0,1),
        Qt::DisplayRole,
        s,
        Qt::MatchContains | Qt::MatchRecursive
        );
    if(list.size() > 0){
        onegram_selectionmodel->select(list[0], QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
        ui->tableView->scrollTo(my_sortfilterproxymodel->mapFromSource(list[0]));
    }
}

void WordTab::on_tableView_left_doubleclick_sig(){
    QModelIndexList to_toggle;
    for(const QModelIndex& index: ui->tableView->selectionModel()->selectedRows()){
        //qDebug() << "row/col" << index.row() << "/" << index.column();
        to_toggle.push_back(my_sortfilterproxymodel->mapToSource(index));
    }
    model.toggleChosen(to_toggle);
    update();
}

void WordTab::applyFilter(int a){
    //    check filters for phrase unqiue words
    qDebug() << "WordTab" << a;
    switch(a){
    case not_chosen_filter: my_sortfilterproxymodel->setFilter("not_chosen"); break;
    case chosen_filter:     my_sortfilterproxymodel->setFilter("chosen");     break;
    case all_filter:        my_sortfilterproxymodel->setFilter("");           break;
    default:;
    }
}
void WordTab::on_tableView_customContextMenuRequested(){
    qDebug() << QCursor().pos().x() << QCursor().pos().y();
    QPoint pt2 = QPoint(QCursor().pos().x() - 100 , QCursor().pos().y()  );
    QMenu contextMenu(tr("NGRAM Filter Menu"), this);
    QAction* all = contextMenu.addAction("Filer: All");
    QAction* chosen = contextMenu.addAction("Filer: Chosen");
    QAction* not_chosen = contextMenu.addAction("Filer: Not Chosen");
    QAction* selectedItem = contextMenu.exec(pt2);

    if (selectedItem == all){ on_tableView_alt_a_sig(); }
    if (selectedItem == chosen){ on_tableView_alt_c_sig(); }
    if (selectedItem == not_chosen){ on_tableView_alt_n_sig(); }

    //    if(selectedItem == all){
//        applyFilter(all_filter);
//    }
//    if(selectedItem == chosen){
//        applyFilter(chosen_filter);
//    }
//    if(selectedItem == not_chosen){
//        applyFilter(not_chosen_filter);
//    }
}

