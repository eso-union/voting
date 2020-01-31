#include "votingPeople.h"

VotingPeople::VotingPeople(
    const Postgresql &db,
    int &indexVoting)
    : db_(db)
{
    addWidget(std::make_unique<Wt::WText>(
        "<h3>Enter the list of <em>email,name</em> of voters</h3>"));

    addWidget(std::make_unique<Wt::WText>(
        "<h4>You may delay this step util you have done all the testing.</h4>"));

    emailVoters_= addWidget(std::make_unique<Wt::WTextArea>());
    emailVoters_->setColumns(40);
    emailVoters_->setRows(10);
    emailVoters_->setPlaceholderText(
        "some@email.com,Some Name\n"
        "another@email.com,Another Name");

    emailVoters_->keyPressed().connect(
        [=] (const Wt::WKeyEvent& e)
        {
            std::cout << "key press ...\n";
            notify(CHANGED);
        });
}

void VotingPeople::save()
{
    std::cout << "[VotingPeople] saving ...\n";
    notify(READY);
};

