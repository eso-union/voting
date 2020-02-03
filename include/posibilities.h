#pragma once

// Wt
#include <Wt/WCheckBox.h>

// Voting
#include "common.h"
#include "panel.h"

/**
 *
 **/
class Posibilities: public Panel
{
    public:

        Posibilities(const Postgresql &db);

        virtual void save();

        virtual void setup(
            const int &type,
            const int &value);

    private:

        std::vector<Wt::WCheckBox*> cInput_;

        void add();

        void add(const bool &value);

        void remove();

        void removeAll();
};
