// C++
#include <sstream>
#include <iostream>
#include <fstream>

// Boost
#include <boost/signals2.hpp>
#include <boost/random/random_device.hpp>
#include <boost/random/uniform_int_distribution.hpp>

// Wt
#include <Wt/WFileUpload.h>
#include <Wt/WProgressBar.h>

// Voting
#include "people.h"

People::People(const Postgresql &db): Panel(db)
{
    settingType_= TYPE_CONFIG;
    step_= STEP_5;
    description_= "Setting up the list of voters";

    setTitle();

    auto rowA=
        wCanvas_->addWidget(std::make_unique<Wt::WContainerWidget>());
    rowA->addStyleClass("row");

    auto buttonRemove=
        rowA->addWidget(
            std::make_unique<Wt::WPushButton>("Remove Alternative"));
    buttonRemove->addStyleClass("btn btn-warning");
    buttonRemove->clicked().connect(this, &People::remove);

    auto buttonAdd=
        rowA->addWidget(
            std::make_unique<Wt::WPushButton>("Add Alternative"));
    buttonAdd->addStyleClass("btn btn-primary");
    buttonAdd->clicked().connect(this, &People::add);

    auto rowC=
        wCanvas_->addWidget(std::make_unique<Wt::WContainerWidget>());

    Wt::WFileUpload *fu= rowC->addWidget(
                std::make_unique<Wt::WFileUpload>());
    fu->setFileTextSize(10); // Set the maximum file size in kB.
    fu->setProgressBar(std::make_unique<Wt::WProgressBar>());
    fu->setMargin(10, Wt::Side::Right);

    fu->changed().connect([=] {
        fu->upload();
        wOut_->setText("File upload is changed.");
    });

    // React to a succesfull upload.
    fu->uploaded().connect([=] {
        wOut_->setText("File upload is finished.");
        Wt::log("info") << "File is located:'" << fu->spoolFileName() << "'";
        fill(fu->spoolFileName());
        notify(CHANGED, EMPTY);
    });

    // React to a file upload problem.
    fu->fileTooLarge().connect([=] {
        wOut_->setText("File is too large.");
    });

    auto rowB=
        wCanvas_->addWidget(std::make_unique<Wt::WContainerWidget>());
    rowB->addStyleClass("row");

    auto cell0=
        rowB->addWidget(std::make_unique<Wt::WContainerWidget>());
    cell0->addStyleClass("col-xs-6");
    cell0->addWidget(std::make_unique<Wt::WText>("Name"));

    auto cell1=
        rowB->addWidget(std::make_unique<Wt::WContainerWidget>());
    cell1->addStyleClass("col-xs-6");
    cell1->addWidget(std::make_unique<Wt::WText>("Email address"));
}

void People::setup(
    const int &type,
    const int &value)
{
    if(type == SELECTED)
    {
        if(idxVoting_ != value)
        {
            idxVoting_= value;
            setData();
        }
    }

    if(isCompleted())
    {
        notify(COMPLETED, id_);
    }
    else
    {
        notify(INCOMPLETE, id_);
    }
}

void People::setData()
{
    removeAll();

    Wt::WString sentence=
        "SELECT name, email "
        "FROM people "
        "WHERE idx_general={1};";

    sentence.arg(idxVoting_);

    pqxx::result answer;
    auto status= db_.execSql(sentence.toUTF8(), answer);
    if(status == NO_ERROR)
    {
        for(auto row: answer)
        {
            const std::string name= row[0].as<std::string>();
            const std::string email= row[1].as<std::string>();
            add(name, email);
        }
        setSaved();
    }
    else
    {
        wOut_->setText(status);
    }
}

void People::save()
{
    std::vector<std::string> codeList=
        generateCodes(cPeopleList_.size());

    std::vector<std::string> bundle;

    std::string delSentence=
        "DELETE FROM people "
        "WHERE idx_general=" + std::to_string(idxVoting_);
    bundle.push_back(delSentence);

    int i= 0;
    for(auto &e: cPeopleList_)
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

    auto status= db_.execSql(bundle);
    if(status != NO_ERROR)
    {
        wOut_->setText(status);
        return;
    }

    setSaved();
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
    auto w= wCanvas_->addWidget(std::move(ne));
    cPeopleList_.push_back(w);
}

void People::remove()
{
    if(cPeopleList_.size()>0)
    {
        wCanvas_->removeWidget(cPeopleList_[cPeopleList_.size()-1]);
        cPeopleList_.pop_back();
        notify(CHANGED, EMPTY);
    }
}

void People::removeAll()
{
    while(cPeopleList_.size() > 0)
    {
        remove();
    }
}

std::vector<std::string>
    People::generateCodes(const unsigned int &size)
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

void People::fill(const std::string &filename)
{
    removeAll();

    std::string content= getFileContents(filename);

    std::string line;
    std::stringstream sstream(content);

    try
    {
        while(std::getline(sstream, line, '\n'))
        {
            if(line != "")
            {
                Wt::log("info") << "line:" << line;

                int pos= line.find(",");
                std::string email= line.substr(0,pos);
                std::string name= line.substr(pos+1);
                // people_list.push_back(union_member);
                add(name, email);
            }
        }
        // generate_codes();
        // insert_into_db();
    }
    catch(const std::exception &e)
    {
        Wt::log("error") << e.what();
        wOut_->setText(e.what());
    }
    catch(...)
    {
        Wt::log("error") << "Unhandled exception";
        wOut_->setText("Unhandled exception");
    }
}

std::string
    People::getFileContents(const std::string &filename)
{
    std::string content;
    std::ifstream input_file(filename.c_str(), std::ios::in | std::ios::binary);
    if(input_file)
    {
        input_file.seekg(0, std::ios::end);
        content.resize(input_file.tellg());
        input_file.seekg(0, std::ios::beg);
        input_file.read(&content[0], content.size());
        input_file.close();
    }
    return(content);
}
