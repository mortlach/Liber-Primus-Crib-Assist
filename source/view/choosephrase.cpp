
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
#include "choosephrase.h"
#include "utilities.h"
#include "ui_choosephrase.h"
#include <QFileInfo>
#include <QSpinBox>


ChoosePhrase::ChoosePhrase(QWidget *parent)
    : QWidget(parent)
    ,    ui(new Ui::ChoosePhrase)
    ,    phrase_word_count(0)
    ,    phrase_ngram_count(0)
    ,    has_sentence_start(false)
    ,    has_sentence_end(false)
    ,    can_load_ngram_files(false)
    ,    schemaGridLayout(nullptr)
{
    qDebug() << "ChoosePhrase::ChoosePhrase";
    ui->setupUi(this);
    ui->ngramFileLoadPushButton->setFont(utilities::getFont());
    ui->filesToFindPlainTextEdit->setFont(utilities::getFont());
    ui->filesNotFoundPlainTextEdit->setFont(utilities::getFont());
    ui->filePathLineEdit->setFont(utilities::getFont());
    ui->titleLabel->setFont(utilities::getFont());
    //ui->pwlLabel->setFont(utilities::getFont());
    ui->generateTemplatePushButton->setFont(utilities::getFont());
    ui->wordLengthLineEdit->setFont(utilities::getFont());
    ui->sentStartCheckBox->setFont(utilities::getFont());
    ui->sentEndCheckBox->setFont(utilities::getFont());
    ui->sentStartCheckBox->setFont(utilities::getFont());
    ui->ngramLabel->setFont(utilities::getFont());
    ui->phrasewl_groupBox->setFont(utilities::getFont());
    ui->rootpath_groupBox->setFont(utilities::getFont());
    ui->missingfiles_groupBox->setFont(utilities::getFont());
    ui->searchfiles_groupBox->setFont(utilities::getFont());
    ui->ngramLabel_3->setFont(utilities::getFont());
    ui->browseLoadPathPushButton->setFont(utilities::getFont());
    ui->browseSavePathPushButton->setFont(utilities::getFont());
    ui->clearAllPushButton->setFont(utilities::getFont());
    ui->saveFilePathLineEdit->setFont(utilities::getFont());
    setPhraseBad();
}
ChoosePhrase::~ChoosePhrase(){
    delete ui;
}
// todo
void ChoosePhrase::on_browseSavePathPushButton_clicked(){
    qDebug() << "on_browseSavePathPushButton_clicked, open dialog";
    QString start_dir;
    QString path;
    QString message = QString("Choose root save directory for phrase data");
    if(utilities::getDirectory(phrase_save_root_dir.absolutePath(),path,message)){
        setPhrasePathSaveRoot(QDir(path));
    }
}
void ChoosePhrase::on_browseLoadPathPushButton_clicked(){
    qDebug() << "on_browseLoadPathPushButton_clicked, open dialog";
    QString start_dir;
    QString path;
    QString message = QString("Choose root directory of n-gram data");
    if(utilities::getDirectory(ngram_root_dir.absolutePath(),path,message)){
        setNgramPathRoot(QDir(path));
    }
}
void ChoosePhrase::on_filePathLineEdit_editingFinished(){
    qDebug() << "on_filePathLineEdit_editingFinished";
}
void ChoosePhrase::on_ngramFileLoadPushButton_clicked(){
    qDebug() << "ChoosePhrase::on_ngramFileLoadPushButton_clicked";
    qDebug() << "new phrase phrase_save_root_dir:" << phrase_save_root_dir.path();
    QString start_dir  = phrase_save_root_dir.path();
    QString chosen_dir = QString("");
    QString message = QString("Chose folder, data in existing folders will be overwritten");
    if(utilities::getDirectory(start_dir, chosen_dir, message)){
        phrase_data_struct.phrase_dir = chosen_dir;
        phrase_data_struct.config_path = phrase_data_struct.phrase_dir.path() + "/" + "phrase_config.ini";
        qDebug() << "new phrase chosen_dir:" << chosen_dir;
        for(auto& item: phrase_data_struct.ngram_meta_data){
            item.phrase_dir = phrase_data_struct.phrase_dir;
            item.phrase_ngram_filepath = item.phrase_dir.path() + QString("/") + item.phrase_ngram_filename;
        }
        emit newPhraseChosen(phrase_data_struct);
    }
    else{
        qDebug() << "failed to find new phrase save directory aborting";
    }
}
void ChoosePhrase::on_clearAllPushButton_clicked(){
    qDebug() << "ChoosePhrase::on_clearAllPushButton_clicked";
    ui->filesToFindPlainTextEdit->clear();
    ui->filesNotFoundPlainTextEdit->clear();
    ui->wordLengthLineEdit->clear();
    ui->sentStartCheckBox->setChecked(false);
    ui->sentEndCheckBox->setChecked(false);
    word_lengths.clear();
    reset_schema_gridlayout();
    ui->ngramFileLoadPushButton->setEnabled(false);
}
void ChoosePhrase::on_wordLengthLineEdit_editingFinished(){
    qDebug() << "\nChoosePhrase::on_wordLengthLineEdit_editingFinished";
    create_phrase_schema();
}
void ChoosePhrase::on_generateTemplatePushButton_clicked(){
    qDebug() << "\nChoosePhrase::on_generateTemplatePushButton_clicked";
    create_phrase_schema();
}
void ChoosePhrase::create_phrase_schema(){
    QLineEdit* t = ui->wordLengthLineEdit;
    QString text = t->text();
    if(setInputPhrase(text, ui->sentStartCheckBox->isChecked(), ui->sentEndCheckBox->isChecked())){
        // cleanup old layout
        ui->filesToFindPlainTextEdit->clear();
        ui->filesNotFoundPlainTextEdit->clear();
        ui->ngramFileLoadPushButton->setEnabled(false);
        reset_schema_gridlayout();
        setPhraseSchemaGridView();
        testNgramSchema();
    }
}
void ChoosePhrase::setWordLengthSpinboxes(){
    qDebug() << "ChoosePhrase::setWordLengthSpinboxes";
    int col = phrase_col_offset + (int)has_sentence_start;
    for(const auto& v : word_lengths){
        QLayoutItem* item = schemaGridLayout->itemAtPosition(0,col);
        ((QSpinBox*)item->widget())->setValue(v);
        ++col;
    }
}
void ChoosePhrase::reset_schema_gridlayout(){
    qDebug() << "\nChoosePhrase::clear_schema_gridlayout";
    if(schemaGridLayout){
        // delete existing widgets and layout, then make and setup new one
        QLayoutItem *child;
        while ((child = schemaGridLayout->takeAt(0)) != nullptr) {
            delete child->widget(); // delete the widget
            delete child;   // delete the layout item
        }
        ui->baseSchemaGridLayout->removeItem(schemaGridLayout);
        delete schemaGridLayout;
        schemaGridLayout = nullptr;
    }
    schemaGridLayout = new QGridLayout();
    ui->baseSchemaGridLayout->addLayout(schemaGridLayout, 0, 0, 1, 1, Qt::AlignCenter);
    ui->baseSchemaGridLayout->update();
    // add column headers
    QLabel* nglen_label = new QLabel("Ngram Length");
    nglen_label->setFont(utilities::getFont());
    schemaGridLayout->addWidget(nglen_label, 0, 0, 1, 1, Qt::AlignCenter);
    QLabel* si_label = new QLabel("Start Index");
    si_label->setFont(utilities::getFont());
    schemaGridLayout->addWidget(si_label, 0, 1, 1, 1, Qt::AlignCenter);
    // set these here, and if above widgets change these change too
    phrase_col_offset = 2;
    row_offset = 1;
}
//-----------------------------------------------------------------------------
void ChoosePhrase::setPhraseSchemaGridView(){
    if(has_sentence_start){
        qDebug() << "has_sent_start";
        QSpinBox* word_length_spinbox = new QSpinBox();
        word_length_spinbox->setFont(utilities::getFont());
        word_length_spinbox->setMinimum(-1);
        word_length_spinbox->setMaximum(-1);
        word_length_spinbox->setEnabled(false);
        word_length_spinbox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        word_length_spinbox->setSpecialValueText(tr("S"));
        schemaGridLayout->addWidget(word_length_spinbox, 0, phrase_col_offset, 1, 1, Qt::AlignCenter);
        ui->sentStartCheckBox->setCheckState(Qt::Checked);
    }
    else{
        ui->sentStartCheckBox->setCheckState(Qt::Unchecked);
    }
    // Add to gridlayout word_length spinboxes for each word in phrase
    for(auto wc = 0; wc < phrase_word_count; ++wc){
        QSpinBox* word_length_spinbox = new QSpinBox();
        word_length_spinbox->setFont(utilities::getFont());
        word_length_spinbox->setMinimum(1);
        word_length_spinbox->setMaximum(14);
        schemaGridLayout->addWidget(word_length_spinbox, 0, wc+phrase_col_offset+(int)has_sentence_start, 1, 1, Qt::AlignCenter);
    }
    // end the top row, is there an end tag ?
    if(has_sentence_end){
        qDebug() << "has_sentence_end";
        QSpinBox* word_length_spinbox = new QSpinBox();
        word_length_spinbox->setFont(utilities::getFont());
        word_length_spinbox->setMinimum(-2);
        word_length_spinbox->setMaximum(-2);
        word_length_spinbox->setEnabled(false);
        word_length_spinbox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        word_length_spinbox->setSpecialValueText(tr("E"));
        schemaGridLayout->addWidget(word_length_spinbox, 0, phrase_token_count+phrase_col_offset-1, 1, 1, Qt::AlignCenter);
        ui->sentEndCheckBox->setCheckState(Qt::Checked);
    }
    else{
        ui->sentEndCheckBox->setCheckState(Qt::Unchecked);
    }
    // total column count
    col_count = schemaGridLayout->columnCount();
    // Set num checkboxes
    num_checkboxes = col_count - phrase_col_offset;
    for(auto ng = 0; ng < max_ngram_count; ++ng){
        // create ng length and start index spinboxesin schemaGridLayout
        QSpinBox* ngram_length_spinbox = new QSpinBox();
        ngram_length_spinbox->setFont(utilities::getFont());
        ngram_length_spinbox->setMinimum(2);
        ngram_length_spinbox->setValue(2); // NO Length 1 Ngrams are allowed!
        ngram_length_spinbox->setMaximum(5);
        schemaGridLayout->addWidget(ngram_length_spinbox, ng+row_offset, 0, 1, 1, Qt::AlignCenter);
        QSpinBox* ngram_index_spinbox = new QSpinBox();
        ngram_index_spinbox->setFont(utilities::getFont());
        ngram_index_spinbox->setMinimum(0);
        ngram_index_spinbox->setValue(ng);
        ngram_index_spinbox->setMaximum(phrase_token_count-2); // -1 as no 1-grams are allowed AND -1 for count from 0
        schemaGridLayout->addWidget(ngram_index_spinbox, ng+row_offset, 1, 1, 1, Qt::AlignCenter);
        // connect with lambda to also pass row index
        connect(ngram_index_spinbox, &QSpinBox::valueChanged,this,
                [=] {emit on_ngramLengthIndexChanged(ngram_length_spinbox->value(),ngram_index_spinbox->value(), ng+row_offset);});
        connect(ngram_length_spinbox, &QSpinBox::valueChanged,this,
                [=] {emit on_ngramLengthIndexChanged(ngram_length_spinbox->value(),ngram_index_spinbox->value(), ng+row_offset);});
        // Add the checkboxes
        for(auto wc = 0; wc < num_checkboxes; ++wc){
            QCheckBox* next_checkbox = new QCheckBox();
            next_checkbox->setEnabled(false);
            // default setup is length 2 ngrams with start index 0,1,2,...phrase_word_count-1
            if(wc == ng || wc == ng + 1){
                next_checkbox->setCheckState(Qt::Checked);
            }
            schemaGridLayout->addWidget(next_checkbox, ng+row_offset, wc+phrase_col_offset, 1, 1, Qt::AlignCenter);
        }
    }
    setWordLengthSpinboxes();
}

