#pragma once

// Wt
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WRadioButton.h>
#include <Wt/WStackedWidget.h>

// Voting
#include "common.h"
#include "panel.h"
#include "stepInfo.h"

class Configuration: public Panel
{
    public:

        Configuration(const Postgresql &db);

    private:

        std::vector<StepInfo*> statusDisplay_;
        Wt::WStackedWidget *wStack_= nullptr;
        Wt::WPushButton *bPrev = nullptr;
        Wt::WPushButton *bSave = nullptr;
        Wt::WPushButton *bDiscard = nullptr;
        Wt::WPushButton *bNext = nullptr;
        Wt::WPushButton *bLogout = nullptr;

        // Propagate the notification to all panels in the stack.
        // void useSelected(int value);

        void previous();

        void next();

        void save();

        void discard();

        void react(
            const int &key,
            const int &value);

        void addInfoBoxes(
            Wt::WContainerWidget *place);
};
