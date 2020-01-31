#include "votingType.h"

VotingType::VotingType(
    const Postgresql &db)
    : Panel(db)
{
    settingType_= TYPE_CONFIG;
    step_= STEP_5;
    description_= "Setting the type of the voting";

    setTitle();

    // addWidget(std::make_unique<Wt::WText>(
    //    "<h3>Choose the type of voting</h3>"));

    group_ = std::make_shared<Wt::WButtonGroup>();

    testingButton_ = addNew<Wt::WRadioButton>("Testing Voting");
    testingButton_->setInline(false);
    group_->addButton(testingButton_, 0);

    realButton_ = addNew<Wt::WRadioButton>("Real Voting");
    realButton_->setInline(false);
    group_->addButton(realButton_, 1);

    group_.get()->checkedChanged().connect(
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
        // Clean up all previous widgets.

        idxVoting_= value;

        // Create new widgets.
        std::string sentence=
            "SELECT type "
            "FROM general "
            "WHERE idx=" + std::to_string(idxVoting_);

        pqxx::result answer;
        db_.execSql(sentence, answer);
        auto row= answer.begin();
        const int type= row[0].as<int>();
        group_->setSelectedButtonIndex(type);
        votingType_= type;
    }

    if(isCompleted())
    {
        notify(COMPLETED, id_);
    }
}

void VotingType::save()
{
    std::cout << "[VotingType] saving ...\n";
    // int chosen= group_.get()->id(selection);
    int chosen= group_->checkedId();
    Wt::log("info") << "chosen: " << chosen << '\n';

    if(chosen == TESTING)
    {
        // Error handling, try-catch.
        std::string sentence=
            "UPDATE general "
            "SET type=" + std::to_string(TESTING) + " "
            "WHERE idx=" + std::to_string(idxVoting_);
        db_.execSql(std::move(sentence));

        std::cout << "sentence: " << sentence << '\n';
    }
    else if (chosen == REAL)
    {
        // There could be only one voting real.
        // Therefore, all other ones are changed to TESTING.
        // And this only one set to REAL.
        std::vector<std::string> bundle;

        std::string sentence=
            "UPDATE general "
            "SET type=" + std::to_string(TESTING) + " "
            "WHERE idx!=" + std::to_string(idxVoting_);
        bundle.push_back(std::move(sentence));

        sentence=
            "UPDATE general "
            "SET type=" + std::to_string(REAL) + " "
            "WHERE idx=" + std::to_string(idxVoting_);
        bundle.push_back(std::move(sentence));

        db_.execSql(bundle);
    }
    notify(SAVED, id_);
    notify(COMPLETED, id_);
    setCompleted();
};
