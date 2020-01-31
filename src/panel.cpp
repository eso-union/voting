// Wt
#include <Wt/WString.h>
#include <Wt/WText.h>

// Voting
#include "common.h"
#include "panel.h"

Panel::Panel(
    const Postgresql &db): db_(db)
{}

Panel::Panel(
    const Postgresql &db,
    const int &idxVoting): db_(db), idxVoting_(idxVoting)
{}

void Panel::setTitle()
{
    Wt::WString title= "<h3>{1}</h3>";
    title.arg(description_);
    addWidget(std::make_unique<Wt::WText>(title));
}

void Panel::save()
{
    std::cout << "[parent] saving ...\n";
}

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
