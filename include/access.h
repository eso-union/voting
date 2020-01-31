#pragma once

// C++
#include <vector>
#include <memory>
#include <thread>
#include <chrono>

// Wt
#include <Wt/WContainerWidget.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WRadioButton.h>

// Voting
#include "common.h"
#include "panel.h"

class Access: public Panel
{
    public:

        Access(
            const Postgresql &db,
            const std::string &lookup);

    private:

        std::string lookup_;
        Wt::WPushButton *bGo= nullptr;
        Wt::WText *tResult= nullptr;
        Wt::WLineEdit *iPassword= nullptr;

        void verify();
};
