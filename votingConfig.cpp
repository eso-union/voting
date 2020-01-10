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

class VotingType : public Wt::WContainerWidget {

    public:

        VotingType(const bool &testing, const bool &real):
            testingEnabled_(testing),
            realEnabled_(real) {

            addWidget(std::make_unique<Wt::WText>("<h3>Choose the type of voting</h3>"));

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

    private:

        Wt::WRadioButton *testingButton_;
        Wt::WRadioButton *realButton_;

        bool testingEnabled_;
        bool testingConfigured_;

        bool realEnabled_;
        bool realConfigured_;
};

/**
 *
 **/
class NameEmail : public Wt::WContainerWidget {

    public:

        NameEmail() {
            auto row = addWidget(std::make_unique<Wt::WContainerWidget>());
            row->addStyleClass("row");

            auto cell0 = row->addWidget(std::make_unique<Wt::WContainerWidget>());
            cell0->addStyleClass("col-xs-6");
            inputName_ = cell0->addWidget(std::make_unique<Wt::WLineEdit>());

            auto cell1 = row->addWidget(std::make_unique<Wt::WContainerWidget>());
            cell1->addStyleClass("col-xs-6");
            inputEmail_ = cell1->addWidget(std::make_unique<Wt::WLineEdit>());
        }

        NameEmail(
            const std::string &name,
            const std::string &email
        ) {
            auto row = addWidget(std::make_unique<Wt::WContainerWidget>());
            row->addStyleClass("row");

            auto cell0 = row->addWidget(std::make_unique<Wt::WContainerWidget>());
            cell0->addStyleClass("col-xs-6");
            inputName_ = cell0->addWidget(std::make_unique<Wt::WLineEdit>(name));

            auto cell1 = row->addWidget(std::make_unique<Wt::WContainerWidget>());
            cell1->addStyleClass("col-xs-6");
            inputEmail_ = cell1->addWidget(std::make_unique<Wt::WLineEdit>(email));
        }

    private:

        Wt::WLineEdit *inputName_;
        Wt::WLineEdit *inputEmail_;
};

/**
 *
 **/
class VotingPeople : public Wt::WContainerWidget {

    public:

        VotingPeople() {

            addWidget(std::make_unique<Wt::WText>("<h3>Enter the list of <em>email,name</em> of voters</h3>"));
            addWidget(std::make_unique<Wt::WText>("<h4>You may delay this step util you have done all the testing.</h4>"));

            emailVoters_ = addWidget(std::make_unique<Wt::WTextArea>());
            emailVoters_->setColumns(40);
            emailVoters_->setRows(10);
            emailVoters_->setPlaceholderText("some@email.com,Some Name\nanother@email.com,Another Name");
        }

    private:

        Wt::WTextArea *emailVoters_;
};

/**
 *
 **/
class VotingTesters : public Wt::WContainerWidget {

    public:

        VotingTesters() {

            addWidget(std::make_unique<Wt::WText>("<h3>[optional] Name and email for testing</h3>"));

            auto row = addWidget(std::make_unique<Wt::WContainerWidget>());
            row->addStyleClass("row");

            auto cell0 = row->addWidget(std::make_unique<Wt::WContainerWidget>());
            cell0->addStyleClass("col-xs-6");
            cell0->addWidget(std::make_unique<Wt::WText>("Name"));

            auto cell1 = row->addWidget(std::make_unique<Wt::WContainerWidget>());
            cell1->addStyleClass("col-xs-6");
            cell1->addWidget(std::make_unique<Wt::WText>("Email address"));

            addWidget(std::make_unique<NameEmail>());
            addWidget(std::make_unique<NameEmail>());
            addWidget(std::make_unique<NameEmail>());
            addWidget(std::make_unique<NameEmail>());
            addWidget(std::make_unique<NameEmail>());
        }

    private:

};

/**
 *
 **/
class VotePosibilities : public Wt::WContainerWidget {

    public:

        VotePosibilities() {
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

    private:

};

/**
 *
 **/
class VoteAlternatives : public Wt::WContainerWidget {

    public:

        VoteAlternatives() {
            addStyleClass("container");

            addWidget(std::make_unique<Wt::WText>("<h3>Voting alternatives</h3>"));

            auto buttonRemove = addWidget(std::make_unique<Wt::WPushButton>("Remove Alternative"));
            buttonRemove->addStyleClass("btn btn-warning");
            buttonRemove->clicked().connect(this, &VoteAlternatives::remove);

            auto buttonAdd = addWidget(std::make_unique<Wt::WPushButton>("Add Alternative"));
            buttonAdd->addStyleClass("btn btn-primary");
            buttonAdd->clicked().connect(this, &VoteAlternatives::add);
        }

