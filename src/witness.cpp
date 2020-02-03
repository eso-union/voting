// C
#include <cassert>

// C++
#include <string>

// Wt
#include <Wt/WBreak.h>

// Voting
#include "witness.h"

Witness::Witness(const Postgresql &db): Panel(db)
{
    settingType_= TYPE_CONFIG;
    step_= STEP_2;
    description_= "Setting up the ministers";
    setTitle();

    // Color changes depending on validity of input.
    wCanvas_->addStyleClass("form-group");

    wInc_= wCanvas_->addWidget(std::make_unique<Wt::WCheckBox>("Include ministers"));
    wInc_->setChecked(false);
    wInc_->setInline(false);
    wInc_->changed().connect(this, &Witness::wIncChanged);

    auto wLabel= wCanvas_->addNew<Wt::WLabel>("Enter a number (1 - 4):");
    wQty_= wCanvas_->addWidget(std::make_unique<Wt::WSpinBox>());
    wQty_->setRange(1,4);
    wQty_->valueChanged().connect(this, &Witness::wQtyValueChanged);
    wLabel->setBuddy(wQty_);

    // Verification of pointers
    assert(wInc_ != nullptr);
    assert(wQty_ != nullptr);
}

void Witness::save()
{
    withMinisters_= "false";
    qtyMinisters_= "1";
    if(wInc_->isChecked())
    {
        if((wQty_->value()<1) ||
           (wQty_->value()>4))
        {
            wOut_->setText(
                "Number of ministers must be in the range 1 to 4.");
            return;
        }
        withMinisters_= "true";
        qtyMinisters_= std::to_string(wQty_->value());
    }
    updateDB();
};

void Witness::updateDB()
{
    try
    {
        Wt::WString sentence=
            "UPDATE general "
            "SET with_ministers={1}, "
            "qty_ministers={2} "
            "WHERE idx={3}";

        sentence
            .arg(withMinisters_)
            .arg(qtyMinisters_)
            .arg(idxVoting_);

        db_.execSql(sentence.toUTF8());

        setSaved();
        setCompleted();
    }
    catch(const std::exception &e)
    {
        Wt::log("error") << e.what();
        wOut_->setText(e.what());
    }
}

void Witness::setup(
    const int &type,
    const int &value)
{
    if(type == SELECTED)
    {
        idxVoting_= value;

        Wt::WString sentence=
            "SELECT with_ministers, qty_ministers "
            "FROM general "
            "WHERE idx={1}";

        sentence.arg(idxVoting_);

        pqxx::result answer= db_.query(sentence.toUTF8());
        if(answer.begin() != answer.end())
        {
            pqxx::result::const_iterator row= answer.begin();

            bool wm= row[0].as(bool());
            if(wm)
            {
                wInc_->setChecked();
                wQty_->enable();
            }
            else
            {
                wInc_->setUnChecked();
                wQty_->disable();
            }

            int qt= row[1].as(int());
            wQty_->setValue(qt);
        }
    }

    if(isCompleted())
    {
        notify(COMPLETED, id_);
    }
}

void Witness::wIncChanged()
{
    if(wInc_->isChecked())
    {
        wQty_->enable();
        notify(CHANGED, EMPTY);
    }
    else
    {
        wQty_->disable();
        notify(CHANGED, EMPTY);
    }
};

void Witness::wQtyValueChanged()
{
    notify(CHANGED, EMPTY);
}
