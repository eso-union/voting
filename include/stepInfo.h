#pragma once

// Wt
#include <Wt/WContainerWidget.h>
#include <Wt/WText.h>

/**
 *
 **/
class StepInfo: public Wt::WContainerWidget
{
    public:

        StepInfo(
            const bool &valid,
            const std::string &text);

        void setText(
            const bool &valid,
            const std::string &text);

        void setCompleted();

        void setIncomplete();

    private:

        Wt::WContainerWidget *msgBox_;
        Wt::WText *msgText_;
};
