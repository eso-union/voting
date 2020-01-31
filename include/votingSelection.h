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

        virtual std::string description();

        // boost::signals2::signal<void(int)> tellSelected;

    private:

        // Postgresql db_;
        // int selected_= -1;
        Wt::WLineEdit   *newName_= nullptr;
        Wt::WPushButton *useSelected_= nullptr;

        void create();
};
