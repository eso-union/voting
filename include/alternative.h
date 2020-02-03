#pragma once

// C++
#include <memory>

// Wt
#include <Wt/WContainerWidget.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WRadioButton.h>
#include <Wt/WTextArea.h>

// Voting
#include "common.h"
#include "panel.h"

class DescriptionTag;

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

        virtual void setData();

    private:

        Wt::WTextArea *wQuest_= nullptr;
        std::vector<DescriptionTag*> cInput_;

        void add();

        void add(
            const std::string &description,
            const std::string &tag);

        void remove();

        void removeAll();

        bool checkValues();

        void keyWentDown(const Wt::WKeyEvent& e);

        bool getAlternatives();
        bool getQuestion();
};