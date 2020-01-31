// Wt
#include <Wt/WSelectionBox.h>
#include <Wt/WStringListModel.h>

// Voting
#include "voteChoosing.h"

VoteChoosing::VoteChoosing(
    const Postgresql &db)
    : Panel(db)
{
    settingType_= TYPE_ADMIN;
    step_= STEP_0;

    addStyleClass("container");

    addWidget(std::make_unique<Wt::WText>("<h3>Select or Create Voting</h3>"));

    auto rowA= addWidget(std::make_unique<Wt::WContainerWidget>());
    rowA->addStyleClass("row");

    auto cellA0 = rowA->addWidget(std::make_unique<Wt::WContainerWidget>());
    cellA0->addStyleClass("col-md-4");

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
                Wt::WString("You selected {1} with index {2}.").arg(text).arg(idxVoting_));
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
                Wt::log("info") << "selected:" << idxVoting_;

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
}

std::string VoteChoosing::description()
{
    std::string text=
        "Selecction of the voting process";
    return text;
}
