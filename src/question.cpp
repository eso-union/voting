// C++
#include <string>
#include <iostream>

// Wt
#include <Wt/WSpinBox.h>
#include <Wt/WText.h>

// Voting
#include "question.h"

Question::Question(const Postgresql &db): Panel(db)
{
    settingType_= TYPE_CONFIG;
    step_= STEP_1;
    description_= "Setting question and convocatory";

    setTitle();

    convocatory_ = addWidget(std::make_unique<Wt::WTextArea>());
    convocatory_->setColumns(40);
    convocatory_->setRows(5);
    convocatory_->keyPressed().connect(
        [=] (const Wt::WKeyEvent& e)
        {
            notify(CHANGED, EMPTY);
        });

    addWidget(std::make_unique<Wt::WText>("Encabezado en voto:"));

    question_ = addWidget(std::make_unique<Wt::WTextArea>());
    question_->setColumns(40);
    question_->setRows(2);
    question_->keyPressed().connect(
        [=] (const Wt::WKeyEvent& e)
        {
            notify(CHANGED, EMPTY);
        });

    /**
     * Number of minister of faith.
     **/
    auto rowC = addWidget(std::make_unique<Wt::WContainerWidget>());
    rowC->addStyleClass("row");

    auto cellC0 = rowC->addWidget(std::make_unique<Wt::WContainerWidget>());
    cellC0->addStyleClass("col-xs-6");
    cellC0->addWidget(std::make_unique<Wt::WText>("Number of minister of faith:"));

    auto cellC1 = rowC->addWidget(std::make_unique<Wt::WContainerWidget>());
    cellC1->addStyleClass("col-xs-6");
    Wt::WSpinBox *sb = cellC1->addWidget(std::make_unique<Wt::WSpinBox>());
    sb->setRange(0,4);
    sb->valueChanged().connect(
        [=] (const int&)
        {
            notify(CHANGED, EMPTY);
        });
}

void Question::save()
{
    std::cout << "[VoterQuestion] saving ...\n";

    // Error handling, try-catch.
    std::string sentence=
        "UPDATE general "
        "SET convocatory='" + convocatory_->text().toUTF8() + "', "
        "header='" + question_->text().toUTF8() + "' "
        "WHERE idx=" + std::to_string(idxVoting_);
    db_.execSql(sentence);
    notify(SAVED, id_);
    notify(COMPLETED, id_);
    setCompleted();
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
            convocatory_->setText(row[0].as(std::string()));
            question_->setText(row[1].as(std::string()));
        }
    }

    if(isCompleted())
    {
        notify(COMPLETED, id_);
    }
}
