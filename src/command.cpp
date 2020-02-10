// C++
#include <thread>
#include <chrono>
#include <ostream>
#include <sstream>
#include <iostream>
#include <fstream>

// Boost
#include <boost/algorithm/string.hpp>
#include <boost/signals2.hpp>
#include <boost/random/random_device.hpp>
#include <boost/random/uniform_int_distribution.hpp>

// Wt
#include <Wt/WString.h>

// Voting
#include "command.h"
#include "common.h"

std::string
    Command::execute(const std::string &command)
{
    std::string output;
    const int BUFSIZE = 128;
    char buf[BUFSIZE];
    FILE *fp;

    if((fp = popen(command.c_str(), "r")) == NULL)
    {
        return("error opening pipe");
    }

    while(fgets(buf, BUFSIZE, fp) != NULL)
    {
        output+= buf;
    }

    if(pclose(fp))
    {
        return("error: command not found or exited with error status");
    }

    return output;
}

void Command::execute(
        const std::string &command,
        Wt::WTextArea *wFeedback)
{
    const std::string promt= "> ";
    std::string output;
    const int BUFSIZE = 128;
    char buf[BUFSIZE];
    FILE *fp;

    output= "\n";
    output= promt + command + "\n";

    if((fp = popen(command.c_str(), "r")) == NULL)
    {
        output+= "error: failure opening pipe\n";
    }

    std::string result;
    while(fgets(buf, BUFSIZE, fp) != NULL)
    {
        result+= buf;
    }

    if(!result.empty())
    {
        output+= result + "\n";
    }

    if(pclose(fp))
    {
        output+= "error: command not found or exited with error status\n";
    }

    wFeedback->setText(wFeedback->text() + output);
}

void Command::sendInvitations(
    const int &idxVoting,
    Postgresql &db,
    Wt::WTextArea *wFeedback)
{
    std::string testing;
    if(Command::getTesting(db))
    {
        testing= "[testing] ";
    }

    const std::string bodyTemplate= getBodyTemplate(idxVoting, db);
    const std::string emailSubject= testing + getEmailSubject(idxVoting, db);
    const std::string votingLink= getVotingLink();
    const std::string originEmailName= getEmailName(db); // "Voting Call";
    const std::string originEmailAddress= getEmailAddress(db);  // "voting@somedomain.org";

    Wt::WString sentence=
        "SELECT name, email, code "
        "FROM people "
        "WHERE idx_general={1} "
        "AND used=false";

    sentence.arg(idxVoting);

    pqxx::result answer;
    auto status= db.execSql(sentence.toUTF8(), answer);
    if(status == NO_ERROR)
    {
        if(answer.begin() == answer.end())
        {
            wFeedback->setText("No one is pending, nothing to send.\n");
            return;
        }

        const std::string workingDir= std::string("/tmp/") + Command::rndSeq(10) + "/";
        const std::string workingFile= workingDir + "msg_body.html";

        std::string mkCommand= std::string("mkdir ") + workingDir;
        Command::execute(mkCommand);

        for(auto row: answer)
        {
            const std::string nameDestination= row[0].as<std::string>();
            const std::string emailDestination= row[1].as<std::string>();
            const std::string code= row[2].as<std::string>();

            // Remove previous message body.
            std::string rmCommand= std::string("rm -f ") + workingFile;
            Command::execute(rmCommand);

            // Replace the name and code in the message body template.
            std::string content= bodyTemplate;
            boost::replace_all(content, "CODE", code);
            boost::replace_all(content, "NAME", nameDestination);
            boost::replace_all(content, "LINK", votingLink);
            writeFile(workingFile, content);

            // Modify if it is plain text
            Wt::WString sendCommand=
                "cat {1} | mail -s \"{2}\" "
                "-a \"Content-type: text/html\" {3} "
                "-aFrom:\"{4}\"\\<{5}\\>";

            sendCommand
                .arg(workingFile)
                .arg(emailSubject)
                .arg(emailDestination)
                .arg(originEmailName)
                .arg(originEmailAddress);

            // Send the mail
            Command::execute(sendCommand.toUTF8(), wFeedback);
            wFeedback->setText(wFeedback->text() + "\n");

            // This force the redrawing of the web page,
            // among other events that may be pending of processing.
            Wt::WApplication::instance()->processEvents();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        std::string rmDirCommand= std::string("rm -rf ") + workingDir;
        Command::execute(rmDirCommand);
    }
    else
    {
        wFeedback->setText(status);
    }
}

std::string Command::rndSeq(const unsigned int &len)
{
    // Definion of allowed characters.
    std::string chars("ABCDEFGHIJKLMNPQRSTUVWXYZ123456789");

    // 'random_device' is used as a source of entropy
    // to make sequences that are not predictable.
    boost::random::random_device rng;

    // Random characters from the string are selected.
    std::string sequence;
    boost::random::uniform_int_distribution<> index_dist(0, chars.size() - 1);
    for(int i= 0; i < len; ++i)
    {
        sequence += chars[index_dist(rng)];
    }

    return sequence;
}

std::string Command::getBodyTemplate(
    const int &idxVoting,
    Postgresql &db)
{
    std::string result;

    Wt::WString sentence=
        "SELECT convocatory "
        "FROM general "
        "WHERE idx={1}";

    sentence.arg(idxVoting);

    pqxx::result answer;
    auto status= db.execSql(sentence.toUTF8(), answer);
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
        Wt::log("info") << "error: " << status;
    }
    return result;
}

