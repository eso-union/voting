// Wt
#include <Wt/WDialog.h>
#include <Wt/WSelectionBox.h>
#include <Wt/WStringListModel.h>

// Voting
#include "command.h"
#include "voteChoosing.h"
#include "closing.h"

VoteChoosing::VoteChoosing(
    const Postgresql &db)
    : Panel(db)
{
    settingType_= TYPE_ADMIN;
    step_= STEP_0;
    description_= "Choose a voting";

    // addStyleClass("container");
    // addWidget(std::make_unique<Wt::WText>("<h3></h3>"));

    setTitle();

    idxVoting_= getActiveVoting();
    if(idxVoting_ != -1)
    {
        stageAct_= getActiveStage();
    }

    /*
    auto rowA= addWidget(std::make_unique<Wt::WContainerWidget>());
    rowA->addStyleClass("row");

    auto cellA0 = rowA->addWidget(std::make_unique<Wt::WContainerWidget>());
    cellA0->addStyleClass("col-md-4");
    */

    // This must be updated after changes as closing a voting.

    wTable_= wCanvas_->addWidget(std::make_unique<Wt::WTable>());
    wTable_->setWidth(Wt::WLength("100%"));
    wTable_->addStyleClass("table table-striped");

    updateInterface();

    // ------------------------------

    /*
    std::string sentence= "SELECT idx, name FROM general;";
    pqxx::result answer;
    db_.execSql(sentence, answer);
    auto model= std::make_shared<Wt::WStringListModel>();
    pqxx::result::const_iterator row= answer.begin();

    int i= 0;
    for(auto row: answer)
    {
        model->addString(row[1].as(std::string()));
        model->setData(i, 0, row[0].as(int()), Wt::ItemDataRole::User);
        i++;
    }

    Wt::WSelectionBox *sb1= cellA0->addWidget(std::make_unique<Wt::WSelectionBox>());
    sb1->setNoSelectionEnabled(true);
    sb1->setModel(model);
    sb1->setMargin(10, Wt::Side::Right);

    Wt::WText *out= addNew<Wt::WText>("");

    sb1->activated().connect(
        [=]
        {
            std::string text= sb1->currentText().toUTF8();
            int index= sb1->currentIndex();
            idxVoting_= Wt::asNumber(
                model->data(
                    model->index(index, 0),
                    Wt::ItemDataRole::User));
            out->setText(
                Wt::WString(
                    "You selected {1} with index {2}.")
                        .arg(text)
                        .arg(idxVoting_));
        });

    useidxVoting_=
        cellA0->addWidget(
            std::make_unique<Wt::WPushButton>("Continue with selection"));
    useidxVoting_->addStyleClass("btn btn-warning");
    useidxVoting_->clicked().connect(
        [=]()
        {
            // Set the active voting.
            if(idxVoting_)
            {
                std::vector<std::string> bundle;

                // Deactivate every other one.
                Wt::WString sentence=
                    "UPDATE general "
                    "SET active=false "
                    "WHERE idx!={1}";
                sentence.arg(idxVoting_);
                bundle.push_back(sentence.toUTF8());

                // Activate the selected one.
                sentence=
                    "UPDATE general "
                    "SET active=true "
                    "WHERE idx={1}";
                sentence.arg(idxVoting_);
                bundle.push_back(sentence.toUTF8());

                db_.execSql(bundle);

                // tellSelected(idxVoting_);
                notify(SELECTED, idxVoting_);
                notify(COMPLETED, id_);
                notify(NEXT, EMPTY);
            }
        });
    */
}

/*
std::string VoteChoosing::description()
{
    std::string text=
        "Selecction of the voting process";
    return text;
}
*/

void VoteChoosing::updateInterface()
{
    showSummary();
    showHelp();
}

std::string
    VoteChoosing::strTesting(const bool &value)
{
    if(value)
    {
        return "testing";
    }
    return "real";
}

std::string
    VoteChoosing::strPhase(const int &value)
{
    if(value == PHASE_STANDBY)
    {
        return "standby";
    }
    else if(value == PHASE_ACTIVE)
    {
        return "active";
    }
    return "closed";
}

/*
std::string
    VoteChoosing::strClosed(const bool &value)
{
    if(value)
    {
        return "closed";
    }
    return "available";
}
*/