void ChoosePhrase::setPhraseBad(){
    ui->ngramFileLoadPushButton->setEnabled(false);
    ui->ngramFileLoadPushButton->setStyleSheet( utilities::getPaleRedString());
    ui->wordLengthLineEdit->setStyleSheet( utilities::getPaleRedString());
}
void ChoosePhrase::setPhraseGood(){
    ui->ngramFileLoadPushButton->setEnabled(true);
    ui->ngramFileLoadPushButton->setStyleSheet( utilities::getPaleGreenString());
    ui->wordLengthLineEdit->setStyleSheet( utilities::getPaleGreenString());
}



bool ChoosePhrase::setInputPhrase(const QString& phrase, bool has_sent_start_in, bool has_sent_end_in){
    qDebug() << "\nChoosePhrase::setInputPhrase got " << phrase << has_sent_start_in << has_sent_end_in;
    QString text = phrase;
    // get individual tokens
    text.replace(",", " ");
    auto text_parts = text.split(u' ',Qt::SkipEmptyParts);
    if(text_parts.size() < 1){
        return false;
    }
    //
    phrase_tokens.clear();
    word_lengths.clear();
    phrase_word_count = 0;
    phrase_token_count = 0;
    max_ngram_count = 0;
    qDebug() << "text_parts ="  << text_parts;
    // start end tags can be defined in phrase OR passed bools
    // which makes the below messier
    qDebug() << (text_parts.first() != "S");
    qDebug() << (text_parts.first() != "s");
    if( (text_parts.first() != "S") && (text_parts.first() != "s")){
        if( has_sent_start_in){
            text_parts.prepend("s");
            qDebug() << "has_sent_start_in but no S in string, prepend s";
        }
    }
    if( (text_parts.last() != "E") && (text_parts.last() != "e") ){
        if( has_sent_end_in){
            text_parts.append("e");
            qDebug() << "has_sent_end_in but no E in string, append s";
        }
    }
    // now parse phrase
    // 1. set start end tags
    qDebug() << "parse text_parts = " << text_parts;
    //
    if( text_parts.first() == "S" || text_parts.first() == "s"){
        text_parts.first() = QString("-1");
        qDebug() << "Found S, check sentStartCheckBox";
        //ui->sentStartCheckBox->setCheckState(Qt::Checked);
        has_sentence_start = true;
    }
    else{
        //ui->sentStartCheckBox->setCheckState(Qt::Unchecked);
        has_sentence_start = false;
    }
    // end tags
    if(text_parts.last() == "E" || text_parts.last() == "e"){
        text_parts.last() = QString("-2");
        qDebug() << "Found E, check sentEndCheckBox";
        //ui->sentEndCheckBox->setCheckState(Qt::Checked);
        has_sentence_end = true;
    }
    else{
        //ui->sentEndCheckBox->setCheckState(Qt::Unchecked);
        has_sentence_end = false;
    }
    // now fill phrase_tokens, check numebrs and positions are valid
    bool no_fails=true;
    for(auto i = 0; i < text_parts.size(); ++i){
        bool ok;
        int num = text_parts.at(i).toInt(&ok);
        if(ok){
            // if within min max word lengths
            if( 0 < num &&  num < 15){
                phrase_tokens.push_back(num);
                word_lengths.push_back(num);
                qDebug() << "word_lengths.push_back"  << num;
            }
            else if( i == 0 && num == -1){
                phrase_tokens.push_back(num);
                qDebug() << "word_lengths.push_back"  << num;
            }
            else if( i == text_parts.size()-1 && num == -2){
                phrase_tokens.push_back(num);
                qDebug() << "word_lengths.push_back"  << num;
            }
            else{
                ok = false;
            }
        }
        if( !ok ){
            no_fails = false;
        }
    }
    if(no_fails){
        // set counts
        phrase_token_count = phrase_tokens.size();
        phrase_word_count = word_lengths.size();
        // for schema-grid gui use max_ngram_count to create max grid size, which is:
        // overlapping 2-grams, no partial n-grams in schema:
        max_ngram_count = phrase_token_count -1;
        // count words (not including start end tags)
        return true;
    }
    qDebug() << "ERROR setInputPhrase returning FALSE";
    return false;
}

