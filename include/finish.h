#pragma once

// Voting
#include "common.h"
#include "panel.h"

class Finish: public Panel
{
    public:

        Finish(const Postgresql &db);

        virtual std::string description();

    private:

};
