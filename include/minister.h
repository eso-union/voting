#pragma once

// Wt
#include <Wt/WPushButton.h>
#include <Wt/WTextArea.h>

// Voting
#include "common.h"
#include "panel.h"
#include "passwCreation.h"

class Minister: public Panel
{
    public:

        Minister(const Postgresql &db);

        virtual void setup(
            const int &type,
            const int &value);

        void react(
            const int &key,
            const int &value);

    private:

        int qtyMinisters_= 4;
        int qtyReady_= 0;
        std::vector<PasswCreation*> collection_;
};
