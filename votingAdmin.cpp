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
 * $ g++ votingAdmin.cpp -o ./voting_admin -lwthttp -lwt
 *
 * Execution
 * $ ./voting_admin --docroot . --http-address 0.0.0.0 --http-port 9090
 **********************************************************************/

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

#include <Wt/WBreak.h>
#include <Wt/WDialog.h>
#include <Wt/WEnvironment.h>
#include <Wt/WLabel.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WRegExpValidator.h>

namespace {

void showDialog(Wt::WObject *owner, Wt::WText *out)
{
    auto dialog = owner->addChild(Wt::cpp14::make_unique<Wt::WDialog>("Go to cell"));

    Wt::WLabel *label =
        dialog->contents()->addNew<Wt::WLabel>("Cell location (A1..Z999)");
    Wt::WLineEdit *edit =
        dialog->contents()->addNew<Wt::WLineEdit>();
    label->setBuddy(edit);

    dialog->contents()->addStyleClass("form-group");

    auto validator =
        std::make_shared<Wt::WRegExpValidator>("[A-Za-z][1-9][0-9]{0,2}");
    validator->setMandatory(true);
    edit->setValidator(validator);

    Wt::WPushButton *ok =
        dialog->footer()->addNew<Wt::WPushButton>("OK");
    ok->setDefault(true);
    if (wApp->environment().ajax())
      ok->disable();

    Wt::WPushButton *cancel =
        dialog->footer()->addNew<Wt::WPushButton>("Cancel");
    dialog->rejectWhenEscapePressed();

    edit->keyWentUp().connect([=] {
        ok->setDisabled(edit->validate() != Wt::ValidationState::Valid);
    });

    /*
     * Accept the dialog
     */
    ok->clicked().connect([=] {
        if (edit->validate() == Wt::ValidationState::Valid)
            dialog->accept();
    });

    /*
     * Reject the dialog
     */
    cancel->clicked().connect(dialog, &Wt::WDialog::reject);

    /*
     * Process the dialog result.
     */
    dialog->finished().connect([=] {
        if (dialog->result() == Wt::DialogCode::Accepted)
	    out->setText("New location: " + edit->text());
	else
	    out->setText("No location selected.");

        owner->removeChild(dialog);
    });

    dialog->show();
}

void passwCreator(Wt::WObject *owner)
{
    auto dialog = owner->addChild(Wt::cpp14::make_unique<Wt::WDialog>("Go to cell"));

    dialog->contents()->addNew<Wt::WLabel>("Password");
    auto pass0 = dialog->contents()->addNew<Wt::WLineEdit>();
    pass0->setEchoMode(Wt::EchoMode::Password);

    dialog->contents()->addNew<Wt::WLabel>("Re-enter password");
    auto pass1 = dialog->contents()->addNew<Wt::WLineEdit>();
    pass1->setEchoMode(Wt::EchoMode::Password);

    dialog->contents()->addStyleClass("form-group");

    Wt::WPushButton *ok = dialog->footer()->addNew<Wt::WPushButton>("OK");
    ok->setDefault(true);
    if (wApp->environment().ajax()) {
        ok->disable();
    }

    Wt::WPushButton *cancel = dialog->footer()->addNew<Wt::WPushButton>("Cancel");
    dialog->rejectWhenEscapePressed();

/*
    edit->keyWentUp().connect([=] {
        ok->setDisabled(edit->validate() != Wt::ValidationState::Valid);
    });
*/
    /*
     * Accept the dialog
     */
    ok->clicked().connect([=] {
        // if (edit->validate() == Wt::ValidationState::Valid)
            dialog->accept();
    });

    /*
     * Reject the dialog
     */
    cancel->clicked().connect(dialog, &Wt::WDialog::reject);

    /*
     * Process the dialog result.
     */
    dialog->finished().connect([=] {

        /*
        if (dialog->result() == Wt::DialogCode::Accepted)
	        out->setText("New location: " + edit->text());
	    else
	        out->setText("No location selected.");
        */

        owner->removeChild(dialog);
    });

    dialog->show();
}

}

/*
          <div class="panel panel-primary">
            <div class="panel-heading">
              <h3 class="panel-title">Panel title</h3>
            </div>
            <div class="panel-body">
              Panel content
            </div>
          </div>
*/

class MinisterAuth : public Wt::WContainerWidget {

    public:

