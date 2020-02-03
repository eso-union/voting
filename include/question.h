#pragma once

// Wt
#include <Wt/WTextArea.h>

// Voting
#include "common.h"
#include "panel.h"

/**
 * Question and convocatory for the voters.
 **/
class Question: public Panel
{
    public:

        Question(const Postgresql &db);

        virtual void save();

        virtual void setup(
            const int &type,
            const int &value);

    private:

        Wt::WTextArea *wConvocat_= nullptr;
        Wt::WTextArea *wQuestion_= nullptr;

        void keyWentDown(const Wt::WKeyEvent& e);
};
