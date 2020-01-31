#pragma once

// C++
#include <vector>
#include <string>

// Boost
#include <boost/signals2.hpp>
#include <boost/random/random_device.hpp>
#include <boost/random/uniform_int_distribution.hpp>

// Wt
#include <Wt/WContainerWidget.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WRadioButton.h>

// Voting
#include "common.h"
#include "panel.h"
#include "nameEmail.h"

/**
 *
 **/
class People: public Panel
{
    public:

        People(const Postgresql &db);

        virtual void setup(
            const int &type,
            const int &value);

        virtual void save();

        void dataChanged(int value);

    private:

        std::vector<NameEmail*> peopleList_;

        void add();

        void add(
            const std::string &name,
            const std::string &email);

        void remove();

        std::vector<std::string>
            generateCodes(const unsigned int &size);
};
