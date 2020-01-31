// C++
#include <thread>
#include <chrono>
#include <ostream>
#include <sstream>
#include <iostream>
#include <fstream>

// Voting
#include "invitations.h"

Invitations::Invitations(const Postgresql &db): Panel(db)
{
    settingType_= TYPE_ADMIN;
    step_= STEP_3;
    description_= "Sending invitations";

    setTitle();
    // std::string msg= "<h3>Send invitation to voters</h3>";
    // addWidget(std::make_unique<Wt::WText>(msg));

    wSend_ = addWidget(std::make_unique<Wt::WPushButton>("Send"));
    wSend_->clicked().connect(this, &Invitations::send);

    wOutput_= addWidget(std::make_unique<Wt::WTextArea>());
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
                wSend_->disable();
                notify(COMPLETED, id_);
            }
            else
            {
                // This step is not completed, is active.
                wSend_->enable();
                notify(INCOMPLETE, step_);

                // Create new widgets.
                std::string sentence=
                    "SELECT type "
                    "FROM general "
                    "WHERE idx=" + std::to_string(idxVoting_);

                pqxx::result answer;
                db_.execSql(sentence, answer);
                auto row= answer.begin();
                votingType_= row[0].as<int>();

                Wt::log("info") << "voting type is: " << votingType_;
            }
        }
    }
}

void Invitations::send()
{
    wSend_->disable();

    Wt::log("info") << "==================== send";
    std::string output;

    const std::string bodyTemplate= getBodyTemplate();
    // const std::string emailSubject= getEmailSubject();
    const std::string emailSubject= "Negociacion Colectiva 2019: votacion para aprobar o rechazar oferta";
    const std::string originEmailName= "Votacion Sindicato";
    const std::string originEmailAddress= "votacion@sindicatoparanal.cl";

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
        writeFile("/tmp/voting/msg_body.html", content);
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
    std::string sentence=
        "SELECT convocatory "
        "FROM general "
        "WHERE idx=" + std::to_string(idxVoting_);

    pqxx::result answer;
    db_.execSql(sentence, answer);
    auto row= answer.begin();
    const std::string result= row[0].as<std::string>();
    return result;
}

void Invitations::writeFile(
    const std::string &location,
    const std::string &content)
{
    std::ofstream file;
    file.open(location.c_str());
    file << content;
    file.close();
}
