#pragma once

// C++
#include <vector>
#include <string>

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

        virtual void setData();

    private:

        std::vector<NameEmail*> cPeopleList_;

        void add();

        void add(
            const std::string &name,
            const std::string &email);

        void remove();

        void removeAll();

        std::vector<std::string>
            generateCodes(const unsigned int &size);

        void fill(const std::string &filename);
        std::string getFileContents(const std::string &filename);
};
