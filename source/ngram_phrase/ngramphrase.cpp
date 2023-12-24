
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
#include "ngramphrase.h"
#include "ngramdata.h"
#include "utilities.h"

QList<NgramData*> NgramPhrase::ngram_data = QList<NgramData*>();
QList<PhraseUniqueWords*> NgramPhrase::phrase_unique_words = QList<PhraseUniqueWords*>();
NgramPhraseMetaData NgramPhrase::ngram_phrase_data = NgramPhraseMetaData();
int NgramPhrase::current_ngram_index = 0;

NgramPhrase::NgramPhrase(){}


void NgramPhrase::copyNgramPhraseData(const NgramPhraseMetaData& s){
    ngram_phrase_data.config_path = s.config_path;
    ngram_phrase_data.raw_ngram_root_dir = s.raw_ngram_root_dir;
    ngram_phrase_data.phrase_dir = s.phrase_dir;
    ngram_phrase_data.phrase_tokens = s.phrase_tokens;
    ngram_phrase_data.phrase_word_lengths = s.phrase_word_lengths;
    ngram_phrase_data.ngram_token_lengths = s.ngram_token_lengths;
    ngram_phrase_data.ngram_lengths = s.ngram_lengths;
    ngram_phrase_data.raw_ngram_filenames = s.raw_ngram_filenames;
    ngram_phrase_data.phrase_ngram_filenames = s.phrase_ngram_filenames;
    ngram_phrase_data.has_sent_start = s.has_sent_start;
    ngram_phrase_data.has_sent_end = s.has_sent_end;
    ngram_phrase_data.ngram_meta_data = s.ngram_meta_data;
}
//
void NgramPhrase::clearAllNgramPhraseData(){
    ngram_phrase_data.ngram_meta_data = QList<NgramMetaData>();
    phrase_unique_words.clear();
    ngram_data.clear();
    ngram_phrase_data.has_sent_end = false;
    ngram_phrase_data.has_sent_start = false;
    ngram_phrase_data.phrase_tokens.clear();
    ngram_phrase_data.phrase_word_lengths.clear();
    ngram_phrase_data.ngram_lengths.clear();
    ngram_phrase_data.ngram_token_lengths.clear();
    ngram_phrase_data.phrase_dir = QDir();
    ngram_phrase_data.phrase_dir = QDir();
    ngram_phrase_data.raw_ngram_root_dir = QString();
}
bool NgramPhrase::setNewPhase(const NgramPhraseMetaData& data_in){
    qDebug() << "NgramPhrase::setNewPhase()";
    clearAllNgramPhraseData();
    copyNgramPhraseData(data_in);
    bool loaded_data = loadNgramData(true);
    if(loaded_data){
        // save ngram data to phrase directory
        if(saveNgramData()){
            // now createPUW
            createPhraseUniqueWords();
            // now save new files
            if(savePUWData()){
                // now save new files
                if(savePhraseSettings()){
                    qDebug() << "NgramPhrase::setNewPhase FIN GOOD";
                    return true;
                }
            }
        }
    }
    qDebug() << "NgramPhrase::setNewPhase FIN";
    return false;
}
//
bool NgramPhrase::savePhraseSettings(){
    //add a config file to the phrase save directory then aa funciton to save all and replace saving in setNewPhase weith it
    qDebug() << "NgramPhrase::savePhraseSettings" << ngram_phrase_data.config_path;
    QSettings settings(ngram_phrase_data.config_path, QSettings::IniFormat);
    settings.setValue("phrase_word_lengths", utilities::toString(
                                                 ngram_phrase_data.phrase_word_lengths));
    settings.setValue("phrase_tokens", utilities::toString(
                                           ngram_phrase_data.phrase_tokens));
    settings.setValue("ngram_lengths", utilities::toString(
                                           ngram_phrase_data.ngram_lengths));
    settings.setValue("has_sent_end", ngram_phrase_data.has_sent_end);
    settings.setValue("has_sent_start", ngram_phrase_data.has_sent_start);
    settings.setValue("ngram_token_lengths", utilities::toString(
                                                 ngram_phrase_data.ngram_token_lengths));

    for(int i=0;i<phrase_unique_words.size();++i){
        const PhraseUniqueWords* t = phrase_unique_words.at(i);
        const QString prefix = "puw"+QString::number(i)+"_";
        settings.setValue(prefix+"word_index_in_phrase", t->word_index_in_phrase);
        settings.setValue(prefix+"word_length", t->word_length);
        settings.setValue(prefix+"filename", t->data_file_name);
    }
    for(int i=0;i<ngram_phrase_data.ngram_meta_data.size();++i){
        const NgramMetaData& t = ngram_phrase_data.ngram_meta_data.at(i);
        saveNgramMetaDataToSettings(ngram_phrase_data.phrase_dir.path(),"phrase_config.ini",t);
    }
    return false;
}
//
bool NgramPhrase::loadPhraseSettings(const QString& dir_path,const QString& filename){
    qDebug() << "NgramPhrase::loadPhraseSettings" << dir_path + "/" + filename;
    QSettings settings(dir_path + "/" + filename, QSettings::IniFormat);
//    qDebug() << settings.allKeys();
    ngram_phrase_data.phrase_dir = QDir(dir_path);
    ngram_phrase_data.phrase_word_lengths = utilities::toIntList(settings.value("phrase_word_lengths").toString());
    ngram_phrase_data.ngram_lengths = utilities::toIntList(settings.value("ngram_lengths").toString());
    ngram_phrase_data.has_sent_end = settings.value("has_sent_end").toBool();
    ngram_phrase_data.has_sent_start = settings.value("has_sent_start").toBool();
    ngram_phrase_data.ngram_token_lengths = utilities::toIntList(settings.value("ngram_token_lengths").toString());
    // Load PUW WORKS
    for(int i=0;i<ngram_phrase_data.phrase_word_lengths.size();++i){
        const QString prefix = "puw"+QString::number(i) +"_";
        // TODO PhraseUniqueWords data_file_name is not used anymore
        const QString data_file_name = settings.value(prefix+"filename").toString();
        const QString data_file_path = ngram_phrase_data.phrase_dir.path();
        Q_ASSERT(settings.value(prefix+"word_index_in_phrase").toInt() == i);
        ngram_phrase_data.phrase_dir.path() + "/" + settings.value(prefix+"word_length").toString();
        int word_length = settings.value(prefix+"word_length").toInt();
        phrase_unique_words.push_back( new PhraseUniqueWords(i, word_length,  data_file_path) );
        phrase_unique_words.back()->word_length = word_length;
    }
    ngram_phrase_data.ngram_meta_data.clear();
    Q_ASSERT(ngram_data.size() == 0);
    Q_ASSERT(ngram_phrase_data.ngram_meta_data.size() == 0);
    for(int i=0;i<ngram_phrase_data.ngram_lengths.size();++i){
        ngram_phrase_data.ngram_meta_data.push_back( NgramMetaData());
        readNgramDataStructToSettings(dir_path,filename,i,ngram_phrase_data.ngram_meta_data.back());
        ngram_data.push_back( new NgramData(ngram_phrase_data.ngram_meta_data.back(), i));//, *raw1grams_ptr) );
        //int ddd = ngram_data.size();
        ngram_data.back()->load(false);
    }
    return true;
}
//
bool NgramPhrase::loadPhraseFromConfig(const QString& dir_path,const QString& filename){
    qDebug() << "NgramPhrase::loadPhrase" << dir_path << filename;
    // clear existing data
    clearAllNgramPhraseData();
    if(loadPhraseSettings(dir_path,filename)){
        return true;
    }
    else{
        qDebug() << "Failed to load settings";
    }
    return false;
}
//
bool NgramPhrase::loadPUWData(){
    bool r = true;
    for(auto&item : phrase_unique_words){
        if(item->loadData()){

        }
        else{
            r = false;
        }
    }
    return r;
}
//
bool NgramPhrase::loadNgramData(){
    bool r = true;
    for(auto&item : ngram_data){
        if(item->load(false)){
        }
        else{
            r = false;
        }
    }
    return r;
}
//
bool NgramPhrase::savePhraseData()const{
    return savePUWData() && saveNgramData();
}
//
bool NgramPhrase::savePUWData()const{
    bool r = true;
    for(const auto& puw: phrase_unique_words){
        if(puw->saveData()){
        }else{
            r = false;
        }
    }
    return r;
}
//
bool NgramPhrase::saveNgramData()const{
    bool r = true;
    for(const auto& ng: ngram_data){
        if(ng->save()){
        }else{
            r = false;
        }
    }
    return r;
}
//
bool NgramPhrase::saveAllPhraseData(){
    return saveNgramData() && savePUWData();
}
void NgramPhrase::rationalizePhrase(bool delete_not_chosen){
    rationalizePhrase(delete_not_chosen, true);
}
//
void NgramPhrase::rationalizePhrase(bool delete_not_chosen, bool puw_has_priority_in){
    Q_UNUSED(puw_has_priority_in);
    /*
    for each PUW
    set the relevant ngram shosen_by_word and chosen to to true / false
    then get TEMP PUW, which are the interseciton of eahc chosen ngram at this word_index
    Then either set all those not in TWMP_PUW to false, (OR DELETE)
    thgis probablu wont work exaclty right
    then get next PUW
    */
    /*
     *
     * if puw_has_priority_in:
     * words in ngrams match chosen flag in PUW
     *
     */
    qDebug() << "Entering rationalizePhrase working loop";
    for(auto& puw : phrase_unique_words){
        PhraseUniqueWords& next_puw = *puw;
        int this_puw_word_index_in_phrase = next_puw.word_index_in_phrase;
        // for each ngram, set chosen by PUW
        qDebug() << "PUW" << this_puw_word_index_in_phrase;
        for(auto& ng : ngram_data){
            ng->setChosenByWordByPUW(next_puw);
            ng->setChosenFalseIfAnyChosenByWordIsFalse();
            if(delete_not_chosen){
                ng->deleteNotChosen();
            }
        }
        QList<QString> words_to_fill;
        getPUWByIntersectionOFChosenNgrams(words_to_fill, this_puw_word_index_in_phrase);
        // either delete PUW or setTheseWordsChosenElseFalse
        if(delete_not_chosen){
            next_puw.keepDatForTheseWordssOnly(words_to_fill);
        }
        else{
            next_puw.setTheseWordsChosenElseFalse(words_to_fill);
        }
    }
}
//
bool NgramPhrase::getPUWByIntersectionOFChosenNgrams(QList<QString>& words_to_fill,
                                                     int i){
    QSet<QString> set_words;
    for(const auto& ng: ngram_data){
        QList<QString> next_words;
        if(ng->getChosenWordsAtWordIndexInPhrase(next_words,i)){
            if(set_words.size() == 0){
                set_words = QSet<QString>(next_words.begin(),
                                          next_words.end());
            }
            else{
                QSet<QString> new_set = QSet<QString>(next_words.begin(),
                                                      next_words.end());
                set_words = set_words.intersect(new_set);
            }
        }
    }
    words_to_fill = set_words.values();
    qDebug() << "NgramPhrase::getPUWByIntersectionOFChosenNgrams found" << words_to_fill.size() << "words";
    return true;
}
//
bool NgramPhrase::loadNgramData(bool from_raw){
    qDebug() << "NgramPhrase::loadNgramData, From raw =" << from_raw;
    for(int i = 0; i < ngram_phrase_data.ngram_meta_data.size(); ++i){
        auto& ngram_file_data_item =  ngram_phrase_data.ngram_meta_data[i];
        ngram_data.push_back( new NgramData(ngram_file_data_item, i));//, *raw1grams_ptr));
        ngram_data.back()->load(from_raw);
    }
    qDebug() << "NgramPhrase::loadNgramData FIN";
    return true;
}
//
void NgramPhrase::createPhraseUniqueWords(){
    qDebug() <<"*** NgramPhrase::createPhraseUniqueWords()";
    auto wl = ngram_phrase_data.phrase_word_lengths;
    qDebug() <<"phrase_word_lengths ="<<wl;
    phrase_unique_words.clear();
    for(int i = 0; i < wl.size(); ++i ){
        QHash<QString, QPair<int,int>> unique_word_hash;
        qDebug() <<"add new PUW, index =" << i << ", word_length =" << wl.at(i);
        for(const auto& data: ngram_data){
            // TODO getUniqueWordsAtPhraseIndex has another version
            data->getUniqueWordsAtPhraseIndex(i, unique_word_hash);
            qDebug() << "unique_word_hash.size() =" << unique_word_hash.size();
        }
        int next_wl = ngram_phrase_data.phrase_word_lengths.at(i);
        phrase_unique_words.push_back(new PhraseUniqueWords(i, next_wl, ngram_phrase_data.phrase_dir.path(), unique_word_hash));
        phrase_unique_words.back()->updateChosenCounts();
        qDebug() <<"...added";
    }
    qDebug() <<"createPhraseUniqueWords complete";
}
//
QList<int> NgramPhrase::getPUWChosenCounts()const{
    QList<int> r;
    for(const auto& item: phrase_unique_words){
        r.push_back(item->getChosenCount());
    }
    return r;
}
//
QList<int> NgramPhrase::getNgramChosenCounts()const{
    QList<int> r;
    for(const auto& item: ngram_data){
        r.push_back(item->getChosenCount());
    }
    return r;
}
//
void NgramPhrase::saveNgramMetaDataToSettings(const QString& dir_path,
                                              const QString& filename,
                                              const NgramMetaData& ngmd){
    qDebug() << "NgramPhrase::saveNgramMetaDataToSettings" << dir_path + "/" + filename;
    QSettings settings(dir_path + "/" + filename, QSettings::IniFormat);
    int i = ngmd.ngram_index;
    const QString prefix = "ngram"+QString::number(i) +"_";
    settings.setValue(prefix+"ngram_root_dir", ngmd.ngram_root_dir.path());
    settings.setValue(prefix+"raw_filepath", ngmd.raw_filepath);
    settings.setValue(prefix+"raw_filename", ngmd.raw_filename);
    settings.setValue(prefix+"raw_filepath", ngmd.raw_filepath);
    settings.setValue(prefix+"phrase_dir", ngmd.phrase_dir.path());
    settings.setValue(prefix+"phrase_ngram_filename", ngmd.phrase_ngram_filename);
    settings.setValue(prefix+"phrase_ngram_filepath", ngmd.phrase_ngram_filepath);
    settings.setValue(prefix+"phrase_tokens", utilities::toString(ngmd.phrase_tokens));
    settings.setValue(prefix+"phrase_word_indices", utilities::toString(ngmd.phrase_word_indices));
    settings.setValue(prefix+"phrase_word_lengths", utilities::toString(ngmd.phrase_word_lengths));
    settings.setValue(prefix+"ngram_word_indices", utilities::toString(ngmd.ngram_word_indices));
    settings.setValue(prefix+"ngram_tokens", utilities::toString(ngmd.ngram_tokens));
    settings.setValue(prefix+"ngram_length", ngmd.ngram_length);
    settings.setValue(prefix+"word_start_index", ngmd.first_word_index_in_phrase_word_indices);
    settings.setValue(prefix+"raw_file_exists", ngmd.raw_file_exists);
    settings.setValue(prefix+"has_sent_start", ngmd.has_sent_start);
    settings.setValue(prefix+"has_sent_end", ngmd.has_sent_end);
    settings.setValue(prefix+"ngram_index", ngmd.ngram_index);
}


