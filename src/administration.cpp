#include "administration.h"
#include "voteChoosing.h"
#include "minister.h"
#include "voterList.h"
#include "invitations.h"
#include "pending.h"
#include "closing.h"

Administration::Administration(const Postgresql &db): Panel(db)
{
    addStyleClass("container");

    /**
        * Row F
        **/

    /*
    auto rowF = addWidget(std::make_unique<Wt::WContainerWidget>());
    rowF->addStyleClass("row");

    auto cellF0 = rowF->addWidget(std::make_unique<Wt::WContainerWidget>());
    cellF0->addStyleClass("col-md-4");
    cellF0->addWidget(std::make_unique<MinisterAuth>());

    auto cellF1 = rowF->addWidget(std::make_unique<Wt::WContainerWidget>());
    cellF1->addStyleClass("col-md-4");
    cellF1->addWidget(std::make_unique<MinisterAuth>());

    auto cellF2 = rowF->addWidget(std::make_unique<Wt::WContainerWidget>());
    cellF2->addStyleClass("col-md-4");
    cellF2->addWidget(std::make_unique<MinisterAuth>());

    // rowF->setDisabled(true);   ??????????????????
    rowF->setHidden(true);
    */

    /**
        * Row A
        **/
    auto rowA = addWidget(std::make_unique<Wt::WContainerWidget>());
    rowA->addStyleClass("row");

    auto cellA0 = rowA->addWidget(std::make_unique<Wt::WContainerWidget>());
    cellA0->addStyleClass("col-md-8");

    wStack_= cellA0->addNew<Wt::WStackedWidget>();

    auto vSel= std::make_unique<VoteChoosing>(db_);
    vSel->notify.connect(boost::bind(&Administration::react, this, _1, _2));
    // vSel->tellSelected.connect(boost::bind(&Administration::useSelected, this, _1));
    wStack_->addWidget(std::move(vSel));

    auto mPassw= std::make_unique<Minister>(db_);
    mPassw->notify.connect(boost::bind(&Administration::react, this, _1, _2));
    wStack_->addWidget(std::move(mPassw));

    auto vList= std::make_unique<VoterList>(db_);
    vList->notify.connect(boost::bind(&Administration::react, this, _1, _2));
    wStack_->addWidget(std::move(vList));

    auto sInv= std::make_unique<Invitations>(db_);
    sInv->notify.connect(boost::bind(&Administration::react, this, _1, _2));
    wStack_->addWidget(std::move(sInv));

    auto lPend= std::make_unique<Pending>(db_);
    lPend->notify.connect(boost::bind(&Administration::react, this, _1, _2));
    wStack_->addWidget(std::move(lPend));

    auto tVot= std::make_unique<Closing>(db_);
    tVot->notify.connect(boost::bind(&Administration::react, this, _1, _2));
    wStack_->addWidget(std::move(tVot));

    wStack_->setCurrentIndex(0);

    auto cellA1 = rowA->addWidget(std::make_unique<Wt::WContainerWidget>());
    cellA1->addStyleClass("col-md-4");

    addInfoBoxes(cellA1);

    /**
     * Row B
     **/
    auto rowB = addWidget(std::make_unique<Wt::WContainerWidget>());
    rowB->addStyleClass("row");

    auto cellB0 = rowB->addWidget(std::make_unique<Wt::WContainerWidget>());
    cellB0->addStyleClass("col-md-4");
    wPrev_= cellB0->addWidget(std::make_unique<Wt::WPushButton>("Previous"));
    wPrev_->addStyleClass("btn btn-primary btn-lg btn-block");
    wPrev_->clicked().connect(this, &Administration::previous);

    auto cellB1 = rowB->addWidget(std::make_unique<Wt::WContainerWidget>());
    cellB1->addStyleClass("col-md-4");
    wNext_= cellB1->addWidget(std::make_unique<Wt::WPushButton>("Next"));
    wNext_->addStyleClass("btn btn-primary btn-lg btn-block");
    wNext_->clicked().connect(this, &Administration::next);

    auto cellB2 = rowB->addWidget(std::make_unique<Wt::WContainerWidget>());
    cellB2->addStyleClass("col-md-4");
    bLogout= cellB2->addWidget(std::make_unique<Wt::WPushButton>("Close"));
    bLogout->addStyleClass("btn btn-default btn-lg btn-block");
    bLogout->clicked().connect(
        [=]()
        {
            notify(LOGOUT, EMPTY);
        });

    widgetSettings();

    Wt::log("info") << "**************** Selected: " << idxVoting_;
}