std::string Command::getEmailSubject(
    const int &idxVoting,
    Postgresql &db)
{
    std::string result;

    // Replace 'name' with 'subject'. <------------- !!
    Wt::WString sentence=
        "SELECT name "
        "FROM general "
        "WHERE idx={1};";

    sentence.arg(idxVoting);

    pqxx::result answer;
    auto status= db.execSql(sentence.toUTF8(), answer);
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
        Wt::log("info") << "error: " << status;
    }
    return result;
}

std::string
    Command::getVotingLink()
{
    boost::property_tree::ptree tree;
    boost::property_tree::read_json(CONFIG_FILE, tree);
    std::string link= tree.get<std::string>("cast.link");
    return link;
}

void Command::writeFile(
    const std::string &location,
    const std::string &content)
{
    std::ofstream file;
    file.open(location.c_str());
    file << content;
    file.close();
}

std::string Command::toString(const bool &value)
{
    if(value)
    {
        return "true";
    }
    return "false";
}

std::string
    Command::genDetail(const std::vector<int> &options)
{
    Wt::WString msg;
    if(options.size() == 0)
    {
        msg= "But you may only send a blank vote.";
    }
    else if(options.size() == 1)
    {
        if(options[0] == 0)
        {
            msg= "But you may only send a blank vote.";
        }
        else
        {
            msg= "But may vote for {1} alternatives.";
            msg.arg(options[0]);
        }
    }
    else
    {
        msg+= "But may vote for ";
        for(int i=0; i<(options.size()-1); i++)
        {
            msg+= std::to_string(options[i]);
        }
        msg+= " or ";
        msg+= std::to_string(options[options.size()-1]);
        msg+= " alternatives.";
    }
    return msg.toUTF8();
}

dbConfig
    Command::getDbConfig(
        const std::string &filename)
{
    dbConfig par;
    boost::property_tree::ptree tree;
    boost::property_tree::read_json(filename, tree);
    par.dbname= tree.get<std::string>("db.name");
    par.host  = tree.get<std::string>("db.host");
    par.user  = tree.get<std::string>("db.user");
    par.passw = tree.get<std::string>("db.passw");
    return par;
}

emailConfig
    Command::getEmailConfig(
        const std::string &filename)
{
    emailConfig par;
    boost::property_tree::ptree tree;
    boost::property_tree::read_json(filename, tree);
    par.name    = tree.get<std::string>("email.name");
    par.address = tree.get<std::string>("email.address");
    return par;
}

void Command::setEmailConfig(
    Postgresql &db,
    const emailConfig &par)
{
    Wt::WString delSentence=
        "DELETE FROM machine ";

    Wt::WString insSentence=
        "INSERT INTO machine(email_name, email_address) "
        "VALUES('{1}', '{2}')";

    insSentence
        .arg(par.name)
        .arg(par.address);

    std::vector<std::string> bundle;
    bundle.push_back(delSentence.toUTF8());
    bundle.push_back(insSentence.toUTF8());
    db.execSql(bundle);
}

bool Command::getTesting(Postgresql &db)
{
    std::string sentence=
        "SELECT testing "
        "FROM general";
    bool value= db.extract<bool>(sentence);
    return value;
}

std::string Command::getEmailName(Postgresql &db)
{
    std::string sentence=
        "SELECT email_name "
        "FROM machine";
    std::string value= db.extract<std::string>(sentence);
    return value;
}

std::string Command::getEmailAddress(Postgresql &db)
{
    std::string sentence=
        "SELECT email_address "
        "FROM machine";
    std::string value= db.extract<std::string>(sentence);
    return value;
}
