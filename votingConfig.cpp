/**********************************************************************
 * Voting System
 *
 * Copyright (C) 2019, Nicolas Slusarenko
 *                     nicolas@thecppdev.com
 *
 * This file is part of Voting System.
 *
 * The Voting System is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The Voting System is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the Voting System. If not, see <http://www.gnu.org/licenses/>.
 **********************************************************************/

/**********************************************************************
 * Compilation
 * $ g++ votingConfig.cpp -o ./voting_config -lwthttp -lwt
 *
 * Execution
 * $ ./voting_config --docroot . --http-address 0.0.0.0 --http-port 8080
 **********************************************************************/

// C++
#include <vector>
#include <memory>
#include <thread>
#include <chrono>

// Wt
#include <Wt/WApplication.h>
#include <Wt/WBootstrapTheme.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WFormWidget.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WStackedWidget.h>
#include <Wt/WTextArea.h>
#include <Wt/WTemplate.h>
#include <Wt/WCalendar.h>
#include <Wt/WDate.h>
#include <Wt/WDateEdit.h>
#include <Wt/WTimeEdit.h>
#include <Wt/WCheckBox.h>
#include <Wt/WSpinBox.h>
#include <Wt/WRadioButton.h>

// Boost
#include <boost/signals2.hpp>

// Voting
#include "postgresql.h"

const int READY        = 0;
const int CHANGED      = 1;
const int GRANT_ACCESS = 2;
const int LOGOUT       = 3;
const int SELECTED     = 4;

class WidgetPanel: public Wt::WContainerWidget
{
    public:

        virtual void save()
        {
            std::cout << "[parent] saving ...\n";
        };

        virtual void setup(
            const int &type,
            const int &value)
        {
            std::cout << "[parent] setting up ...\n";
        }

        boost::signals2::signal<void(int)> notify;
        // boost::signals2::signal<void(int, int)> setup;
};

class VotingType : public WidgetPanel {
    public:

        VotingType(
            const Postgresql &db,
            int &indexVoting): db_(db)
        {
            addWidget(std::make_unique<Wt::WText>(
                "<h3>Choose the type of voting</h3>"));

            testingButton_ = addNew<Wt::WRadioButton>("Testing Voting");
            testingButton_->setInline(false);
            if(testingEnabled_) {
                testingButton_->enable();
            } else {
                testingButton_->disable();
            }

            realButton_ = addNew<Wt::WRadioButton>("Real Voting");
            realButton_->setInline(false);
            if(realEnabled_) {
                realButton_->enable();
            } else {
                realButton_->disable();
            }
        }

        virtual void save()
        {
            std::cout << "[VotingType] saving ...\n";
            notify(READY);
        };

    private:

        Postgresql db_;

        Wt::WRadioButton *testingButton_;
        Wt::WRadioButton *realButton_;

        bool testingEnabled_= false;
        bool testingConfigured_= false;

        bool realEnabled_;
        bool realConfigured_;
};

/**
 *
 **/
class NameEmail : public Wt::WContainerWidget
{
    public:

        NameEmail(
            const std::string &name,
            const std::string &email
        ) {
            auto row = addWidget(std::make_unique<Wt::WContainerWidget>());
            row->addStyleClass("row");

            auto cell0 = row->addWidget(std::make_unique<Wt::WContainerWidget>());
            cell0->addStyleClass("col-xs-6");
            inputName_= cell0->addWidget(std::make_unique<Wt::WLineEdit>(name));
            inputName_->keyPressed().connect(
                [=] (const Wt::WKeyEvent& e)
                {
                    notify(CHANGED);
                });

            auto cell1 = row->addWidget(std::make_unique<Wt::WContainerWidget>());
            cell1->addStyleClass("col-xs-6");
            inputEmail_= cell1->addWidget(std::make_unique<Wt::WLineEdit>(email));
            inputEmail_->keyPressed().connect(
                [=] (const Wt::WKeyEvent& e)
                {
                    notify(CHANGED);
                });
        }

        NameEmail(): NameEmail{"", ""}{};

        std::string getName()
        {
            std::string name;
            if(inputName_ != nullptr)
            {
                name= inputName_->text().toUTF8();
            }
            return name;
        }

        std::string getEmail()
        {
            std::string email;
            if(inputEmail_ != nullptr)
            {
                email= inputEmail_->text().toUTF8();
            }
            return email;
        }

        boost::signals2::signal<void(int)> notify;

    private:

        Wt::WLineEdit *inputName_= nullptr;
        Wt::WLineEdit *inputEmail_= nullptr;
};

/**
 *
 **/
class VotingPeople : public WidgetPanel
{
    public:

