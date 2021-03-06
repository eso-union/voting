// C
#include <cassert>

// C++
#include <thread>
#include <chrono>
#include <ostream>
#include <sstream>
#include <iostream>
#include <fstream>

// Voting
#include "command.h"
#include "invitations.h"

Invitations::Invitations(const Postgresql &db): Panel(db)
{
    settingType_= TYPE_ADMIN;
    step_= STEP_3;
    description_= "Sending invitations";

    setTitle();

    wSend_ =
        wCanvas_->addWidget(
            std::make_unique<Wt::WPushButton>("Send"));
    wSend_->addStyleClass("btn btn-success");
    wSend_->clicked().connect(this, &Invitations::send);

    wOutput_=
        wCanvas_->addWidget(std::make_unique<Wt::WTextArea>());
    wOutput_->setColumns(20);
    wOutput_->setRows(15);
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
    assert(wSend_   != nullptr);
    assert(wOutput_ != nullptr);
}

void Invitations::setup(
    const int &type,
    const int &value)
{
    if(type == SELECTED)
    {
        if(idxVoting_ != value)
        {
            idxVoting_= value;
            wOutput_->setText("");

            if(isCompleted())
            {
                // This step is completed, is not active.
                // Better: DONE than COMPLETED.
                wSend_->disable();
                notify(COMPLETED, id_);
                wOut_->setText("Invitations already sent");
            }
            else
            {
                // This step is not completed, is pending.
                wSend_->enable();
                notify(INCOMPLETE, step_);
                wOut_->setText("Invitations are pending to sent");
            }

            Wt::WString sentence=
                "SELECT testing "
                "FROM general "
                "WHERE idx={1}";

            sentence.arg(idxVoting_);

            pqxx::result answer;
            auto status= db_.execSql(sentence.toUTF8(), answer);
            if(status == NO_ERROR)
            {
                if(answer.begin() != answer.end())
                {
                    auto row= answer.begin();
                    testing_= row[0].as<bool>();
                }
            }
            else
            {
                wOut_->setText(status);
            }
        }
    }
}

void Invitations::send()
{
    wSend_->disable();

    Command::sendInvitations(
        idxVoting_,
        db_,
        wOutput_);

    // Upon no error
    notify(COMPLETED, id_);
    setCompleted();
}

void Invitations::send_x()
{
    wSend_->disable();

    Wt::log("info") << "==================== send";
    std::string output;

    const std::string bodyTemplate= getBodyTemplate();
    std::string emailSubject= getEmailSubject();
    // const std::string emailSubject= "Negociacion Colectiva 2019: votacion para aprobar o rechazar oferta";
    const std::string originEmailName= "Votacion Sindicato";
    const std::string originEmailAddress= "votacion@sindicatoparanal.cl";

    if(testing_)
    {
        emailSubject= std::string("[testing] ") + emailSubject;
    }

    std::string sentence=
        "SELECT name, email, code "
        "FROM people "
        "WHERE idx_general=" + std::to_string(idxVoting_);

    pqxx::result answer;
    db_.execSql(sentence, answer);
    for(auto row: answer)
    {
        const std::string nameDestination= row[0].as<std::string>();
        const std::string emailDestination= row[1].as<std::string>();
        const std::string code= row[2].as<std::string>();
        Wt::log("info") << "sending to :" << nameDestination << " -- " << emailDestination;

        // std::string command= "sending to :" + name + " -- " + email;
        // Wt::log("info") << command;

        // Remove previous message body.
        std::string rm_command= "rm -f /tmp/voting/msg_body.html";
        Wt::log("info") << "rm_command: " << rm_command;
        system(rm_command.c_str());

        system("mkdir /tmp/voting");

        // Replace the name and code in the message body template.
        std::string content= bodyTemplate;
        boost::replace_first(content, "CODE", code);
        boost::replace_first(content, "NAME", nameDestination);
        Command::writeFile("/tmp/voting/msg_body.html", content);
        Wt::log("info") << "just write file";

        // Generate command
        std::string mail_command=
            "cat /tmp/voting/msg_body.html "
            "| mail -s \"" + emailSubject  + "\" "
            "-a \"Content-type: text/html\" " + emailDestination + " "
            "-aFrom:\"" + originEmailName + "\"\\<" + originEmailAddress + "\\>";

        Wt::log("info") << "mail_command: " << mail_command;

        output+= mail_command + "\n\n";
        wOutput_->setText(output);
        Wt::WApplication::instance()->processEvents();
        // std::string output = execute("ls -l");
        // commandOutput_->setText(output);

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // Upon no error
    notify(COMPLETED, id_);
    setCompleted();
    wSend_->enable();
}

/*
void Invitations::setup(
    const int &type,
    const int &value)
{
    if(type == SELECTED)
    {
        if(idxVoting_ != value)
        {
            idxVoting_= value;
            wOutput_->setText("");
        }

        if(isCompleted())
        {
            wSend_->disable();
            notify(COMPLETED, id_);
        }
        else
        {
            wSend_->enable();
        }
    }
}
*/

std::string Invitations::getBodyTemplate()
{
    std::string result;

    Wt::WString sentence=
        "SELECT convocatory "
        "FROM general "
        "WHERE idx={1}";

    sentence.arg(idxVoting_);

    pqxx::result answer;
    auto status= db_.execSql(sentence.toUTF8(), answer);
    if(status == NO_ERROR)
    {
        if(answer.begin() != answer.end())
        {
            auto row= answer.begin();
            result= row[0].as<std::string>();
        }
    }
    else
    {
        wOut_->setText(status);
    }
    return result;


    /*
    std::string sentence=
        "SELECT convocatory "
        "FROM general "
        "WHERE idx=" + std::to_string(idxVoting_);

    pqxx::result answer;
    db_.execSql(sentence, answer);
    auto row= answer.begin();
    const std::string result= row[0].as<std::string>();
    return result;
    */
}

std::string Invitations::getEmailSubject()
{
    std::string result;

    // Replace 'name' with 'subject'. <------------- !!
    Wt::WString sentence=
        "SELECT name "
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
            result= row[0].as<std::string>();
        }
    }
    else
    {
        wOut_->setText(status);
    }
    return result;

    /*
    pqxx::result answer;
    db_.execSql(sentence, answer);
    auto row= answer.begin();
    const std::string result= row[0].as<std::string>();
    return result;
    */
}

/*
void Invitations::writeFile(
    const std::string &location,
    const std::string &content)
{
    std::ofstream file;
    file.open(location.c_str());
    file << content;
    file.close();
}
*/
