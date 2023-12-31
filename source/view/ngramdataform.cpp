
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
#include "NGramDataForm.h"
#include "utilities.h"
#include "ui_NGramDataForm.h"
#include <QMenu>

NGramDataForm::NGramDataForm(QWidget *parent)
    : BaseForm(1,parent)
    , model(NGramDataModel())
    , ui(new Ui::NGramDataForm)
    , ngram_selectionmodel(new QItemSelectionModel())
{
    qDebug() << "NGramDataForm::NGramDataForm";
    ui->setupUi(this);
    ui->wordList_general_controls->setDataIndex();
    QObject::connect(ui->wordList_general_controls, &WordListGeneralControlsForm::newDataChosen,this, &NGramDataForm::on_newDataChosen);
    QObject::connect(ui->wordList_general_controls, &WordListGeneralControlsForm::loadData,this, &NGramDataForm::on_loadData);
    QObject::connect(ui->wordList_general_controls, &WordListGeneralControlsForm::saveData,this, &NGramDataForm::on_saveData);
    QObject::connect(ui->wordList_general_controls, &WordListGeneralControlsForm::deleteData,this, &NGramDataForm::on_deleteData);
    QObject::connect(ui->wordList_general_controls, &WordListGeneralControlsForm::findData,this, &NGramDataForm::on_findData);
    QObject::connect(ui->wordList_general_controls, &WordListGeneralControlsForm::saveAllData,this, &NGramDataForm::on_saveAllData);
    QObject::connect(ui->wordList_general_controls, &WordListGeneralControlsForm::changeFilter,this, &NGramDataForm::on_changeFilter);
    my_sortfilterproxymodel->setSourceModel(&model);
    ngram_selectionmodel->setModel(my_sortfilterproxymodel);
    ui->tableView->setModel(my_sortfilterproxymodel);
    ui->tableView->setSelectionModel(ngram_selectionmodel);
    ui->tableView->show();        //
}

void NGramDataForm::on_changeFilter(int filter_num){
    applyFilter(filter_num);
}

NGramDataForm::~NGramDataForm(){
    delete ui;
}

void NGramDataForm::setNgramChosenCounts(const QList<int>& counts){
    //qDebug() << "NGramDataForm::setNgramChosenCounts";
    ui->ngram_puw_rationlize->setNgramCountLabel(counts);
}

void NGramDataForm::setPUWChosenCounts(const QList<int>& counts){
    //qDebug() << "NGramDataForm::setNgramChosenCounts";
    ui->ngram_puw_rationlize->setPUWCountLabel(counts);
}

void NGramDataForm::setNgramData(QList<NgramData*>& ptr){
    //ngrams_ptr = &ptr;
    //data_hub
    ui->tableView->show();
    //qDebug() << "NGramDataForm::setNgramData ng count" << (*ptr).size();
    ui->wordList_general_controls->setup(0, ptr.size());
    ui->wordList_general_controls->setDataIndex();
}

// set word data
void  NGramDataForm::setNewNgramDataInGUI(int ngram_index){
    qDebug() << "\nNGramDataForm::setNewWordData" << ngram_index;
    // crash after this somewhere
    NgramDataHub data_hub_local = NgramDataHub();

    if(data_hub_local.getNgramPhraseRef().setCurrentNgramDataIndex(ngram_index))
    {
        current_ngram_index = ngram_index;
//        auto& phrase = data_hub.getNgramPhraseRef();
//        NgramData& ngd = phrase.getCurrentNgramDataRef();
        // TODO TODO TODO
        //the below has no data, btu it is loaded correclty so lost in the

        Q_ASSERT(data_hub_local.ngram_phrase.ngram_data.size() > 0);
        qDebug() << "data_hub_local.ngram_phrase.ngram_data.size()" << data_hub_local.ngram_phrase.ngram_data.size();

        NgramData& ngd = *(data_hub_local.ngram_phrase.ngram_data[current_ngram_index]);
        model.setNewData( ngd );
        ui->tableView->resizeColumnsToContents();
        QStringList newlabel;
        newlabel << QString("Phrase Index:");
        newlabel << QString::number(ngram_index);
        newlabel << QString("Len:");

        qDebug() << ngd.my_ngram_meta_data.ngram_tokens;
        qDebug() << utilities::toString(ngd.my_ngram_meta_data.ngram_tokens);
        if(ngd.my_ngram_meta_data.has_sent_start){
            newlabel << "S," + utilities::toString(ngd.my_ngram_meta_data.ngram_tokens);
        }
        else if(ngd.my_ngram_meta_data.has_sent_end){
            newlabel << utilities::toString(ngd.my_ngram_meta_data.ngram_tokens)+ ",E" ;
        }
        else{
            newlabel << utilities::toString(ngd.my_ngram_meta_data.ngram_tokens);
        }
        qDebug() <<"new label" << newlabel.join(" ");
        setWordListGeneralLabel(newlabel.join(" "));
#ifdef QT_SIG
        QObject::connect(&ngd,
                         &NgramData::updateNgramChosen,
                         ui->wordList_general_controls,
                         &WordListGeneralControlsForm::on_updateChosenNotChosen);
#endif
        ngd.updateChosenCounts();
    }
}