        MinisterAuth() {
            addStyleClass("panel panel-primary");

            auto heading = addWidget(std::make_unique<Wt::WContainerWidget>());
            heading->addStyleClass("panel-heading");
            // auto title = heading->addWidget(std::make_unique<Wt::WText>("<h3>First Minister</h3>"));
            auto title = heading->addWidget(std::make_unique<Wt::WText>("First Minister"));
            title->setStyleClass("panel-title");

            auto body = addWidget(std::make_unique<Wt::WContainerWidget>());
            body->addStyleClass("panel-body");

            // body->addWidget(std::make_unique<Wt::WText>("Minister Name"));
            // body->addWidget(std::make_unique<Wt::WLineEdit>());

            body->addWidget(std::make_unique<Wt::WText>("Password"));
            auto pass0 = body->addWidget(std::make_unique<Wt::WLineEdit>());
            pass0->setEchoMode(Wt::EchoMode::Password);

            body->addWidget(std::make_unique<Wt::WText>("Re-enter password"));
            auto pass1 = body->addWidget(std::make_unique<Wt::WLineEdit>());
            pass1->setEchoMode(Wt::EchoMode::Password);
        }

    private:

};

/**
 * The message box for each step.
 **/
class StepInfo : public Wt::WContainerWidget {

    public:

        StepInfo(
            const bool &valid,
            const std::string &text
        ) {
            msgBox_ = addWidget(std::make_unique<Wt::WContainerWidget>());
            if(valid) {
                msgBox_->setStyleClass("alert alert-success");
            } else {
                msgBox_->setStyleClass("alert alert-warning");
            }
            msgText_ = msgBox_->addWidget(std::make_unique<Wt::WText>(text));
        }

        void setText(
            const bool &valid,
            const std::string &text
        ) {
            if(valid) {
                msgBox_->setStyleClass("alert alert-success");
            } else {
                msgBox_->setStyleClass("alert alert-warning");
            }
            msgText_->setText(text);
        }

    private:

        Wt::WContainerWidget *msgBox_;
        Wt::WText *msgText_;
};

#include <Wt/WTable.h>
#include <Wt/WTableCell.h>

/**
 *
 **/
class SendInvitations : public Wt::WContainerWidget {

    public:

        SendInvitations() {

            addWidget(std::make_unique<Wt::WText>("<h3>Send invitation to voters</h3>"));

        }

    private:

};


/**
 *
 **/
class VotingList : public Wt::WContainerWidget {

    public:

        VotingList() {

            addWidget(std::make_unique<Wt::WText>("<h3>Approve the list of of voters</h3>"));

            auto table = addWidget(Wt::cpp14::make_unique<Wt::WTable>());
            table->setHeaderCount(1);
            table->setWidth(Wt::WLength("100%"));

            table->elementAt(0, 0)->addNew<Wt::WText>("Email");
            table->elementAt(0, 1)->addNew<Wt::WText>("Name");

            table->elementAt(1, 0)->addNew<Wt::WText>("some@email.com");
            table->elementAt(1, 1)->addNew<Wt::WText>("Some Name");

            table->elementAt(2, 0)->addNew<Wt::WText>("another@email.com");
            table->elementAt(2, 1)->addNew<Wt::WText>("Another Name");

        }

    private:

};

class MinisterPassw : public Wt::WContainerWidget {

    public:

        MinisterPassw() {

            Wt::WText *out = addNew<Wt::WText>();
            out->setStyleClass("help-block");

            auto button = addWidget(std::make_unique<Wt::WPushButton>("Create password"));

            // auto c = this->get();
            button->clicked().connect([=] {
                // showDialog(this, out);
                passwCreator(this);
            });
        }

    private:

};

/**
 * The web widget.
 **/
class AdminLayout : public Wt::WContainerWidget {

    public:

