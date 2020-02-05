// C
#include <cassert>

// Wt
#include <Wt/WTextArea.h>

// Voting
#include "voterList.h"

VoterList::VoterList(const Postgresql &db): Panel(db)
{
    settingType_= TYPE_ADMIN;
    step_= STEP_2;
    description_= "Approving of voters list";

    setTitle();

    wApprove_ = wCanvas_->addWidget(std::make_unique<Wt::WPushButton>("Approve"));
    wApprove_->clicked().connect(this, &VoterList::approve);

    wTable_= wCanvas_->addWidget(Wt::cpp14::make_unique<Wt::WTable>());
    wTable_->setHeaderCount(1);
    wTable_->setWidth(Wt::WLength("100%"));

    wOut_->setText("Hello -- Voter list is already approved");

    // Verify
    assert(wTable_   != nullptr);
    assert(wApprove_ != nullptr);
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
            wOut_->setText("Voter list is already approved");
        }
        else
        {
            wApprove_->enable();
            notify(INCOMPLETE, step_);
            wOut_->setText("Voter list is pending approval");
        }

        // Clean up the table.
        wTable_->clear();
        wTable_->elementAt(0, 0)->addNew<Wt::WText>("Name");
        wTable_->elementAt(0, 1)->addNew<Wt::WText>("Email");

        Wt::WString sentence=
            "SELECT name, email "
            "FROM people "
            "WHERE idx_general={1} "
            "ORDER BY name ASC";

        sentence.arg(idxVoting_);

        pqxx::result answer;
        auto status= db_.execSql(sentence.toUTF8(), answer);
        if(status == NO_ERROR)
        {
            int i= 1;
            for(auto const &row: answer)
            {
                std::string name=  row[0].as<std::string>();
                std::string email= row[1].as<std::string>();

                wTable_->elementAt(i, 0)->addNew<Wt::WText>(name);
                wTable_->elementAt(i, 1)->addNew<Wt::WText>(email);

                i++;
            }
        }
        else
        {
            wOut_->setText(status);
        }
    }
}

void VoterList::approve()
{
    wApprove_->disable();
    notify(COMPLETED, id_);
    setCompleted();
}
