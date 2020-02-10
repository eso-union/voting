// C++
#include <cassert>
#include <iostream>

// Wt
#include <Wt/WLogger.h>

// Voting
#include "postgresql.h"

// Postgresql::Postgresql(const std::string &name)
Postgresql::Postgresql(const dbConfig &par)
{
    // The database connection parameters
    // are set in a string.
    std::string connParam;

    if(par.host != "")
    {
        connParam+= "host=" + par.host + " ";
    }

    if(par.dbname != "")
    {
        connParam+= "dbname=" + par.dbname + " ";
    }

    if(par.user != "")
    {
        connParam+= "user=" + par.user + " ";
    }

    if(par.passw != "")
    {
        connParam+= "password=" + par.passw + " ";
    }

    try
    {
        // dbConnection= new pqxx::connection(connParam);
        dbConnection= std::make_shared<pqxx::connection>(connParam);
        Wt::log("info") << "connected to database";
        Wt::log("info") << "backend version: " << dbConnection->server_version();
        Wt::log("info") << "protocol version: " << dbConnection->protocol_version();

    }
    catch(const std::exception &e)
    {
        Wt::log("error") << "exception: " << e.what();
    }
}

Postgresql::Postgresql(const Postgresql &db)
{
    dbConnection= db.dbConnection;
}

Postgresql& Postgresql::operator=(const Postgresql &db)
{
    if(this == &db)
    {
        return *this;
    }
    dbConnection= db.dbConnection;
    return *this;
}

/*
Postgresql::Postgresql(const std::string &host,
                               const std::string &port,
                               const std::string &name,
                               const std::string &user,
                               const std::string &pass)
{
    std::string connParameters;
    connParameters+= "host=" + host + " ";
    connParameters+= "port=" + port + " ";
    connParameters+= "dbname=" + name + " ";
    connParameters+= "user=" + user + " ";
    connParameters+= "password=" + pass + " ";

    try
    {
        dbConnection= new pqxx::connection(connParameters);
        std::cout << "connected to database \n";
        std::cout << "backend version: " << dbConnection->server_version() << "\n";
        std::cout << "protocol version: " << dbConnection->protocol_version() << std::endl;

    }
    catch(const std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
        throw Exception(COULD_NOT_CONNECT_TO_DATABASE);
    }
}
*/

std::string
    Postgresql::execSql(const std::string &sentence)
{
    std::string error;
    try
    {
        /**
         * Begin a transaction acting on our current connection.
         **/
        pqxx::work T(*dbConnection, "execute_no_answer");

        /**
         * Perform a query on the database, there are no results.
         **/
        T.exec(sentence);

        /**
         * Tell the transaction that it has been successful.
         * This is not really necessary if no modifications are made
         * to commit.
         **/
        T.commit();
    }
    catch(const pqxx::sql_error &e)
    {
        Wt::log("error") << "exception: " << e.what();
        error= e.what();
    }
    catch(const std::exception &e)
    {
        Wt::log("error") << "exception: " << e.what();
        error= e.what();
    }
    catch(...)
    {
        Wt::log("error") << "unhandled exception";
        error= "unhandled exception";
    }
    return error;
}

std::string
    Postgresql::execSql(
        const std::string &sentence,
        pqxx::result &answer)
{
    std::string error;
    try
    {
        /**
         * Begin a transaction acting on our current connection.
         **/
        pqxx::work T(*dbConnection, "execute");

        /**
         * Perform a query on the database, storing result in answer.
         **/
        answer= T.exec(sentence);

        /**
         * Tell the transaction that it has been successful.
         * This is not really necessary if no modifications are made
         * to commit.
         **/
        T.commit();
    }
    catch(const pqxx::sql_error &e)
    {
        Wt::log("error") << "exception: " << e.what();
        error= e.what();
    }
    catch(const std::exception &e)
    {
        Wt::log("error") << "exception: " << e.what();
        error= e.what();
    }
    catch(...)
    {
        Wt::log("error") << "unhandled exception\n";
        error= "unhandled exception";
    }
    return error;
}

std::string
    Postgresql::execSql(
        std::vector<std::string> &bundle)
{
    std::string error;
    try
    {
        /**
         * Begin a transaction acting on our current connection.
         **/
        pqxx::work T(*dbConnection, "execute_several_no_answer");

        for(
            auto it= bundle.begin();
            it != bundle.end();
            ++it)
        {
            const std::string sentence= *it;
            T.exec(sentence);
        }

        T.commit();
    }
    catch(const pqxx::sql_error &e)
    {
        Wt::log("error") << "exception: " << e.what();
        error= e.what();
    }
    catch(const std::exception &e)
    {
        Wt::log("error") << "exception: " << e.what();
        error= e.what();
    }
    catch(...)
    {
        Wt::log("error") << "unhandled exception\n";
        error= "unhandled exception";
    }
    return error;
}

pqxx::result Postgresql::query(
    const std::string &sentence)
{
    pqxx::result answer;
    try
    {
        // Begin a transaction acting on our current connection.
        pqxx::work T(*dbConnection, "execute");

        // Perform a query on the database, storing result in answer.
        answer= T.exec(sentence);

        // Tell the transaction that it has been successful.
        // This is not really necessary if no modifications are made
        // to commit.
        T.commit();
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

    // Return the answer
    return answer;
}

/*
void Postgresql::extract(const pqxx::result &answer, int &value)
{
    const pqxx::result::const_iterator row = answer.begin();
    if (row != answer.end())
    {
        // value= row[0].as(int());
        value= row[0].as<int>();
    }
}

void Postgresql::extract(const pqxx::result &answer, bool &value)
{
    const pqxx::result::const_iterator row = answer.begin();
    if (row != answer.end())
    {
        // value= row[0].as(bool());
        value= row[0].as<bool>();
    }
}
*/
