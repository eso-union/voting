#pragma once

// Boost
#include <boost/signals2.hpp>
#include <boost/algorithm/string.hpp>

// Wt
#include <Wt/WApplication.h>
#include <Wt/WBootstrapTheme.h>
#include <Wt/WContainerWidget.h>

// Voting
#include "postgresql.h"

class Panel: public Wt::WContainerWidget
{
    public:

        Panel(const Postgresql &db);

        Panel(
            const Postgresql &db,
            const int &idxVoting);

        virtual void setTitle();

        virtual void save();

        virtual void discard();

        virtual void setData();

        virtual void setup(
            const int &type,
            const int &value);

        virtual void setId(const int &value);

        virtual int id();

        virtual std::string description();

        virtual int getActive();

        virtual void updateInterface();

        boost::signals2::signal<void (int, int)> notify;

    protected:

        Postgresql db_;
        int id_= -1;
        int idxVoting_= -1;
        int settingType_= -1;
        int step_= -1;
        std::string description_= "(empty)";
        Wt::WContainerWidget *wCanvas_= nullptr;
        Wt::WText            *wOut_   = nullptr;
        Wt::WText            *wHelp_  = nullptr;

        void setCompleted();

        void setSaved();

        bool isCompleted();
};
