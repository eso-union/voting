#pragma once

// Wt
#include <Wt/WPushButton.h>
#include <Wt/WTable.h>

// Voting
#include "common.h"
#include "panel.h"

class VoterList: public Panel
{
    public:

        VoterList(const Postgresql &db);

        virtual void setup(
            const int &type,
            const int &value);

    private:

        Wt::WTable      *wTable_  = nullptr;
        Wt::WPushButton *wApprove_= nullptr;

        void approve();
};