// Propagate the notification to all panels in the stack.
/*
void Administration::useSelected(int value)
{
    idxVoting_= value;

    Wt::log("info") << "**************** idxVoting_ in use: " << idxVoting_;

    for(int i=0; i<wStack_->count(); i++)
    {
        Wt::WWidget *p= wStack_->widget(i);
        dynamic_cast<Panel*>(p)->setup(SELECTED, idxVoting_);
    }
    next();
}
*/

void Administration::previous()
{
    int current= wStack_->currentIndex();
    if(current > 0)
    {
        current--;
        wStack_->setCurrentIndex(current);

        Wt::WWidget *p= wStack_->widget(current);
        dynamic_cast<Panel*>(p)->updateInterface();
    }
    widgetSettings();
}

void Administration::next()
{
    int current= wStack_->currentIndex();
    if(current < (wStack_->count()-1))
    {
        current++;
        wStack_->setCurrentIndex(current);

        Wt::WWidget *p= wStack_->widget(current);
        dynamic_cast<Panel*>(p)->updateInterface();
    }
    widgetSettings();
}

void Administration::widgetSettings()
{
    int current= wStack_->currentIndex();

    if(current == 0)
    {
        wPrev_->disable();
    }

    if(current == (wStack_->count()-1))
    {
        wNext_->disable();
    }

    if(
        (current != 0) &&
        (current != (wStack_->count()-1)))
    {
        wPrev_->enable();
        wNext_->enable();
    }

    // There is no need to ask for this information
    // to the widget, this could be queried from the DB.
    // Anyway, this is possible only after all the widgets
    // in the stack has been created.
    // This is relevant to set the button NEXT.
    Wt::WWidget *p= wStack_->widget(0);
    int idx= dynamic_cast<Panel*>(p)->getActive();
    if(idx == -1)
    {
        // Is not possible to go forward without
        // an active voting.
        wNext_->disable();
    }
    else
    {
        react(SELECTED, idx);
        wNext_->enable();
    }
}

void Administration::react(
    const int &key,
    const int &value)
{
    Wt::log("info")
        << "key: " << key << ", value: " << value;

    switch(key)
    {
        case SELECTED:
        {
            idxVoting_= value;
            for(int i=0; i<wStack_->count(); i++)
            {
                Wt::log("info") << "== i: " << i;
                Wt::WWidget *p= wStack_->widget(i);
                dynamic_cast<Panel*>(p)->setup(SELECTED, idxVoting_);
            }
            if(wStack_->currentIndex())
            {
                wNext_->enable();
            }
            break;
        }
        case COMPLETED:
        {
            statusDisplay_[value]->setCompleted();
            break;
        }
        case INCOMPLETE:
        {
            statusDisplay_[value]->setIncomplete();
            break;
        }
        case NEXT:
        {
            next();
            break;
        }
        default:
        {
            Wt::log("info")
                << "UNHANDLED key:"
                << key
                << " value:"
                << value;
        }
    }
}

void Administration::addInfoBoxes(
    Wt::WContainerWidget *place)
{
    for(int i= 0; i<wStack_->count(); i++)
    {
        Wt::WString text= "<strong>Step {1}</strong> {2}";

        Wt::WWidget *w= wStack_->widget(i);
        Panel *p= dynamic_cast<Panel*>(w);
        p->setId(i);
        std::string description= p->description();

        text.arg(i+1)
            .arg(description);

        auto msgBox=
            place->addWidget(
                std::make_unique<StepInfo>(
                    false,
                    text.toUTF8()));

        statusDisplay_.push_back(msgBox);
    }
}
