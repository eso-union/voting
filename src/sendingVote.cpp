// Wt
#include <Wt/WMessageBox.h>

// Voting
#include "sendingVote.h"

SendingVote::SendingVote(
    const Postgresql &db,
    const int &idxVoting)
    : Panel(db, idxVoting)
{
    addWidget(
        std::make_unique<Wt::WText>(
            "<h3>Enter your vote</h3>"));

    // wGroup_= std::make_shared<Wt::WButtonGroup>();

    std::string sentence=
        "SELECT idx, value "
        "FROM alternative "
        "WHERE idx_general=" + std::to_string(idxVoting_);

    Wt::log("info") << "sentence:" << sentence;

    pqxx::result answer;
    db_.execSql(sentence, answer);
    for(auto row: answer)
    {
        const int idx= row[0].as<int>();
        const std::string text= row[1].as<std::string>();

        Wt::WCheckBox *cb = addNew<Wt::WCheckBox>(text);
        cb->setInline(false);
        cb->setChecked(false);
        // altList_.push_back(cb);
        options_[idx]= cb;
    }

    /*
    auto wCode_= addWidget(std::make_unique<Wt::WLineEdit>());
    wCode_->keyPressed().connect(
        [=] (const Wt::WKeyEvent& e)
        {
            if(e.key() == Wt::Key::Enter)
            {
                std::cout << "right!\n";
                // verify();
            }
        }); */

    auto wNext_= addWidget(std::make_unique<Wt::WPushButton>("Cast Vote"));
    wNext_->addStyleClass("btn btn-primary btn-lg btn-block");
    wNext_->clicked().connect(this, &SendingVote::process);
}

void SendingVote::setVotingData(
    const int &idPerson,
    const std::string &code)
{
    idxPeople_= idPerson;
    code_= code;
}

void SendingVote::process()
{
    Wt::log("info") << "alternative selected ...";

    int total= 0;
    for(auto it= options_.begin(); it != options_.end(); ++it)
    {
        Wt::log("info")
        << "option idx: "
        << it->first
        << " text: "
        << it->second->text()
        << " checked: "
        << it->second->isChecked();

        if(it->second->isChecked())
        {
            total++;
        }
    }

    Wt::log("info") << "total:" << total;

    // Verify that 'total' is less than the max options allowed.
    std::vector<int> options;
    // options.push_back(0);
    // options.push_back(2);

    Wt::WString selOption=
        "SELECT quantity, allowed "
        "FROM option "
        "WHERE idx_general={1} "
        "AND allowed=true";

    selOption.arg(idxVoting_);

    Wt::log("info") << "selOption: " << selOption;

    pqxx::result ansOption= db_.query(selOption.toUTF8());
    for(auto row: ansOption)
    {
        int value= row[0].as<int>();
        options.push_back(value);
    }

    // Wt::log("info") << "options: " << options;
    // std::cout << "options: " << options << "\n";

    auto it= std::find(std::begin(options), std::end(options), total);
    if(it == std::end(options))
    {
        // This option is not valid.
        Wt::WMessageBox::show(
            "No valid",
            "<p>You selected 3 options.</p>",
            Wt::StandardButton::Ok);
        return;
    }

    // Ask for confirmation to the user.

    // Upon positve reply, save the vote, i.e. the options.
    int idxCode= -1;
    std::string sentence=
        "INSERT INTO code(idx_general, code) "
        "VALUES(" + std::to_string(idxVoting_) + ", '" + code_ + "') "
        "RETURNING idx";
    pqxx::result answer;
    db_.execSql(sentence, answer);
    auto row= answer.begin();
    if(row != answer.end())
    {
        idxCode= row[0].as<int>();
    }
    else
    {
        Wt::log("error") << "fail to insert the code";
    }

    // Insert all the options selected by voter.
    std::vector<std::string> bundle;
    for(auto it= options_.begin(); it != options_.end(); ++it)
    {
        Wt::log("info")
            << "option idx: "
            << it->first
            << " text: "
            << it->second->text()
            << " checked: "
            << it->second->isChecked();

        std::string sentence;
        sentence = std::string("INSERT INTO vote(idx_code, idx_alternative, value) ");
        sentence+= std::string("VALUES(");
        sentence+= std::to_string(idxCode) + ", ";
        sentence+= std::to_string(it->first) + ", ";
        sentence+= toString(it->second->isChecked()) + ")";
        bundle.push_back(std::move(sentence));
    }
    // SQL sentence to mark the code as used.
    sentence=
        "UPDATE people "
        "SET used=true "
        "WHERE idx=" + std::to_string(idxPeople_);
    bundle.push_back(std::move(sentence));

    // All the sentences executed in the same transaction.
    db_.execSql(bundle);

    notify(READY, EMPTY);
}

std::string SendingVote::toString(bool value)
{
    if(value)
    {
        return "true";
    }
    return "false";
}
