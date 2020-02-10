// Wt
#include <Wt/WMessageBox.h>

// Voting
#include "command.h"
#include "sendingVote.h"

SendingVote::SendingVote(
    const Postgresql &db,
    const int &idxVoting)
    : Panel(db, idxVoting)
{
    // Set the quetion or header of the vote on top.
    std::string question= getQuestion();
    auto title= Wt::WString("<h3>{1}</h3>").arg(question);
    addWidget(std::make_unique<Wt::WText>(title));

    // Set the alternatives.
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
        options_[idx]= cb;
    }

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

    pqxx::result ansOption= db_.query(selOption.toUTF8());
    for(auto row: ansOption)
    {
        int value= row[0].as<int>();
        options.push_back(value);
    }

    auto it= std::find(std::begin(options), std::end(options), total);
    if(it == std::end(options))
    {
        Wt::WString detail= Command::genDetail(options);
        Wt::WString message= "<p>You selected {1} alternatives. {2}</p>";
        message
            .arg(total)
            .arg(detail);

        // This option is not valid.
        Wt::WMessageBox::show(
            "No valid",
            message,
            Wt::StandardButton::Ok);
        return;
    }

    // Ask for confirmation to the user.

    // Upon positve reply, save the vote, i.e. the options.
    int idxCode= -1;
    std::string sentence=
        "INSERT INTO code(idx_general, value) "
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
        sentence+= Command::toString(it->second->isChecked()) + ")";
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

std::string SendingVote::getQuestion()
{
    Wt::WString sentence=
        "SELECT header "
        "FROM general "
        "WHERE idx={1};";

    sentence.arg(idxVoting_);

    pqxx::result answer;
    auto status= db_.execSql(sentence.toUTF8(), answer);
    if(status == NO_ERROR)
    {
        if(answer.begin() != answer.end())
        {
            auto row= answer.begin();
            std::string value= row[0].as<std::string>();
            return value;
        }
    }
    else
    {
        wOut_->setText(status);
    }
}
