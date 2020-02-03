#include "configuration.h"
#include "votingSelection.h"
#include "question.h"
#include "alternative.h"
#include "posibilities.h"
#include "people.h"
#include "votingType.h"
#include "witness.h"

Configuration::Configuration(const Postgresql &db): Panel(db)
{
    addStyleClass("container");

    /**
     * Row A
     **/
    auto rowA = addWidget(std::make_unique<Wt::WContainerWidget>());
    rowA->addStyleClass("row");

    auto cellA0= rowA->addWidget(std::make_unique<Wt::WContainerWidget>());
    cellA0->addStyleClass("col-md-4");

    auto cellA1 = rowA->addWidget(std::make_unique<Wt::WContainerWidget>());
    cellA1->addStyleClass("col-md-8");

    wStack_= cellA1->addNew<Wt::WStackedWidget>();

    insertPanel<VotingSelection>();
    insertPanel<Question>();
    insertPanel<Witness>();
    insertPanel<Alternative>();
    insertPanel<Posibilities>();
    insertPanel<People>();
    insertPanel<VotingType>();

    /*
    auto vSel= std::make_unique<VotingSelection>(db_);
    vSel->notify.connect(boost::bind(&Configuration::react, this, _1, _2));
    wStack_->addWidget(std::move(vSel));

    auto vQuest= std::make_unique<Question>(db_);
    vQuest->notify.connect(boost::bind(&Configuration::react, this, _1, _2));
    wStack_->addWidget(std::move(vQuest));

    auto vAlt= std::make_unique<Alternative>(db_);
    vAlt->notify.connect(boost::bind(&Configuration::react, this, _1, _2));
    wStack_->addWidget(std::move(vAlt));

    auto vPos= std::make_unique<Posibilities>(db_);
    vPos->notify.connect(boost::bind(&Configuration::react, this, _1, _2));
    wStack_->addWidget(std::move(vPos));

    auto vTest= std::make_unique<People>(db_);
    vTest->notify.connect(boost::bind(&Configuration::react, this, _1, _2));
    wStack_->addWidget(std::move(vTest));
    */

    /*
    auto vPeople= std::make_unique<VotingPeople>(db_);
    vPeople->notify.connect(boost::bind(&Configuration::dataChanged, this, _1));
    wStack_->addWidget(std::move(vPeople));
    */

    /*
    auto vType= std::make_unique<VotingType>(db_);
    vType->notify.connect(boost::bind(&Configuration::react, this, _1, _2));
    wStack_->addWidget(std::move(vType));
    */

    wStack_->setCurrentIndex(0);
    addInfoBoxes(cellA0);

    /**
     * Row B
     **/
    auto rowB = addWidget(std::make_unique<Wt::WContainerWidget>());
    rowB->addStyleClass("row");

    // cellB1
    auto cellB1 = rowB->addWidget(std::make_unique<Wt::WContainerWidget>());
    cellB1->addStyleClass("col-md-4");
    bLogout= cellB1->addWidget(std::make_unique<Wt::WPushButton>("Close"));
    bLogout->addStyleClass("btn btn-default btn-lg btn-block");
    bLogout->clicked().connect(
        [=]()
        {
            notify(LOGOUT, EMPTY);
        });

    // cellB2 will have a row with two cells.
    auto cellB2 = rowB->addWidget(std::make_unique<Wt::WContainerWidget>());
    cellB2->addStyleClass("col-md-4");

    auto rowB2 = cellB2->addWidget(std::make_unique<Wt::WContainerWidget>());
    rowB2->addStyleClass("row");

    auto cellB21 = rowB2->addWidget(std::make_unique<Wt::WContainerWidget>());
    cellB21->addStyleClass("col-xs-6");

    bPrev = cellB21->addWidget(std::make_unique<Wt::WPushButton>("Previous"));
    bPrev->addStyleClass("btn btn-primary btn-lg btn-block");
    bPrev->clicked().connect(this, &Configuration::previous);

    auto cellB22 = rowB2->addWidget(std::make_unique<Wt::WContainerWidget>());
    cellB22->addStyleClass("col-xs-6");

    bSave = cellB22->addWidget(std::make_unique<Wt::WPushButton>("Save"));
    bSave->addStyleClass("btn btn-success btn-lg btn-block");
    bSave->clicked().connect(this, &Configuration::save);

    // cellB3 will have a row with two cells.
    auto cellB3 = rowB->addWidget(std::make_unique<Wt::WContainerWidget>());
    cellB3->addStyleClass("col-md-4");

    auto rowB3 = cellB3->addWidget(std::make_unique<Wt::WContainerWidget>());
    rowB3->addStyleClass("row");

    auto cellB31 = rowB3->addWidget(std::make_unique<Wt::WContainerWidget>());
    cellB31->addStyleClass("col-xs-6");

    bDiscard = cellB31->addWidget(std::make_unique<Wt::WPushButton>("Discard"));
    //bCancel->addStyleClass("btn btn-primary btn-lg btn-warning");
    bDiscard->addStyleClass("btn btn-warning btn-lg btn-block");
    bDiscard->clicked().connect(this, &Configuration::discard);

    auto cellB32 = rowB3->addWidget(std::make_unique<Wt::WContainerWidget>());
    cellB32->addStyleClass("col-xs-6");

    bNext = cellB32->addWidget(std::make_unique<Wt::WPushButton>("Next"));
    bNext->addStyleClass("btn btn-primary btn-lg btn-block");
    bNext->clicked().connect(this, &Configuration::next);
}

// Propagate the notification to all panels in the stack.
/*
void Configuration::useSelected(int value)
{
    for(int i=0; i<wStack_->count(); i++)
    {
        Wt::WWidget *p= wStack_->widget(i);
        dynamic_cast<Panel*>(p)->setup(SELECTED, value);
    }
    next();
}
*/

void Configuration::previous()
{
    int current= wStack_->currentIndex();
    if(current > 0)
    {
        current--;
        wStack_->setCurrentIndex(current);
    }
}

void Configuration::next()
{
    int current= wStack_->currentIndex();
    if(current < (wStack_->count()-1))
    {
        current++;
        wStack_->setCurrentIndex(current);
    }
}

void Configuration::save()
{
    Wt::WWidget *p= wStack_->currentWidget();
    dynamic_cast<Panel*>(p)->save();
}

void Configuration::discard()
{
    // Wt::WWidget *p= wStack_->currentWidget();
    // dynamic_cast<WidgetPanel*>(p)->discard();  // Not implemented yet.
}

void Configuration::react(
    const int &key,
    const int &value)
{
    Wt::log("info")
        << "key:" << key << " value:" << value;

    switch(key)
    {
        case SELECTED:
        {
            for(int i=0; i<wStack_->count(); i++)
            {
                Wt::log("info") << "i: " << i;
                Wt::WWidget *p= wStack_->widget(i);
                dynamic_cast<Panel*>(p)->setup(SELECTED, value);
            }
            break;
        }
        case SAVED:
        {
            // std::cout << "===== before" << std::endl;
            // statusDisplay_[value]->setCompleted();
            // std::cout << "===== after" << std::endl;
            bPrev->enable();
            bSave->disable();
            bDiscard->disable();
            bNext->enable();
            bLogout->enable();
            break;
        }
        case CHANGED:
        {
            bPrev->disable();
            bSave->enable();
            bDiscard->enable();
            bNext->disable();
            bLogout->disable();
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

        /*
        case READY:
        {
            Wt::log("info")
                << "Got a READY notification.";
            break;
        }
        */

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

void Configuration::addInfoBoxes(
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
