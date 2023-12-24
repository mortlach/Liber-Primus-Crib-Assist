
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
#ifndef NGRAMPHRASE_H
#define NGRAMPHRASE_H
#include <QObject>
#include <QSharedPointer>
#include "phraseuniquewords.h"
#include "ngramdata.h"
#include "ngrammetadata.h"
/*!
 * \brief The NgramPhrase class
 */
class NgramPhrase : public QObject
{
    Q_OBJECT
public:
    NgramPhrase();
    ~NgramPhrase(){
        deleteLater();
    };
    /*!
     * \brief rationalizePhrase
     * \param delete_not_chosen
     */
    void rationalizePhrase(bool delete_not_chosen = true);
    void rationalizePhrase(bool delete_not_chosen, bool puw_has_priority_in );
    /*!
     * \brief setNewPhase
     * \param data_in
     * \return
     */
    bool setNewPhase(const NgramPhraseMetaData& data_in);
    /*!
     * \brief saveAllPhraseData
     * \param is_new_phrase
     * \return
     */
    bool saveAllPhraseData();
    /*!
     * \brief clearData
     */
    void clearAllNgramPhraseData();
    /*!
     * \brief ngram_phrase_data
     * handy data to define the phase
     */
    static NgramPhraseMetaData ngram_phrase_data;
    /*!
     * \brief getNgramPhraseDataPtr
     * \return
     */
    NgramPhraseMetaData& getNgramPhraseDataRef(){ return ngram_phrase_data; }
    // a phrase consists of ngram data
    /*!
     * \brief ngram_data
     */
    static QList<NgramData*> ngram_data;
    static int current_ngram_index;
    inline bool setCurrentNgramDataIndex(int current_ngram_index_in){
        Q_ASSERT(current_ngram_index>-1);
        Q_ASSERT(current_ngram_index<ngram_data.size());
        current_ngram_index=current_ngram_index_in;
        if(current_ngram_index>-1){
            if(current_ngram_index<ngram_data.size()){
                return true;
            }
        }
        return false;
    }
    inline NgramData& getCurrentNgramDataRef(){
        return *ngram_data[current_ngram_index];
    }
    /*!
     * \brief getCurrentNgramDataCRef
     * \return
     */
    inline const NgramData& getCurrentNgramDataCRef()const{
        return *ngram_data.at(current_ngram_index);
    }
    /*!
     * \brief getNgramDataPtr
     * \return
     */
    QList<NgramData*>& getNgramDataRef(){ return ngram_data; }
    // from ngram_data we can generate unique words for each real word in phrase
    /*!
     * \brief phrase_unique_words
     */
    static QList<PhraseUniqueWords*> phrase_unique_words;
    /*!
     * \brief getPhraseUniqueWordsPtr
     * \return
     */
    QList<PhraseUniqueWords*>& getPhraseUniqueWordsRef(){ return phrase_unique_words; }
    /*!
     * \brief getPUWChosenCounts
     * \return
     */
    QList<int> getPUWChosenCounts()const;
    /*!
     * \brief getNgramChosenCounts
     * \return
     */
    QList<int> getNgramChosenCounts()const;
    /*!
     * \brief savePhraseData
     * \return
     */
    bool savePhraseData()const;
    /*!
     * \brief savePUWData
     * \return
     */
    bool savePUWData()const;
    /*!
     * \brief saveNgramData
     * \return
     */
    bool saveNgramData()const;
    /*!
     * \brief loadPhrase
     * \param dir_path
     * \param filename
     * \return
     */
    bool loadPhraseFromConfig(const QString& dir_path,const QString& filename);

private:
    /*!
     * \brief copyNgramPhraseData
     * \param s
     */
    void copyNgramPhraseData(const NgramPhraseMetaData& s);
    /*!
     * \brief getPUWByIntersectionOFChosenNgrams
     * \param words_to_fill
     * \param i
     * \return
     */
    bool getPUWByIntersectionOFChosenNgrams(QList<QString>& words_to_fill,int i);
    /*!
     * \brief loadNgramData
     * \param from_raw
     * \return
     */
    bool loadNgramData(bool from_raw);
    /*!
     * \brief savePhraseSettings
     * \return
     */
    bool savePhraseSettings();
    /*!
     * \brief loadPhraseSettings
     * \param dir_path
     * \param filename
     * \return
     */
    bool loadPhraseSettings(const QString& dir_path,const QString& filename);
    /*!
     * \brief loadPUWData
     * \return
     */
    bool loadPUWData();
    /*!
     * \brief loadNgramData
     * \return
     */
    bool loadNgramData();
    /*!
     * \brief createPhraseUniqueWords
     */
    void createPhraseUniqueWords();

    void saveNgramMetaDataToSettings(const QString& dir_path,
                                     const QString& filename,
                                     const NgramMetaData& ngds);

    void readNgramDataStructToSettings(const QString& dir_path,
                                       const QString& filename,
                                       int ngram_index,
                                       NgramMetaData& new_data);

};

#endif // NGRAMPHRASE_H
