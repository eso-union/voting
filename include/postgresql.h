#pragma once

// C++
#include <string>
#include <vector>

// Boost
#include <boost/scoped_ptr.hpp>

// Postgresql
#include <pqxx/pqxx>

// Voting
#include "common.h"

/**
 *
 **/
class Postgresql
{
    public:

        // Postgresql(const std::string &name);
        Postgresql(const dbConfig &par);

        Postgresql(const Postgresql &db);

        Postgresql& operator=(const Postgresql &db);

        /*
        Postgresql(const std::string &host,
                    const std::string &port,
                    const std::string &name,
                    const std::string &user,
                    const std::string &pass);
        */

        // ~Postgresql();

        std::string
            execSql(
                const std::string &sentence);

        std::string
            execSql(
                const std::string &sentence,
                pqxx::result &answer);

        std::string
            execSql(
                std::vector<std::string> &bundle);

        pqxx::result query(
            const std::string &sentence);

/*
        static void extract(
            const pqxx::result &answer,
            int &value);

        static void extract(
            const pqxx::result &answer,
            bool &value);
*/

        template<typename V>
        V extract(const std::string &sentence)
        {
            V value;
            try
            {
                pqxx::work T(*dbConnection, "extract");
                pqxx::result answer= T.exec(sentence);
                if(answer.begin() != answer.end())
                {
                    auto row= answer.begin();
                    value= row[0].as<V>();
                }
                //T.commit();
            }
            catch(const pqxx::sql_error &e)
            {
                Wt::log("error") << "exception: " << e.what();
            }
            catch(const std::exception &e)
            {
                Wt::log("error") << "exception: " << e.what();
            }
            catch(...)
            {
                Wt::log("error") << "unhandled exception\n";
            }
            return value;
        }

    private:

        // pqxx::connection *dbConnection;
        std::shared_ptr<pqxx::connection> dbConnection;
};
