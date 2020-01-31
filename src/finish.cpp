// Wt
#include <Wt/WText.h>

// Voting
#include "finish.h"

Finish::Finish(const Postgresql &db): Panel(db)
{
    addWidget(
        std::make_unique<Wt::WText>(
            "<h3>Your vote was received.</h3>"));
}

std::string Finish::description()
{
    std::string text=
        "Terminate voting";
    return text;
}
