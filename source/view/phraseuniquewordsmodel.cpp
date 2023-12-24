
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
#include "phraseuniquewordsmodel.h"
#include <QColor>

PhraseUniqueWordsModel::PhraseUniqueWordsModel(QObject *parent)
    : QAbstractTableModel(parent)
    , raw_data(nullptr)
    , words(nullptr)
    , chosen(nullptr)
    , counts(nullptr)
    , count_in_ngrams(nullptr)
    , editrole_list(QList<int>{Qt::EditRole})
{
}
int PhraseUniqueWordsModel::rowCount(const QModelIndex&) const{
    if(words){
        return words->size();
    }
    return 0;
}

void PhraseUniqueWordsModel::clear(){
    beginResetModel();
    words = nullptr;
    chosen = nullptr;
    counts = nullptr;
    count_in_ngrams = nullptr;
    raw_data = nullptr;
    endResetModel();
}

QVariant PhraseUniqueWordsModel::data(const QModelIndex& index, int role)const{
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
        if(index.column() == ngrams_with_this_word_col){
            return QVariant((*count_in_ngrams)[index.row()]).toInt();
        }
        if(index.column() == total_counts_in_all_ngrams_col){
            return  QVariant((*counts)[index.row()]).toULongLong();
        }
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
QVariant PhraseUniqueWordsModel::headerData(int section, Qt::Orientation orientation, int role)const{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch(section ){
        case chosen_col: return QVariant(QString("chosen"));
        case word_col: return QVariant(QString("word"));
        case total_counts_in_all_ngrams_col: return QVariant(QString("counts"));
        case ngrams_with_this_word_col: return QVariant(QString("ngrams"));
        }
    }
    return QVariant();
}

void PhraseUniqueWordsModel::setNewData(PhraseUniqueWords& raw_data_in){
    beginResetModel();
    words = &raw_data_in.words;
    chosen = &raw_data_in.chosen;
    counts = &raw_data_in.total_counts_in_all_ngrams;
    count_in_ngrams = &raw_data_in.ngrams_with_this_word;
    raw_data = &raw_data_in;
    endResetModel();
}

bool PhraseUniqueWordsModel::getChosen(int row)const{
    return raw_data->getChosen(row);
}
bool PhraseUniqueWordsModel::isChosen(const QModelIndex& index)const{
    return raw_data->isChosen(index.row());
}

void PhraseUniqueWordsModel::setChosen(const QModelIndexList&rows){
    for(const QModelIndex &in: rows){
        raw_data->setChosen(in.row(), true);
        emit dataChanged(index(in.row(), chosen_col),index(in.row(), chosen_col));
    }
}
void PhraseUniqueWordsModel::setNotChosen(const QModelIndexList&rows){
    for(const QModelIndex &in: rows){
        raw_data->setChosen(in.row(), false);
        emit dataChanged(index(in.row(), chosen_col),index(in.row(), chosen_col));
    }
}
void PhraseUniqueWordsModel::toggleChosen(const QModelIndexList&rows){
    for(const QModelIndex &in: rows){
        raw_data->toggleChosen(in.row());
        emit dataChanged(index(in.row(), chosen_col),index(in.row(), chosen_col));
    }
}
Qt::ItemFlags PhraseUniqueWordsModel::flags(const QModelIndex&){
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled  | Qt::ItemIsEditable;
}
