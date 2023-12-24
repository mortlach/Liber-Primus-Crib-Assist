
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
#ifndef NGRAMPHRASEWORKER_H
#define NGRAMPHRASEWORKER_H

#include <QObject>
#include "ngrammetadata.h"

class NgramPhraseWorker : public QObject {
    Q_OBJECT
public:
    NgramPhraseWorker();
    ~NgramPhraseWorker(){};

    void setPath(const QString& chosen_dir_in, const QString& chosen_fn_in){
        dir_path = chosen_dir_in;
        filename = chosen_fn_in;
    }
    void setDoDelete(bool s){
        do_delete = s;
    }
    void setPUWHasPrioriyt(bool s){
        puw_has_priority = s;
    }

    void setNgramPhraseDataStruct(const NgramPhraseMetaData& s)
    {
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
    inline bool lastProcessWasASuccess()const{ return last_process_was_success;}
    inline bool isWorking()const{ return is_working;}
    inline bool isNotWorking()const{ return !isWorking();}

public slots:
    void processLoadPhrase();
    void processNewPhrase();
    void processRationalizePhrase();

signals:
    void finished(bool success);
    void error(QString err);

private:
    bool last_process_was_success;
    bool is_working;
    QString dir_path;
    QString filename;
    bool do_delete;
    bool puw_has_priority;
    NgramPhraseMetaData ngram_phrase_data;
};

#endif // NGRAMPHRASEWORKER_H
