// Wt
#include <Wt/WLabel.h>
#include <Wt/WTableCell.h>

// Voting
#include "passwCreation.h"

PasswCreation::PasswCreation(
    const Postgresql &db,
    const int &id)
    : Panel(db), ministerId_(id)
{
    std::string text= "Password for Minister " + std::to_string(ministerId_ + 1);
    addWidget(std::make_unique<Wt::WLabel>(text));
    wTable_= addWidget(Wt::cpp14::make_unique<Wt::WTable>());
    wTable_->setWidth(Wt::WLength("100%"));
    wTable_->addStyleClass("table table-striped");

    wPass0_= wTable_->elementAt(0, 0)->addNew<Wt::WLineEdit>();
    wPass0_->setEchoMode(Wt::EchoMode::Password);
    wTable_->elementAt(0, 0)->setContentAlignment(Wt::AlignmentFlag::Middle);
    wTable_->elementAt(0, 0)->setPadding(5);

    wPass1_= wTable_->elementAt(0, 1)->addNew<Wt::WLineEdit>();
    wPass1_->setEchoMode(Wt::EchoMode::Password);
    wTable_->elementAt(0, 1)->setContentAlignment(Wt::AlignmentFlag::Middle);
    wTable_->elementAt(0, 1)->setPadding(5);

    wCreate_= wTable_->elementAt(0, 2)->addNew<Wt::WPushButton>("Create");
    wCreate_->setStyleClass("btn btn-success btn-block");
    wCreate_->clicked().connect(this, &PasswCreation::create);
    wTable_->elementAt(0, 2)->setContentAlignment(Wt::AlignmentFlag::Middle);
    wTable_->elementAt(0, 2)->setPadding(5);

    wStatus_= wTable_->elementAt(1, 0)->addNew<Wt::WText>("<em>Pending</em>");
    wTable_->elementAt(1, 0)->setColumnSpan(2);
    wTable_->elementAt(1, 0)->setContentAlignment(Wt::AlignmentFlag::Top);
}

void PasswCreation::setup(
    const int &type,
    const int &value)
{
    if(type == SELECTED)
    {
        idxVoting_= value;
    }

    if(checkExistence())
    {
        wTable_->disable();
        wPass0_->setText("------");
        wPass1_->setText("------");
        wStatus_->setText("Password exist");
    }
    else
    {
        wTable_->enable();
        wPass0_->setText("");
        wPass1_->setText("");
        wStatus_->setText("Pending");
    }
}

void PasswCreation::create()
{
    std::string pass0= wPass0_->text().toUTF8();
    std::string pass1= wPass1_->text().toUTF8();

    if(pass0 != pass1)
    {
        wStatus_->setText("Fail: passwords are different.");
        return;
    }

    if(pass0.length() < 6)
    {
        wStatus_->setText("Fail: password must be at least 6 characters long.");
        return;
    }

    if(pass0.length() > 10)
    {
        wStatus_->setText("Fail: password must be at most 10 characters long.");
        return;
    }

    storePassw();
    notify(COMPLETED, ministerId_);
}

void PasswCreation::storePassw()
{
    std::string passw= wPass0_->text().toUTF8();

    Wt::WString sentence=
        "INSERT INTO minister(idx_general, id, password) "
        "VALUES({1}, {2}, '{3}');";

    sentence
        .arg(idxVoting_)
        .arg(ministerId_)
        .arg(passw);

    db_.execSql(sentence.toUTF8());

    wTable_->disable();
    wStatus_->setText("Success: password created.");
}

bool PasswCreation::checkExistence()
{
    Wt::WString sentence=
        "SELECT idx "
        "FROM minister "
        "WHERE idx_general={1} "
        "AND id={2};";

    sentence
        .arg(idxVoting_)
        .arg(ministerId_);

    Wt::log("info") << "sentence: " << sentence;

    pqxx::result answer= db_.query(sentence.toUTF8());
    if(answer.begin() == answer.end())
    {
        return false;
        notify(INCOMPLETE, ministerId_);
    }
    notify(COMPLETED, ministerId_);
    return true;
}
