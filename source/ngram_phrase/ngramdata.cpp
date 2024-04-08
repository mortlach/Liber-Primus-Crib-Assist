
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
#include "ngramdata.h"
#include "utilities.h"
#include <QVariant>
#include <string>


NgramData::NgramData( NgramMetaData& my_ngram_meta_data_in
                     ,int passed_ngram_index_in_phrase)
    : my_ngram_meta_data(my_ngram_meta_data_in)
    , data_file_name(QString(""))
    , data_file_path(QString(""))
    , data_path(QString(""))
{
    Q_UNUSED(passed_ngram_index_in_phrase);
    qDebug() << "New Ngram data Object Created at" << my_ngram_meta_data.ngram_index;
    qDebug() << "NgramData start tokens" << my_ngram_meta_data.ngram_tokens;

}

void NgramData::finishAFterDataAddedOrDeleted(){
    // TODO add stuff when
    //    setWordToIndex();
    //    updateChosenSummary();
    //    Q_ASSERT(chosen_words.size() == chosen_count);
    //    Q_ASSERT(not_chosen_words.size() == not_chosen_count);
}
bool NgramData::setChosenDataFromRaw(){
    qDebug() << "NgramData::setChosenDataFromRaw";
    // these lists need setting based on the entires in the 1-grams data
    auto raw1grams = Raw1Grams();
    for(int index = 0; index < my_ngram_meta_data.ngram_length; ++index){
        int int_word_length = my_ngram_meta_data.ngram_tokens[index];
        qDebug() << "word_length =" << int_word_length;
        const RawWordData& rawworddataptr = raw1grams.getRawWordDataRef(int_word_length);
        for(int row = 0; row < words.size(); ++row){
            QString & next_word = words[row][index];
            bool & c = chosen_by_word[row][index];
            c = rawworddataptr.isChosen(next_word);
            if(c == false){
                //qDebug() << words[row][index] << "chosen" << chosen_by_word[row][index];
                //chosen.replace(row, false);
                chosen[row] = false;
            }
        }
    }
    return true;
}
int NgramData::getNgramIndexForWordIndexInPhrase(int word_index_in_phrase)const{
    qDebug() << "NgramData::getNgramIndexForWordIndexInPhrase";
    qDebug() << "my_ngram_meta_data.ngram_word_indices" << my_ngram_meta_data.ngram_word_indices;
    Q_ASSERT(my_ngram_meta_data.ngram_word_indices.size() > 0);
    Q_ASSERT(my_ngram_meta_data.ngram_word_indices.size() < 6);
    Q_ASSERT(my_ngram_meta_data.ngram_word_indices.size() == my_ngram_meta_data.ngram_length);
    for(int i = 0; i < my_ngram_meta_data.ngram_word_indices.size(); ++i){
        //qDebug() << ngram_data->ngram_word_indices;
        if( my_ngram_meta_data.ngram_word_indices.at(i) == word_index_in_phrase){
            //qDebug() << word_index_in_phrase << "word index IS in this ngram" << ngram_data->ngram_word_indices;
            return i;
        }
    }
    //qDebug() << word_index_in_phrase << "word index not in this ngram" << ngram_data->ngram_word_indices;
    return -1;
}
bool NgramData::setChosenByWordByPUW(const PhraseUniqueWords& puw ){
    int word_index_in_ngram_index = getNgramIndexForWordIndexInPhrase(puw.getWordIndexInPhrase());
    if(word_index_in_ngram_index < 0){
        return true;
    }
    qDebug() << "NgramData::setChosenByWordByPUW, found word index = "  << word_index_in_ngram_index;
    Q_ASSERT(words.at(0).size() > word_index_in_ngram_index);
    Q_ASSERT(words.size() == chosen_by_word.size());
    // loop over each ngram word at word_index_in_ngram_index
    for(int i = 0; i < words.size(); ++i){
        const QString& next_word = words.at(i).at(word_index_in_ngram_index);
        chosen_by_word[i].replace(word_index_in_ngram_index,
                                  puw.isChosen(next_word));
    }
    updateChosenCounts();
    return true;
}
bool NgramData::setChosenFalseIfAnyChosenByWordIsFalse(){
    // if any chosen_by_word is false, chosen is false
    // (NB chosen my also be false in any case, therefore DO NOT set chosen = true if all chosen_by_word is true)
    Q_ASSERT(chosen.size() == chosen_by_word.size());
    for(int i = 0; i < chosen.size();++i){
        const QList<bool>& this_chosen_by_word = chosen_by_word.at(i);
        for(const auto& item: this_chosen_by_word){
            if(!item){
                chosen.replace(i,false);
                break;
            }
        }
    }
    updateChosenCounts();
    return true;
}

