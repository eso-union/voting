#include "people.h"

People::People(const Postgresql &db): Panel(db)
{
    settingType_= TYPE_CONFIG;
    step_= STEP_4;
    description_= "Setting up the list of voters";

    // addWidget(std::make_unique<Wt::WText>("<h3>[optional] Name and email for testing</h3>"));

    setTitle();

    auto rowA= addWidget(std::make_unique<Wt::WContainerWidget>());
    rowA->addStyleClass("row");

    auto buttonRemove= rowA->addWidget(std::make_unique<Wt::WPushButton>("Remove Alternative"));
    buttonRemove->addStyleClass("btn btn-warning");
    buttonRemove->clicked().connect(this, &People::remove);

    auto buttonAdd= rowA->addWidget(std::make_unique<Wt::WPushButton>("Add Alternative"));
    buttonAdd->addStyleClass("btn btn-primary");
    buttonAdd->clicked().connect(this, &People::add);

    auto rowB= addWidget(std::make_unique<Wt::WContainerWidget>());
    rowB->addStyleClass("row");

    auto cell0= rowB->addWidget(std::make_unique<Wt::WContainerWidget>());
    cell0->addStyleClass("col-xs-6");
    cell0->addWidget(std::make_unique<Wt::WText>("Name"));

    auto cell1= rowB->addWidget(std::make_unique<Wt::WContainerWidget>());
    cell1->addStyleClass("col-xs-6");
    cell1->addWidget(std::make_unique<Wt::WText>("Email address"));
}

void People::setup(
    const int &type,
    const int &value)
{
    if(type == SELECTED)
    {
        // Clean up all previous widgets.

        idxVoting_= value;

        // Create new widgets.
        std::string sentence=
            "SELECT name, email "
            "FROM people "
            "WHERE idx_general=" + std::to_string(idxVoting_);

        pqxx::result answer;
        db_.execSql(sentence, answer);
        for(auto row: answer)
        {
            // const std::string name= row[0].as(std::string());
            // const std::string email= row[1].as(std::string());
            const std::string name= row[0].as<std::string>();
            const std::string email= row[1].as<std::string>();
            add(name, email);
        }
    }

    if(isCompleted())
    {
        notify(COMPLETED, id_);
    }
}

void People::save()
{
    Wt::log("info") << "[People] saving ...";

    std::vector<std::string> codeList= generateCodes(peopleList_.size());

    Wt::log("info") << "1";

    std::vector<std::string> bundle;

    // Error handling, try-catch.
    std::string delSentence=
        "DELETE FROM people "
        "WHERE idx_general=" + std::to_string(idxVoting_);
    bundle.push_back(delSentence);

    int i= 0;
    for(auto &e: peopleList_)
    {
        const std::string name= e->getName();
        const std::string email= e->getEmail();
        const std::string code= codeList[i];
        if((name != "") && (email != ""))
        {
            std::string insertSentence=
                std::string("INSERT INTO people(idx_general, name, email, code, used) ") +
                "VALUES(" + std::to_string(idxVoting_) + ", '" +
                name + "', '" +
                email + "', '" +
                code + "', false)";

            bundle.push_back(insertSentence);
        }
        i++;
    }
    db_.execSql(bundle);

    notify(SAVED, id_);
    notify(COMPLETED, id_);
    setCompleted();
};

void People::dataChanged(int value)
{
    notify(CHANGED, EMPTY);
}

void People::add()
{
    add("", "");
}

void People::add(
    const std::string &name,
    const std::string &email)
{
    auto ne = std::make_unique<NameEmail>(name, email);
    ne->notify.connect(boost::bind(&People::dataChanged, this, _1));
    auto w= addWidget(std::move(ne));
    peopleList_.push_back(w);
}

void People::remove()
{
    if(peopleList_.size()>0)
    {
        removeWidget(peopleList_[peopleList_.size()-1]);
        peopleList_.pop_back();
        notify(CHANGED, EMPTY);
    }
}

std::vector<std::string> People::generateCodes(const unsigned int &size)
{
    std::vector<std::string> codeList;
    // Definion of allowed characters.
    std::string chars("ABCDEFGHIJKLMNPQRSTUVWXYZ123456789");

    // 'random_device' is used as a source of entropy
    // to make sequences that are not predictable.
    boost::random::random_device rng;

    // Random characters from the string are selected.
    for(unsigned int j= 0; j <size; j++)
    {
        boost::random::uniform_int_distribution<> index_dist(0, chars.size() - 1);
        std::string code;
        for(int i= 0; i < 6; ++i)
        {
            code += chars[index_dist(rng)];
        }
        codeList.push_back(code);
    }
    return codeList;
}
