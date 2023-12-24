
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
//#include "ngramdatamodel.h"
#include "NGramDataModel.h"
#include <QColor>

NGramDataModel::NGramDataModel(QObject *parent)
    : QAbstractTableModel(parent)
    , raw_data(nullptr)
    , words(nullptr)
    , chosen(nullptr)
    , counts(nullptr)
    , chosen_by_word_transposed_ptr(nullptr)
    , editrole_list(QList<int>{Qt::EditRole})
{
}
int NGramDataModel::rowCount(const QModelIndex&) const{
    if(words){
        return words->size();
    }
    return 0;
}

void NGramDataModel::clear(){
    beginResetModel();
    words = nullptr;
    chosen = nullptr;
    counts = nullptr;
    raw_data = nullptr;
    chosen_by_word_transposed.clear();
    endResetModel();
}

QVariant NGramDataModel::data(const QModelIndex& index, int role)const{
    if (!index.isValid()) {
        return QVariant();
    }
    if(role == Qt::DisplayRole){
        if(index.column() == word_col){
            return QVariant((*words)[index.row()]).toString();
        }
        if(index.column() == chosen_col){
            return QVariant::fromValue(
                       getChosen(index.row())
                       ).toBool();
        }
        if(index.column() == counts_col) return QVariant::fromValue((*counts)[index.row()]).toLongLong();
//        if(index.column() == chosen_col){
//            return QVariant::fromValue(
//                       getChosen(index.row())
//                       ).toBool();
//        }
    }
    if(role == Qt::TextAlignmentRole) {
        return Qt::AlignCenter;
    }
    if(role == Qt::ForegroundRole ) {
        if( (index.column() == chosen_col) && isChosen(index) ){
            return QVariant(QColor::fromRgb(255,0,0));
        }
        return QVariant(QColor::fromRgb(0,0,0));
    }
    return QVariant();
}
QVariant NGramDataModel::headerData(int section, Qt::Orientation orientation, int role)const{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch(section ){
        case chosen_col: return QVariant(QString("chosen"));
        case word_col: return QVariant(QString("word"));
        case counts_col: return QVariant(QString("counts"));
        }
    }
    return QVariant();
}

void NGramDataModel::setNewData(NgramData& raw_data_in){
    beginResetModel();
    words = &raw_data_in.ngram_string;
    if(words->size() > 0){
        qDebug() << "NGramDataModel::setNewData (*words)[0]=" << (*words)[0];
        chosen = &raw_data_in.chosen;
        counts = &raw_data_in.counts;
        for(int i=0; i< raw_data_in.chosen_by_word.at(0).size(); ++i){
            QList<bool> next_chosen_by_word_transposed;
            for(const auto& item : raw_data_in.chosen_by_word){
                next_chosen_by_word_transposed.push_back(item.at(i));
            }
            chosen_by_word_transposed.push_back(next_chosen_by_word_transposed);
        }
        chosen_by_word_transposed_ptr = &chosen_by_word_transposed;
        raw_data = &raw_data_in;
        qDebug() << "NGramDataModel::setNewData words size" << words->size();
    }
    else{
        qDebug() << "??ERROR?? NGramDataModel::setNewData passed words with 0 size!!";
    }
    endResetModel();

}

bool NGramDataModel::getChosen(int row)const{
    return raw_data->getChosen(row);
}
bool NGramDataModel::isChosen(const QModelIndex& index)const{
    return raw_data->isChosen(index.row());
}

void NGramDataModel::setChosen(const QModelIndexList&rows){
    for(const QModelIndex &in: rows){
        raw_data->setChosen(in.row(), true);
        emit dataChanged(index(in.row(), chosen_col),index(in.row(), chosen_col));
    }
}
void NGramDataModel::setNotChosen(const QModelIndexList&rows){
    for(const QModelIndex &in: rows){
        raw_data->setChosen(in.row(), false);
        emit dataChanged(index(in.row(), chosen_col),index(in.row(), chosen_col));
    }
}
void NGramDataModel::toggleChosen(const QModelIndexList&rows){
    for(const QModelIndex &in: rows){
        raw_data->toggleChosen(in.row());
        emit dataChanged(index(in.row(), chosen_col),index(in.row(), chosen_col));
    }
}
Qt::ItemFlags NGramDataModel::flags(const QModelIndex&){
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled  | Qt::ItemIsEditable;
}
