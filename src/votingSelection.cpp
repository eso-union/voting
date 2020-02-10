// C
#include <cassert>

// Voting
#include "votingSelection.h"

VotingSelection::VotingSelection(const Postgresql &db): Panel(db)
{
    settingType_= TYPE_CONFIG;
    step_= STEP_0;
    description_= "Chose or create a voting";

    setTitle();

    // addStyleClass("container");

    wTable_= wCanvas_->addWidget(std::make_unique<Wt::WTable>());
    wTable_->setWidth(Wt::WLength("100%"));
    wTable_->addStyleClass("table table-striped");

    updateInterface();


    // addWidget(std::make_unique<Wt::WText>("<h3>Select or Create Voting</h3>"));

/*
    auto rowA = wCanvas_->addWidget(std::make_unique<Wt::WContainerWidget>());
    rowA->addStyleClass("row");

    auto cellA0 = rowA->addWidget(std::make_unique<Wt::WContainerWidget>());
    cellA0->addStyleClass("col-md-4");
    // cellB0->addWidget(std::make_unique<Wt::WText>("Begining:"));

    std::string sentence= "SELECT idx, name FROM general;";
    pqxx::result answer;
    db_.execSql(sentence, answer);
    auto model= std::make_shared<Wt::WStringListModel>();
    pqxx::result::const_iterator row= answer.begin();

    int i= 0;
    for(auto row: answer)
    {
        std::cout
            << "************ row: "
            << row[0].as(std::string()) << ", "
            << row[1].as(std::string()) << "\n";

        model->addString(row[1].as(std::string()));
        model->setData(i, 0, row[0].as(int()), Wt::ItemDataRole::User);
        i++;
    }

    Wt::WSelectionBox *sb1= cellA0->addWidget(std::make_unique<Wt::WSelectionBox>());
    sb1->setNoSelectionEnabled(true);
    sb1->setModel(model);
    sb1->setMargin(10, Wt::Side::Right);

    sb1->activated().connect(
        [=]
        {
            std::string text= sb1->currentText().toUTF8();
            int index= sb1->currentIndex();
            // Wt::WString info= Wt::asString(model->data(model->index(index,0), Wt::ItemDataRole::User)); // it works!
            // int info= Wt::asNumber(model->data(model->index(index,0), Wt::ItemDataRole::User));
            idxVoting_= Wt::asNumber(
                model->data(
                    model->index(index, 0),
                    Wt::ItemDataRole::User));

            // out->setText(Wt::WString("You selected {1}.").arg(sb1->currentText()));
            wOut_->setText(
                Wt::WString(
                    "You selected '{1}' with index {2}.").arg(text).arg(idxVoting_));
        });

    wUseSel_= cellA0->addWidget(
        std::make_unique<Wt::WPushButton>("Continue with selection"));
    wUseSel_->addStyleClass("btn btn-warning");
    wUseSel_->clicked().connect(
        [=]()
        {
            Wt::log("info") << "idxVoting_: " << idxVoting_;
            if(idxVoting_ != -1)
            {
                Wt::log("info") << "sending notifications";
                notify(SELECTED, idxVoting_);
                notify(COMPLETED, id_);
                notify(NEXT, EMPTY);
            }
        });
    */

    auto rowB= wCanvas_->addWidget(std::make_unique<Wt::WContainerWidget>());
    rowB->addStyleClass("row");

    auto cellB0= rowB->addWidget(std::make_unique<Wt::WContainerWidget>());
    cellB0->addStyleClass("col-md-4");
    wNewName_= cellB0->addWidget(std::make_unique<Wt::WLineEdit>());

    auto cellB1= rowB->addWidget(std::make_unique<Wt::WContainerWidget>());
    cellB1->addStyleClass("col-md-4");
    auto createVoting= cellB1->addWidget(std::make_unique<Wt::WPushButton>("Create Voting"));
    createVoting->addStyleClass("btn btn-primary");
    createVoting->clicked().connect(this, &VotingSelection::create);

    // Verify pointers
    // assert(wNewName_ != nullptr);
    // assert(wUseSel_  != nullptr);
}

