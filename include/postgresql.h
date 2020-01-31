#ifndef POSTGRESQL_H
#define POSTGRESQL_H

/// C++
#include <string>
#include <vector>

/// Boost
#include <boost/scoped_ptr.hpp>

/// Postgresql
#include <pqxx/pqxx>

/**
 *
 **/
class Postgresql
{
    public:

        Postgresql(const std::string &name);

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

        void execSql(const std::string &sentence);

        void execSql(
            const std::string &sentence,
            pqxx::result &answer);

        void execSql(std::vector<std::string> &bundle);

        pqxx::result query(
            const std::string &sentence);

        static void extract(
            const pqxx::result &answer,
            int &value);

        static void extract(
            const pqxx::result &answer,
            bool &value);

    private:

        // pqxx::connection *dbConnection;
        std::shared_ptr<pqxx::connection> dbConnection;
};

#endif /// POSTGRESQL_H
