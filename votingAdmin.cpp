/**********************************************************************
 * Voting System
 *
 * Copyright (C) 2019, Nicolas Slusarenko
 *                     nicolas@thecppdev.com
 *
 * This file is part of Voting System.
 *
 * The Voting System is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The Voting System is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the Voting System. If not, see <http://www.gnu.org/licenses/>.
 **********************************************************************/

/**********************************************************************
 * Compilation
 * $ g++ votingAdmin.cpp -o ./voting_admin -lwthttp -lwt
 *
 * Execution
 * $ ./voting_admin --docroot . --http-address 0.0.0.0 --http-port 9090
 **********************************************************************/

// C++
#include <vector>
#include <memory>
#include <thread>
#include <chrono>

// Wt
#include <Wt/WApplication.h>
#include <Wt/WBootstrapTheme.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WFormWidget.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WStackedWidget.h>
#include <Wt/WTextArea.h>
#include <Wt/WTemplate.h>
#include <Wt/WCalendar.h>
#include <Wt/WDate.h>
#include <Wt/WDateEdit.h>
#include <Wt/WTimeEdit.h>
#include <Wt/WCheckBox.h>
#include <Wt/WSpinBox.h>
#include <Wt/WRadioButton.h>

#include <Wt/WBreak.h>
#include <Wt/WDialog.h>
#include <Wt/WEnvironment.h>
#include <Wt/WLabel.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WRegExpValidator.h>

// Boost
#include <boost/signals2.hpp>
#include <boost/algorithm/string.hpp>

// Voting
#include "postgresql.h"

namespace {

#include <stdio.h>



}

// Boost
#include <boost/signals2.hpp>

// Voting
#include "postgresql.h"



#include <Wt/WTable.h>
#include <Wt/WTableCell.h>

// This renders correctly inside a row


/*
class MinisterAuth: public WidgetPanel
{
    public:

        MinisterAuth()
        {
            addStyleClass("panel panel-primary");

            auto heading = addWidget(std::make_unique<Wt::WContainerWidget>());
            heading->addStyleClass("panel-heading");
            // auto title = heading->addWidget(std::make_unique<Wt::WText>("<h3>First Minister</h3>"));
            auto title = heading->addWidget(std::make_unique<Wt::WText>("First Minister"));
            title->setStyleClass("panel-title");

            auto body = addWidget(std::make_unique<Wt::WContainerWidget>());
            body->addStyleClass("panel-body");

            // body->addWidget(std::make_unique<Wt::WText>("Minister Name"));
            // body->addWidget(std::make_unique<Wt::WLineEdit>());

            body->addWidget(std::make_unique<Wt::WText>("Password"));
            auto pass0 = body->addWidget(std::make_unique<Wt::WLineEdit>());
            pass0->setEchoMode(Wt::EchoMode::Password);

            body->addWidget(std::make_unique<Wt::WText>("Re-enter password"));
            auto pass1 = body->addWidget(std::make_unique<Wt::WLineEdit>());
            pass1->setEchoMode(Wt::EchoMode::Password);
        }

    private:

};
*/


/**
 *
 **/
// class TerminateVoting : public Wt::WContainerWidget

/**
 *
 **/
// class ListOfPending : public Wt::WContainerWidget

#include <Wt/WTable.h>
#include <Wt/WTableCell.h>
#include <Wt/WFont.h>

/// C++
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>

/**
 *
 **/
// class SendInvitations : public Wt::WContainerWidget

/**
 *
 **/
// class VotingList : public Wt::WContainerWidget



/**
 * The web widget.
 **/

/**
 * The web application.
 **/


/*
int main(int argc, char **argv)
{
    return Wt::WRun(
        argc,
        argv,
        [](const Wt::WEnvironment &env)
        {
            return std::make_unique<AdminApp>(env);
        }
    );
}
*/