void ChoosePhrase::setRowDisabled(int row){
    for(auto c = 2; c < schemaGridLayout->columnCount(); c++ ){
        QLayoutItem* item = schemaGridLayout->itemAtPosition(row, c);
        QCheckBox* check_box = (QCheckBox*)item->widget();
        check_box->setCheckState(Qt::Unchecked);
    }
}
void ChoosePhrase::on_ngramLengthIndexChanged(int ngram_length,
                                              int ngram_start_index,
                                              int ngram_row_index)
{
    // max ngram_start_index is set by ngram_length and phrase_length
    QLayoutItem* item = schemaGridLayout->itemAtPosition(ngram_row_index,1);
    QSpinBox* sb = (QSpinBox*)item->widget();
    int max_index = phrase_token_count - ngram_length;
    sb->setMaximum(max_index);
    qDebug() << "\non_ngramLengthIndexChanged got" << ngram_length << ngram_start_index <<  ngram_row_index;
    if(ngram_length < 2){
        setRowDisabled(ngram_row_index);
    }
    else{
        // in below 2 due to first few labels !
        int col_count = schemaGridLayout->columnCount();
        QList<int> index_to_check((qsizetype)ngram_length);
        for(auto i = 0; i < ngram_length; ++i){
            index_to_check[i] = i+ngram_start_index+2;
        }
        qDebug() << "index_to_check =" << index_to_check << "," << "col_count =" << col_count << "," << "ngram_length-1 =" << ngram_length-1 << ", index_to_check.size() " << index_to_check.size();
        if(index_to_check.back() <= col_count-1){
            for(int col = 0; col < col_count; col++ ){
                if(col>1){
                    QCheckBox* next_checkbox= (QCheckBox*)schemaGridLayout->itemAtPosition(ngram_row_index,col)->widget();
                    if( index_to_check.contains(col)){
                        next_checkbox->setCheckState(Qt::Checked);
                    }
                    else{
                        next_checkbox->setCheckState(Qt::Unchecked);
                    }
                }
            }
        }
        else{
            qDebug() << "Entire NGram NOT in phrase, index_to_check.back() <= col_count, " << index_to_check.back() << "<=" << col_count;
            setRowDisabled(ngram_row_index);
        }
    }
    testNgramSchema();
}
void ChoosePhrase::testNgramSchema(){
    ngram_schema_good = isNgramSchemaGood();
    canLookForFiles();
    if(ngram_schema_good){
        setNgramFileData();
    }
}
void ChoosePhrase::canLookForFiles(){
    ngram_schema_good = isNgramSchemaGood();
    if(ngram_schema_good){
        setPhraseGood();
        //ui->ngramFileLoadPushButton->setDisabled(false);
        // todo more stuff
    }
    else{
        setPhraseBad();
        //ui->ngramFileLoadPushButton->setDisabled(true);
    }
}

