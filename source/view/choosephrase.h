
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
#ifndef CHOOSEPHRASE_H
#define CHOOSEPHRASE_H

#include <QWidget>
#include <QDir>
#include <QGridLayout>
#include "ngrammetadata.h"

namespace Ui {
class ChoosePhrase;
}
/*!
 * \brief The ChoosePhrase class
 *
 * gui to define a phrase to import word lists for
 * enter word lengths, e.g. 3,12,4
 * optional include start end tags: s,3,12,4,e
 * or use checkboxes
 * after phrase token structure is defined a "schema-grid" is produced
 *
 * the "schema-grid" allows you to choose:
 * the number of words in each ngram,
 * the start position of the ngram in the phrase
 * (the above two controls will enable/disable checkboxes to visualize the schema)
 * the word lengths, set by spin boxes at the top of each column
 * dynamic checking for requested files will determine if the current
 * phrase schema can be created
 * Will generate phrase on succesful clicking of "load ngram file data"
 * Phrase-data will be saved to new folder (which cna be changed)
 */
class ChoosePhrase : public QWidget
{

    Q_OBJECT

public:
    explicit ChoosePhrase(QWidget *parent = nullptr);
    ~ChoosePhrase();
    void setNgramPathRoot(const QDir& dir);
    void setPhrasePathSaveRoot(const QDir& dir);
    bool setNgramPathSave(const QString& new_dir);
    void setPhraseGood();
    void setPhraseBad();


public slots:
    void on_generateTemplatePushButton_clicked();
    // enables/disables groups of checkboxes in the schema-grid-gui, ensuring mal-fromed requests are caught
    void on_ngramLengthIndexChanged(int ngram_length, int ngram_start_index, int ngram_row_index);
    void on_wordLengthLineEdit_editingFinished();
    void on_browseLoadPathPushButton_clicked();
    void on_browseSavePathPushButton_clicked();
    void on_filePathLineEdit_editingFinished();
    void on_clearAllPushButton_clicked();
    void on_ngramFileLoadPushButton_clicked();

signals:
    void newPhraseChosen(const NgramPhraseMetaData& ngram_file_data);

friend class MainWindow;
friend class WordListGeneralControlsForm;
friend class WordTab;
friend class NgramPUWRationlizeForm;

private:
    Ui::ChoosePhrase *ui;
    /*!
     * \brief reset_schema_gridlayout
     * clear the current schemna grid layout of all widgets etc.
     */
    void reset_schema_gridlayout();
    /*!
     * \brief setInputPhrase
     *
     * general function to set class variables for phrase, seperate from gui
     * \param phrase
     * \param has_sent_start_in
     * \param has_sent_end_in
     * \return
     */
    bool setInputPhrase(const QString& phrase, bool has_sent_start_in, bool has_sent_end_in);
    /*!
     * \brief setPhraseSchemaGridView
     */
    void setPhraseSchemaGridView();
    void create_phrase_schema();
    void setRowDisabled(int row);
    bool isNgramSchemaGood();
    bool isNgramSchemaBad();
    //
    void canLookForFiles();
    // test the ngram scahema, is it good and do files exist
    void testNgramSchema();
    //void lookForFiles(const QList<QPair<int, QList<int>>>& ngram_schema_data );
    void lookForFilesAndSetGUI();
    /*!
     * \brief setNgramFilePaths, if the schema is good this create a data struct to pass to ngramDtaahub
     */
    void setNgramFileData();
    void setNgramFileData2();
    // gui function to set schmea-grid-gui column header spinboxes with defined word lengths
    void setWordLengthSpinboxes();
    // how many real words are in the phrase (not tags lige start, end)
    int phrase_word_count;
    // how many tokens words plus tags in the phrase
    int phrase_token_count;
    // how many ngrams in the phrase
    int phrase_ngram_count;
    // detailed data for a phrase
    void cleanPhraseDataStruct();
    NgramPhraseMetaData phrase_data_struct;
    // include S tag at start of phrase
    bool has_sentence_start;
    // include E tag at start of phrase
    bool has_sentence_end;
    //
    bool ngram_schema_good;
    //
    bool can_load_ngram_files;
    // max number of ngrams possible for a given schema (?)
    int max_ngram_count;
    // gui offset columns for phrase picker
    int phrase_col_offset;
    // gui offset rows for phrase picker
    int row_offset;
    // total number of cols in phrase picker
    int col_count;
    // total number of checkboxes in a row
    int num_checkboxes;
    // word lengths or tags that defien main tokens in phrase
    QList<int> phrase_tokens;
    // word lengths
    QList<int> word_lengths;
    // this grid layout is dynamically filled based on phrase token count
    QGridLayout* schemaGridLayout;
    // where ngrams are saved
    QDir ngram_root_dir;
    QDir phrase_save_root_dir;

};


#endif // CHOOSEPHRASE_H
