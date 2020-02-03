// C
#include <cassert>

// C++
#include <string>
#include <iostream>

// Wt
#include <Wt/WBreak.h>
#include <Wt/WText.h>

// Voting
#include "question.h"

Question::Question(const Postgresql &db): Panel(db)
{
    settingType_= TYPE_CONFIG;
    step_= STEP_1;
    description_= "Setting question and convocatory";

    setTitle();

    wConvocat_= wCanvas_->addWidget(std::make_unique<Wt::WTextArea>());
    wConvocat_->setColumns(40);
    wConvocat_->setRows(5);
    wConvocat_->keyWentDown().connect(this, &Question::keyWentDown);

    wCanvas_->addWidget(std::make_unique<Wt::WText>("Encabezado en voto:"));

    wQuestion_= wCanvas_->addWidget(std::make_unique<Wt::WTextArea>());
    wQuestion_->setColumns(40);
    wQuestion_->setRows(2);
    wQuestion_->keyWentDown().connect(this, &Question::keyWentDown);

    // auto wBreak= addWidget(std::make_unique<Wt::WBreak>());
    // wOut_= addWidget(std::make_unique<Wt::WText>());

    // Verifying pointers
    assert(wConvocat_ != nullptr);
    assert(wQuestion_ != nullptr);
}

void Question::save()
{
    try
    {
        std::string sentence=
            "UPDATE general "
            "SET convocatory='" + wConvocat_->text().toUTF8() + "', "
            "header='" + wQuestion_->text().toUTF8() + "' "
            "WHERE idx=" + std::to_string(idxVoting_);
        db_.execSql(sentence);

        setSaved();
        setCompleted();
    }
    catch(const std::exception& e)
    {
        Wt::log("error") << e.what();
        wOut_->setText(e.what());
    }
};

void Question::setup(
    const int &type,
    const int &value)
{
    if(type == SELECTED)
    {
        idxVoting_= value;

        std::string sentence=
            "SELECT convocatory, header "
            "FROM general "
            "WHERE idx=" + std::to_string(idxVoting_);

        pqxx::result answer;
        db_.execSql(sentence, answer);
        pqxx::result::const_iterator row= answer.begin();
        if(row != answer.end())
        {
            wConvocat_->setText(row[0].as(std::string()));
            wQuestion_->setText(row[1].as(std::string()));
        }
        setSaved();
    }

    if(isCompleted())
    {
        notify(COMPLETED, id_);
    }
}

void Question::keyWentDown(const Wt::WKeyEvent& e)
{
    notify(CHANGED, EMPTY);
    wOut_->setText("Modified");
}