#include "castVote.h"
#include "code.h"
#include "finish.h"

CastVote::CastVote(const Postgresql &db): Panel(db)
{
    idxVoting_= getActiveVoting();

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

    auto cEnter= std::make_unique<Code>(db_, idxVoting_);
    // cEnter->notify.connect(boost::bind(&CastVote::react, this, _1, _2));
    cEnter->tellCode.connect(boost::bind(&CastVote::setCode, this, _1, _2));
    // cEnter->setup(SELECTED, idxVoting_);
    wStack_->addWidget(std::move(cEnter));

    auto sVote= std::make_unique<SendingVote>(db_, idxVoting_);
    vi_= sVote.get();
    sVote->notify.connect(boost::bind(&CastVote::react, this, _1));
    // sVote->setup(SELECTED, idxVoting_);
    wStack_->addWidget(std::move(sVote));

    auto cClose= std::make_unique<Finish>(db_);
    cClose->notify.connect(boost::bind(&CastVote::react, this, _1));
    wStack_->addWidget(std::move(cClose));
}

boost::signals2::signal<void(int)> notify;

void CastVote::next()
{
    int current= wStack_->currentIndex();
    if(current < 2)
    {
        current++;
        wStack_->setCurrentIndex(current);
    }
}

void CastVote::react(const int &value)
{
    if(value == READY)
    {
        next();
    }
}

void CastVote::setCode(
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
void CastVote::setVoting(int value)
{
    for(int i=0; i<wStack_->count(); i++)
    {
        Wt::WWidget *p= wStack_->widget(i);
        dynamic_cast<WidgetPanel*>(p)->setup(SELECTED, value);
    }
}
*/

int CastVote::getActiveVoting()
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
