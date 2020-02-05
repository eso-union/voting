// C++
#include <cassert>

// Wt
#include <Wt/WColor.h>
#include <Wt/WFont.h>
#include <Wt/WPushButton.h>

// Voting
#include "command.h"
#include "pending.h"

Pending::Pending(const Postgresql &db): Panel(db)
{
    settingType_= TYPE_ADMIN;
    step_= STEP_4;
    description_= "Who is pending and resend of invitations";

    // addWidget(std::make_unique<Wt::WText>("<h3>Pending People to Vote</h3>"));
    setTitle();

    auto wPending= wCanvas_->addWidget(std::make_unique<Wt::WPushButton>("Result"));
    wPending->clicked().connect(this, &Pending::pending);

    auto wResend= wCanvas_->addWidget(std::make_unique<Wt::WPushButton>("Resend"));
    wResend->clicked().connect(this, &Pending::resend);

    wOutput_= wCanvas_->addWidget(std::make_unique<Wt::WTextArea>());
    wOutput_->setColumns(40);
    wOutput_->setRows(20);
    wOutput_->setReadOnly(true);

    Wt::WFont mono;
    mono.setFamily(Wt::FontFamily::Monospace, "'Courier New'");
    mono.setSize(12);
    wOutput_->decorationStyle().setFont(mono);

    Wt::WColor bgColor(10, 10, 10);
    wOutput_->decorationStyle().setBackgroundColor(bgColor);

    Wt::WColor fgColor(245, 245, 245);
    wOutput_->decorationStyle().setForegroundColor(fgColor);

    // Verifying pointers
    assert(wOutput_ != nullptr);
}

void Pending::setup(
    const int &type,
    const int &value)
{
    if(type == SELECTED)
    {
        idxVoting_= value;
    }
}

void Pending::pending()
{
    showPending();
    notify(COMPLETED, id_);
}

void Pending::resend()
{
    Command::sendInvitations(
        idxVoting_,
        db_,
        wOutput_);
}

void Pending::showPending()
{
    Wt::WString sentence=
        "SELECT name "
        "FROM people "
        "WHERE used=false "
        "AND idx_general={1}";

    sentence.arg(idxVoting_);

    pqxx::result answer= db_.query(sentence.toUTF8());
    if(answer.begin() == answer.end())
    {
        wOutput_->setText("All voted, no one is pending to vote.");
    }
    else
    {
        std::string pending;
        for(auto row: answer)
        {
            std::string value= row[0].as<std::string>();
            pending+= value + '\n';
        }
        wOutput_->setText(pending);
    }
}