        VotingPeople(
            const Postgresql &db,
            int &indexVoting): db_(db)
        {
            addWidget(std::make_unique<Wt::WText>(
                "<h3>Enter the list of <em>email,name</em> of voters</h3>"));

            addWidget(std::make_unique<Wt::WText>(
                "<h4>You may delay this step util you have done all the testing.</h4>"));

            emailVoters_= addWidget(std::make_unique<Wt::WTextArea>());
            emailVoters_->setColumns(40);
            emailVoters_->setRows(10);
            emailVoters_->setPlaceholderText(
                "some@email.com,Some Name\n"
                "another@email.com,Another Name");

            emailVoters_->keyPressed().connect(
                [=] (const Wt::WKeyEvent& e)
                {
                    std::cout << "key press ...\n";
                    notify(CHANGED);
                });
        }

        virtual void save()
        {
            std::cout << "[VotingPeople] saving ...\n";
            notify(READY);
        };

    private:

        Postgresql db_;
        Wt::WTextArea *emailVoters_;
};

/**
 *
 **/
class VotingTesters : public WidgetPanel
{
    public:

        VotingTesters(
            const Postgresql &db,
            int &indexVoting): db_(db)
        {
            addWidget(std::make_unique<Wt::WText>("<h3>[optional] Name and email for testing</h3>"));

            auto row = addWidget(std::make_unique<Wt::WContainerWidget>());
            row->addStyleClass("row");

            auto cell0 = row->addWidget(std::make_unique<Wt::WContainerWidget>());
            cell0->addStyleClass("col-xs-6");
            cell0->addWidget(std::make_unique<Wt::WText>("Name"));

            auto cell1 = row->addWidget(std::make_unique<Wt::WContainerWidget>());
            cell1->addStyleClass("col-xs-6");
            cell1->addWidget(std::make_unique<Wt::WText>("Email address"));

            for(int i=0; i<5; i++)
            {
                auto ne = std::make_unique<NameEmail>();
                ne->notify.connect(boost::bind(&VotingTesters::dataChanged, this, _1));
                auto w= addWidget(std::move(ne));
                testersList_.push_back(w);
            }
        }

        /*
        int size() const
        {
            return testersList.size();
        }

        std::string name(const int &index) const
        {
            const std::string name= testersList.at(index)->getName();
            return name;
        }

        std::string email(const int &index) const
        {
            const std::string email= testersList.at(index)->getEmail();
            return email;
        }
        */

        virtual void setup(
            const int &type,
            const int &value)
        {
            if(type == SELECTED)
            {
                selected_= value;

                std::string sentence=
                    "SELECT name, email "
                    "FROM testers "
                    "WHERE idx_general=" + std::to_string(selected_);

                std::cout << "sentence:" << sentence << "\n";

                pqxx::result answer;
                db_.execSql(sentence, answer);
                pqxx::result::iterator row= answer.begin();
                if(row != answer.end())
                {
                    std::string name= row[0].as(std::string());
                    std::string email= row[1].as(std::string());
                    auto ne = std::make_unique<NameEmail>(name, email);
                    ne->notify.connect(boost::bind(&VotingTesters::dataChanged, this, _1));
                    auto w= addWidget(std::move(ne));
                    testersList_.push_back(w);
                    row++;
                }
            }
        }

        virtual void save()
        {
            std::cout << "[VotingTesters] saving ...\n";
            std::vector<std::string> bundle;

            // Error handling, try-catch.
            std::string delSentence=
                "DELETE FROM testers "
                "WHERE idx_general=" + std::to_string(selected_);
            bundle.push_back(delSentence);

            for(auto &e: testersList_)
            {
                const std::string name= e->getName();
                const std::string email= e->getEmail();
                if((name != "") && (email != ""))
                {
                    std::string insertSentence=
                        "INSERT INTO testers(idx_general, name, email) "
                        "VALUES(" + std::to_string(selected_) + ", '" + name + "', '" + email + "')";
                    bundle.push_back(insertSentence);
                }
            }

            db_.execSql(bundle);
            notify(READY);
        };

        void dataChanged(int value)
        {
            std::cout << "Tester voters changed!\n";
            notify(value);
        }

    private:

        int selected_= -1;
        Postgresql db_;
        std::vector<NameEmail*> testersList_;
};

/**
 *
 **/
class VotePosibilities : public WidgetPanel
{
    public:

        VotePosibilities(
            const Postgresql &db,
            int &indexVoting): db_(db)
        {
            addStyleClass("container");

            addWidget(std::make_unique<Wt::WText>("<h3>Voting possibilities</h3>"));

            Wt::WCheckBox *cb;

            cb = addNew<Wt::WCheckBox>("Blank, no one selected.");
            cb->setInline(false);

            cb = addNew<Wt::WCheckBox>("1 of the alternatives.");
            cb->setInline(false);

            cb = addNew<Wt::WCheckBox>("2 of the alternatives.");
            cb->setInline(false);

            cb = addNew<Wt::WCheckBox>("3 of the alternatives.");
            cb->setInline(false);
        }

