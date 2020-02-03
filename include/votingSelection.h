#pragma once

// Wt
#include <Wt/WContainerWidget.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WRadioButton.h>
#include <Wt/WSelectionBox.h>
#include <Wt/WStringListModel.h>

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

    private:

        Wt::WLineEdit   *wNewName_ = nullptr;
        Wt::WPushButton *wUseSel_  = nullptr;

        void create();
};
