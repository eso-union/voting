#include "access.h"

Access::Access(
    const Postgresql &db,
    const std::string &lookup)
    : Panel(db), lookup_(lookup)
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
        });

    auto cellC2 = rowC->addWidget(std::make_unique<Wt::WContainerWidget>());
    cellC2->addStyleClass("col-md-4");
}

void Access::verify()
{
    std::string passw;
    std::cout << "Go!\n";\
    if(iPassword != nullptr)
    {
        passw= iPassword->text().toUTF8();
        if(passw == "hola")
        {
            notify(GRANT_ACCESS, EMPTY);
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
