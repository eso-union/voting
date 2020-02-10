#pragma once

// Wt
#include <Wt/WPushButton.h>
#include <Wt/WTextArea.h>

// Voting
#include "common.h"
#include "panel.h"

class Closing: public Panel
{
    public:

        Closing(const Postgresql &db);

        virtual void setup(
            const int &type,
            const int &value);

        void showOnly();

    private:

        Wt::WPushButton *wTerminate_;
        Wt::WTextArea *wOutput_;
        Wt::WText            *wTitle_  = nullptr;

        void terminate();

        void generateResult();

        void setVotingClosed();
};