        virtual void save()
        {
            std::cout << "[VotePosibilities] saving ...\n";
        };

    private:

        Postgresql db_;
};

/**
 *
 **/
class VoteAlternatives : public WidgetPanel
{
    public:

        VoteAlternatives(
            const Postgresql &db,
            int &indexVoting): db_(db)
        {
            addStyleClass("container");

            addWidget(std::make_unique<Wt::WText>("<h3>Voting alternatives</h3>"));

            auto buttonRemove = addWidget(std::make_unique<Wt::WPushButton>("Remove Alternative"));
            buttonRemove->addStyleClass("btn btn-warning");
            buttonRemove->clicked().connect(this, &VoteAlternatives::remove);

            auto buttonAdd = addWidget(std::make_unique<Wt::WPushButton>("Add Alternative"));
            buttonAdd->addStyleClass("btn btn-primary");
            buttonAdd->clicked().connect(this, &VoteAlternatives::add);
        }

        virtual void save()
        {
            std::cout << "[VoteAlternatives] saving ...\n";
            std::vector<std::string> bundle;

            // Error handling, try-catch.
            std::string delSentence=
                "DELETE FROM alternatives "
                "WHERE idx_general=" + std::to_string(selected_);
            bundle.push_back(delSentence);

            for(auto &e: input_)
            {
                std::string insertSentence=
                    "INSERT INTO alternatives(idx_general, value) "
                    "VALUES(" + std::to_string(selected_) + ", '" + e->text().toUTF8() + "')";
                bundle.push_back(insertSentence);
            }

            db_.execSql(bundle);
            notify(READY);
        };

        virtual void setup(
            const int &type,
            const int &value)
        {
            if(type == SELECTED)
            {
                selected_= value;

                std::string sentence=
                    "SELECT value "
                    "FROM alternatives "
                    "WHERE idx_general=" + std::to_string(selected_);

                std::cout << "sentence:" << sentence << "\n";

                pqxx::result answer;
                db_.execSql(sentence, answer);
                pqxx::result::iterator row= answer.begin();
                if(row != answer.end())
                {
                    std::cout << "row[0]:" << row[0].as(std::string()) << "\n";
                    add(row[0].as(std::string()));
                    row++;
                }
            }
        }

    private:

        int selected_= -1;
        Postgresql db_;
        std::vector<Wt::WLineEdit*> input_;

        void add()
        {
            add("");
        }

        void add(const std::string &text)
        {
            Wt::WLineEdit *edit = addWidget(std::make_unique<Wt::WLineEdit>(text));
            edit->addStyleClass("form-control");
            edit->setPlaceholderText("Alternative " + std::to_string(input_.size()));
            edit->setInline(false);
            edit->keyPressed().connect(
                [=] (const Wt::WKeyEvent& e)
                {
                    notify(CHANGED);
                });
            input_.push_back(edit);
        }

        void remove()
        {
            if(input_.size()>0)
            {
                removeWidget(input_[input_.size()-1]);
                input_.pop_back();
                notify(CHANGED);
            }
        }
};

/**
 * Form: question or convocatory for the voters.
 **/
class VoterQuestion : public WidgetPanel
{
    public:

        VoterQuestion(
            const Postgresql &db,
            const int &indexVoting): db_(db)
        {
            addWidget(std::make_unique<Wt::WText>("<h3>General settings</h3>"));
            addWidget(std::make_unique<Wt::WText>("Convocatory to voters:"));

            convocatory_ = addWidget(std::make_unique<Wt::WTextArea>());
            convocatory_->setColumns(40);
            convocatory_->setRows(5);
            convocatory_->keyPressed().connect(
                [=] (const Wt::WKeyEvent& e)
                {
                    notify(CHANGED);
                });

            addWidget(std::make_unique<Wt::WText>("Encabezado en voto:"));

            question_ = addWidget(std::make_unique<Wt::WTextArea>());
            question_->setColumns(40);
            question_->setRows(2);
            question_->keyPressed().connect(
                [=] (const Wt::WKeyEvent& e)
                {
                    notify(CHANGED);
                });

            /**
             * Number of minister of faith.
             **/
            auto rowC = addWidget(std::make_unique<Wt::WContainerWidget>());
            rowC->addStyleClass("row");

            auto cellC0 = rowC->addWidget(std::make_unique<Wt::WContainerWidget>());
            cellC0->addStyleClass("col-xs-6");
            cellC0->addWidget(std::make_unique<Wt::WText>("Number of minister of faith:"));

            auto cellC1 = rowC->addWidget(std::make_unique<Wt::WContainerWidget>());
            cellC1->addStyleClass("col-xs-6");
            Wt::WSpinBox *sb = cellC1->addWidget(std::make_unique<Wt::WSpinBox>());
            sb->setRange(0,4);
            sb->valueChanged().connect(
                [=] (const int&)
                {
                    notify(CHANGED);
                });
        }