bool NgramData::getChosenWordsAtWordIndexInPhrase(QList<QString>& unique_words,
                                                  int word_index_in_phrase)const
{
    int ngram_index = getNgramIndexForWordIndexInPhrase(word_index_in_phrase);
    if(ngram_index < 0){
        return false;
    }
    else{
        //int start_size = unique_words.size();
        for(int i = 0; i < words.size(); ++i){
            const QString& next_word = words.at(i).at(ngram_index);
            const bool& is_chosen = chosen.at(i);
            if(is_chosen){
                unique_words.push_back(next_word);
            }
        }
        qDebug() << "NgramData::addUniqueChosenWordsAtWordIndexInPhrase, After Ngram" << my_ngram_meta_data.ngram_index << "," << unique_words.size() << "unique words";
    }
    return true;
}
bool NgramData::getUniqueWordsAtPhraseIndex(int index_of_word_in_phrase,
                                            QHash<QString, QPair<int,int>>& unique_word_hash )const{
    qDebug() << "NgramData::getUniqueWordsAtPhraseIndex, check" << my_ngram_meta_data.ngram_word_indices;
    int word_part = -1;
    for(int i = 0; i < my_ngram_meta_data.ngram_length; ++i){
        if(my_ngram_meta_data.first_word_index_in_phrase_word_indices +i == index_of_word_in_phrase ){
            word_part = i;
            qDebug() << "found word index" << index_of_word_in_phrase << "at local index" << i;
            break;
        }
    }
    // if this ngram has this phrase_word_index, get unique words
    if(word_part > -1){
        qDebug() << "adding words,  words.size()" <<  words.size();
        for(int i=0; i < words.size();++i){
            const QString& next_word = words.at(i).at(word_part);
            const int next_count = counts.at(i);
            if(unique_word_hash.contains(next_word)){
                unique_word_hash[next_word].first += next_count;
                unique_word_hash[next_word].second += 1;
                //qDebug() << "unique_word_hash[next_word].first" << unique_word_hash[next_word].first;
                //qDebug() << "unique_word_hash[next_word].second" << unique_word_hash[next_word].second;
            }
            else{
                QPair<int,int> next_entry;
                next_entry.first = next_count;
                next_entry.second = 1;
                unique_word_hash[next_word] = next_entry;
                // qDebug() << "unique_word_hash[next_word].first" << unique_word_hash[next_word].first;
                // qDebug() << "unique_word_hash[next_word].second" << unique_word_hash[next_word].second;
            }
        }
    }
    qDebug() << "unique_word_hash.size():" << unique_word_hash.size();
    return true;
}

bool NgramData::deleteNotChosen(){
    // Create new lists and copy seems faster than delete from orignal lists
    QList<unsigned int> chosen_indexes = utilities::findIndexesOf<bool>(chosen,true);
    unsigned int start_size = chosen.size();
    unsigned int end_size = chosen_indexes.size();
    unsigned int delete_size = start_size - end_size;
    QList<QList<QString>> words2;
    words2.reserve(chosen_indexes.size()) ;
    QList<unsigned long long> counts2;
    counts2.reserve(chosen_indexes.size()) ;
    QList<bool> chosen2;
    chosen2.reserve(chosen_indexes.size()) ;
    QList<QList<bool>> chosen_by_word2;
    chosen_by_word2.reserve(chosen_indexes.size()) ;
    QList<QString> ngram_string2;
    ngram_string2.reserve(chosen_indexes.size()) ;
    for(const auto& i : chosen_indexes){
        words2.push_back( words.at(i));
        chosen2.push_back( chosen.at(i));
        ngram_string2.push_back( ngram_string.at(i));
        counts2.push_back( counts.at(i));
        chosen_by_word2.push_back(chosen_by_word.at(i));
    }
    words = words2;
    chosen = chosen2;
    counts = counts2;
    ngram_string = ngram_string2;
    chosen_by_word = chosen_by_word2;
    updateChosenCounts();
    // tests
    Q_ASSERT( end_size == words.size());
    Q_ASSERT( end_size == chosen.size());
    Q_ASSERT( end_size == chosen_by_word.size());
    Q_ASSERT( end_size == ngram_string.size());
    Q_ASSERT( end_size == chosen_by_word.size());
    qDebug() << "NgramData::deleteNotChosen,"
             << my_ngram_meta_data.ngram_index
             << "start/end/dif ="
             << start_size << end_size << delete_size;
    return true;
}

