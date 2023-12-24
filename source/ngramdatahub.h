
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
#ifndef NGRAMDATAHUB_H
#define NGRAMDATAHUB_H

#include "raw1grams.h"
#include "ngram_phrase/ngramphrase.h"
#include <QObject>
#include <QSharedPointer>

/*!
 * \brief The NgramDataHub class
 * holds main objects and functions for
 *
 */
class NgramDataHub : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief NgramDataHub
     * \param settings_in
     */
    NgramDataHub(QSettings& settings_in);
    /*!
     * \brief NgramDataHub use this constructor to get access to the data
     */
    NgramDataHub();
    ~NgramDataHub(){};
    // local varibales for loading data so they can easily happen in seperate thread
    /*!
     * \brief raw1gram
     */
    static Raw1Grams raw1gram;
    /*!
     * \brief ngram_phrase
     */
    static NgramPhrase ngram_phrase;
    /*!
     * \brief ngram_phrase_data
     */
    static NgramPhraseMetaData ngram_phrase_data;
    /*!
     * \brief chosen_dir
     */
    static QString chosen_dir;
    /*!
     * \brief chosen_fn
     */
    static QString chosen_fn;
    /*!
     * \brief setPath
     * \param chosen_dir_in
     * \param chosen_fn_in
     */
    inline void setPath(const QString& chosen_dir_in, const QString& chosen_fn_in){
        chosen_dir = chosen_dir_in;
        chosen_fn = chosen_fn_in;
    }
    /*!
     * \brief loadSetPhrasePhrase
     * \return
     */
    bool loadSetPhrasePhrase(){ return loadPhrase(chosen_dir,chosen_fn);}
    /*!
     * \brief get1GramPtr
     * \return
     */
    inline Raw1Grams& get1GramRef(){return raw1gram;}
    /*!
     * \brief getNgramPhrasePtr
     * \return
     */
    inline NgramPhrase& getNgramPhraseRef(){return ngram_phrase;}
    /*!
     * \brief getNgramPhraseCRef
     * \return
     */
    inline const NgramPhrase& getNgramPhraseCRef()const{return ngram_phrase;}
    /*!
     * \brief newPhrase
     * \param data_in
     * \return
     */
    bool newPhrase(const NgramPhraseMetaData& data_in);
    /*!
     * \brief clearPhraseData
     */
    void clearPhraseData();
    /*!
     * \brief copyNgramPhraseData
     * \param s
     */
    void copyNgramPhraseData(const NgramPhraseMetaData& s);
    /*!
     * \brief loadRaw1grams
     * \param settings
     * \return
     */
    bool loadRaw1grams(QSettings& settings);
    /*!
     * \brief loadPhrase
     * \param dir_path
     * \param filename
     * \return
     */
    bool loadPhrase(const QString& dir_path,const QString& filename);
    /*!
     * \brief getAndPrintNgramPUWChosenCounts
     * \param ngram_chosen_counts
     * \param puw_chosen_counts
     */
    void getAndPrintNgramPUWChosenCounts(QList<int>& ngram_chosen_counts, QList<int>& puw_chosen_counts);
    /*!
     * \brief rationalizeNgramData
     * \param delete_not_chosen_ngrams
     * \return
     */
    bool rationalizeNgramData(bool delete_not_chosen_ngrams = true);
    /*!
     * \brief rationalizeNgramData
     * \param delete_not_chosen_ngrams
     * \param puw_has_priority_in
     * \return
     */
    bool rationalizeNgramData(bool delete_not_chosen_ngrams, bool puw_has_priority_in);
    /*!
     * \brief rationalizePhraseWithDelete
     * \return
     */
    bool rationalizePhraseWithDelete();
    /*!
     * \brief savePhrase
     * \return
     */
    bool savePhrase();
};

#endif // NGRAMDATAHUB_H