void VoteChoosing::showSummary()
{
    wTable_->clear();

    Wt::WString sentence=
        "SELECT idx, name, testing, phase "
        "FROM general "
        "ORDER BY phase ASC";

    pqxx::result answer;
    auto status= db_.execSql(sentence.toUTF8(), answer);
    if(status == NO_ERROR)
    {
        int i= 0;
        for(auto row: answer)
        {
            int idx= row[0].as<int>();
            wTable_->elementAt(i, 0)->addNew<Wt::WText>(std::to_string(idx));
            wTable_->elementAt(i, 1)->addNew<Wt::WText>(row[1].as<std::string>());

            bool testing= row[2].as<bool>();
            int phase= row[3].as<int>();

            if(phase == PHASE_CLOSED)
            {
                wTable_->elementAt(i, 2)->addNew<Wt::WText>(strTesting(testing));
                wTable_->elementAt(i, 3)->addNew<Wt::WText>("closed");
                auto wReview=
                    wTable_->elementAt(i, 4)->addNew<Wt::WPushButton>("Review");
                wReview->clicked().connect(
                    [=]()
                    {
                        const int index= idx;
                        showResults(index);
                    });
            }
            else
            {
                wTable_->elementAt(i, 2)->addNew<Wt::WText>(strTesting(testing));
                wTable_->elementAt(i, 3)->addNew<Wt::WText>(strPhase(phase));
                if(idx != idxVoting_)
                {
                    auto wActivate=
                        wTable_->elementAt(i, 4)->addNew<Wt::WPushButton>("Activate");
                    wActivate->clicked().connect(
                        [=]()
                        {
                            const int index= idx;
                            activateVoting(index);
                            updateInterface();
                        });

                    // if((!active) && (stageAct_ >= STEP_3))
                    if(stageAct_ >= STEP_3)
                    {
                        wActivate->disable();
                    }
                }
            }
            i++;
        }
    }
    else
    {
        wOut_->setText(status);
    }
}

int VoteChoosing::getActiveVoting()
{
    Wt::WString sentence=
        "SELECT idx "
        "FROM general "
        "WHERE phase={1}";

    sentence.arg(PHASE_ACTIVE);

    pqxx::result answer;
    auto status= db_.execSql(sentence.toUTF8(), answer);
    if(status == NO_ERROR)
    {
        for(auto row: answer)
        {
            const int value= row[0].as<int>();
            return value;
        }
    }
    else
    {
        wOut_->setText(status);
    }
    return -1;
}

void VoteChoosing::activateVoting(const int &index)
{
    idxVoting_= index;
    notify(SELECTED, idxVoting_);

    std::vector<std::string> bundle;

    // Deactivate every other one.
    Wt::WString sentence=
        "UPDATE general "
        "SET phase={1} "
        "WHERE phase={2}";

    sentence
        .arg(PHASE_STANDBY)
        .arg(PHASE_ACTIVE);

    bundle.push_back(sentence.toUTF8());

    // Activate the selected one.
    sentence=
        "UPDATE general "
        "SET phase={1} "
        "WHERE idx={2}";

    sentence
        .arg(PHASE_ACTIVE)
        .arg(index);

    bundle.push_back(sentence.toUTF8());

    auto status= db_.execSql(bundle);
    if(status != NO_ERROR)
    {
        wOut_->setText(status);
    }
}

int VoteChoosing::getActiveStage()
{
    Wt::WString sentence=
        "SELECT MAX(value) "
        "FROM state "
        "WHERE idx_general={1} "
        "AND type={2}";

    sentence
        .arg(idxVoting_)
        .arg(TYPE_ADMIN);

    Wt::log("info") << "sentence:" << sentence;

    pqxx::result answer;
    auto status= db_.execSql(sentence.toUTF8(), answer);
    if(status == NO_ERROR)
    {
        // for(auto row: answer)
        if(answer.begin() != answer.end())
        {
            std::cout << "****************** 0" << std::endl;
            auto row= answer.begin();
            if(row[0].is_null())
            {
                return -1;
            }
            const int value= row[0].as<int>();
            std::cout << "****************** 1" << std::endl;
            return value;
        }
    }
    else
    {
        wOut_->setText(status);
    }
    return -1;
}

void VoteChoosing::showHelp()
{
    if((idxVoting_ != -1) && (stageAct_ >= STEP_3))
    {
        Wt::WString text=
            "A voting is already active, and the invitations has been send. "
            "To activate a new one, the current one must be "
            "terminated i.e. closed.";
        wHelp_->setText(text);
    }
    else if((idxVoting_ != -1) && (stageAct_ < STEP_3))
    {
        Wt::WString text=
            "A voting is already active, and the invitations are not send. "
            "You may activate another one.";
        wHelp_->setText(text);
    }
    else if(idxVoting_ == -1)
    {
        Wt::WString text=
            "To continue you must activate a voting.";
        wHelp_->setText(text);
    }
}

void VoteChoosing::showResults(const int &index)
{
    auto dialog = wCanvas_->addChild(std::make_unique<Wt::WDialog>("Voting Results"));

    auto res= std::make_unique<Closing>(db_);
    res->setup(SELECTED, index);
    res->showOnly();
    dialog->contents()->addWidget(std::move(res));

    Wt::WPushButton *ok= dialog->footer()->addNew<Wt::WPushButton>("OK");
    ok->setDefault(true);
    // if (wApp->environment().ajax())  ok->disable();

    ok->clicked().connect([=] {
        dialog->accept();
    });

    dialog->show();
}
