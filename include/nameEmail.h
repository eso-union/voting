#pragma once

// C++
#include <vector>
#include <string>

// Boost
#include <boost/signals2.hpp>

// Wt
#include <Wt/WApplication.h>
#include <Wt/WBootstrapTheme.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WFormWidget.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>

// Voting
#include "common.h"
#include "panel.h"
#include "postgresql.h"

/**
 *
 **/
class NameEmail: public Wt::WContainerWidget
{
    public:

        NameEmail(
            const std::string &name,
            const std::string &email);

        NameEmail(): NameEmail{"", ""}{};

        std::string getName();

        std::string getEmail();

        boost::signals2::signal<void(int)> notify;

    private:

        Wt::WLineEdit *inputName_= nullptr;
        Wt::WLineEdit *inputEmail_= nullptr;
};