#pragma once

// C++
#include <memory>

// Wt
#include <Wt/WContainerWidget.h>
#include <Wt/WPushButton.h>
#include <Wt/WRadioButton.h>

// Voting
#include "common.h"
#include "panel.h"

class VotingType: public Panel
{
    public:

        VotingType(
            const Postgresql &db);

        virtual void setup(
            const int &type,
            const int &value);

        virtual void save();

    private:

        int votingType_= -1;
        std::shared_ptr<Wt::WButtonGroup> group_;
        Wt::WRadioButton *testingButton_= nullptr;
        Wt::WRadioButton *realButton_= nullptr;
};