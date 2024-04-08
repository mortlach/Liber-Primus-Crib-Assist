
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
#ifndef PHRASEUNIQUEWORDS_H
#define PHRASEUNIQUEWORDS_H

#include <algorithm>
#include <QObject>
#include <QDir>
#include <QHash>
#include <any>
#include <string>
#include <vector>
#include "basedata.h"

class PhraseUniqueWords : public QObject , public BaseData
{
    Q_OBJECT

public:
    PhraseUniqueWords();
    PhraseUniqueWords(int word_index, int word_length,  const QString& data_file_path_in, const QHash<QString, QPair<int,int>>& hash_in);
    PhraseUniqueWords(int word_index, int word_length, const QString& data_file_name_in);
    ~PhraseUniqueWords(){
    };

    inline int getWordIndexInPhrase()const{return word_index_in_phrase;}

    bool setTheseWordsChosenElseFalse(const QList<QString>& chosen_words);

    void setNewWords(const QList<QString>& new_words,
                     const QList<unsigned long long>& new_total_counts_in_all_ngrams,
                     const QList<int>& new_ngrams_with_this_word);

    void keepDatForTheseWordssOnly(const QList<QString>& new_words);

    void updateWordToIndex();
    void getChosenWords(QList<QString>& new_words)const;


    inline QString setFilePath(const QString& data_file_path_in, const QString& data_file_name_in){
        data_file_path = data_file_path_in;
        data_file_name = data_file_name_in;
        data_path = data_file_path+QString("/")+data_file_name;
        return data_path;
    }

    inline QList<QString> getChosenWordsInCountOrder()const{
        using qp = QPair<QString,unsigned long long >;
        QList<QString> r;
        QList<QPair<QString,unsigned long long >> chosen_word_counts;
        for(int i = 0; i < words.size(); ++i){
            if(chosen.at(i)){
                qp p{words.at(i),total_counts_in_all_ngrams.at(i)};
                chosen_word_counts << p;
            }
        }
        std::sort(chosen_word_counts.begin(), chosen_word_counts.end(),
                  [](qp& a, qp& b){return a.second > b.second;}
                  );
        for(const auto& item: chosen_word_counts){
            r << item.first;
        }
        return r;
    }


signals:
    void updatePUWChosen(unsigned int chosen_count, unsigned int not_chosen_count);

public:
    bool setWords(const  QList<QString>& words_in){
        words = words_in;
        chosen = QList<bool>(words.size(), true);
        updateWordToIndex();
        return true;
    }
    /*!
     * \brief word_index_in_phrase
     */
    int word_index_in_phrase;
    /*!
     * \brief words
     */
    QList<QString> words; //
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
    QString data_file_name;
    QString data_file_path;
    QString data_path;
    /*!
     * \brief the total of each ngram_count for each word
     */
    QList<unsigned long long> total_counts_in_all_ngrams;
    /*!
     * \brief how mnay ngrams are there that contain this word
     */
    QList<int> ngrams_with_this_word;
    /*!
     * \brief lookup table to retrireve data quickly
     */
    QHash<QString, int> word_to_index;
    /*!
     * \brief loadData
     * \return
     */
    bool loadData();//{ return false;}
    /*!
     * \brief saveData
     * \return
     */
    bool saveData()const;//{ return false;}
    /*!
     * \brief deleteNotChosen2
     * \return
     */
    bool deleteNotChosen2(){ return false;}
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
    inline size_t size()const{ return chosen.size(); }
    inline bool isChosen(int i)const{ return getChosen(i);}
    inline bool isNotChosen(int i)const{ return getChosen(i) == false;}

    inline void updateChosenCounts(){
        chosen_count = chosen.count(true);
        not_chosen_count = chosen.size() - chosen_count;
        emit updatePUWChosen(chosen_count, not_chosen_count);
    }
    inline int getChosenCount()const{ return chosen_count;}

    inline bool isChosen(const QString& word)const{
        int i = word_to_index.value(word,-1);
        if(i<0){
            //qDebug() << "Warning/ERROR passed word that does nto exist in my data:" << word;
            return false;
        }
        return isChosen(i);
    }
    inline int deleteItems(QList<int>& items){
        // remove items in reverse order, first sort descending
        std::sort(items.begin(), items.end(), std::greater<int>());
        for (auto& i : items) {
            qDebug() << "PhraseUniqueWords::deleteItems" << i;
            words.remove(i);
//            counts.remove(i);
//            chosen.remove(i);
        }
        qDebug() << "REM finished ";
        words.squeeze();
        counts.squeeze();
        chosen.squeeze();
        updateChosenCounts();
        // return new row index to be selected after delete
        int new_selection = items.last() - 1;
        if(new_selection  < 0)
            new_selection =0;
        qDebug() << "Del finished ";
        return new_selection;
    }


};

#endif // PHRASEUNIQUEWORDS_H
