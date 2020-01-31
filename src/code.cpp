#include "code.h"

Code::Code(
    const Postgresql &db,
    const int &idxVoting)
    : Panel(db, idxVoting)
{
    addWidget(
        std::make_unique<Wt::WText>(
            "<h3>Enter your code</h3>"));

    wCode_= addWidget(std::make_unique<Wt::WLineEdit>());
    wCode_->setMaxLength(10);
    wCode_->setInline(false);
    wCode_->keyWentDown().connect(
        [=](const Wt::WKeyEvent& e)
        {
            wMsg_->setText("");
            if(e.key() == Wt::Key::Enter)
            {
                verify();
            }
        });

    wMsg_= addWidget(std::make_unique<Wt::WText>(""));
    wMsg_->setInline(false);

    auto wNext=
        addWidget(std::make_unique<Wt::WPushButton>("Verify"));
    wNext->setInline(false);
    wNext->addStyleClass("btn btn-primary btn-lg btn-block");
    wNext->clicked().connect(this, &Code::verify);
}

void Code::verify()
{
    Wt::log("info") << "idxVoting_:" << idxVoting_;

    std::string code= wCode_->text().toUTF8();

    if(code.length()<6)
    {
        wMsg_->setText(
            "<strong>The code is not valid or it was used.</strong>");
        return;
    }

    Wt::WString sentence=
        "SELECT idx "
        "FROM people "
        "WHERE code='{1}' "
        "AND idx_general={2} "
        "AND used=false";

    sentence
        .arg(code)
        .arg(idxVoting_);

    pqxx::result answer= db_.query(sentence.toUTF8());
    if(answer.begin() == answer.end())
    {
        wMsg_->setText(
            "<strong>The code is not valid or it was used.</strong>");
    }
    else
    {
        auto row= answer.begin();
        int idx= row[0].as<int>();
        tellCode(idx, code);
    }
}
