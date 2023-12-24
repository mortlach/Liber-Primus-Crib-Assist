
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
#include "ngramdatahub.h"
#include "utilities.h"

Raw1Grams NgramDataHub::raw1gram = Raw1Grams();
NgramPhrase NgramDataHub::ngram_phrase = NgramPhrase();
NgramPhraseMetaData NgramDataHub::ngram_phrase_data = NgramPhraseMetaData();
QString NgramDataHub::chosen_dir = QString("chosen_dir NOT SET");
QString NgramDataHub::chosen_fn = QString("chosen_fn NOT SET");

NgramDataHub::NgramDataHub(QSettings& settings_in)
    : NgramDataHub()
{
    loadRaw1grams(settings_in);
}

NgramDataHub::NgramDataHub(){
}

bool NgramDataHub::loadRaw1grams(QSettings& settings){
    if(raw1gram.applySettingsAndLoad(settings)){
        qDebug() << "NgramDataHub::loadRaw1grams applySettingsAndLoad complete";
        return true;
    }
    qDebug() << "NgramDataHub::loadRaw1grams applySettingsAndLoad failed";
    return false;
}
bool NgramDataHub::newPhrase(const NgramPhraseMetaData& data_in){
    // print data
    utilities::getNgramPhraseDataAsString(data_in, true);
    // set phrase
    if(ngram_phrase.setNewPhase(data_in)){
        qDebug() << "NgramDataHub::newPhrase setNewPhase complete";
        return true;
    }
    qDebug() << "NgramDataHub::newPhrase setNewPhase failed";
    return false;
}
void NgramDataHub::clearPhraseData(){
    ngram_phrase.clearAllNgramPhraseData();
}

bool NgramDataHub::loadPhrase(const QString& dir_path,const QString& filename){
    qDebug() << "NgramDataHub::loadPhrase" << dir_path << filename;
    return ngram_phrase.loadPhraseFromConfig(dir_path,filename);
}
bool NgramDataHub::rationalizeNgramData(bool delete_not_chosen_ngrams){
    return rationalizeNgramData(delete_not_chosen_ngrams, true);
}

bool NgramDataHub::rationalizeNgramData(bool delete_not_chosen_ngrams, bool puw_has_priority_in)
{
    qDebug() << "\n***** NgramDataHub::rationalizeNgramData *****";
    // TODO make this work
    bool re_do = true;
    int count = 0;
    while(re_do){
        count+=1;
        qDebug() << "NgramDataHub::rationlizeNgramData ITERATION" << count;
        // record number of chosen items in each data set
        QList<int> ngram_chosen_counts_1;
        QList<int> puw_chosen_counts_1;
        getAndPrintNgramPUWChosenCounts(ngram_chosen_counts_1,puw_chosen_counts_1);
        ngram_phrase.rationalizePhrase(delete_not_chosen_ngrams, puw_has_priority_in);
        QList<int> ngram_chosen_counts_3;
        QList<int> puw_chosen_counts_3;
        getAndPrintNgramPUWChosenCounts(ngram_chosen_counts_3,puw_chosen_counts_3);
        re_do = false;
        for(int i = 0; i < puw_chosen_counts_3.size(); ++i ){
            if(puw_chosen_counts_3.at(i) != puw_chosen_counts_1.at(i))
            {
                re_do = true;
                break;
            }
        }
    }
    return true;
}

void NgramDataHub::getAndPrintNgramPUWChosenCounts(QList<int>& ngram_chosen_counts, QList<int>& puw_chosen_counts){
    ngram_chosen_counts.clear();
    puw_chosen_counts.clear();
    QString ng_str = QString("ngram_chosen_counts ");
    for(auto& item: ngram_phrase.ngram_data){
        item->updateChosenCounts();
        ngram_chosen_counts.push_back(item->chosen_count);
        ng_str += QString::number(item->chosen_count);
        ng_str += " ";
    }
    qDebug() << ng_str;
    QString puw_str = QString("puw_chosen_counts ");
    for(const auto& item: ngram_phrase.phrase_unique_words){
        item->updateChosenCounts();
        puw_chosen_counts.push_back(item->chosen_count);
        puw_str += QString::number(item->chosen_count);
        puw_str +=  " ";
    }
    qDebug() << puw_str;
}


