/// Boost
#include <boost/bind.hpp>

// Wt
#include <Wt/WApplication.h>
#include <Wt/WBootstrapTheme.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WFormWidget.h>

// Voting
#include "postgresql.h"
#include "switcher.h"
#include "castVote.h"

/**
 * The web application.
 **/
class BasicApp : public Wt::WApplication
{
    public:

        // BasicApp(
        //    const Wt::WEnvironment& env,
        //    std::shared_ptr<Postgresql> database): WApplication(env), db(database)

        BasicApp(
            const Wt::WEnvironment& env,
            const Postgresql &db)
            : WApplication(env), db_(db)
        {
            setTitle("Voting System: configuration of the voting");

            if(appRoot().empty())
            {
                Wt::log("info") << "the approot looks suspect, is empty!";
            }

            auto bootstrapTheme= std::make_shared<Wt::WBootstrapTheme>();
            bootstrapTheme->setVersion(Wt::BootstrapVersion::v3);
            bootstrapTheme->setResponsive(true);
            setTheme(bootstrapTheme);

            useStyleSheet("resources/themes/bootstrap/3/bootstrap-theme.min.css");
            useStyleSheet("https://getbootstrap.com/docs/3.4/dist/css/bootstrap.min.css");
            // useStyleSheet("https://getbootstrap.com/docs/3.4/examples/starter-template/starter-template.css");
            useStyleSheet("https://getbootstrap.com/docs/3.4/examples/jumbotron/jumbotron.css");
            // useStyleSheet("https://getbootstrap.com/docs/3.4/examples/grid/grid.css");

            // useStyleSheet("https://getbootstrap.com/docs/3.4/examples/cover/cover.css");

            root()->addNew<CastVote>(db_);
        }

    private:

        Postgresql db_;

};

class AppGenerator
{
    public:
        AppGenerator(const Postgresql &db): db_(db)
        {}

        std::unique_ptr<BasicApp> createApp(const Wt::WEnvironment& env)
        {
            return std::make_unique<BasicApp>(env, db_);
        }

    private:

        Postgresql db_;
};

/**
 * Main
 **/
int main(int argc, char **argv)
{
    Postgresql db("voting00");

    AppGenerator ag(db);

    return Wt::WRun(
        argc,
        argv,
        boost::bind(&AppGenerator::createApp, &ag, _1));
}
