// C++
#include <vector>
#include <memory>
#include <thread>
#include <chrono>
#include <ostream>

// Wt
#include <Wt/WApplication.h>
#include <Wt/WBootstrapTheme.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WFormWidget.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WStackedWidget.h>
#include <Wt/WTextArea.h>
#include <Wt/WTemplate.h>
#include <Wt/WCalendar.h>
#include <Wt/WDate.h>
#include <Wt/WDateEdit.h>
#include <Wt/WTimeEdit.h>
#include <Wt/WCheckBox.h>
#include <Wt/WSpinBox.h>
#include <Wt/WRadioButton.h>

// Boost
#include <boost/signals2.hpp>

// Voting
#include "postgresql.h"

const int READY        = 0;
const int CHANGED      = 1;
const int GRANT_ACCESS = 2;
const int LOGOUT       = 3;
const int SELECTED     = 4;

const int TESTING      = 0;
const int REAL         = 1;

template<typename T>
std::ostream& operator<<(std::ostream& ost, const std::vector<T>& pack)
{
    char comma[3]= {'\0', ' ', '\0'};
    ost.put('{');
    for(auto e: pack)
    {
        ost << comma << e;
        comma[0]= ',';
    }
    ost.put('}');
    return ost;
}

class WidgetPanel: public Wt::WContainerWidget
{
    public:

        virtual void save()
        {
            std::cout << "[parent] saving ...\n";
        };

        virtual void setup(
            const int &type,
            const int &value)
        {
            std::cout << "[parent] setting up ...\n";
        }

        boost::signals2::signal<void(int)> notify;
        // boost::signals2::signal<void(int, int)> setup;

    protected:

        int selected_= -1;
};


#include <Wt/WCheckBox.h>
#include <Wt/WMessageBox.h>



class VoteCastApp: public Wt::WContainerWidget
{
    public:

        VoteCastApp(const Postgresql &db): db_(db)
        {
            const int votingId = getActiveVoting();

            addStyleClass("container");

            /**
             * Row A
             **/
            auto rowA= addWidget(std::make_unique<Wt::WContainerWidget>());
            rowA->addStyleClass("row");

            auto cellA0= rowA->addWidget(std::make_unique<Wt::WContainerWidget>());
            cellA0->addStyleClass("col-md-4");

            auto cellA1= rowA->addWidget(std::make_unique<Wt::WContainerWidget>());
            cellA1->addStyleClass("col-md-8");

            wStack_ = cellA1->addNew<Wt::WStackedWidget>();

            auto cEnter= std::make_unique<CodeEnter>(db_, votingId);
            // cEnter->notify.connect(boost::bind(&VoteCastApp::react, this, _1));
            cEnter->tellCode.connect(boost::bind(&VoteCastApp::setCode, this, _1, _2));
            // cEnter->setActiveVotingId(votingId);
            wStack_->addWidget(std::move(cEnter));

            auto vIssue= std::make_unique<VoteIssue>(db_, votingId);
            vi_= vIssue.get();
            vIssue->notify.connect(boost::bind(&VoteCastApp::react, this, _1));
            // vIssue->setActiveVotingId(votingId);
            wStack_->addWidget(std::move(vIssue));

            auto cClose= std::make_unique<CastClose>(db_);
            cClose->notify.connect(boost::bind(&VoteCastApp::react, this, _1));
            wStack_->addWidget(std::move(cClose));
        }

        boost::signals2::signal<void(int)> notify;

    private:

        Postgresql db_;
        // int idVoting= 1;
        // int activeVoting_= -1;
        int idPerson_= -1;
        std::string code_;
        Wt::WLineEdit *wCode_= nullptr;
        Wt::WStackedWidget *wStack_= nullptr;
        Wt::WPushButton *wNext_= nullptr;
        VoteIssue *vi_= nullptr;

        void next()
        {
            int current= wStack_->currentIndex();
            if(current < 2)
            {
                current++;
                wStack_->setCurrentIndex(current);
            }
        }

        void react(const int &value)
        {
            if(value == READY)
            {
                next();
            }
        }

        void setCode(
            int idPerson,
            std::string code)
        {
            // idPerson_= idx;
            // code_= code;
            if(vi_ != nullptr)
            {
                vi_->setVotingData(idPerson, code);
            }
            next();
        }

/*
        void setVoting(int value)
        {
            for(int i=0; i<wStack_->count(); i++)
            {
                Wt::WWidget *p= wStack_->widget(i);
                dynamic_cast<WidgetPanel*>(p)->setup(SELECTED, value);
            }
        }
*/

        int getActiveVoting()
        {
            Wt::WString sentence=
                "SELECT idx FROM general WHERE active=true";

            // try-catch
            pqxx::result answer;
            db_.execSql(sentence.toUTF8(), answer);
            auto row= answer.begin();
            if(row != answer.end())
            {
                const int value= row[0].as<int>();
                return value;
            }

            // No one active, report this problem.
            return -1;
        }

};


