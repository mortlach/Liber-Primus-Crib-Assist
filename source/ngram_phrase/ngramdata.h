
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
#ifndef NGRAMDATA_H
#define NGRAMDATA_H

#include "phraseuniquewords.h"
#include "ngrammetadata.h"
#include "raw1grams.h"
#include "basedata.h"
#include <QObject>
#include <QHash>
#include <QDir>
#include <any>
#include <vector>
/*!
 * \brief The NgramData class
 *
 * Holds all the word/ count / chosen data found in an ngram file
 * 2 type sof file: raw and phrase
 * raw files do not have a chosen column, and are ONLY EVR REAAD
 * phrase files are working files accosicatoed wwith a phrase
 *
 */
class NgramData : public QObject , public BaseData
{
    using VVAny = std::vector<std::vector<std::any>>;

    Q_OBJECT
public:
    NgramData(NgramMetaData& file_data_in,
              int passed_ngram_index_in_phrase
              );
    ~NgramData(){};
    /*!
     * \brief setChosenByWordByPUW
     * \param puw
     * \return
     */
    bool setChosenByWordByPUW(const PhraseUniqueWords& puw );
    /*!
     * \brief setChosenFalseIfAnyChosenByWordIsFalse
     * \return
     */
    bool setChosenFalseIfAnyChosenByWordIsFalse();
    /*!
     * \brief getNgramIndexForWordIndexInPhrase
     * \param word_index_in_phrase
     * \return
     */
    int getNgramIndexForWordIndexInPhrase(int word_index_in_phrase)const;
    /*!
     * \brief getUniqueWordsAtPhraseIndex
     * \param index_of_word_in_phrase
     * \param unique_word_hash
     * \return
     */
    bool getUniqueWordsAtPhraseIndex(int index_of_word_in_phrase,
                                     QHash<QString, QPair<int,int>>& unique_word_hash )const;
    /*!
     * \brief getChosenWordsAtWordIndexInPhrase
     * \param unique_words
     * \param word_index_in_phrase
     * \return
     */
    bool getChosenWordsAtWordIndexInPhrase(QList<QString>& unique_words,
                                           int word_index_in_phrase)const;
    /*!
     * \brief setFilePath
     * \param data_file_path_in
     * \param data_file_name_in
     * \return
     */
    inline QString setFilePath(const QString& data_file_path_in, const QString& data_file_name_in){
        data_file_path = data_file_path_in;
        data_file_name = data_file_name_in;
        data_path = data_file_path+QString("/")+data_file_name;
        qDebug() << "setFilePath" << data_file_path << "/" << data_file_name <<  "data_path = " << data_path;
        return data_path;
    }

signals:
    void message(const QString& str);
    void updateNgramChosen(unsigned int chosen_count,unsigned int not_chosen_count);

public:
    /*!
     * \brief my_ngram_meta_data
     * all file data info here, and general settings
     */
    NgramMetaData my_ngram_meta_data;
    /*!
     * \brief ngram_string
     */
    QList<QString> ngram_string; // each ngram as a single string (for display purposes) TODO RENAME
    /*!
     * \brief word_chosen
     */
    QList<QList<bool>> chosen_by_word; //previously word_chosen; // each word in each ngram can be individual chosen / not chosen
    /*!
     * \brief words
     */
    QList<QList<QString>> words;
    /*!
     * \brief lookup table to retrireve data quickly
     */
    QHash<QString, int> word_to_index; // TODO not used ??
    /*!
     * \brief updateChosenCounts
     */
    inline void updateChosenCounts(){
        chosen_count = chosen.count(true);
        not_chosen_count = chosen.size() - chosen_count;
        emit updateNgramChosen(chosen_count, not_chosen_count);
    }
    /*!
     * \brief size
     * \return
     */
    unsigned int size()const;
    /*!
     * \brief setChosenDataFromRaw
     * \return
     */
    bool setChosenDataFromRaw();
    /*!
     * \brief deleteNotChosen
     * \return
     */
    bool deleteNotChosen();
    /*!
     * \brief data_path The default Ngram file name in the data (based on
     * the word lengths in the ngram). Default files DO NOT HAVE ANY CHOSEN COLUMN
     * (it's a waste of space) instead local copies are made for a particular solution
     * and the chosen column is added on creation
     */
    QString data_file_name;
    QString data_file_path;
    QString data_path;
    /*!
     * \brief load outside this class always use load, can only ever load from 1 of two places
     * \param default_location, use default locatino to laod, or not
     *
     * will also reset ngram_length if loading from raw and
     *
     * \return, success
     */
    bool load(bool default_location = true); // Can only ever load from 1 of 2 places
    /*!
     * \brief finishAFterDataAddedOrDeleted
     */
    void finishAFterDataAddedOrDeleted();
    // no never
    bool saveRawData()const{return false;}
    // use this
    /*!
     * \brief save
     * \return
     */
    bool save()const;
    /*!
     * \brief getAsFileData: get class data lists into format for passiung to fileIO // has chosen columns!
     * \param r
     * \return succeded
     */
    bool getAsFileData(VVAny& r)const;
    /*!
     * \brief saveNGramFileData
     * \param full_path
     * \param data_to_write
     * \param ngram_length
     * \return
     */
    bool saveNGramFileData(const QString& full_path,
                           const VVAny& data_to_write,
                           unsigned int ngram_length)const;

    // set class lists from a VVAny with chosen data
    bool setFromAnyDataWithChosen(const VVAny& raw_data,
                                  bool has_sent_start = false,
                                  bool has_sent_end = false); // has chosen column!
    // set class lists from a VVAny with no chosen data
    bool setFromAnyDataNoChosen(VVAny& any_data,
                                bool has_sent_start,
                                bool has_sent_end);
    // load a raw ngram file
    bool loadNGramNoChosen(const QString& full_path,
                           VVAny& data_to_fill,
                           unsigned int ngram_length)const;
    // load a ngram file from a working phrase
    bool loadNGramWithChosen(const QString& full_path,
                             VVAny& data_to_fill,
                             unsigned int ngram_length)const;

    // generic functions that all these class seem to have TODO unify in base?
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
    inline size_t size(){ return chosen.size(); }
    inline bool isChosen(int i)const{ return getChosen(i);}
    inline bool isNotChosen(int i)const{ return getChosen(i) == false;}
    inline int getChosenCount()const{ return chosen_count;}
    void updateWordToIndex();
    inline bool isChosen(const QString& word)const{
        int i = word_to_index.value(word,-1);
        if(i<0){
            //qDebug() << "Warning/ERROR passed word that does nto exist in my data:" << word;
            return false;
        }
        return isChosen(i);
    }
};

#endif // NGRAMDATA_H