void ChoosePhrase::setNgramFileData2(){
    phrase_data_struct.raw_ngram_root_dir = ngram_root_dir;
    // where new phrase data well be saves , not yet set
    phrase_data_struct.phrase_dir = QString("phrase_dir at ChoosePhrase");
    // phrase tokens word lengths + start end tags
    phrase_data_struct.phrase_tokens = phrase_tokens;
    // from which set phrase word lengths and PUW file names
    for(const auto& item: phrase_data_struct.phrase_tokens){
        if(item>0){
            phrase_data_struct.phrase_word_lengths.push_back(item);
        }
    }
    // has The Phrase got sent start/end tag?
    if(phrase_tokens.first()==-1){
        phrase_data_struct.has_sent_start = true;
    }
    if(phrase_tokens.last()==-2){
        phrase_data_struct.has_sent_end = true;
    }
    // now set individual raw_ngram filenames, and phrase_ngram_filenames filenames
    int start_col = phrase_col_offset;
    int end_col = schemaGridLayout->columnCount()-1;
    int start_row = row_offset;
    int end_row = schemaGridLayout->rowCount()-1;
    ui->filesToFindPlainTextEdit->clear();
    for(int r = start_row; r <= end_row; ++r){
        // new individual ngram data struct to add to phrase_data_struct.ngram_data_structs
        NgramMetaData new_ngramfiledata= NgramMetaData();
        //
        // phrase_tokens
        // phrase_tokens are all tokens in phrase (include s,e tags)
        new_ngramfiledata.phrase_tokens = phrase_tokens;
        //
        // phrase_start_index
        // the index of the first token in this ngram in the phrase
        QLayoutItem* item = schemaGridLayout->itemAtPosition(r,1); // MAGIC 1
        new_ngramfiledata.first_token_index_in_phrase = ((QSpinBox*)item->widget())->value();
        //
        // phrase_word_lengths
        // WORD LENGTHS (NO S OR E TAG)
        //qDebug() << "Schema-grid-gui row" << r << "ngram start index=" << new_ngramfiledata.phrase_start_index;
        new_ngramfiledata.phrase_word_lengths = phrase_data_struct.phrase_word_lengths;
        // "words" is the data without start end tags included
        //
        new_ngramfiledata.first_word_index_in_phrase_word_indices = new_ngramfiledata.first_token_index_in_phrase;
        if(new_ngramfiledata.phrase_tokens.constFirst() == -1){
            if(new_ngramfiledata.first_token_index_in_phrase > 0){
                new_ngramfiledata.first_word_index_in_phrase_word_indices = new_ngramfiledata.first_token_index_in_phrase-1;
            }
            else{
            }
        }
        //  to save data
        new_ngramfiledata.ngram_root_dir = ngram_root_dir;
        // add phrase_start_index
        //bool find_start_index = true;
        for(auto c = start_col; c <= end_col; ++c){
            QLayoutItem* item = schemaGridLayout->itemAtPosition(r,c);
            QCheckBox* checkbox = (QCheckBox*)item->widget();
            if(checkbox->isChecked()){
                QLayoutItem* word_len_item= schemaGridLayout->itemAtPosition(0,c);
                new_ngramfiledata.ngram_tokens.push_back(((QSpinBox*)word_len_item->widget())->value());
            }
        }
        //
        // W
        // add the indices of the words
        qDebug() << "add the indices of the words";
        int word_counter = 0;
        for(const auto& item: new_ngramfiledata.ngram_tokens){
            if(item >0){
                new_ngramfiledata.ngram_word_indices.push_back(new_ngramfiledata.first_word_index_in_phrase_word_indices + word_counter);
                ++word_counter;
            }
        }
        qDebug() << "new_ngramfiledata.ngram_word_indices" << new_ngramfiledata.ngram_word_indices;

        new_ngramfiledata.ngram_index = new_ngramfiledata.ngram_word_indices.constFirst();
        qDebug() << "new_ngramfiledata.ngram_index" << new_ngramfiledata.ngram_index;
        new_ngramfiledata.ngram_length = new_ngramfiledata.ngram_tokens.size();
        phrase_data_struct.ngram_token_lengths.push_back(new_ngramfiledata.ngram_length);
        // TODO filenames are set
        qDebug() << "setNgramFileData2 Schema-grid-gui row" << r << "data =" << new_ngramfiledata.ngram_tokens;
        // this converts the tags as well as word lengths to filepath
        auto fn_fp = utilities::ngramSchemaDataToFilePath(ngram_root_dir, new_ngramfiledata.ngram_tokens);
        //
        new_ngramfiledata.raw_filename = fn_fp.first;
        new_ngramfiledata.raw_filepath = fn_fp.second;
        // for files to be saved to phrase data, prepend with the start index to give unique filename
        //new_ngramfiledata.phrase_ngram_filename = QString::number(r-row_offset) + "_" + new_ngramfiledata.raw_filename;
        if(new_ngramfiledata.ngram_tokens.first()==-1){
            new_ngramfiledata.has_sent_start = true;
        }
        if(new_ngramfiledata.ngram_tokens.last()==-2){
            new_ngramfiledata.has_sent_end = true;
        }
        // update output text
        ui->filesToFindPlainTextEdit->appendPlainText(new_ngramfiledata.raw_filepath);
        // add to class member container
        if(new_ngramfiledata.hasAnyDefaultSetttings()){
            //std::cin.get();
        }
        phrase_data_struct.ngram_meta_data.push_back(new_ngramfiledata);
    }
}

