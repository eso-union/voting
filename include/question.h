#pragma once

// Wt
#include <Wt/WCheckBox.h>
#include <Wt/WLineEdit.h>
#include <Wt/WTextArea.h>

// Voting
#include "common.h"
#include "panel.h"

/**
 * Question and convocatory for the voters.
 **/
class Question: public Panel
{
    public:

        Question(const Postgresql &db);

        virtual void save();

        virtual void setup(
            const int &type,
            const int &value);

        virtual void setData();

    private:

        Wt::WTextArea *wConvocat_= nullptr;
        Wt::WCheckBox *wHtml_    = nullptr;
        Wt::WLineEdit *wSubject_ = nullptr;

        void keyWentDown(const Wt::WKeyEvent& e);
};
