// Wt
#include <Wt/WPushButton.h>

// Voting
#include "posibilities.h"

Posibilities::Posibilities(const Postgresql &db): Panel(db)
{
    settingType_= TYPE_CONFIG;
    step_= STEP_3;
    description_= "Setting up voting posibilities";

    setTitle();

    // addStyleClass("container");
    // addWidget(std::make_unique<Wt::WText>("<h3>Voting Possibilities</h3>"));

    auto wRemove= addWidget(std::make_unique<Wt::WPushButton>("Remove Possibility"));
    wRemove->addStyleClass("btn btn-warning");
    wRemove->clicked().connect(this, &Posibilities::remove);

    auto wAdd= addWidget(std::make_unique<Wt::WPushButton>("Add Possibility"));
    wAdd->addStyleClass("btn btn-primary");
    wAdd->clicked().connect(this, &Posibilities::add);
}

void Posibilities::save()
{
    std::cout << "[VotePosibilities] saving ...\n";

    std::vector<std::string> bundle;

    Wt::WString delSentence=
        "DELETE FROM option "
        "WHERE idx_general={1}";

    delSentence.arg(idxVoting_);

    bundle.push_back(delSentence.toUTF8());

    for(int i=0; i<input_.size(); i++)
    {
        Wt::WString insSentence=
            "INSERT INTO option(idx_general, quantity, allowed) "
            "VALUES({1}, {2}, {3})";

        std::string value= "false";
        if(input_[i]->isChecked())
        {
            value= "true";
        }

        insSentence
            .arg(idxVoting_)
            .arg(i)
            .arg(value);

        bundle.push_back(insSentence.toUTF8());
    }
    db_.execSql(bundle);
    notify(SAVED, id_);
    notify(COMPLETED, id_);
    setCompleted();
}

void Posibilities::setup(
    const int &type,
    const int &value)
{
    if(type == SELECTED)
    {
        idxVoting_= value;

        std::string sentence=
            "SELECT quantity, allowed "
            "FROM option "
            "WHERE idx_general=" + std::to_string(idxVoting_) + " "
            "ORDER BY quantity ASC";

        std::cout << "sentence:" << sentence << "\n";

        pqxx::result answer;
        db_.execSql(sentence, answer);
        for(auto row: answer)
        {
            bool allowed= row[1].as<bool>();
            // std::cout << "row[0]:" << row[0].as(std::string()) << "\n";
            add(allowed);  // There is a potential of unsycn between
                    // the quantity and the one represented by the checkbox.
        }
    }

    if(isCompleted())
    {
        notify(COMPLETED, id_);
    }
}

void Posibilities::add()
{
    add(false);
}

void Posibilities::add(const bool &value)
{
    const int total= input_.size();
    std::string text= std::to_string(total) + " of the alternatives";
    Wt::WCheckBox *cb= addNew<Wt::WCheckBox>(text);
    // cb->addStyleClass("form-control");
    cb->setInline(false);
    cb->setChecked(value);
    cb->changed().connect(
        [=]()
        {
            notify(CHANGED, EMPTY);
        });
    input_.push_back(cb);
    notify(CHANGED, EMPTY);
}

void Posibilities::remove()
{
    if(input_.size()>0)
    {
        removeWidget(input_[input_.size()-1]);
        input_.pop_back();
        notify(CHANGED, EMPTY);
    }
}