void ChoosePhrase::setNgramFileData(){
    if(ngram_schema_good){
        cleanPhraseDataStruct();
        //here we can set as much file paths etc as possible
        //phrase_data_struct.puw_filenames.clear();
        phrase_data_struct.phrase_ngram_filenames.clear();
        phrase_data_struct.raw_ngram_filenames.clear();
        // root directory for raw ngrams
        phrase_data_struct.raw_ngram_root_dir = ngram_root_dir;
        //
        // phrase_data_struct.phrase_dir = QDir("NOT_SET_YET");
        // phrase tokens are given from GUI
        phrase_data_struct.phrase_tokens = phrase_tokens;
        // from which set phrase word lengths and PUW file names
        int puw_count = 0;
        for(const auto& item: phrase_data_struct.phrase_tokens){
            if(item>0){
                phrase_data_struct.phrase_word_lengths.push_back(item);
                puw_count +=1;
            }
        }
        // has The Phrase got sent start/end tag?
        if(phrase_tokens.first()==-1){
            phrase_data_struct.has_sent_start = true;
        }
        if(phrase_tokens.last()==-2){
            phrase_data_struct.has_sent_end = true;
        }
        // now set individual raw_ngram filenames, and phrase_ngram_filenames filenames
        int start_col = phrase_col_offset;
        int end_col = schemaGridLayout->columnCount()-1;
        int start_row = row_offset;
        int end_row = schemaGridLayout->rowCount()-1;
        ui->filesToFindPlainTextEdit->clear();
        for(int r = start_row; r <= end_row; ++r){
            qDebug() << "Process row = " << r;
            NgramMetaData new_ngramfiledata = NgramMetaData();
            // the index of this ngram
            new_ngramfiledata.ngram_index = r - start_row;
            qDebug() << "new_ngramfiledata.ngram_index" << new_ngramfiledata.ngram_index;
            // phrase_tokens are all tokens in phrase (include s,e tags)
            new_ngramfiledata.phrase_tokens = phrase_tokens;
            qDebug() << "new_ngramfiledata.phrase_tokens" << new_ngramfiledata.phrase_tokens;
            // the index of the first token in this ngram in the phrase
            QLayoutItem* item = schemaGridLayout->itemAtPosition(r,1); // MAGIC
            new_ngramfiledata.first_token_index_in_phrase = ((QSpinBox*)item->widget())->value();
            qDebug() << "new_ngramfiledata.first_token_index_in_phrase" << new_ngramfiledata.first_token_index_in_phrase;
            //qDebug() << "Schema-grid-gui row" << r << "ngram start index=" << new_ngramfiledata.phrase_start_index;
            new_ngramfiledata.phrase_word_lengths = phrase_data_struct.phrase_word_lengths;
            qDebug() << "new_ngramfiledata.phrase_word_lengths" << new_ngramfiledata.phrase_word_lengths;
            // "words" is the data without start end tags included
            new_ngramfiledata.first_word_index_in_phrase_word_indices = new_ngramfiledata.first_token_index_in_phrase;
            if(new_ngramfiledata.phrase_tokens.constFirst() == -1){
                if(new_ngramfiledata.first_token_index_in_phrase > 0){
                    new_ngramfiledata.first_word_index_in_phrase_word_indices = new_ngramfiledata.first_token_index_in_phrase-1;
                }
                else{
                }
            }
            qDebug() << "new_ngramfiledata.first_word_index_in_phrase_word_indices" << new_ngramfiledata.first_word_index_in_phrase_word_indices;
            //  to save data
            new_ngramfiledata.ngram_root_dir = ngram_root_dir;
            // add phrase_start_index
            //bool find_start_index = true;
            for(auto c = start_col; c <= end_col; ++c){
                QLayoutItem* item = schemaGridLayout->itemAtPosition(r,c);
                QCheckBox* checkbox = (QCheckBox*)item->widget();
                if(checkbox->isChecked()){
                    QLayoutItem* word_len_item= schemaGridLayout->itemAtPosition(0,c);
                    new_ngramfiledata.ngram_tokens.push_back(((QSpinBox*)word_len_item->widget())->value());
                    //new_data.ngram_length += 1;
                }
            }
            qDebug() << "new_ngramfiledata.ngram_tokens" << new_ngramfiledata.ngram_tokens;
            // add the indices of the words
            int word_counter = 0;
            for(const auto& item: new_ngramfiledata.ngram_tokens){
                if(item >0){
                    new_ngramfiledata.ngram_word_indices.push_back(new_ngramfiledata.first_word_index_in_phrase_word_indices + word_counter);
                    ++word_counter;
                }
            }
            new_ngramfiledata.ngram_length = new_ngramfiledata.ngram_tokens.size();
            qDebug()<< "new_ngramfiledata.ngram_length" << new_ngramfiledata.ngram_length;
            phrase_data_struct.ngram_token_lengths.push_back(new_ngramfiledata.ngram_length);
            qDebug() << "phrase_data_struct.ngram_token_lengths" << phrase_data_struct.ngram_token_lengths;

            qDebug() << "setNgramFileData Schema-grid-gui row" << r << "data =" << new_ngramfiledata.ngram_tokens;
            // this converts the tags as well as word lengths to filepath
            auto fn_fp = utilities::ngramSchemaDataToFilePath(ngram_root_dir, new_ngramfiledata.ngram_tokens);
            //
            new_ngramfiledata.raw_filename = fn_fp.first;
            new_ngramfiledata.raw_filepath = fn_fp.second;
            // for files to be saved to phrase data, prepend with the start index to give unique filename
            new_ngramfiledata.phrase_ngram_filename = QString::number(r-row_offset) + "_" + new_ngramfiledata.raw_filename;

            if(new_ngramfiledata.ngram_tokens.first()==-1){
                new_ngramfiledata.has_sent_start = true;
            }
            if(new_ngramfiledata.ngram_tokens.last()==-2){
                new_ngramfiledata.has_sent_end = true;
            }
            // update output text
            ui->filesToFindPlainTextEdit->appendPlainText(new_ngramfiledata.raw_filepath);
            // add to class member container
            phrase_data_struct.ngram_meta_data.push_back(new_ngramfiledata);
        }
        // hack - but
        phrase_data_struct.ngram_lengths = phrase_data_struct.ngram_token_lengths;
        if(phrase_data_struct.has_sent_start){
            phrase_data_struct.ngram_lengths.first() -=1;
        }
        if(phrase_data_struct.has_sent_end){
            phrase_data_struct.ngram_lengths.last() -=1;
        }
        // go and look for files and write output to gui
        lookForFilesAndSetGUI();
    }
}