    private:

        std::vector<Wt::WLineEdit*> input_;

        void add() {
            Wt::WLineEdit *edit = addWidget(std::make_unique<Wt::WLineEdit>());
            edit->addStyleClass("form-control");
            edit->setPlaceholderText("Alternative " + std::to_string(input_.size()));
            edit->setInline(false);
            input_.push_back(edit);
        }

        void remove() {
            if(input_.size()>0) {
                removeWidget(input_[input_.size()-1]);
                input_.pop_back();
            }
        }
};

/**
 * Form: question or convocatory for the voters.
 **/
class VoterQuestion : public Wt::WContainerWidget {

    public:

        VoterQuestion() {

            addWidget(std::make_unique<Wt::WText>("<h3>General settings</h3>"));

            addWidget(std::make_unique<Wt::WText>("Convocatory to voters:"));

            convocatory_ = addWidget(std::make_unique<Wt::WTextArea>());
            convocatory_->setColumns(40);
            convocatory_->setRows(5);

            addWidget(std::make_unique<Wt::WText>("Encabezado en voto:"));

            question_ = addWidget(std::make_unique<Wt::WTextArea>());
            question_->setColumns(40);
            question_->setRows(2);

            /**
             * Beginning of the voting.
             **/

            auto rowA = addWidget(std::make_unique<Wt::WContainerWidget>());
            rowA->addStyleClass("row");

            auto cellA0 = rowA->addWidget(std::make_unique<Wt::WContainerWidget>());
            cellA0->addStyleClass("col-md-4");
            cellA0->addWidget(std::make_unique<Wt::WText>("Begining:"));

            auto cellA1 = rowA->addWidget(std::make_unique<Wt::WContainerWidget>());
            cellA1->addStyleClass("col-md-4");
            auto beginDay = cellA1->addWidget(std::make_unique<Wt::WDateEdit>());
            beginDay->setDate(Wt::WDate::currentServerDate().addDays(1));

            auto cellA2 = rowA->addWidget(std::make_unique<Wt::WContainerWidget>());
            cellA2->addStyleClass("col-md-4");
            auto beginHour = cellA2->addWidget(std::make_unique<Wt::WTimeEdit>());
            beginHour->setTime(Wt::WTime::currentTime());

            /**
             * Beginning of the voting.
             **/

            auto rowB = addWidget(std::make_unique<Wt::WContainerWidget>());
            rowB->addStyleClass("row");

            auto cellB0 = rowB->addWidget(std::make_unique<Wt::WContainerWidget>());
            cellB0->addStyleClass("col-md-4");
            cellB0->addWidget(std::make_unique<Wt::WText>("Begining:"));

            auto cellB1 = rowB->addWidget(std::make_unique<Wt::WContainerWidget>());
            cellB1->addStyleClass("col-md-4");
            auto endingDay = cellB1->addWidget(std::make_unique<Wt::WDateEdit>());
            endingDay->setDate(Wt::WDate::currentServerDate().addDays(2));

            auto cellB2 = rowB->addWidget(std::make_unique<Wt::WContainerWidget>());
            cellB2->addStyleClass("col-md-4");
            auto endingHour = cellB2->addWidget(std::make_unique<Wt::WTimeEdit>());
            endingHour->setTime(Wt::WTime::currentTime());

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
        }

    private:

        Wt::WTextArea *convocatory_;
        Wt::WTextArea *question_;
};

#include <Wt/WSelectionBox.h>

/**
 *
 **/
class VotingSelection : public Wt::WContainerWidget
{
    public:

