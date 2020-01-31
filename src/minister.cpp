// Voting
#include "minister.h"

Minister::Minister(const Postgresql &db): Panel(db)
{
    settingType_= TYPE_ADMIN;
    step_= STEP_1;
    description_= "Creation of Minister's passwords";

    Wt::WString title= "<h3>{1}</h3>";
    title.arg(description_);
    addWidget(std::make_unique<Wt::WText>(title));

    // How many?
    for(int id= 0; id<qtyMinisters_; id++)
    {
        auto mp= std::make_unique<PasswCreation>(db_, id);
        mp->notify.connect(boost::bind(&Minister::react, this, _1, _2));
        collection_.push_back(addWidget(std::move(mp)));
    }
}

void Minister::setup(
    const int &type,
    const int &value)
{
    Wt::log("info") << "A1 -- id_: " << id_;

    if(type == SELECTED)
    {
        if(idxVoting_ != value)
        {
            idxVoting_= value;

            if(isCompleted())
            {
                // This step is completed, is not active.
                notify(COMPLETED, id_);

                for(auto &e: collection_)
                {
                    e->disable();
                    e->setup(SELECTED, idxVoting_);
                }
            }
            else
            {
                // This step is not completed, is active.
                notify(INCOMPLETE, id_);

                for(auto &e: collection_)
                {
                    e->enable();
                    e->setup(SELECTED, idxVoting_);
                }
            }
        }
    }
}

void Minister::react(
    const int &key,
    const int &value)
{
    Wt::log("info") << "A0 -- id_: " << id_;

    if(key == COMPLETED)
    {
        qtyReady_++;

        Wt::log("info") << "qtyReady_: " << qtyReady_;

        if(qtyReady_ == qtyMinisters_)
        {
            Wt::log("info") << "minsters are completed";
            notify(COMPLETED, id_);
            setCompleted();
        }
    }
    else if(key == INCOMPLETE)
    {
        notify(INCOMPLETE, id_);
    }
}
