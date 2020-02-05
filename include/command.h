// C
#include <stdio.h>

// C++
#include <vector>
#include <string>

// Wt
#include <Wt/WApplication.h>
#include <Wt/WTextArea.h>

// Voting
#include "postgresql.h"

class Command
{
    public:

        static std::string
            execute(const std::string &command);

        static void
            execute(
                const std::string &command,
                Wt::WTextArea *wFeedback);

        static void
            sendInvitations();

        static void
            sendInvitations(
                const int &idxVoting,
                Postgresql &db,
                Wt::WTextArea *wFeedback);

        static std::string
            getBodyTemplate(
                const int &idxVoting,
                Postgresql &db);

        static std::string
            getEmailSubject(
                const int &idxVoting,
                Postgresql &db);

        static std::string
            getVotingLink(
                const int &idxVoting,
                Postgresql &db);

        static bool
            isTesting(Postgresql &db);

        static std::string
            rndSeq(const unsigned int &len);

        static void
            writeFile(
                const std::string &location,
                const std::string &content);

        static std::string
            toString(const bool &value);

        static std::string
            genDetail(const std::vector<int> &options);
};
