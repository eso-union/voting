#pragma once

// Wt
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WRadioButton.h>
#include <Wt/WStackedWidget.h>

// Voting
#include "common.h"
#include "panel.h"
#include "sendingVote.h"

class CastVote: public Panel
{
    public:

        CastVote(const Postgresql &db);

    private:

        int idPerson_= -1;
        std::string code_;
        Wt::WLineEdit *wCode_= nullptr;
        Wt::WStackedWidget *wStack_= nullptr;
        Wt::WPushButton *wNext_= nullptr;
        SendingVote *vi_= nullptr;

        void next();

        void react(const int &value);

        void setCode(
            int idPerson,
            std::string code);

        int getActiveVoting();
};
