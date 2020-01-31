#pragma once

// Wt
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WRadioButton.h>
#include <Wt/WStackedWidget.h>

// Voting
#include "common.h"
#include "panel.h"

class Code: public Panel
{
    public:
        Code(
            const Postgresql &db,
            const int &idxVoting);

        boost::signals2::signal<void (int, std::string)> tellCode;

    private:

        // Postgresql db_;
        // int votingId_= -1;
        Wt::WLineEdit* wCode_;
        Wt::WText* wMsg_;

        void verify();
};
