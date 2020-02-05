#pragma once

// Wt
#include <Wt/WTextArea.h>

// Voting
#include "common.h"
#include "panel.h"

class Pending: public Panel
{
    public:

        Pending(const Postgresql &db);

        virtual void setup(
            const int &type,
            const int &value);

    private:

        Wt::WTextArea *wOutput_ = nullptr;

        void pending();

        void showPending();

        void resend();
};
