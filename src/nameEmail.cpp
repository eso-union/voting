// Voting
#include "nameEmail.h"

NameEmail::NameEmail(
    const std::string &name,
    const std::string &email)
{
    auto row= addWidget(std::make_unique<Wt::WContainerWidget>());
    row->addStyleClass("row");

    auto cell0= row->addWidget(std::make_unique<Wt::WContainerWidget>());
    cell0->addStyleClass("col-xs-6");
    inputName_= cell0->addWidget(std::make_unique<Wt::WLineEdit>(name));
    inputName_->keyPressed().connect(
        [=] (const Wt::WKeyEvent& e)
        {
            notify(CHANGED);
        });

    auto cell1= row->addWidget(std::make_unique<Wt::WContainerWidget>());
    cell1->addStyleClass("col-xs-6");
    inputEmail_= cell1->addWidget(std::make_unique<Wt::WLineEdit>(email));
    inputEmail_->keyPressed().connect(
        [=] (const Wt::WKeyEvent& e)
        {
            notify(CHANGED);
        });
}

std::string NameEmail::getName()
{
    std::string name= inputName_->text().toUTF8();
    return name;
}

std::string NameEmail::getEmail()
{
    std::string email= inputEmail_->text().toUTF8();
    return email;
}
