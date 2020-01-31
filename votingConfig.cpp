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

const int TESTING      = 0;
const int REAL         = 1;

// Boost
#include <boost/signals2.hpp>


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

    protected:

        int selected_= -1;
};





/**
 *
 **/








/**
 * The message box for each step.
 **/
/*********** NOT USING THIS IMPLEMENTATION
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
*******************************************/

/**
 * The web widget.
 **/
// class GeneralLayout : public Wt::WContainerWidget

// class FrontAccess : public Wt::WContainerWidget

/*********** NOT USING THIS IMPLEMENTATION
class FrontAccess: public WidgetPanel
{
    public:

        FrontAccess(
            const Postgresql &db,
            const std::string &lookup)
            : db_(db), lookup_(lookup)
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
                        verify();
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

        Postgresql db_;
        std::string lookup_;
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
******************************/

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

/*********** NOT USING THIS IMPLEMENTATION
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

        Postgresql db_;
        std::string lookup_;
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
*********************************************/

/**
 * The web application.
 **/
