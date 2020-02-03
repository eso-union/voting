// Wt
#include <Wt/WPushButton.h>

// Voting
#include "posibilities.h"

Posibilities::Posibilities(const Postgresql &db): Panel(db)
{
    settingType_= TYPE_CONFIG;
    step_= STEP_4;
    description_= "Setting up voting posibilities";

    setTitle();

    auto wRemove= wCanvas_->addWidget(
        std::make_unique<Wt::WPushButton>("Remove Possibility"));
    wRemove->addStyleClass("btn btn-warning");
    wRemove->clicked().connect(this, &Posibilities::remove);

    auto wAdd= wCanvas_->addWidget(
        std::make_unique<Wt::WPushButton>("Add Possibility"));
    wAdd->addStyleClass("btn btn-primary");
    wAdd->clicked().connect(this, &Posibilities::add);
}

void Posibilities::save()
{
    std::vector<std::string> bundle;

    Wt::WString delSentence=
        "DELETE FROM option "
        "WHERE idx_general={1}";

    delSentence.arg(idxVoting_);

    bundle.push_back(delSentence.toUTF8());

    for(int i=0; i<cInput_.size(); i++)
    {
        Wt::WString insSentence=
            "INSERT INTO option(idx_general, quantity, allowed) "
            "VALUES({1}, {2}, {3})";

        std::string value= "false";
        if(cInput_[i]->isChecked())
        {
            value= "true";
        }

        insSentence
            .arg(idxVoting_)
            .arg(i)
            .arg(value);

        bundle.push_back(insSentence.toUTF8());
    }

    auto status= db_.execSql(bundle);
    if(status != NO_ERROR)
    {
        wOut_->setText(status);
        return;
    }

    setSaved();
    setCompleted();
}

void Posibilities::setup(
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
}

void Posibilities::setData()
{
    removeAll();

    Wt::WString sentence=
        "SELECT quantity, allowed "
        "FROM option "
        "WHERE idx_general={1} "
        "ORDER BY quantity ASC";

    sentence.arg(idxVoting_);

    pqxx::result answer;
    auto status= db_.execSql(sentence.toUTF8(), answer);
    if(status == NO_ERROR)
    {
        for(auto row: answer)
        {
            bool allowed= row[1].as<bool>();
            add(allowed);  // There is a potential of unsycn between
                            // the ordinal and the one represented
                            // by the checkbox.
        }
        setSaved();
        return;
    }
    wOut_->setText(status);
}

void Posibilities::add()
{
    add(false);
}

void Posibilities::add(const bool &value)
{
    const int total= cInput_.size();
    std::string text= std::to_string(total) + " of the alternatives";
    Wt::WCheckBox *cb= wCanvas_->addNew<Wt::WCheckBox>(text);
    cb->setInline(false);
    cb->setChecked(value);
    cb->changed().connect(
        [=]()
        {
            notify(CHANGED, EMPTY);
        });
    cInput_.push_back(cb);
    notify(CHANGED, EMPTY);
}

void Posibilities::remove()
{
    if(cInput_.size()>0)
    {
        wCanvas_->removeWidget(cInput_[cInput_.size()-1]);
        cInput_.pop_back();
        notify(CHANGED, EMPTY);
    }
}

void Posibilities::removeAll()
{
    while(cInput_.size() > 0)
    {
        remove();
    }
}