void ChoosePhrase::lookForFilesAndSetGUI(){
    qDebug() << "ChoosePhrase::lookForFilesAndSetGUI";
    ui->filesNotFoundPlainTextEdit->clear();
    // ready to build ngrams filepaths:
    bool missing_files = false;
    for(auto& item : phrase_data_struct.ngram_meta_data){
        QFileInfo check_file(item.raw_filepath);
        // file exists and Is it really a file?
        if (check_file.exists() && check_file.isFile()){
            item.raw_file_exists = true;
        }
        else{
            missing_files = true;
            item.raw_file_exists = false;
            qDebug() << "Missing file" << item.raw_filepath;
            ui->filesNotFoundPlainTextEdit->appendPlainText(item.raw_filepath);
        }
    }
    can_load_ngram_files = !missing_files;
    ui->ngramFileLoadPushButton->setEnabled(can_load_ngram_files);
}
bool ChoosePhrase::isNgramSchemaBad(){
    return !isNgramSchemaGood();
}
bool ChoosePhrase::isNgramSchemaGood(){
    // check that each word-column has at least two ngram checkboxes checked
    // first and last columns must have at least 1
    int start_col = phrase_col_offset;
    int end_col = schemaGridLayout->columnCount()-1;
    int start_row = row_offset;
    int end_row = schemaGridLayout->rowCount()-1;
    qDebug() << "cs ce:" << start_row << end_col;
    qDebug() << "rs re:" << start_row << end_row;
    for(auto c = start_col; c <= end_col; ++c){
        // for this  column, count how mnay check boxes are checked
        int tick_count = 0;
        for(int r = start_row; r <= end_row; ++r){
            //qDebug() << "c r:" << c << r;
            QLayoutItem* item = schemaGridLayout->itemAtPosition(r,c);
            if(item){
                QCheckBox* checkbox = (QCheckBox*)item->widget();
                if(checkbox->isChecked()){
                    tick_count += 1;
                    //qDebug() << "c r:" << c << r << "tick_count =" << tick_count;
                }
            }
        }
        if(c == start_col){
            if(tick_count == 0){
                qDebug() << "start coland tick_count == 0, schema must be bad";
                return false;
            }
        }
        else if( c == end_col){
            if(tick_count == 0){
                qDebug() << "end coland tick_count == 0, schema must be bad";
                return false;
            }
        }
        else{ // Other columns must have at least 2
            if(tick_count < 2){
                qDebug() << "col" << c << "tick_count < 2,  schema must be bad";
                return false;
            }
        }
    }
    return true;
}
void ChoosePhrase::setNgramPathRoot(const QDir& dir){
    ngram_root_dir = dir;
    qDebug() << "ChoosePhrase::setNgramPathRoot raw ngram root path set to:" << ngram_root_dir.path();
    ui->filePathLineEdit->setText(ngram_root_dir.path());
}
void ChoosePhrase::setPhrasePathSaveRoot(const QDir& dir){
    phrase_save_root_dir = dir;
    qDebug() << "ChoosePhrase::setNgramPathSave ngram_save_dir set to:" << phrase_save_root_dir.path();
    ui->saveFilePathLineEdit->setText(phrase_save_root_dir.path());
}
bool ChoosePhrase::setNgramPathSave(const QString& new_dir){
    // TODO clean up
    QString curretn_path = phrase_save_root_dir.path();
    // append string to existing path
    phrase_save_root_dir.path();
    int i = 0;
    while( i < 10){
        if( i < 1){
            QString newdir = new_dir;
        }
        else{
            QString newdir = new_dir + "_" + QString::number(i);
        }
        if(phrase_save_root_dir.cd(new_dir)){ // if directory exists

        }
        else{
            break;
        }
        ++i;
    }
    if(i < 10){
        if(phrase_save_root_dir.mkdir(new_dir)){
            QString s = phrase_save_root_dir.relativeFilePath("new_dir/phrase_config.ini");
            phrase_save_root_dir = QDir(s);
            return true;
        }
    }
    return false;
}

void ChoosePhrase::cleanPhraseDataStruct(){
    phrase_data_struct.config_path = QString();
    phrase_data_struct.raw_ngram_root_dir = QDir();
    phrase_data_struct.phrase_dir = QDir();
    phrase_data_struct.phrase_tokens = QList<int>();
    phrase_data_struct.phrase_word_lengths = QList<int>();
    phrase_data_struct.ngram_token_lengths = QList<int>();
    phrase_data_struct.ngram_lengths = QList<int>();
    phrase_data_struct.phrase_ngram_filenames = QList<QString>();
    phrase_data_struct.raw_ngram_filenames = QList<QString>();
    phrase_data_struct.has_sent_start = false;
    phrase_data_struct.has_sent_end = false;
    phrase_data_struct.ngram_meta_data = QList<NgramMetaData>();
}



