#pragma once

// Wt
#include <Wt/WPushButton.h>
#include <Wt/WTextArea.h>

// Voting
#include "common.h"
#include "panel.h"

class Invitations: public Panel
{
    public:

        Invitations(const Postgresql &db);

        virtual void setup(
            const int &type,
            const int &value);

        void send();
        void send_x();

        /*
        virtual void setup(
            const int &type,
            const int &value);
        */

    private:

        bool testing_= -1;
        Wt::WPushButton *wSend_  = nullptr;
        Wt::WTextArea   *wOutput_= nullptr;

        std::string getBodyTemplate();

        std::string getEmailSubject();

        /*
        void writeFile(
            const std::string &location,
            const std::string &content);
        */
};