bool NgramData::getAsFileData(VVAny& data_to_fill)const{
    data_to_fill.clear();
    data_to_fill.reserve(words.size());
    for(unsigned int i = 0; i < words.size(); ++i){ // !
        std::vector<std::any> next_line;
        next_line.reserve(my_ngram_meta_data.ngram_length*2 + 2);
        for(const auto& w : words.at(i)){
            std::string nw = w.toStdString();
            next_line.push_back( std::any( nw ) );
        }
        for(const auto& w : chosen_by_word.at(i)){
            next_line.push_back( std::any( w ) );
        }
        next_line.push_back( std::any(counts.at(i)) );
        next_line.push_back( std::any(chosen.at(i)) );
        data_to_fill.push_back(next_line);
    }
    return true;
}

bool NgramData::saveNGramFileData(const QString& full_path,
                                  const VVAny& data_to_write,
                                  unsigned int ngram_length)const{
    std::vector<std::vector<std::string>> string_data;
    unsigned int count_index, chosen_index=0, items_per_line = 0;
    // work out ngram size
    if(data_to_write.size() > 0){
        items_per_line = data_to_write[0].size();
        qDebug() << "save_ngitems_per_line" << items_per_line << ",(2 * ngram_length + 2)=" << 2 * ngram_length + 2;
        Q_ASSERT( items_per_line == 2 * ngram_length + 2);
        count_index = items_per_line - 2;
        chosen_index = items_per_line - 1;
    }
    for(const auto& item: data_to_write){
        std::vector<std::string> next_line;
        next_line.reserve(items_per_line);
        for(unsigned int i = 0; i < ngram_length; ++i ){
            next_line.push_back(std::any_cast<std::string>(item.at(i)));
        }
        for(unsigned int i = 0; i < ngram_length; ++i ){
            next_line.push_back(std::to_string(std::any_cast<bool>(item.at(ngram_length+i))));
        }
        next_line.push_back(std::to_string(std::any_cast<unsigned long long>(item.at(count_index))));
        next_line.push_back(std::to_string(std::any_cast<bool>(item.at(chosen_index))));
        string_data.push_back(next_line);
    }
    return utilities::writeCSVFile(full_path.toStdString(), string_data);
}

bool NgramData::save()const{
    VVAny data;
    if( getAsFileData(data)){
        return saveNGramFileData(my_ngram_meta_data.phrase_ngram_filepath, data, my_ngram_meta_data.ngram_length);
    }
    return false;
}

bool NgramData::load(bool default_location){

    VVAny data;
    if(default_location){
        qDebug() << "Call NgramData::loadNGramNoChosen " << my_ngram_meta_data.raw_filepath
                 << my_ngram_meta_data.has_sent_start << my_ngram_meta_data.has_sent_end;
        if(!loadNGramNoChosen(my_ngram_meta_data.raw_filepath, data, my_ngram_meta_data.ngram_length)){
            return false;
        }
        setFromAnyDataNoChosen(data,
                               my_ngram_meta_data.has_sent_start,
                               my_ngram_meta_data.has_sent_end);

        setChosenDataFromRaw();

        if(my_ngram_meta_data.has_sent_start){
            my_ngram_meta_data.ngram_length -= 1;
            //ngram_length=ngram_data->ngram_length;
            my_ngram_meta_data.ngram_tokens.takeFirst();
            qDebug() << "Change ngram_length" << my_ngram_meta_data.ngram_length+1 << "to" << my_ngram_meta_data.ngram_length;
            qDebug() << "words.at(0).size() =" << words.at(0).size();
        }
        else if(my_ngram_meta_data.has_sent_end){
            my_ngram_meta_data.ngram_length -= 1;
            //ngram_length=ngram_data->ngram_length;
            my_ngram_meta_data.ngram_tokens.takeLast();
            qDebug() << "Change ngram_length" << my_ngram_meta_data.ngram_length+1 << "to" << my_ngram_meta_data.ngram_length;
            qDebug() << "words.at(0).size() =" << words.at(0).size();
        }
    }
    else{
        qDebug() << my_ngram_meta_data.phrase_dir.path();
        qDebug() << my_ngram_meta_data.phrase_ngram_filename;
        setFilePath(my_ngram_meta_data.phrase_dir.path(),my_ngram_meta_data.phrase_ngram_filename);

        qDebug() << "Call NgramData::loadNGramWithChosen " << data_path;
        if(!loadNGramWithChosen(data_path, data, my_ngram_meta_data.ngram_length)){
            return false;
        }
        setFromAnyDataWithChosen(data,
                                 my_ngram_meta_data.has_sent_start,
                                 my_ngram_meta_data.has_sent_end);
    }
    qDebug() << "ngram_data->ngram_tokens" << my_ngram_meta_data.ngram_tokens;
    if(deleteNotChosen()){
    }
    Q_ASSERT(my_ngram_meta_data.ngram_length == words.at(0).size());
    Q_ASSERT(my_ngram_meta_data.ngram_length == chosen_by_word.at(0).size());
    return true;
}

