#pragma once

// C++
#include <map>
#include <algorithm>

// Wt
#include <Wt/WCheckBox.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WRadioButton.h>
#include <Wt/WStackedWidget.h>

// Voting
#include "common.h"
#include "panel.h"

class SendingVote: public Panel
{
    public:
        SendingVote(
            const Postgresql &db,
            const int &idxVoting);

        void setVotingData(
            const int &idPerson,
            const std::string &code);

    private:

        // Postgresql db_;
        // int votingId_= -1;

        std::map<int, Wt::WCheckBox*> options_;
        int idxPeople_= -1;
        std::string code_;

        void process();

        std::string getQuestion();
};
