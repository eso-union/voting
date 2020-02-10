// C
#include <cassert>

// C++
#include <string>
#include <iostream>

// Wt
#include <Wt/WBreak.h>
#include <Wt/WLabel.h>
#include <Wt/WText.h>
#include <Wt/WPushButton.h>

// Voting
#include "question.h"

Question::Question(const Postgresql &db): Panel(db)
{
    settingType_= TYPE_CONFIG;
    step_= STEP_1;
    description_= "Setting email message";

    setTitle();

    auto wLabel= wCanvas_->addWidget(std::make_unique<Wt::WLabel>("Subject"));

    wSubject_= wCanvas_->addWidget(std::make_unique<Wt::WLineEdit>());
    wSubject_->setInline(false);
    wSubject_->keyWentDown().connect(this, &Question::keyWentDown);
    wLabel->setBuddy(wSubject_);

    wConvocat_= wCanvas_->addWidget(std::make_unique<Wt::WTextArea>());
    wConvocat_->setColumns(40);
    wConvocat_->setRows(20);
    wConvocat_->keyWentDown().connect(this, &Question::keyWentDown);

    auto wGetHtml= wCanvas_->addWidget(std::make_unique<Wt::WPushButton>("Replace with HTML Template"));
    wGetHtml->addStyleClass("btn btn-success");
    wGetHtml->clicked().connect(
        [=]()
        {
            Wt::WString basic= Wt::WString::tr("convocatory-template-html");
            wConvocat_->setText(basic);
            notify(CHANGED, EMPTY);
            wOut_->setText("Modified");
        });

    auto wGetPlain= wCanvas_->addWidget(std::make_unique<Wt::WPushButton>("Replace with Plain Template"));
    wGetPlain->addStyleClass("btn btn-info");
    wGetPlain->clicked().connect(
        [=]()
        {
            Wt::WString basic= Wt::WString::tr("convocatory-template-plain");
            wConvocat_->setText(basic);
            notify(CHANGED, EMPTY);
            wOut_->setText("Modified");
        });

    wHtml_= wCanvas_->addWidget(std::make_unique<Wt::WCheckBox>("HTML format"));
    wHtml_->setChecked(true);
    wHtml_->setInline(false);
    wHtml_->changed().connect(
        [=]()
        {
            notify(CHANGED, EMPTY);
            wOut_->setText("Modified");
        });

    // Verifying pointers
    assert(wConvocat_ != nullptr);
    assert(wHtml_     != nullptr);
    assert(wSubject_  != nullptr);
}

void Question::save()
{
    std::string html= "false";
    if(wHtml_->isChecked())
    {
        html= "true";
    }

    Wt::WString sentence=
        "UPDATE general "
        "SET convocatory='{1}', subject='{2}', html={3} "
        "WHERE idx={4}";

    sentence
        .arg(wConvocat_->text().toUTF8())
        .arg(wSubject_->text().toUTF8())
        .arg(html)
        .arg(idxVoting_);

    auto status= db_.execSql(sentence.toUTF8());
    if(status != NO_ERROR)
    {
        wOut_->setText(status);
        return;
    }

    setSaved();
    setCompleted();
};

void Question::setup(
    const int &type,
    const int &value)
{
    if(type == SELECTED)
    {
        if(idxVoting_ != value)
        {
            idxVoting_= value;
            setData();
        }
    }

    if(isCompleted())
    {
        notify(COMPLETED, id_);
    }
    else
    {
        notify(INCOMPLETE, id_);
    }
}

void Question::setData()
{
    Wt::WString sentence=
        "SELECT convocatory, subject, html "
        "FROM general "
        "WHERE idx={1}";

    sentence.arg(idxVoting_);

    pqxx::result answer;
    auto status= db_.execSql(sentence.toUTF8(), answer);
    if(status == NO_ERROR)
    {
        if(answer.begin() != answer.end())
        {
            auto row= answer.begin();
            wConvocat_->setText(row[0].as(std::string()));
            wSubject_->setText(row[1].as(std::string()));
            wHtml_->setChecked(row[2].as(bool()));
        }
        setSaved();
    }
    else
    {
        wOut_->setText(status);
    }
}

void Question::keyWentDown(const Wt::WKeyEvent& e)
{
    notify(CHANGED, EMPTY);
    wOut_->setText("Modified");
}