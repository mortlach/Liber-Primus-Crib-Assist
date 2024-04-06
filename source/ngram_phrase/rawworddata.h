
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
#ifndef RAWWORDDATA_H
#define RAWWORDDATA_H

#include "qdebug.h"
#include <any>
#include <string>
#include <vector>
#include <QObject>
#include <QString>
#include <QVector>
#include <QHash>
#include "basedata.h"

// TODO Rename ton RAW1grams or something
class RawWordData : public QObject, public BaseData
{
#ifdef QT_SIG
    Q_OBJECT
public:
signals:
    void message(const QString& str);
    void updateRawWordDataChosen(unsigned int chosen_count, unsigned int not_chosen_count);

#endif
    /*!
 * \brief RawWordData class to hold raw 1 gram data and basic fucntions to
 * manipulate / load and save data
 * \param word_length what length word am I? 1 to 14
 * \param full_path_in, where is  my data stored? this can be changed, but
 * on instationitation data is loaded from settings file path
 */
public:
    RawWordData(unsigned int word_length, const QString& full_path_in);
    ~RawWordData(){
    };
    /*!
     * \brief count of each word in words
     */
    QVector<unsigned long long> counts;
    /*!
     * \brief chosen flag for each word in words
     */
    QVector<bool> chosen;
    /*!
     * \brief count of true in chosen
     */
    unsigned int chosen_count;
    /*!
     * \brief count of false in chosen
     */
    unsigned int not_chosen_count;
    /*!
     * \brief words
     */
    QVector<QString> words; //
    /*!
     * \brief chosen words
     */
    QVector<QString> chosen_words;
    /*!
     * \brief not chosen words
     */
    QVector<QString> not_chosen_words;
    /*!
     * \brief runes words converted to runes
     */
    QVector<QString> runes;
    /*!
     * \brief gematria_sum words word's gematria sum
     */
    QVector<unsigned int> gematria_sum;
    /*!
     * \brief lookup tabel to retrireve data quickly
     */
    QHash<QString, int> word_to_index;
    /*!
     * \brief compact way of holding all data of different types, see .cpp for encoding
     */
    std::vector<std::vector<std::any>> any_data;
    /*!
     * \brief string version of data to write / read from file, see .cpp for encoding
     */
    std::vector<std::vector<std::string>> string_data;
    /*!
     * \brief word length in runes
     */
    unsigned int word_length;
    /*!
     * \brief has data succesfully been loaded
     */
    bool has_data;
    /*!
     * \brief path to data file
     */
    QString data_file_path;
    /*!
     * \brief loadData
     * \return
     */
    bool loadData();
    /*!
     * \brief saveData
     * \return
     */
    bool saveData()const;
    /*!
     * \brief deleteNotChosen2
     * \return
     */
    bool deleteNotChosen2();
    /*!
     * \brief setFromAnyData
     * \param raw_data
     * \return
     */
    bool setFromAnyData(const std::vector<std::vector<std::any>>& raw_data);
    /*!
     * \brief getAsAnyData
     * \param data_to_fill
     * \return
     */
    bool getAsAnyData(std::vector<std::vector<std::any>>& data_to_fill)const;
    inline QString setDataFullPath(const QString& data_file_path_in){
        data_file_path = data_file_path_in;
        return data_file_path;
    }
    inline bool setChosen(int i, bool value){
        Q_ASSERT( i <= chosen.size() );
        chosen.replace(i, value);
        updateChosenCounts();
        return chosen.at(i);
    }
    inline bool getChosen(int i)const{
        Q_ASSERT( i < chosen.size() );
        return chosen.at(i);
    }
    inline void toggleChosen(int i){
        Q_ASSERT( i < chosen.size() );
        chosen.replace(i, !chosen.at(i));
        updateChosenCounts();
    }

    inline void deleteItem(int i){
        Q_ASSERT( i < chosen.size() );
        words.remove(i);
        counts.remove(i);
        runes.remove(i);
        chosen.remove(i);
        gematria_sum.remove(i);

        words.squeeze();
        counts.squeeze();
        runes.squeeze();
        chosen.squeeze();
        gematria_sum.squeeze();


        updateChosenCounts();
    }


    inline int deleteItems(QList<int>& items){
        // remove items in reverse order, first sort descending
        std::sort(items.begin(), items.end(), std::greater<int>());
        for (auto& i : items) {
            words.remove(i);
            counts.remove(i);
            runes.remove(i);
            chosen.remove(i);
            gematria_sum.remove(i);
        }
        words.squeeze();
        counts.squeeze();
        runes.squeeze();
        chosen.squeeze();
        gematria_sum.squeeze();
        updateChosenCounts();
        // return new row index to be selected after delete
        int new_selection = items.last() - 1;
        if(new_selection  < 0)
            new_selection =0;
        return new_selection;
    }



    inline size_t size(){ return chosen.size(); }
    inline bool isChosen(int i)const{ return getChosen(i);}
    inline bool isNotChosen(int i)const{ return getChosen(i) == false;}
    inline void updateChosenCounts(){
        chosen_count = chosen.count(true);
        not_chosen_count = chosen.size() - chosen_count;
        emit updateRawWordDataChosen(chosen_count, not_chosen_count);
    }
    inline int getChosenCount(){ return chosen_count;}
    /*!
     * \brief updateWordToIndex
     */
    void updateWordToIndex();
    /*!
     * \brief isChosen
     * \param word
     * \return
     */
    inline bool isChosen(const QString& word)const{
        int i = word_to_index.value(word,-1);
        if(i<0){
            //qDebug() << "Warning/ERROR passed word that does nto exist in my data:" << word;
            //qDebug() << "Warning/ERROR passed word that does nto exist in my data:" << word;
            return false;
        }
        return isChosen(i);
    }



};

#endif // RAWWORDDATA_H