        virtual void save()
        {
            std::cout << "[VoterQuestion] saving ...\n";

            // Error handling, try-catch.
            std::string sentence=
                "UPDATE general "
                "SET convocatory='" + convocatory_->text().toUTF8() + "', "
                "header='" + question_->text().toUTF8() + "' "
                "WHERE idx=" + std::to_string(selected_);
            db_.execSql(sentence);
            notify(READY);
        };

        virtual void setup(
            const int &type,
            const int &value)
        {
            if(type == SELECTED)
            {
                selected_= value;

                std::string sentence=
                    "SELECT convocatory, header "
                    "FROM general "
                    "WHERE idx=" + std::to_string(value);

                pqxx::result answer;
                db_.execSql(sentence, answer);
                pqxx::result::const_iterator row= answer.begin();
                if(row != answer.end())
                {
                    convocatory_->setText(row[0].as(std::string()));
                    question_->setText(row[1].as(std::string()));
                }
            }
        }

    private:

        int selected_= -1;
        Postgresql db_;
        Wt::WTextArea *convocatory_= nullptr;
        Wt::WTextArea *question_= nullptr;
};

#include <Wt/WSelectionBox.h>
#include <Wt/WStringListModel.h>

/**
 *
 **/
class VotingSelection : public WidgetPanel
{
    public:

        VotingSelection(
            const Postgresql &db,
            int &indexVoting): db_(db)
        {
            addStyleClass("container");

            addWidget(std::make_unique<Wt::WText>("<h3>Select or Create Voting</h3>"));

            auto rowA = addWidget(std::make_unique<Wt::WContainerWidget>());
            rowA->addStyleClass("row");

            auto cellA0 = rowA->addWidget(std::make_unique<Wt::WContainerWidget>());
            cellA0->addStyleClass("col-md-4");
            // cellB0->addWidget(std::make_unique<Wt::WText>("Begining:"));

            std::string sentence= "SELECT idx, name FROM general;";
            pqxx::result answer;
            db_.execSql(sentence, answer);
            auto model= std::make_shared<Wt::WStringListModel>();
            pqxx::result::const_iterator row= answer.begin();
            int i= 0;
            if(row != answer.end())
            {
                std::cout
                    << "************ row: "
                    << row[0].as(std::string()) << ", "
                    << row[1].as(std::string()) << "\n";

                model->addString(row[1].as(std::string()));
                // model->setData(i, 0, row[0].as(std::string()), Wt::ItemDataRole::User);  // it works!
                model->setData(i, 0, row[0].as(int()), Wt::ItemDataRole::User);
                row++;
                i++;
            }

            Wt::WSelectionBox *sb1= cellA0->addWidget(std::make_unique<Wt::WSelectionBox>());
            sb1->setNoSelectionEnabled(true);
            sb1->setModel(model);
            sb1->setMargin(10, Wt::Side::Right);

            Wt::WText *out= addNew<Wt::WText>("");

            sb1->activated().connect(
                [=]
                {
                    std::string text= sb1->currentText().toUTF8();
                    int index = sb1->currentIndex();
                    // Wt::WString info= Wt::asString(model->data(model->index(index,0), Wt::ItemDataRole::User)); // it works!
                    // int info= Wt::asNumber(model->data(model->index(index,0), Wt::ItemDataRole::User));
                    selected_= Wt::asNumber(
                        model->data(
                            model->index(index, 0),
                            Wt::ItemDataRole::User));

                    // out->setText(Wt::WString("You selected {1}.").arg(sb1->currentText()));
                    out->setText(Wt::WString("You selected {1} with index {2}.").arg(text).arg(selected_));
                });

            useSelected_= cellA0->addWidget(std::make_unique<Wt::WPushButton>("Continue with selection"));
            useSelected_->addStyleClass("btn btn-warning");
            useSelected_->clicked().connect(
                [=]()
                {
                    tellSelected(selected_);
                });

            auto rowB= addWidget(std::make_unique<Wt::WContainerWidget>());
            rowB->addStyleClass("row");

            auto cellB0= rowB->addWidget(std::make_unique<Wt::WContainerWidget>());
            cellB0->addStyleClass("col-md-4");

            newName_= cellB0->addWidget(std::make_unique<Wt::WLineEdit>());

            auto createVoting = cellB0->addWidget(std::make_unique<Wt::WPushButton>("Create Voting"));
            createVoting->addStyleClass("btn btn-primary");
            createVoting->clicked().connect(this, &VotingSelection::create);
        }