        VotingSelection()
        {
            addStyleClass("container");

            addWidget(std::make_unique<Wt::WText>("<h3>Select or Create Voting</h3>"));

            auto rowA = addWidget(std::make_unique<Wt::WContainerWidget>());
            rowA->addStyleClass("row");

            auto cellA0 = rowA->addWidget(std::make_unique<Wt::WContainerWidget>());
            cellA0->addStyleClass("col-md-4");
            // cellB0->addWidget(std::make_unique<Wt::WText>("Begining:"));

            // Wt::WSelectionBox *sb1 = addNew<Wt::WSelectionBox>();
            Wt::WSelectionBox *sb1 = cellA0->addWidget(std::make_unique<Wt::WSelectionBox>());
            sb1->addItem("Heavy");
            sb1->addItem("Medium");
            sb1->addItem("Light");
            // sb1->setCurrentIndex(0); // Check if at least exist one element
            sb1->setMargin(10, Wt::Side::Right);
            Wt::WText *out = addNew<Wt::WText>("");

            sb1->activated().connect(
                [=]
                {
                    // out->setText(Wt::WString("You selected {1}.").arg(sb1->currentText()));
                    selected_= sb1->currentText().toUTF8();
                    out->setText(Wt::WString("You selected {1}.").arg(sb1->currentText()));
                });

            // auto continue = addWidget(std::make_unique<Wt::WPushButton>("Continue with selection"));
            useSelected_= cellA0->addWidget(std::make_unique<Wt::WPushButton>("Continue with selection"));
            useSelected_->addStyleClass("btn btn-warning");
            useSelected_->clicked().connect(this, &VotingSelection::selected);

            auto rowB = addWidget(std::make_unique<Wt::WContainerWidget>());
            rowB->addStyleClass("row");

            auto cellB0 = rowB->addWidget(std::make_unique<Wt::WContainerWidget>());
            cellB0->addStyleClass("col-md-4");

            newName_ = cellB0->addWidget(std::make_unique<Wt::WLineEdit>());

            auto createVoting = cellB0->addWidget(std::make_unique<Wt::WPushButton>("Create Voting"));
            createVoting->addStyleClass("btn btn-primary");
            createVoting->clicked().connect(this, &VotingSelection::create);
        }

    private:

        std::string selected_;
        Wt::WLineEdit *newName_;
        Wt::WPushButton *useSelected_;

        // std::vector<Wt::WLineEdit*> input_;

        void create()
        {

        }

        void selected()
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
class GeneralLayout : public Wt::WContainerWidget
{
    public:

        GeneralLayout() {
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
            stack->addNew<VotingSelection>();
            stack->addNew<VoterQuestion>();
            stack->addNew<VoteAlternatives>();
            stack->addNew<VotePosibilities>();
            stack->addNew<VotingTesters>();
            stack->addNew<VotingPeople>();
            stack->addNew<VotingType>(true, false);
            stack->setCurrentIndex(0);

            /**
             * Row B
             **/
            auto rowB = addWidget(std::make_unique<Wt::WContainerWidget>());
            rowB->addStyleClass("row");

            auto cellB1 = rowB->addWidget(std::make_unique<Wt::WContainerWidget>());
            cellB1->addStyleClass("col-md-4");

            auto cellB2 = rowB->addWidget(std::make_unique<Wt::WContainerWidget>());
            cellB2->addStyleClass("col-md-4");
            auto buttonPrev = cellB2->addWidget(std::make_unique<Wt::WPushButton>("Previous"));
            buttonPrev->addStyleClass("btn btn-primary btn-lg btn-block");
            buttonPrev->clicked().connect(this, &GeneralLayout::previous);

            auto cellB3 = rowB->addWidget(std::make_unique<Wt::WContainerWidget>());
            cellB3->addStyleClass("col-md-4");
            auto buttonNext = cellB3->addWidget(std::make_unique<Wt::WPushButton>("Next"));
            buttonNext->addStyleClass("btn btn-primary btn-lg btn-block");
            buttonNext->clicked().connect(this, &GeneralLayout::next);
        }

    private:

        Wt::WStackedWidget *stack;

        void previous()
        {
            int current = stack->currentIndex();
            if(current > 0)
            {
                current--;
                stack->setCurrentIndex(current);
            }
        }

        void next()
        {
            int current = stack->currentIndex();
            if(current < 6)
            {
                current++;
                stack->setCurrentIndex(current);
            }
        }
};

/**
 * The web application.
 **/
class BasicApp : public Wt::WApplication
{
    public:

        BasicApp(const Wt::WEnvironment& env) : WApplication(env)
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
            useStyleSheet("https://getbootstrap.com/docs/3.4/examples/grid/grid.css");

            root()->addNew<GeneralLayout>();
        }
};

/**
 * Main
 **/
int main(int argc, char **argv)
{
    return Wt::WRun(
        argc,
        argv,
        [](const Wt::WEnvironment &env)
        {
            return std::make_unique<BasicApp>(env);
        });
}
