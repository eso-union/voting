// C
#include <cassert>

// Wt
#include <Wt/WString.h>
#include <Wt/WText.h>

// Voting
#include "common.h"
#include "panel.h"

Panel::Panel(
    const Postgresql &db): db_(db)
{
    wCanvas_= addWidget(std::make_unique<Wt::WContainerWidget>());

    addWidget(std::make_unique<Wt::WBreak>());

    auto wMsg= addWidget(std::make_unique<Wt::WContainerWidget>());
    wOut_= wMsg->addWidget(std::make_unique<Wt::WText>());

    Wt::WColor bgColor(206, 242, 255);
    wOut_->decorationStyle().setBackgroundColor(bgColor);

    Wt::WColor fgColor(0, 60, 255);
    wOut_->decorationStyle().setForegroundColor(fgColor);

    // Verify pointers
    assert(wCanvas_ != nullptr);
    assert(wOut_    != nullptr);
}

Panel::Panel(
    const Postgresql &db,
    const int &idxVoting): db_(db), idxVoting_(idxVoting)
{}

void Panel::setTitle()
{
    Wt::WString title= "<h3>{1}</h3>";
    title.arg(description_);
    wCanvas_->addWidget(std::make_unique<Wt::WText>(title));
}

void Panel::save()
{
    std::cout << "[parent] saving ...\n";
}

void Panel::discard()
{
    setData();
    notify(SAVED, EMPTY);
}

void Panel::setData()
{}

void Panel::setup(
    const int &type,
    const int &value)
{
    if(type == SELECTED)
    {
        idxVoting_= value;
    }
}

void Panel::setId(const int &value)
{
    id_= value;
}

int Panel::id()
{
    return id_;
}

std::string Panel::description()
{
    return description_;
}

void Panel::setCompleted()
{
    Wt::WString sentence=
        "INSERT INTO state(idx_general, type, value) "
        "VALUES({1}, {2}, {3});";

    sentence
        .arg(idxVoting_)
        .arg(settingType_)
        .arg(step_);

    db_.execSql(sentence.toUTF8());

    notify(COMPLETED, id_);
}

void Panel::setSaved()
{
    notify(SAVED, id_);
    wOut_->setText("Saved");
}

bool Panel::isCompleted()
{
    Wt::WString sentence=
        "SELECT idx "
        "FROM state "
        "WHERE idx_general={1} "
        "AND type={2} "
        "AND value={3};";

    sentence
        .arg(idxVoting_)
        .arg(settingType_)
        .arg(step_);

    pqxx::result answer= db_.query(sentence.toUTF8());
    if(answer.begin() == answer.end())
    {
        return false;
    }
    return true;
}