        boost::signals2::signal<void(int)> tellSelected;

    private:

        Postgresql db_;
        int selected_= -1;
        Wt::WLineEdit   *newName_= nullptr;
        Wt::WPushButton *useSelected_= nullptr;

        void create()
        {

        }
};


/**
 * The message box for each step.
 **/
class StepInfo : public Wt::WContainerWidget
{
    public:

        StepInfo(
            const bool &valid,
            const std::string &text)
        {
            msgBox_ = addWidget(std::make_unique<Wt::WContainerWidget>());
            if(valid)
            {
                msgBox_->setStyleClass("alert alert-success");
            }
            else
            {
                msgBox_->setStyleClass("alert alert-warning");
            }
            msgText_ = msgBox_->addWidget(std::make_unique<Wt::WText>(text));
        }

        void setText(
            const bool &valid,
            const std::string &text)
        {
            if(valid)
            {
                msgBox_->setStyleClass("alert alert-success");
            }
            else
            {
                msgBox_->setStyleClass("alert alert-warning");
            }
            msgText_->setText(text);
        }

    private:

        Wt::WContainerWidget *msgBox_;
        Wt::WText *msgText_;
};

/**
 * The web widget.
 **/
// class GeneralLayout : public Wt::WContainerWidget
class GeneralLayout : public WidgetPanel
{
    public:

        GeneralLayout(const Postgresql &db): db_(db)
        {
            addStyleClass("container");

            /**
             * Row A
             **/
            auto rowA = addWidget(std::make_unique<Wt::WContainerWidget>());
            rowA->addStyleClass("row");

            auto cellA0 = rowA->addWidget(std::make_unique<Wt::WContainerWidget>());
            cellA0->addStyleClass("col-md-4");

            auto msgBox0 = cellA0->addWidget(std::make_unique<StepInfo>(true,  "<strong>Step1</strong> Setting up the voting."));
            auto msgBox1 = cellA0->addWidget(std::make_unique<StepInfo>(false, "<strong>Step2</strong> Setting up the voting."));
            auto msgBox2 = cellA0->addWidget(std::make_unique<StepInfo>(false, "<strong>Step3</strong> Setting up the voting."));
            auto msgBox3 = cellA0->addWidget(std::make_unique<StepInfo>(false, "<strong>Step4</strong> Setting up the voting."));
            auto msgBox4 = cellA0->addWidget(std::make_unique<StepInfo>(false, "<strong>Step5</strong> Setting up the voting."));

            auto cellA1 = rowA->addWidget(std::make_unique<Wt::WContainerWidget>());
            cellA1->addStyleClass("col-md-8");

            stack = cellA1->addNew<Wt::WStackedWidget>();

            auto vSel= std::make_unique<VotingSelection>(db_, indexVoting_);
            vSel->notify.connect(boost::bind(&GeneralLayout::dataChanged, this, _1));
            vSel->tellSelected.connect(boost::bind(&GeneralLayout::useSelected, this, _1));
            stack->addWidget(std::move(vSel));

            auto vQuest= std::make_unique<VoterQuestion>(db_, indexVoting_);
            vQuest->notify.connect(boost::bind(&GeneralLayout::dataChanged, this, _1));
            stack->addWidget(std::move(vQuest));

            auto vAlt= std::make_unique<VoteAlternatives>(db_, indexVoting_);
            vAlt->notify.connect(boost::bind(&GeneralLayout::dataChanged, this, _1));
            stack->addWidget(std::move(vAlt));

            auto vPos= std::make_unique<VotePosibilities>(db_, indexVoting_);
            vPos->notify.connect(boost::bind(&GeneralLayout::dataChanged, this, _1));
            stack->addWidget(std::move(vPos));

            auto vTest= std::make_unique<VotingTesters>(db_, indexVoting_);
            vTest->notify.connect(boost::bind(&GeneralLayout::dataChanged, this, _1));
            stack->addWidget(std::move(vTest));

            auto vPeople= std::make_unique<VotingPeople>(db_, indexVoting_);
            vPeople->notify.connect(boost::bind(&GeneralLayout::dataChanged, this, _1));
            stack->addWidget(std::move(vPeople));

            auto vType= std::make_unique<VotingType>(db_, indexVoting_);
            vType->notify.connect(boost::bind(&GeneralLayout::dataChanged, this, _1));
            stack->addWidget(std::move(vType));

            stack->setCurrentIndex(0);

            /**
             * Row B
             **/
            auto rowB = addWidget(std::make_unique<Wt::WContainerWidget>());
            rowB->addStyleClass("row");

            // cellB1
            auto cellB1 = rowB->addWidget(std::make_unique<Wt::WContainerWidget>());
            cellB1->addStyleClass("col-md-4");
            bLogout= cellB1->addWidget(std::make_unique<Wt::WPushButton>("Close"));
            bLogout->addStyleClass("btn btn-default btn-lg btn-block");
            bLogout->clicked().connect(
                [=]()
                {
                    notify(LOGOUT);
                });

            // cellB2 will have a row with two cells.
            auto cellB2 = rowB->addWidget(std::make_unique<Wt::WContainerWidget>());
            cellB2->addStyleClass("col-md-4");

            auto rowB2 = cellB2->addWidget(std::make_unique<Wt::WContainerWidget>());
            rowB2->addStyleClass("row");

            auto cellB21 = rowB2->addWidget(std::make_unique<Wt::WContainerWidget>());
            cellB21->addStyleClass("col-xs-6");

            bPrev = cellB21->addWidget(std::make_unique<Wt::WPushButton>("Previous"));
            bPrev->addStyleClass("btn btn-primary btn-lg btn-block");
            bPrev->clicked().connect(this, &GeneralLayout::previous);

            auto cellB22 = rowB2->addWidget(std::make_unique<Wt::WContainerWidget>());
            cellB22->addStyleClass("col-xs-6");

            bSave = cellB22->addWidget(std::make_unique<Wt::WPushButton>("Save"));
            bSave->addStyleClass("btn btn-success btn-lg btn-block");
            bSave->clicked().connect(this, &GeneralLayout::save);

            // cellB3 will have a row with two cells.
            auto cellB3 = rowB->addWidget(std::make_unique<Wt::WContainerWidget>());
            cellB3->addStyleClass("col-md-4");

            auto rowB3 = cellB3->addWidget(std::make_unique<Wt::WContainerWidget>());
            rowB3->addStyleClass("row");

            auto cellB31 = rowB3->addWidget(std::make_unique<Wt::WContainerWidget>());
            cellB31->addStyleClass("col-xs-6");

            bDiscard = cellB31->addWidget(std::make_unique<Wt::WPushButton>("Discard"));
            //bCancel->addStyleClass("btn btn-primary btn-lg btn-warning");
            bDiscard->addStyleClass("btn btn-warning btn-lg btn-block");
            bDiscard->clicked().connect(this, &GeneralLayout::discard);

            auto cellB32 = rowB3->addWidget(std::make_unique<Wt::WContainerWidget>());
            cellB32->addStyleClass("col-xs-6");

            bNext = cellB32->addWidget(std::make_unique<Wt::WPushButton>("Next"));
            bNext->addStyleClass("btn btn-primary btn-lg btn-block");
            bNext->clicked().connect(this, &GeneralLayout::next);
        }

