
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
#ifndef PHRASEUNIQUEWORDSMODEL_H
#define PHRASEUNIQUEWORDSMODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include "phraseuniquewords.h"

class PhraseUniqueWordsModel : public QAbstractTableModel
{
#ifdef QT_SIG
    Q_OBJECT
public:
signals:
    void message(const QString& str);
#endif
public:
    PhraseUniqueWordsModel(QObject *parent = nullptr);
    ~PhraseUniqueWordsModel(){};
    inline void beginResetThisModel(){ beginResetModel();}
    inline void endResetThisModel(){ endResetModel();}
    PhraseUniqueWords* raw_data;
    void setNewData(PhraseUniqueWords& raw_data);
    QList<QString>* words;
    QList<bool>* chosen;
    QList<unsigned long long>* counts;
    QList<int>* count_in_ngrams;
    // QT stuff
    QList<int> editrole_list;
    // required virtual overloads, columnCount done in derived classes
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    inline int columnCount(const QModelIndex &parent = QModelIndex()) const override {Q_UNUSED(parent);return col_count; };
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    // function used when filtering on chosen
    bool isChosen(const QModelIndex& index)const; // todo  needed?
    bool isNotChosen(const QModelIndex& index)const; // todo needed?
    void setNewData(PhraseUniqueWords* raw_data_in);
    // toggle the chosen value for rows
    void toggleChosen(const QModelIndexList &rows);
    bool getChosen(int row)const;
    void setChosen(const QModelIndexList &rows);
    void setNotChosen(const QModelIndexList &rows);
    // required for editable table
    using QAbstractTableModel::flags;
    Qt::ItemFlags flags(const QModelIndex &index);
    QVariant headerData(int section, Qt::Orientation orientation, int role)const override;
    void clear();
    enum Constants
    {
        word_col =  1,
        chosen_col = 0,
        total_counts_in_all_ngrams_col =2,
        ngrams_with_this_word_col = 3,
        col_count = 4
    };

};

#endif // RAW1GRAMMODEL_H
