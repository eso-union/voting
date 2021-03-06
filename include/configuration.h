#pragma once

// Wt
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WRadioButton.h>
#include <Wt/WStackedWidget.h>
#include <Wt/WText.h>

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
        Wt::WText          *wTitle_  = nullptr;
        Wt::WPushButton    *bPrev    = nullptr;
        Wt::WPushButton    *bSave    = nullptr;
        Wt::WPushButton    *bDiscard = nullptr;
        Wt::WPushButton    *bNext    = nullptr;
        Wt::WPushButton    *bLogout  = nullptr;
        Wt::WStackedWidget *wStack_  = nullptr;

        // Propagate the notification to all panels in the stack.
        // void useSelected(int value);

        template<typename T>
        void insertPanel()
        {
            auto p= std::make_unique<T>(db_);
            p->notify.connect(
                boost::bind(&Configuration::react, this, _1, _2));
            wStack_->addWidget(std::move(p));
        }

        void previous();

        void next();

        void save();

        void discard();

        void react(
            const int &key,
            const int &value);

        void addInfoBoxes(
            Wt::WContainerWidget *place);

        void setTitle(const int &idx);
};
