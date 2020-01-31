#include "stepInfo.h"

StepInfo::StepInfo(
    const bool &valid,
    const std::string &text)
{
    msgBox_= addWidget(std::make_unique<Wt::WContainerWidget>());
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

void StepInfo::setText(
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

void StepInfo::setCompleted()
{
        msgBox_->setStyleClass("alert alert-success");
}

void StepInfo::setIncomplete()
{
        msgBox_->setStyleClass("alert alert-warning");
}
