// C++
#include <cassert>
#include <iostream>

//
// #include "common.h"
// #include "exception.h"
#include "postgresql.h"

Postgresql::Postgresql(const std::string &name)
{
    std::string connParameters;

    if(name != "")
    {
        connParameters+= "dbname=" + name + " ";
    }

    try
    {
        // dbConnection= new pqxx::connection(connParameters);
        dbConnection= std::make_shared<pqxx::connection>(connParameters);
        std::cout << "connected to database \n";
        std::cout << "backend version: " << dbConnection->server_version() << "\n";
        std::cout << "protocol version: " << dbConnection->protocol_version() << std::endl;

    }
    catch(const std::exception &e)
    {
        std::cerr << "exception: " << e.what() << std::endl;
    }
}

Postgresql::Postgresql(const Postgresql &db)
{
    dbConnection = db.dbConnection;
}

Postgresql& Postgresql::operator=(const Postgresql &db)
{
    if(this == &db)
    {
        return *this;
    }
    dbConnection = db.dbConnection;
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

void Postgresql::execSql(const std::string &sentence)
{
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
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    catch(const std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    catch(...)
    {
        std::cerr << "Unhandled exception\n";
    }
}

void Postgresql::execSql(
    const std::string &sentence,
    pqxx::result &answer)
{
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
        throw(e);
    }
    catch(const std::exception &e)
    {
        /**
         * All exceptions thrown by libpqxx are
         * derived from std::exception.
         **/
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    catch(...)
    {
        /**
         * This is unexpected.
         **/
        std::cerr << "Unhandled exception\n";
    }
}

void Postgresql::execSql(std::vector<std::string> &bundle)
{
    try
    {
        /**
         * Begin a transaction acting on our current connection.
         **/
        pqxx::work T(*dbConnection, "execute_several_no_answer");

        for(std::vector<std::string>::iterator it= bundle.begin(); it!=bundle.end(); ++it)
        {
            const std::string sentence= *it;
            T.exec(sentence);
        }

        T.commit();
    }
    catch(const pqxx::sql_error &e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    catch(const std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    catch(...)
    {
        std::cerr << "Unhandled exception\n";
    }
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
        throw(e);
    }
    catch(const std::exception &e)
    {
        /**
         * All exceptions thrown by libpqxx are
         * derived from std::exception.
         **/
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    catch(...)
    {
        /**
         * This is unexpected.
         **/
        std::cerr << "Unhandled exception\n";
    }

    // Return the answer
    return answer;
}

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
