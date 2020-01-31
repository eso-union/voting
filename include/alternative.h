#pragma once

// C++
#include <memory>

// Wt
#include <Wt/WContainerWidget.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WRadioButton.h>

// Voting
#include "common.h"
#include "panel.h"

/**
 *
 **/
class Alternative: public Panel
{
    public:

        Alternative(const Postgresql &db);

        virtual void save();

        virtual void setup(
            const int &type,
            const int &value);

    private:


        std::vector<Wt::WLineEdit*> input_;

        void add();

        void add(const std::string &text);

        void remove();
};