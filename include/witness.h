#pragma once

// Wt
#include <Wt/WCheckBox.h>
#include <Wt/WLabel.h>
#include <Wt/WSpinBox.h>
#include <Wt/WText.h>

// Voting
#include "common.h"
#include "panel.h"

/**
 *
 **/
class Witness: public Panel
{
    public:

        Witness(const Postgresql &db);

        virtual void save();

        virtual void setup(
            const int &type,
            const int &value);

    private:

        std::string withMinisters_;
        std::string qtyMinisters_;

        Wt::WCheckBox *wInc_= nullptr;
        Wt::WSpinBox  *wQty_= nullptr;

        void wIncChanged();
        void wQtyValueChanged();
        void updateDB();
};