        AdminLayout() {
            addStyleClass("container");

            /**
             * Row F
             **/
            auto rowF = addWidget(std::make_unique<Wt::WContainerWidget>());
            rowF->addStyleClass("row");

            auto cellF0 = rowF->addWidget(std::make_unique<Wt::WContainerWidget>());
            cellF0->addStyleClass("col-md-4");
            cellF0->addWidget(std::make_unique<MinisterAuth>());

            auto cellF1 = rowF->addWidget(std::make_unique<Wt::WContainerWidget>());
            cellF1->addStyleClass("col-md-4");
            cellF1->addWidget(std::make_unique<MinisterAuth>());

            auto cellF2 = rowF->addWidget(std::make_unique<Wt::WContainerWidget>());
            cellF2->addStyleClass("col-md-4");
            cellF2->addWidget(std::make_unique<MinisterAuth>());

            /**
             * Row A
             **/
            auto rowA = addWidget(std::make_unique<Wt::WContainerWidget>());
            rowA->addStyleClass("row");

            auto cellA0 = rowA->addWidget(std::make_unique<Wt::WContainerWidget>());
            cellA0->addStyleClass("col-md-8");

            stack = cellA0->addNew<Wt::WStackedWidget>();
            stack->addNew<Wt::WText>("Just a single widget.");
            stack->addNew<MinisterPassw>();
            stack->addNew<VotingList>();
            stack->setCurrentIndex(0);

            auto cellA1 = rowA->addWidget(std::make_unique<Wt::WContainerWidget>());
            cellA1->addStyleClass("col-md-4");

            auto msgBox0 = cellA1->addWidget(std::make_unique<StepInfo>(true,  "<strong>Step1</strong> Setting up the voting."));
            auto msgBox1 = cellA1->addWidget(std::make_unique<StepInfo>(false, "<strong>Step2</strong> Setting up the voting."));
            auto msgBox2 = cellA1->addWidget(std::make_unique<StepInfo>(false, "<strong>Step3</strong> Setting up the voting."));
            auto msgBox3 = cellA1->addWidget(std::make_unique<StepInfo>(false, "<strong>Step4</strong> Setting up the voting."));
            auto msgBox4 = cellA1->addWidget(std::make_unique<StepInfo>(false, "<strong>Step5</strong> Setting up the voting."));

            /*
            stack->addNew<VoterQuestion>();
            stack->addNew<VoteAlternatives>();
            stack->addNew<VotePosibilities>();
            stack->addNew<VotingTesters>();
            stack->addNew<VotingPeople>();
            stack->addNew<VotingType>(true, false);
            */

            /**
             * Row B
             **/
            auto rowB = addWidget(std::make_unique<Wt::WContainerWidget>());
            rowB->addStyleClass("row");

            auto cellB0 = rowB->addWidget(std::make_unique<Wt::WContainerWidget>());
            cellB0->addStyleClass("col-md-4");
            auto buttonPrev = cellB0->addWidget(std::make_unique<Wt::WPushButton>("Previous"));
            buttonPrev->addStyleClass("btn btn-primary btn-lg btn-block");
            buttonPrev->clicked().connect(this, &AdminLayout::previous);

            auto cellB1 = rowB->addWidget(std::make_unique<Wt::WContainerWidget>());
            cellB1->addStyleClass("col-md-4");
            auto buttonNext = cellB1->addWidget(std::make_unique<Wt::WPushButton>("Next"));
            buttonNext->addStyleClass("btn btn-primary btn-lg btn-block");
            buttonNext->clicked().connect(this, &AdminLayout::next);

            auto cellB2 = rowB->addWidget(std::make_unique<Wt::WContainerWidget>());
            cellB2->addStyleClass("col-md-4");
        }

    private:

        Wt::WStackedWidget *stack;

        void previous() {
            int current = stack->currentIndex();
            if (current > 0) {
                current--;
                stack->setCurrentIndex(current);
            }
        }

        void next() {
            int current = stack->currentIndex();
            if (current < 2) {
                current++;
                stack->setCurrentIndex(current);
            }
        }
};

/**
 * The web application.
 **/
class AdminApp : public Wt::WApplication {

    public:

        AdminApp(const Wt::WEnvironment& env) : WApplication(env) {
            setTitle("Voting System: administration of the voting");

            if(appRoot().empty()) {
                Wt::log("warning") << "the approot looks suspect, is empty!";
            }

            auto bootstrapTheme = std::make_shared<Wt::WBootstrapTheme>();
            bootstrapTheme->setVersion(Wt::BootstrapVersion::v3);
            bootstrapTheme->setResponsive(true);
            setTheme(bootstrapTheme);

            useStyleSheet("resources/themes/bootstrap/3/bootstrap-theme.min.css");
            useStyleSheet("https://getbootstrap.com/docs/3.4/dist/css/bootstrap.min.css");
            // useStyleSheet("https://getbootstrap.com/docs/3.4/examples/starter-template/starter-template.css");
            // useStyleSheet("https://getbootstrap.com/docs/3.4/examples/jumbotron/jumbotron.css");
            useStyleSheet("https://getbootstrap.com/docs/3.4/examples/grid/grid.css");
            // useStyleSheet("https://getbootstrap.com/docs/3.4/examples/theme/theme.css");

            root()->addNew<AdminLayout>();
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
            return std::make_unique<AdminApp>(env);
        }
    );
}