    private:

        int indexVoting_= -1;
        Postgresql db_;
        Wt::WStackedWidget *stack;

        Wt::WPushButton *bPrev = nullptr;
        Wt::WPushButton *bSave = nullptr;
        Wt::WPushButton *bDiscard = nullptr;
        Wt::WPushButton *bNext = nullptr;
        Wt::WPushButton *bLogout = nullptr;

        // Propagate the notification to all panels in the stack.
        void useSelected(int value)
        {
            for(int i=0; i<stack->count(); i++)
            {
                Wt::WWidget *p= stack->widget(i);
                dynamic_cast<WidgetPanel*>(p)->setup(SELECTED, value);
            }
            next();
        }

        void previous()
        {
            int current= stack->currentIndex();
            if(current > 0)
            {
                current--;
                stack->setCurrentIndex(current);
            }
        }

        void next()
        {
            int current= stack->currentIndex();
            if(current < 6)
            {
                current++;
                stack->setCurrentIndex(current);
            }
        }

        void save()
        {
            Wt::WWidget *p= stack->currentWidget();
            dynamic_cast<WidgetPanel*>(p)->save();
        }

        void discard()
        {
            // Wt::WWidget *p= stack->currentWidget();
            // dynamic_cast<WidgetPanel*>(p)->discard();  // Nos implemented yet.
            dataChanged(READY);
        }

