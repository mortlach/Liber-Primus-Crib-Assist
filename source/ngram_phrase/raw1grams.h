
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
#ifndef RAW1GRAMS_H
#define RAW1GRAMS_H

#include <QSettings>
#include <QString>
#include <QDir>
#include <QMap>
#include "rawworddata.h"


class Raw1Grams : public QObject
{
#ifdef QT_SIG
    Q_OBJECT
public:
signals:
    void message(const QString& str);
#endif

/*!
* \brief Raw1Grams holds all the individual 1gram data, data is static for portabilty
* \param settings_in, settings file with default paths for 1 grams
*/
public:
    // will just startup, use this for accessing this data in general
    Raw1Grams();
    // will laod data on instatntiation
    Raw1Grams(QSettings& settings_in);
    ~Raw1Grams();
    /*!
     * \brief applySettingsAndLoad
     * \param settings_in
     * \return
     */
    bool applySettingsAndLoad(QSettings& settings_in);
    /*!
     * \brief settings
     */
    static QSettings* settings;
    /*!
     * \brief dummy_model_data
     */
    static RawWordData* dummy_model_data;
    /*!
     * \brief raw_word_data // TODO rename 1 grams ?
     */
    static QList<RawWordData*> raw_word_data;
    /*!
     * \brief phrase_word_data
     */
    static QList<RawWordData*> phrase_word_data;
    /*!
     * \brief loaded_raw1gram_files
     */
    static bool loaded_raw1gram_files;
    /*!
     * \brief loaded_local1gram_files
     */
    static bool loaded_local1gram_files;
    /*!
     * \brief loaded_phrase_local1gram_files
     */
    static bool loaded_phrase_local1gram_files;
    /*!
     * \brief onegram_file_count
     */
    static unsigned int onegram_file_count;
    /*!
     * \brief setLocalDataFileDirectory
     */
    void setLocalDataFileDirectory();
    // when new config is loaded / created run this
    /*!
     * \brief newConfig
     * \return
     */
    bool newConfig();
    /*!
     * \brief loadRaw1grams
     * \return
     */
    bool loadRaw1grams();
    /*!
     * \brief saveRaw1grams
     * \return
     */
    bool saveRaw1grams()const;
    /*!
     * \brief getRawWordData
     * \param word_len
     * \return
     */
    const RawWordData* getRawWordData(int word_len)const{
        return raw_word_data.at(word_len-1);
    }
    /*!
     * \brief getRawWordDataRef
     * \param word_len
     * \return
     */
    const RawWordData& getRawWordDataRef(int word_len)const{
        // todo what if passed bad value ?
        if(word_len <= raw_word_data.size()){
            qDebug() << "word_len-1=" << word_len- 1 << "/ raw_word_data.size()=" << raw_word_data.size();
            return *raw_word_data.at(word_len-1);
        }
        else{
            qDebug() << "RawWordData::getRawWordDataRef raw_word_data.size()" << raw_word_data.size();
            qDebug() << "ERROR, asked for item" << word_len << "with raw_word_data.size() = " << raw_word_data.size();
        }
        return *raw_word_data.at(0);
    }

};

#endif // RAW1GRAMS_H
