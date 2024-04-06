
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
#ifndef RAW1GRAMMODEL_H
#define RAW1GRAMMODEL_H

#include <QAbstractTableModel>
#include "rawworddata.h"


class Raw1GramModel : public QAbstractTableModel
{
#ifdef QT_SIG
    Q_OBJECT
public:
signals:
    void message(const QString& str);
#endif
public:
    Raw1GramModel(QObject *parent = nullptr);
    ~Raw1GramModel(){
        clear();
    };
    RawWordData* raw_data;
    void setNewData(RawWordData& raw_data);

    QList<QString> * words;
    QList<unsigned long long> * counts;
    QList<QString> * runes;
    QList<unsigned int> * gematria_sum;

    void clear();
    // QT stuff
    QList<int> editrole_list;
    // required virtual overloads, columnCount done in derived classes
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    inline int columnCount(const QModelIndex &parent = QModelIndex()) const override { Q_UNUSED(parent);return col_count; };
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    // function used when filtering on chosen
    bool isChosen(const QModelIndex& index)const; // todo  needed?
    bool isNotChosen(const QModelIndex& index)const; // todo needed?
    void setNewData(RawWordData* gng_database_in);
    // toggle the chosen value for rows
    void toggleChosen(const QModelIndexList &rows);
    bool getChosen(int row)const;
    void setChosen(const QModelIndexList &rows);
    void setNotChosen(const QModelIndexList &rows);
    //  PERMANENTLY delete selected items
    int deleteSelected(const QModelIndexList &rows);

    // required for editable table

    using QAbstractTableModel::flags;
    Qt::ItemFlags flags(const QModelIndex &index);

    QVariant headerData(int section, Qt::Orientation orientation, int role)const override;

    enum Constants
    {
        word_col =  1,
        counts_col = 2,
        chosen_col = 0,
        runes_col = 4,
        gematria_sum_col = 3,
        col_count = 5
    };

};

#endif // RAW1GRAMMODEL_H