        void dataChanged(int value)
        {
            // std::cout << "Data changed! value: " << value << '\n';
            switch(value)
            {
                case READY:
                {
                    if((bPrev != nullptr) &&
                        (bSave != nullptr) &&
                        (bDiscard != nullptr) &&
                        (bNext != nullptr) &&
                        (bLogout != nullptr))
                    {
                        bPrev->enable();
                        bSave->disable();
                        bDiscard->disable();
                        bNext->enable();
                        bLogout->enable();
                    }
                    break;
                }

                case CHANGED:
                {
                    if((bPrev != nullptr) &&
                        (bSave != nullptr) &&
                        (bDiscard != nullptr) &&
                        (bNext != nullptr) &&
                        (bLogout != nullptr))
                    {
                        bPrev->disable();
                        bSave->enable();
                        bDiscard->enable();
                        bNext->disable();
                        bLogout->disable();
                    }
                    break;
                }
            }
        }
};

// class FrontAccess : public Wt::WContainerWidget
class FrontAccess: public WidgetPanel
{
    public:

        FrontAccess(
            const Postgresql &db,
            const std::string &lookup): db_(db), lookup_(lookup)
        {
            auto rowA = addWidget(std::make_unique<Wt::WContainerWidget>());
            rowA->addStyleClass("row");

            auto cellA0 = rowA->addWidget(std::make_unique<Wt::WContainerWidget>());
            cellA0->addStyleClass("col-md-4");

            auto cellA1 = rowA->addWidget(std::make_unique<Wt::WContainerWidget>());
            cellA1->addStyleClass("col-md-4");

            iPassword= cellA1->addWidget(std::make_unique<Wt::WLineEdit>());
            iPassword->setEchoMode(Wt::EchoMode::Password);
            iPassword->keyPressed().connect(
                [=] (const Wt::WKeyEvent& e)
                {
                    std::cout << "key press\n";
                    if(e.key() == Wt::Key::Enter)
                    {
                        std::cout << "right!\n";
                        if(bGo != nullptr)
                        {
                            verify();
                        }
                    }
                });

            auto cellA2 = rowA->addWidget(std::make_unique<Wt::WContainerWidget>());
            cellA2->addStyleClass("col-md-4");

            // rowB

            auto rowB = addWidget(std::make_unique<Wt::WContainerWidget>());
            rowB->addStyleClass("row");

            auto cellB0 = rowB->addWidget(std::make_unique<Wt::WContainerWidget>());
            cellB0->addStyleClass("col-md-4");

            auto cellB1 = rowB->addWidget(std::make_unique<Wt::WContainerWidget>());
            cellB1->addStyleClass("col-md-4");

            tResult= cellB1->addWidget(std::make_unique<Wt::WText>());

            auto cellB2 = rowB->addWidget(std::make_unique<Wt::WContainerWidget>());
            cellB2->addStyleClass("col-md-4");

            // rowC

            auto rowC = addWidget(std::make_unique<Wt::WContainerWidget>());
            rowC->addStyleClass("row");

            auto cellC0 = rowC->addWidget(std::make_unique<Wt::WContainerWidget>());
            cellC0->addStyleClass("col-md-4");

            auto cellC1 = rowC->addWidget(std::make_unique<Wt::WContainerWidget>());
            cellC1->addStyleClass("col-md-4");

            bGo= cellC1->addWidget(std::make_unique<Wt::WPushButton>("Go"));
            bGo->addStyleClass("btn btn-primary btn-lg btn-block btn-default");
            bGo->clicked().connect(
                [=]()
                {
                    verify();
                }
            );

            auto cellC2 = rowC->addWidget(std::make_unique<Wt::WContainerWidget>());
            cellC2->addStyleClass("col-md-4");
        }

    private:

        std::string lookup_;
        Postgresql db_;
        Wt::WPushButton *bGo= nullptr;
        Wt::WText *tResult= nullptr;
        Wt::WLineEdit *iPassword= nullptr;

        void verify()
        {
            std::string passw;
            std::cout << "Go!\n";\
            if(iPassword != nullptr)
            {
                passw= iPassword->text().toUTF8();
                if(passw == "hola")
                {
                    notify(GRANT_ACCESS);
                }
                else
                {
                    if((tResult != nullptr) && (iPassword != nullptr))
                    {
                        tResult->setText("wrong");
                        iPassword->disable();
                        bGo->disable();
                        Wt::WApplication::instance()->processEvents();
                        std::cout << "================ disabling ... ================\n";

                        std::this_thread::sleep_for(std::chrono::seconds(3));

                        iPassword->enable();
                        bGo->enable();
                        tResult->setText("");
                        iPassword->setText("");
                        Wt::WApplication::instance()->processEvents();
                        std::cout << "================ ... enabling ================\n";
                    }
                }
            }
        }
};

