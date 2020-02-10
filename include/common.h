#pragma once

// Boost
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

// Voting
#include <string>
#include <vector>

struct dbConfig
{
    std::string host;
    std::string dbname;
    std::string user;
    std::string passw;
};

struct emailConfig
{
    std::string name;
    std::string address;
};

const std::string CONFIG_FILE= "/usr/local/etc/voting.json";

const int EMPTY        =  -1;
const int READY        =   0;
const int CHANGED      =   1;
const int GRANT_ACCESS =   2;
const int LOGOUT       =   3;
const int SELECTED     =   4;
const int MINISTER_ID  =   5;
const int COMPLETED    =   6;
const int INCOMPLETE   =   7;
const int NEXT         =   8;
const int SAVED        =   9;

const int TESTING      =   0;
const int REAL         =   1;

const int TYPE_CONFIG  = 100;
const int TYPE_ADMIN   = 200;

const int STEP_0 = 0;
const int STEP_1 = 1;
const int STEP_2 = 2;
const int STEP_3 = 3;
const int STEP_4 = 4;
const int STEP_5 = 5;
const int STEP_6 = 6;

const std::string NO_ERROR = "";

const int PHASE_ACTIVE     =   0;
const int PHASE_STANDBY    =   1;
const int PHASE_CLOSED     =   2;
