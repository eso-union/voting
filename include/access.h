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

        Wt::WPushButton *wGo_    = nullptr;
        Wt::WLineEdit   *wUser_  = nullptr;
        Wt::WLineEdit   *wPassw_ = nullptr;

        void verify();
        bool verifyUserPassw();
};