/*
class FrontSwitch : public Wt::WContainerWidget
{
    public:

        FrontSwitch(const Postgresql &db): db_(db)
        {
            addStyleClass("container");
            fa= addWidget(std::make_unique<FrontAccess>(db_));
            fa->notify.connect(boost::bind(&FrontSwitch::react, this, _1));
        }

    private:

        Postgresql db_;
        FrontAccess *fa = nullptr;
        GeneralLayout *gl = nullptr;

        void react(int value)
        {
            switch(value)
            {
                case GRANT_ACCESS:
                {
                    if(fa != nullptr)
                    {
                        removeWidget(fa);
                        gl= addWidget(std::make_unique<GeneralLayout>(db_));
                        gl->notify.connect(boost::bind(&FrontSwitch::react, this, _1));
                    }
                    break;
                }

                case LOGOUT:
                {
                    if(gl != nullptr)
                    {
                        removeWidget(gl);
                        fa= addWidget(std::make_unique<FrontAccess>(db_));
                        fa->notify.connect(boost::bind(&FrontSwitch::react, this, _1));
                    }
                }
            }
        }
};
*/

template <typename T>
class FrontSwitch : public Wt::WContainerWidget
{
    public:

        FrontSwitch(
            const Postgresql &db,
            const std::string &lookup): db_(db), lookup_(lookup)
        {
            addStyleClass("container");
            fa= addWidget(std::make_unique<FrontAccess>(db_, lookup_));
            fa->notify.connect(boost::bind(&FrontSwitch::react, this, _1));
        }

    private:

        std::string lookup_;
        Postgresql db_;
        FrontAccess *fa = nullptr;
        T *gl = nullptr;

        void react(int value)
        {
            switch(value)
            {
                case GRANT_ACCESS:
                {
                    if(fa != nullptr)
                    {
                        removeWidget(fa);
                        gl= addWidget(std::make_unique<T>(db_));
                        gl->notify.connect(boost::bind(&FrontSwitch::react, this, _1));
                    }
                    break;
                }

                case LOGOUT:
                {
                    if(gl != nullptr)
                    {
                        removeWidget(gl);
                        fa= addWidget(std::make_unique<FrontAccess>(db_, lookup_));
                        fa->notify.connect(boost::bind(&FrontSwitch::react, this, _1));
                    }
                    break;
                }
            }
        }
};

/**
 * The web application.
 **/
class BasicApp : public Wt::WApplication
{
    public:

        // BasicApp(
        //    const Wt::WEnvironment& env,
        //    std::shared_ptr<Postgresql> database): WApplication(env), db(database)

        BasicApp(
            const Wt::WEnvironment& env,
            const Postgresql &db): WApplication(env), db_(db)
        {
            setTitle("Voting System: configuration of the voting");

            if(appRoot().empty())
            {
                Wt::log("warning") << "the approot looks suspect, is empty!";
            }

            auto bootstrapTheme = std::make_shared<Wt::WBootstrapTheme>();
            bootstrapTheme->setVersion(Wt::BootstrapVersion::v3);
            bootstrapTheme->setResponsive(true);
            setTheme(bootstrapTheme);

            useStyleSheet("resources/themes/bootstrap/3/bootstrap-theme.min.css");
            useStyleSheet("https://getbootstrap.com/docs/3.4/dist/css/bootstrap.min.css");
            // useStyleSheet("https://getbootstrap.com/docs/3.4/examples/starter-template/starter-template.css");
            useStyleSheet("https://getbootstrap.com/docs/3.4/examples/jumbotron/jumbotron.css");
            // useStyleSheet("https://getbootstrap.com/docs/3.4/examples/grid/grid.css");

            // useStyleSheet("https://getbootstrap.com/docs/3.4/examples/cover/cover.css");

            // root()->addNew<GeneralLayout>(db_);
            // root()->addNew<FrontSwitch>(db_);
            root()->addNew<FrontSwitch<GeneralLayout>>(db_, "configuration");
        }

    private:

        Postgresql db_;

};

/// Wt
#include <Wt/WApplication.h>

/// Boost
#include <boost/bind.hpp>

class AppGenerator
{
    public:
        AppGenerator(const Postgresql &db): db_(db)
        {}

        std::unique_ptr<BasicApp> createApp(const Wt::WEnvironment& env)
        {
            return std::make_unique<BasicApp>(env, db_);
        }

    private:

        Postgresql db_;
};

/**
 * Main
 **/
int main(int argc, char **argv)
{
    Postgresql db("voting00");

    /*
    pqxx::result answer;
    std::string sentence= "SELECT * FROM general;";
    db.execSql(sentence, answer);
    const pqxx::result::const_iterator row= answer.begin();
    if(row != answer.end())
    {
        std::cout
            << "row: "
            << row[0].as(std::string()) << ", "
            << row[1].as(std::string()) << "\n";
    }
    */

    AppGenerator ag(db);

    return Wt::WRun(
        argc,
        argv,
        boost::bind(&AppGenerator::createApp, &ag, _1));
}
