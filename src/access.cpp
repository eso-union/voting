// C
#include <cassert>

// Voting
#include "access.h"

Access::Access(
    const Postgresql &db,
    const std::string &lookup)
    : Panel(db), lookup_(lookup)
{
    auto rowA= wCanvas_->addWidget(std::make_unique<Wt::WContainerWidget>());
    rowA->addStyleClass("row");

    auto cellA0 = rowA->addWidget(std::make_unique<Wt::WContainerWidget>());
    cellA0->addStyleClass("col-md-4");

    auto cellA1 = rowA->addWidget(std::make_unique<Wt::WContainerWidget>());
    cellA1->addStyleClass("col-md-4");

    wUser_= cellA1->addWidget(std::make_unique<Wt::WLineEdit>());
    wUser_->enterPressed().connect(
        [=]()
        {
            wPassw_->setFocus();
        });

    wPassw_= cellA1->addWidget(std::make_unique<Wt::WLineEdit>());
    wPassw_->setEchoMode(Wt::EchoMode::Password);
    wPassw_->enterPressed().connect(
        [=]()
        {
            verify();
        });

    auto cellA2= rowA->addWidget(std::make_unique<Wt::WContainerWidget>());
    cellA2->addStyleClass("col-md-4");

    // rowB
    auto rowB= wCanvas_->addWidget(std::make_unique<Wt::WContainerWidget>());
    rowB->addStyleClass("row");

    auto cellB0= rowB->addWidget(std::make_unique<Wt::WContainerWidget>());
    cellB0->addStyleClass("col-md-4");

    auto cellB1= rowB->addWidget(std::make_unique<Wt::WContainerWidget>());
    cellB1->addStyleClass("col-md-4");

    auto cellB2 = rowB->addWidget(std::make_unique<Wt::WContainerWidget>());
    cellB2->addStyleClass("col-md-4");

    // rowC
    auto rowC= wCanvas_->addWidget(std::make_unique<Wt::WContainerWidget>());
    rowC->addStyleClass("row");

    auto cellC0= rowC->addWidget(std::make_unique<Wt::WContainerWidget>());
    cellC0->addStyleClass("col-md-4");

    auto cellC1= rowC->addWidget(std::make_unique<Wt::WContainerWidget>());
    cellC1->addStyleClass("col-md-4");

    wGo_= cellC1->addWidget(std::make_unique<Wt::WPushButton>("Go"));
    wGo_->addStyleClass("btn btn-primary btn-lg btn-block btn-default");
    wGo_->clicked().connect(
        [=]()
        {
            verify();
        });

    auto cellC2= rowC->addWidget(std::make_unique<Wt::WContainerWidget>());
    cellC2->addStyleClass("col-md-4");

    // Verify pointers
    assert(wGo_    != nullptr);
    assert(wUser_  != nullptr);
    assert(wPassw_ != nullptr);
}

void Access::verify()
{
    bool valid= verifyUserPassw();

    if(valid)
    {
        notify(GRANT_ACCESS, EMPTY);
    }
    else
    {
        wOut_->setText("wrong user or password");
        wUser_->disable();
        wPassw_->disable();
        wGo_->disable();
        Wt::WApplication::instance()->processEvents();

        std::this_thread::sleep_for(std::chrono::seconds(3));

        wOut_->setText("");
        wUser_->setText("");
        wPassw_->setText("");
        wUser_->enable();
        wPassw_->enable();
        wGo_->enable();
        Wt::WApplication::instance()->processEvents();
    }
}

bool Access::verifyUserPassw()
{
    std::string user=  wUser_->text().toUTF8();
    std::string passw= wPassw_->text().toUTF8();

    Wt::WString sentence=
        "SELECT idx "
        "FROM access "
        "WHERE name='{1}' "
        "AND passw='{2}'";

    sentence
        .arg(user)
        .arg(passw);

    pqxx::result answer;
    auto status= db_.execSql(sentence.toUTF8(), answer);
    if(status == NO_ERROR)
    {
        if(answer.begin() != answer.end())
        {
            return true;
        }
    }
    return false;
}