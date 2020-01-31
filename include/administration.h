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

class Administration: public Panel
{
    public:

        Administration(const Postgresql &db);

    private:

        std::vector<StepInfo*> statusDisplay_;
        Wt::WStackedWidget *wStack_;
        Wt::WPushButton *wPrev_= nullptr;
        Wt::WPushButton *wNext_= nullptr;
        Wt::WPushButton *bLogout= nullptr;

        void useSelected(int value);

        void previous();

        void next();

        void widgetSettings();

        void react(
            const int &key,
            const int &value);

        void addInfoBoxes(
            Wt::WContainerWidget *place);
};