bool NgramData::loadNGramNoChosen(const QString& full_path,
                                  VVAny& data_to_fill,
                                  unsigned int ngram_length)const{
    qDebug() << "loadNGramNoChosen" << full_path;
    std::vector<std::vector<std::string>> file_string_data;
    if(utilities::readCSVFile(full_path.toStdString(), file_string_data)){
        // check ngram size
        // easy format for this, where x is ngram_length
        // including start/end tags as this is from raw data
        // FORMAT:
        // word1 word2 wordX counts
        if(file_string_data.size() > 0 ){
            qDebug() << "read_items_per_line" << file_string_data.at(0).size() << "ngram_length" << ngram_length;
            // chosen files have ngram_lengths + 1 columns (counts )
            Q_ASSERT(ngram_length == (file_string_data.at(0).size() - 1));
        }
        data_to_fill.clear();
        data_to_fill.reserve(file_string_data.size());
        for(const auto& item : file_string_data){
            std::vector<std::any> next_line_data;
            next_line_data.reserve(item.size());
            for (unsigned int i = 0; i < ngram_length; ++i) {
                next_line_data.push_back(std::any(item.at(i)));
            }
            // ***************
            // USED to add in extra features here, but DO NOT do that, instead do in later function to ease refactoring
            // insert chosen
            //            for (unsigned int i = 0; i < ngram_length; ++i) {
            //                next_line_data.push_back(std::any(true));
            //            }
            // ***************
            next_line_data.push_back( std::any(std::stoull(item.at(ngram_length))));
            data_to_fill.push_back(next_line_data);
        }
        return true;
    }
    return false;
}
bool NgramData::loadNGramWithChosen(const QString& full_path,
                                    VVAny& data_to_fill,
                                    unsigned int ngram_length)const{
    std::vector<std::vector<std::string>> file_string_data;
    if(utilities::readCSVFile(full_path.toStdString(), file_string_data)){
        unsigned int items_per_line = 0, count_index, chosen_index;
        // check ngram size
        if(file_string_data.size() > 0 ){
            items_per_line = file_string_data.at(0).size();
            qDebug() << "items_per_line" << items_per_line << "ngram_length" << ngram_length;
            Q_ASSERT(ngram_length == (items_per_line - 2) / 2);
            count_index = items_per_line - 2;
            chosen_index = items_per_line - 1;
        }
        data_to_fill.clear();
        data_to_fill.reserve(file_string_data.size());
        for(const auto& item : file_string_data){
            std::vector<std::any> next_line_data;
            next_line_data.reserve(items_per_line);
            bool bool_value;
            for (unsigned int i = 0; i < ngram_length; ++i) {
                next_line_data.push_back(std::any(item.at(i)));
            }
            for (unsigned int i = ngram_length; i < 2*ngram_length; ++i) {
                Q_ASSERT( (item.at(i) == "1") || (item.at(i) == "0"));
                utilities::stringToBool(item.at(i), bool_value);
                next_line_data.push_back(std::any(bool_value));
            }
            next_line_data.push_back(std::any(std::stoull(item.at(count_index))));
            Q_ASSERT( (item.at(chosen_index) == "1") || (item.at(chosen_index) == "0"));
            utilities::stringToBool(item.at(chosen_index), bool_value);
            next_line_data.push_back(std::any(bool_value));
            data_to_fill.push_back(next_line_data);
        }
        return true;
    }
    return false;
}

