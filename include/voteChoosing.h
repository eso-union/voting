#pragma once

// Wt
#include <Wt/WPushButton.h>
#include <Wt/WTextArea.h>

// Voting
#include "common.h"
#include "panel.h"

class VoteChoosing: public Panel
{
    public:

        VoteChoosing(const Postgresql &db);

        virtual std::string description();

    private:

        Wt::WLineEdit   *newName_= nullptr;
        Wt::WPushButton *useidxVoting_= nullptr;
};
