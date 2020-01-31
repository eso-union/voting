#include "alternative.h"

Alternative::Alternative(const Postgresql &db): Panel(db)
{
    settingType_= TYPE_CONFIG;
    step_= STEP_2;
    description_= "Setting up voting alternatives";

    // addStyleClass("container");
    // addWidget(std::make_unique<Wt::WText>("<h3>Voting alternative</h3>"));

    setTitle();

    auto wRemove= addWidget(std::make_unique<Wt::WPushButton>("Remove Alternative"));
    wRemove->addStyleClass("btn btn-warning");
    wRemove->clicked().connect(this, &Alternative::remove);

    auto wAdd= addWidget(std::make_unique<Wt::WPushButton>("Add Alternative"));
    wAdd->addStyleClass("btn btn-primary");
    wAdd->clicked().connect(this, &Alternative::add);
}

void Alternative::save()
{
    std::cout << "[Alternative] saving ...\n";
    std::vector<std::string> bundle;

    // Error handling, try-catch.
    std::string delSentence=
        "DELETE FROM alternative "
        "WHERE idx_general=" + std::to_string(idxVoting_);
    bundle.push_back(delSentence);

    for(auto &e: input_)
    {
        std::string insertSentence=
            "INSERT INTO alternative(idx_general, value) "
            "VALUES(" + std::to_string(idxVoting_) + ", '" + e->text().toUTF8() + "')";
        bundle.push_back(insertSentence);
    }

    db_.execSql(bundle);
    notify(SAVED, id_);
    notify(COMPLETED, id_);
    setCompleted();
};

void Alternative::setup(
    const int &type,
    const int &value)
{
    if(type == SELECTED)
    {
        idxVoting_= value;

        std::string sentence=
            "SELECT value "
            "FROM alternative "
            "WHERE idx_general=" + std::to_string(idxVoting_);

        std::cout << "sentence:" << sentence << "\n";

        pqxx::result answer;
        db_.execSql(sentence, answer);
        for(auto row: answer)
        {
            std::cout
                << "row[0]:" << row[0].as(std::string()) << "\n";
            add(row[0].as(std::string()));
        }
    }

    if(isCompleted())
    {
        notify(COMPLETED, id_);
    }
}

void Alternative::add()
{
    add("");
}

void Alternative::add(const std::string &text)
{
    Wt::WLineEdit *edit=
        addWidget(std::make_unique<Wt::WLineEdit>(text));
    edit->addStyleClass("form-control");
    edit->setPlaceholderText("Alternative " + std::to_string(input_.size()));
    edit->setInline(false);
    edit->keyPressed().connect(
        [=] (const Wt::WKeyEvent& e)
        {
            notify(CHANGED, EMPTY);
        });
    input_.push_back(edit);
}

void Alternative::remove()
{
    if(input_.size()>0)
    {
        removeWidget(input_[input_.size()-1]);
        input_.pop_back();
        notify(CHANGED, EMPTY);
    }
}