void VotingSelection::create()
{
    std::string name= wNewName_->text().toUTF8();
    if(name.length() < 5)
    {
        wOut_->setText("Name must be at least 5 chars long.");
        return;
    }

    std::string convocatory= Wt::WString::tr("convocatory-template-html").toUTF8();
    std::string header= "(empty)";
    std::string subject= "(empty)";

    Wt::WString sentence=
        "INSERT INTO general(name, convocatory, header, subject, "
        "with_ministers, qty_ministers, html, testing, phase) "
        "VALUES('{1}', '{2}', '{3}', '{4}', false, 1, true, true, {5}) "
        "RETURNING idx";

    sentence
        .arg(name)
        .arg(convocatory)
        .arg(header)
        .arg(subject)
        .arg(PHASE_STANDBY);

    try
    {
        pqxx::result answer;
        db_.execSql(sentence.toUTF8(), answer);
        auto row= answer.begin();
        if(row != answer.end())
        {
            idxVoting_= row[0].as<int>();
        }

        notify(SELECTED, idxVoting_);
        notify(COMPLETED, id_);
        // notify(NEXT, EMPTY);

        wNewName_->setText("");
    }
    catch(const std::exception &e)
    {
        Wt::log("error") << e.what();
        Wt::log("error") << sentence;
    }
    updateInterface();
}

void VotingSelection::updateInterface()
{
    showSummary();
    // showHelp();
}

void VotingSelection::showSummary()
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

            if(testing && (phase == PHASE_CLOSED))
            {
                wTable_->elementAt(i, 2)->addNew<Wt::WText>(strTesting(testing));
                wTable_->elementAt(i, 3)->addNew<Wt::WText>("closed");
                auto wConfig=
                    wTable_->elementAt(i, 4)->addNew<Wt::WPushButton>("Config");
                wConfig->clicked().connect(
                    [=]()
                    {
                        const int index= idx;
                        reopen(index);
                        updateInterface();
                    });
            }
            else if(phase == PHASE_STANDBY)
            {
                wTable_->elementAt(i, 2)->addNew<Wt::WText>(strTesting(testing));
                wTable_->elementAt(i, 3)->addNew<Wt::WText>(strPhase(phase));

                if(idx != idxVoting_)
                {
                    auto wSelect=
                        wTable_->elementAt(i, 4)->addNew<Wt::WPushButton>("Select");
                    wSelect->clicked().connect(
                        [=]()
                        {
                            const int index= idx;
                            // activateVoting(index);
                            notify(SELECTED, index);
                            notify(COMPLETED, id_);
                            updateInterface();
                        });
                }
                else
                {
                    wTable_->elementAt(i, 4)->addNew<Wt::WText>("Configuring");
                }
            }
            else if(phase == PHASE_ACTIVE)
            {
                wTable_->elementAt(i, 2)->addNew<Wt::WText>(strTesting(testing));
                wTable_->elementAt(i, 3)->addNew<Wt::WText>(strPhase(phase));
                wTable_->elementAt(i, 4)->addNew<Wt::WText>("Can't modify");
            }

            i++;
        }
    }
    else
    {
        wOut_->setText(status);
    }
}

std::string
    VotingSelection::strTesting(const bool &value)
{
    if(value)
    {
        return "testing";
    }
    return "real";
}

std::string
    VotingSelection::strPhase(const int &value)
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

void VotingSelection::reopen(const int &index)
{
    Wt::WString delPeople=
        "DELETE FROM people "
        "WHERE idx_general={1}";
    delPeople.arg(index);

    Wt::WString delCode=
        "DELETE FROM code "
        "WHERE idx_general={1}";
    delCode.arg(index);

    Wt::WString updGen=
        "UPDATE general "
        "SET phase={1} "
        "WHERE idx={2}";
    updGen
        .arg(PHASE_STANDBY)
        .arg(index);

    std::vector<std::string> bundle;
    bundle.push_back(delPeople.toUTF8());
    bundle.push_back(delCode.toUTF8());
    bundle.push_back(updGen.toUTF8());
    auto status= db_.execSql(bundle);
    if(status != NO_ERROR)
    {
        wOut_->setText(status);
    }
}
