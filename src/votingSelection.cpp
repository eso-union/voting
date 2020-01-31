#include "votingSelection.h"

VotingSelection::VotingSelection(const Postgresql &db): Panel(db)
{
    addStyleClass("container");

    addWidget(std::make_unique<Wt::WText>("<h3>Select or Create Voting</h3>"));

    auto rowA = addWidget(std::make_unique<Wt::WContainerWidget>());
    rowA->addStyleClass("row");

    auto cellA0 = rowA->addWidget(std::make_unique<Wt::WContainerWidget>());
    cellA0->addStyleClass("col-md-4");
    // cellB0->addWidget(std::make_unique<Wt::WText>("Begining:"));

    std::string sentence= "SELECT idx, name FROM general;";
    pqxx::result answer;
    db_.execSql(sentence, answer);
    auto model= std::make_shared<Wt::WStringListModel>();
    pqxx::result::const_iterator row= answer.begin();

    /*
    int i= 0;
    if(row != answer.end())
    {
        std::cout
            << "************ row: "
            << row[0].as(std::string()) << ", "
            << row[1].as(std::string()) << "\n";

        model->addString(row[1].as(std::string()));
        // model->setData(i, 0, row[0].as(std::string()), Wt::ItemDataRole::User);  // it works!
        model->setData(i, 0, row[0].as(int()), Wt::ItemDataRole::User);
        row++;
        i++;
    }*/

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

    Wt::WText *out= addNew<Wt::WText>("");

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
            out->setText(
                Wt::WString(
                    "You selected {1} with index {2}.").arg(text).arg(idxVoting_));
        });

    useSelected_= cellA0->addWidget(
        std::make_unique<Wt::WPushButton>("Continue with selection"));
    useSelected_->addStyleClass("btn btn-warning");
    useSelected_->clicked().connect(
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

    auto rowB= addWidget(std::make_unique<Wt::WContainerWidget>());
    rowB->addStyleClass("row");

    auto cellB0= rowB->addWidget(std::make_unique<Wt::WContainerWidget>());
    cellB0->addStyleClass("col-md-4");

    newName_= cellB0->addWidget(std::make_unique<Wt::WLineEdit>());

    auto createVoting = cellB0->addWidget(std::make_unique<Wt::WPushButton>("Create Voting"));
    createVoting->addStyleClass("btn btn-primary");
    createVoting->clicked().connect(this, &VotingSelection::create);
}

std::string VotingSelection::description()
{
    std::string text= "Chose or create a voting";
    return text;
}

void VotingSelection::create()
{
    // if(newName != nullptr)
    if(newName_)
    {
        Wt::log("info") << "executing create()";

        std::string convocatory= "some text"; // getConvTempalte();
        std::string header= "some text"; // getHeadTemplate();
        std::string name= newName_->text().toUTF8();

        // 'name' must be unique
        Wt::WString sentence=
            "INSERT INTO general(name, convocatory, header, type, active) "
            "VALUES('{1}', '{2}', '{3}', {4}, false) "
            "RETURNING idx";

        sentence
            .arg(name)
            .arg(convocatory)
            .arg(header)
            .arg(TESTING);

        // try-catch, idx must be different form -1.
        pqxx::result answer;
        db_.execSql(sentence.toUTF8(), answer);
        auto row= answer.begin();
        if(row != answer.end())
        {
            idxVoting_= row[0].as<int>();
        }

        notify(SELECTED, idxVoting_);
        notify(COMPLETED, id_);
        notify(NEXT, EMPTY);
    }
}