void NGramDataForm::on_loadData(){
    //qDebug() << "on_loadData";
    NgramDataHub data_hub_local = NgramDataHub();
    NgramData& ngd = data_hub_local.getNgramPhraseRef().getCurrentNgramDataRef();
    ngd.load(false);
}
void NGramDataForm::on_saveData()const{
    //qDebug() << "NGramDataForm::on_saveData";
    NgramDataHub data_hub_local = NgramDataHub();
    const NgramData& ngd = data_hub_local.getNgramPhraseCRef().getCurrentNgramDataCRef();
    ngd.save();
}
void NGramDataForm::on_saveAllData()const{
    qDebug() << "NGramDataForm::on_saveAllData";
    NgramDataHub data_hub_local = NgramDataHub();
    const NgramPhrase& ngp = data_hub_local.getNgramPhraseCRef();
    ngp.saveNgramData();
}

void NGramDataForm::on_deleteData(){
    //qDebug() << "NGramDataForm::on_deleteData";
    NgramDataHub data_hub_local = NgramDataHub();
    NgramData& ngd = data_hub_local.getNgramPhraseRef().getCurrentNgramDataRef();
    model.beginResetThisModel();
    ngd.deleteNotChosen();
    model.endResetThisModel();
}
void NGramDataForm::on_newDataChosen(int index){
    //qDebug() << "NGramDataForm::on_newDataChosen" << index;
    setNewNgramDataInGUI(index);
}
void NGramDataForm::setWordListGeneralLabel(const QString& text){
    ui->wordList_general_controls->setLabel(text);
}
void NGramDataForm::update(){
    updateLabel();
    updateTable();
}
void NGramDataForm::updateLabel(){
    //    setLabel();
}
void NGramDataForm::updateTable(){
    ui->tableView->update();
}

void NGramDataForm::on_tableView_space_bar_sig(){
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

void NGramDataForm::on_findData(const QString& find_string){
    qDebug() << "NGramDataForm::on_findData" << find_string;
    //void NGramDataForm::on_findPushButton_clicked(){
    //    QVariant s = QVariant (ui->findLineEdit->text());
    QVariant s = QVariant (find_string);
    //startrow =
    QList<QModelIndex> list = model.match(
        model.index(0,0),
        Qt::DisplayRole,
        s,
        Qt::MatchContains | Qt::MatchRecursive
        );
    if(list.size() > 0){
        ngram_selectionmodel->select(list[0], QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
        ui->tableView->scrollTo(my_sortfilterproxymodel->mapFromSource(list[0]));
        qDebug() << "meh" << list;
    }
}

void NGramDataForm::on_tableView_left_doubleclick_sig(){
    QModelIndexList to_toggle;
    for(const QModelIndex& index: ui->tableView->selectionModel()->selectedRows()){
        qDebug() << "row/col" << index.row() << "/" << index.column();
        to_toggle.push_back(my_sortfilterproxymodel->mapToSource(index));
    }
    model.toggleChosen(to_toggle);
    update();
}

void NGramDataForm::applyFilter(int a){
    //    check filters for phrase unqiue words
    qDebug() << "ngramApplyFilter" << a;
    switch(a){
    case not_chosen_filter: my_sortfilterproxymodel->setFilter("not_chosen"); break;
    case chosen_filter:     my_sortfilterproxymodel->setFilter("chosen");     break;
    case all_filter:        my_sortfilterproxymodel->setFilter("");           break;
    default:;
    }
}
