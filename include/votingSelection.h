#pragma once

// Wt
#include <Wt/WContainerWidget.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WRadioButton.h>
#include <Wt/WSelectionBox.h>
#include <Wt/WStringListModel.h>
#include <Wt/WTable.h>

// Voting
#include "common.h"
#include "panel.h"

/**
 *
 **/
class VotingSelection: public Panel
{
    public:

        VotingSelection(const Postgresql &db);

        virtual void updateInterface();

    private:

        Wt::WTable      *wTable_   = nullptr;
        Wt::WLineEdit   *wNewName_ = nullptr;
        Wt::WPushButton *wUseSel_  = nullptr;

        void create();

        std::string strTesting(const bool &value);

        std::string strPhase(const int &value);

        void showSummary();

        void reopen(const int &index);
};
