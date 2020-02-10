#pragma once

// Wt
#include <Wt/WPushButton.h>
#include <Wt/WTable.h>
#include <Wt/WTextArea.h>

// Voting
#include "common.h"
#include "panel.h"

class VoteChoosing: public Panel
{
    public:

        VoteChoosing(const Postgresql &db);

        // virtual std::string description();

        virtual void updateInterface();

    private:

        int stageAct_= -1;

        Wt::WTable      *wTable_      = nullptr;
        Wt::WLineEdit   *newName_     = nullptr;
        Wt::WPushButton *useidxVoting_= nullptr;

        std::string strTesting(const bool &value);

        std::string strActive(const bool &value);

        // std::string strClosed(const bool &value);

        void showSummary();

        int getActiveVoting();

        int getActiveStage();

        void showHelp();

        void showResults(const int &index);

        void activateVoting(const int &index);
};
