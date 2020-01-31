#pragma once

// Wt
#include <Wt/WTable.h>
#include <Wt/WText.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>

// Voting
#include "common.h"
#include "panel.h"

class PasswCreation: public Panel
{
    public:

        PasswCreation(
            const Postgresql &db,
            const int &id);

        virtual void setup(
            const int &type,
            const int &value);

    private:

        int ministerId_= -1;
        Wt::WText *wStatus_= nullptr;
        Wt::WLineEdit *wPass0_= nullptr;
        Wt::WLineEdit *wPass1_= nullptr;
        Wt::WPushButton *wCreate_= nullptr;
        Wt::WTable *wTable_= nullptr;

        void create();

        void storePassw();

        bool checkExistence();
};
