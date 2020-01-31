#pragma once

// C++
#include <memory>

// Boost
#include <boost/signals2.hpp>

// Wt
#include <Wt/WContainerWidget.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WRadioButton.h>

// Voting
#include "common.h"
#include "panel.h"

class VotingPeople : public WidgetPanel
{
    public:

        VotingPeople(
            const Postgresql &db,
            int &indexVoting);

        virtual void save();

    private:

        Postgresql db_;
        Wt::WTextArea *emailVoters_;
};
