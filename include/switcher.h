#pragma once

// Voting
#include "access.h"
#include "postgresql.h"

template <typename T>
class Switcher: public Wt::WContainerWidget
{
    public:

        Switcher(
            const Postgresql &db,
            const std::string &lookup): db_(db), lookup_(lookup)
        {
            addStyleClass("container");
            fa= addWidget(std::make_unique<Access>(db_, lookup_));
            fa->notify.connect(
                boost::bind(&Switcher::react, this, _1, _2));
        }

    private:

        std::string lookup_;
        Postgresql db_;
        Access* fa= nullptr;
        T* gl= nullptr;

        void react(int key, int value)
        {
            switch(key)
            {
                case GRANT_ACCESS:
                {
                    if(fa)
                    {
                        removeWidget(fa);

                        gl= addWidget(
                            std::make_unique<T>(db_));

                        gl->notify.connect(
                            boost::bind(
                                &Switcher::react, this, _1, _2));
                    }
                    break;
                }

                case LOGOUT:
                {
                    if(gl)
                    {
                        removeWidget(gl);

                        fa= addWidget(
                            std::make_unique<Access>(db_, lookup_));

                        fa->notify.connect(
                            boost::bind(&Switcher::react, this, _1, _2));
                    }
                    break;
                }
            }
        }
};
