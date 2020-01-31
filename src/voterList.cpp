// Wt
#include <Wt/WTable.h>
#include <Wt/WTextArea.h>

// Voting
#include "voterList.h"

VoterList::VoterList(const Postgresql &db): Panel(db)
{
    settingType_= TYPE_ADMIN;
    step_= STEP_2;
    description_= "Approving of voters list";

    setTitle();

    // addWidget(std::make_unique<Wt::WText>("<h3>Approve the list of of voters</h3>"));

    wApprove_ = addWidget(std::make_unique<Wt::WPushButton>("Approve"));
    wApprove_->clicked().connect(this, &VoterList::approve);

    auto table= addWidget(Wt::cpp14::make_unique<Wt::WTable>());
    table->setHeaderCount(1);
    table->setWidth(Wt::WLength("100%"));

    table->elementAt(0, 0)->addNew<Wt::WText>("Email");
    table->elementAt(0, 1)->addNew<Wt::WText>("Name");

    table->elementAt(1, 0)->addNew<Wt::WText>("some@email.com");
    table->elementAt(1, 1)->addNew<Wt::WText>("Some Name");

    table->elementAt(2, 0)->addNew<Wt::WText>("another@email.com");
    table->elementAt(2, 1)->addNew<Wt::WText>("Another Name");
}

void VoterList::setup(
    const int &type,
    const int &value)
{
    if(type == SELECTED)
    {
        idxVoting_= value;
        if(isCompleted())
        {
            wApprove_->disable();
            notify(COMPLETED, id_);
        }
        else
        {
            wApprove_->enable();
            notify(INCOMPLETE, step_);
        }
    }
}

void VoterList::approve()
{
    wApprove_->disable();
    notify(COMPLETED, id_);
    setCompleted();
}
