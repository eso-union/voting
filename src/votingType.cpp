// C
#include <cassert>

// Voting
#include "votingType.h"

VotingType::VotingType(
    const Postgresql &db)
    : Panel(db)
{
    settingType_= TYPE_CONFIG;
    step_= STEP_6;
    description_= "Setting the type of the voting";

    setTitle();

    wGroup_= std::make_shared<Wt::WButtonGroup>();

    auto wTesting=
        wCanvas_->addNew<Wt::WRadioButton>("Testing Voting");
    wTesting->setInline(false);
    wGroup_->addButton(wTesting, 0);

    auto wReal=
        wCanvas_->addNew<Wt::WRadioButton>("Real Voting");
    wReal->setInline(false);
    wGroup_->addButton(wReal, 1);

    wGroup_.get()->checkedChanged().connect(
        [=] (Wt::WRadioButton *selection)
        {
            notify(CHANGED, EMPTY);
        });
}

void VotingType::setup(
    const int &type,
    const int &value)
{
    if(type == SELECTED)
    {
        idxVoting_= value;

        Wt::WString sentence=
            "SELECT testing "
            "FROM general "
            "WHERE idx={1}";

        sentence.arg(idxVoting_);

        pqxx::result answer= db_.query(sentence.toUTF8());
        if(answer.begin() != answer.end())
        {
            auto row= answer.begin();
            const bool testing= row[0].as<bool>();
            if(testing)
            {
                wGroup_->setSelectedButtonIndex(0);
            }
            else
            {
                wGroup_->setSelectedButtonIndex(1);
            }
        }
    }

    if(isCompleted())
    {
        notify(COMPLETED, id_);
    }
}

void VotingType::save()
{
    std::string value= "false";
    if(wGroup_->checkedId() == TESTING)
    {
        value= "true";
    }

    Wt::WString sentence=
        "UPDATE general "
        "SET testing={1} "
        "WHERE idx={2} ";

    sentence
        .arg(value)
        .arg(idxVoting_);

    auto status= db_.execSql(std::move(sentence.toUTF8()));
    if(status != NO_ERROR)
    {
        wOut_->setText(status);
        return;
    }

    setSaved();
    setCompleted();
}