bool NgramData::setFromAnyDataNoChosen(VVAny& raw_data,
                                       bool has_sent_start,
                                       bool has_sent_end){
    if(raw_data.size() < 1){
        return false;
    }
    if(has_sent_start && has_sent_end){
        return false;
    }
    // easy format for this, where x is ngram_length
    // including start/end tags as this is from raw data
    // FORMAT:
    // word1 word2 wordX counts
    Q_ASSERT((int)raw_data[0].size() == my_ngram_meta_data.ngram_length + 1 );
    words.clear();
    ngram_string.clear();
    counts.clear();
    chosen.clear();
    chosen_by_word.clear();
    words.reserve(raw_data.size());
    ngram_string.reserve(raw_data.size());
    counts.reserve(raw_data.size());
    chosen.reserve(raw_data.size());
    chosen_by_word.reserve(raw_data.size());
    qDebug() << "start loop";
    for(const auto& item : raw_data )
    {   // fill ngram_length words
        QList<QString> next_words;
        next_words.reserve(my_ngram_meta_data.ngram_length);
        // bit odd // it works
        std::vector<std::any>::const_iterator it = item.begin();
        for(int i = 0; i < my_ngram_meta_data.ngram_length; ++i){
            next_words.push_back(QString::fromStdString(std::any_cast<std::string>(*it)));
            ++it;
        }
        counts.push_back(std::any_cast<unsigned long long>(*it));
        // based on start end tags remove items
        //QList<QString> t;
        if(has_sent_start ){
            int sss = next_words.size();
            next_words.takeFirst();
            int sss2 = next_words.size();
            Q_ASSERT(sss != sss2);
        }
        else if(has_sent_end){
            next_words.takeLast();
        }
        words.push_back(next_words);
        ngram_string.push_back(next_words.join(" "));
        // as setting from raw, chosen & chosen_by_word have all true default values
        chosen.push_back(true);
        QList<bool> default_chosen_by_word(next_words.size(), true);
        chosen_by_word.push_back(default_chosen_by_word);
    }
    return true;
}
bool NgramData::setFromAnyDataWithChosen(const VVAny& raw_data,
                                         bool has_sent_start,
                                         bool has_sent_end) {
    if(raw_data.size() < 1){
        return false;
    }
    if(has_sent_start && has_sent_end){
        return false;
    }
    qDebug() << "setFromFileData, has_sent_start" << has_sent_start;
    qDebug() << "setFromFileData, has_sent_end" << has_sent_end;
    Q_ASSERT((int)raw_data[0].size() == 2* my_ngram_meta_data.ngram_length + 2 );
    words.clear();
    ngram_string.clear();
    counts.clear();
    chosen.clear();
    chosen_by_word.clear();
    words.reserve(raw_data.size());
    ngram_string.reserve(raw_data.size());
    counts.reserve(raw_data.size());
    chosen.reserve(raw_data.size());
    chosen_by_word.reserve(raw_data.size());
    qDebug() << "start loop";
    bool  printv = true;
    for(const auto& item : raw_data )
    {   // bit odd, but works
        QVector<QString> next_words;
        next_words.reserve(my_ngram_meta_data.ngram_length);
        QVector<bool> base_word_chosen;
        base_word_chosen.reserve(my_ngram_meta_data.ngram_length);
        std::vector<std::any>::const_iterator it = item.begin();
        for(int i = 0; i < (int)my_ngram_meta_data.ngram_length; ++i){
            //next_words.push_back(QString::fromStdString(std::any_cast<std::string>(*it)));
            next_words.push_back(QString::fromStdString(std::any_cast<std::string>(item.at(i))));
            ++it;
        }
        for(int i = 0; i < (int)my_ngram_meta_data.ngram_length; ++i){
            base_word_chosen.push_back(std::any_cast<bool>(*it));
            ++it;
        }
        counts.push_back(std::any_cast<unsigned long long>(*it));
        ++it;
        chosen.push_back(std::any_cast<bool>(*it));
        words.push_back(next_words);
        ngram_string.push_back(next_words.join(" "));
        if(printv){
            qDebug("printv");
            qDebug() << "next_words" <<  next_words;
            qDebug() << "words" <<  next_words.constLast();
            qDebug() << "ngram_string" <<  ngram_string.constLast();
            qDebug() << "counts" <<  counts.constLast();
            printv = false;
        }
        //qDebug() << "next ngram_string =" << ngram_string.constLast();
        chosen_by_word.push_back(base_word_chosen);
    }
    // TODO is this function needed
    finishAFterDataAddedOrDeleted();
    return true;
}
