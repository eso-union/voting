// C
#include <cassert>

// Wt
#include <Wt/WLabel.h>

// Voting
#include "alternative.h"

class DescriptionTag: public Wt::WContainerWidget
{
    public:

        DescriptionTag(
            const std::string &description,
            const std::string &tag)
        {
            auto wRow= addWidget(std::make_unique<Wt::WContainerWidget>());
            wRow->addStyleClass("row");

            auto wCell0= wRow->addWidget(std::make_unique<Wt::WContainerWidget>());
            wCell0->addStyleClass("col-xs-8");
            wDesc_= wCell0->addWidget(std::make_unique<Wt::WLineEdit>(description));
            wDesc_->keyWentDown().connect(
                [=] (const Wt::WKeyEvent& e)
                {
                    notify(CHANGED, EMPTY);
                });

            auto wCell1= wRow->addWidget(std::make_unique<Wt::WContainerWidget>());
            wCell1->addStyleClass("col-xs-4");
            wTag_= wCell1->addWidget(std::make_unique<Wt::WLineEdit>(tag));
            wTag_->keyWentDown().connect(
                [=] (const Wt::WKeyEvent& e)
                {
                    notify(CHANGED, EMPTY);
                });

            // Verify pointers
            assert(wDesc_ != nullptr);
            assert(wTag_  != nullptr);
        }

        DescriptionTag(): DescriptionTag{"", ""}{};

        std::string getDescription()
        {
            std::string value= wDesc_->text().toUTF8();
            return value;
        }

        std::string getTag()
        {
            std::string value= wTag_->text().toUTF8();
            return value;
        }

        boost::signals2::signal<void(int, int)> notify;

    private:

        Wt::WLineEdit *wDesc_= nullptr;
        Wt::WLineEdit *wTag_ = nullptr;
};

Alternative::Alternative(const Postgresql &db): Panel(db)
{
    settingType_= TYPE_CONFIG;
    step_= STEP_3;
    description_= "Setting up voting alternatives";

    setTitle();

    // The button to remove an alternative
    auto wRemove=
        wCanvas_->addWidget(
            std::make_unique<Wt::WPushButton>("Remove Alternative"));
    wRemove->addStyleClass("btn btn-warning");
    wRemove->clicked().connect(this, &Alternative::remove);

    // The button to add an alternative
    auto wAdd=
        wCanvas_->addWidget(
            std::make_unique<Wt::WPushButton>("Add Alternative"));
    wAdd->addStyleClass("btn btn-primary");
    wAdd->clicked().connect(this, &Alternative::add);

    // A description for the user
    auto wRow= wCanvas_->addWidget(std::make_unique<Wt::WContainerWidget>());
    wRow->addStyleClass("row");

    auto wCell0= wRow->addWidget(std::make_unique<Wt::WContainerWidget>());
    wCell0->addStyleClass("col-xs-8");
    wCell0->addWidget(std::make_unique<Wt::WLabel>("Description"));

    auto wCell1= wRow->addWidget(std::make_unique<Wt::WContainerWidget>());
    wCell1->addStyleClass("col-xs-4");
    wCell1->addWidget(std::make_unique<Wt::WLabel>("Tag (low case, no spaces)"));
}

void Alternative::save()
{
    if(!checkValues())
    {
        return;
    }

    std::vector<std::string> bundle;

    Wt::WString delSentence=
        "DELETE FROM alternative "
        "WHERE idx_general={1}";

    delSentence.arg(idxVoting_);
    bundle.push_back(delSentence.toUTF8());

    for(auto &e: cInput_)
    {
        Wt::WString insertSentence=
            "INSERT INTO alternative(idx_general, value, tag) "
            "VALUES({1}, '{2}', '{3}')";

        insertSentence
            .arg(idxVoting_)
            .arg(e->getDescription())
            .arg(e->getTag());

        bundle.push_back(insertSentence.toUTF8());
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

void Alternative::setup(
    const int &type,
    const int &value)
{
    if(type == SELECTED)
    {
        if(idxVoting_ != value)
        {
            removeAll();

            idxVoting_= value;

            Wt::WString sentence=
                "SELECT value, tag "
                "FROM alternative "
                "WHERE idx_general={1};";

            sentence.arg(idxVoting_);
            pqxx::result answer;
            auto status= db_.execSql(sentence.toUTF8(), answer);
            if(status == NO_ERROR)
            {
                for(auto row: answer)
                {
                    std::string desc= row[0].as(std::string());
                    std::string tag=  row[1].as(std::string());
                    add(desc, tag);
                }
            }
            else
            {
                wOut_->setText(status);
                return;
            }
        }

        if(isCompleted())
        {
            notify(COMPLETED, id_);
        }
    }
}

void Alternative::add()
{
    add("", "");
}

void Alternative::add(
    const std::string &description,
    const std::string &tag)
{
    DescriptionTag *wDescTag=
        wCanvas_->addWidget(std::make_unique<DescriptionTag>(description, tag));
    wDescTag->setInline(false);
    wDescTag->notify.connect(
        [=] (const int &key, const int &value)
        {
            notify(CHANGED, EMPTY);
        });
    cInput_.push_back(wDescTag);
}

void Alternative::remove()
{
    if(cInput_.size()>0)
    {
        wCanvas_->removeWidget(cInput_[cInput_.size()-1]);
        cInput_.pop_back();
        notify(CHANGED, EMPTY);
    }
}

void Alternative::removeAll()
{
    while(cInput_.size() > 0)
    {
        remove();
    }
}

bool Alternative::checkValues()
{
    // Description and tag must not empty
    for(auto &e: cInput_)
    {
        if((e->getDescription() == "") ||
           (e->getTag() == ""))
        {
            wOut_->setText("Description and tag must not empty.");
            return false;
        }
    }

    // No spaces allowed in tags
    for(auto &e: cInput_)
    {
        auto pos= e->getTag().find(" ");
        if(pos != std::string::npos)
        {
            wOut_->setText("Tags must not have spaces.");
            return false;
        }
    }

    // Tags must be unique
    for(int i=0; i<cInput_.size(); i++)
    {
        for(int j=0; j<cInput_.size(); j++)
        {
            if(i != j)
            {
                if(cInput_[i]->getTag() == cInput_[j]->getTag())
                {
                    wOut_->setText("Tags must be unique.");
                    return false;
                }
            }
        }
    }

    // Description must be unique
    for(int i=0; i<cInput_.size(); i++)
    {
        for(int j=0; j<cInput_.size(); j++)
        {
            if(i != j)
            {
                if(cInput_[i]->getDescription() == cInput_[j]->getDescription())
                {
                    wOut_->setText("Descriptions must be unique.");
                    return false;
                }
            }
        }
    }

    return true;
}