void NgramPhrase::readNgramDataStructToSettings(const QString& dir_path,
                                                         const QString& filename,
                                                         int ngram_index,
                                                         NgramMetaData& new_data)
{
    qDebug() << "NgramPhrase::readNgramDataStructToSettings" << dir_path + "/" + filename;
    QSettings settings(dir_path + "/" + filename, QSettings::IniFormat);
    const QString prefix = "ngram"+QString::number(ngram_index) +"_";
//    // fill up struct with data
    new_data.ngram_root_dir = QDir(settings.value(prefix+"ngram_root_dir").toString());
    new_data.raw_filepath = settings.value(prefix+"raw_filepath").toString();
    new_data.raw_filename = settings.value(prefix+"raw_filename").toString();

    new_data.phrase_dir = QDir(settings.value(prefix+"phrase_dir").toString());
    new_data.phrase_ngram_filename = settings.value(prefix+"phrase_ngram_filename").toString();

    qDebug() << "parse " + prefix+"ngram_tokens" << settings.value(prefix+"ngram_tokens").toString();
    new_data.ngram_tokens = utilities::toIntList(settings.value(prefix+"ngram_tokens").toString());
    qDebug() << "new_data.ngram_tokens" << new_data.ngram_tokens;

    new_data.phrase_tokens = utilities::toIntList(settings.value(prefix+"phrase_tokens").toString());
    qDebug() << "new_data.phrase_tokens" << new_data.phrase_tokens;

    new_data.phrase_word_indices = utilities::toIntList(settings.value(prefix+"phrase_word_indices").toString());
    qDebug() << "new_data.phrase_word_indices" << new_data.phrase_word_indices;

    new_data.phrase_word_lengths = utilities::toIntList(settings.value(prefix+"phrase_word_lengths").toString());
    qDebug() << "new_data.phrase_word_lengths" << new_data.phrase_word_lengths;

    new_data.ngram_word_indices = utilities::toIntList(settings.value(prefix+"ngram_word_indices").toString());
    qDebug() << "new_data.ngram_word_indices" << new_data.ngram_word_indices;


    new_data.ngram_length = settings.value(prefix+"ngram_length").toInt();
    qDebug() << "new_data.ngram_length" << new_data.ngram_length;

    new_data.first_word_index_in_phrase_word_indices = settings.value(prefix+"word_start_index").toInt();
    qDebug() << "new_data.first_word_index_in_phrase_word_indices" << new_data.first_word_index_in_phrase_word_indices;

    new_data.raw_file_exists = settings.value(prefix+"raw_file_exists").toBool();
    new_data.has_sent_start = settings.value(prefix+"has_sent_start").toBool();
    new_data.has_sent_end = settings.value(prefix+"has_sent_end").toBool();
    new_data.ngram_index = settings.value(prefix+"ngram_index").toInt();
    qDebug() << "new_data.ngram_index" << new_data.ngram_index;
    qDebug() << "ngram_index" << ngram_index;
    Q_ASSERT(new_data.ngram_length == new_data.ngram_tokens.size());
    Q_ASSERT(new_data.ngram_length == new_data.ngram_word_indices.size());
    new_data.phrase_word_indices = utilities::toIntList(settings.value(prefix+"phrase_word_indices").toString());
    //pass these into the consturctor to load NGRAM data in constructor
    qDebug() << "new_data.hasAnyDefaultSetttings:" << new_data.hasAnyDefaultSetttings();
//    ngram_phrase_data.ngram_data_structs.push_back(new_data);
//    ngram_data.push_back( new NgramData(ngram_phrase_data.ngram_data_structs.back(), i, *raw1grams_ptr) );
//    ngram_data.back()->load(false);
}



