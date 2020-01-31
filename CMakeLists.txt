cmake_minimum_required(VERSION 3.1)

project(
    Voting VERSION 1.0
    DESCRIPTION "Online Voting System"
    LANGUAGES CXX)

#file(GLOB SOURCES "src/*.cpp")
#add_executable(testStudent ${SOURCES})

add_executable(voting_config
               src/mainConfig.cpp
               src/stepInfo.cpp
               src/access.cpp
               src/question.cpp
               src/posibilities.cpp
               src/people.cpp
               src/postgresql.cpp
               src/panel.cpp
               src/nameEmail.cpp
               src/configuration.cpp
               src/votingSelection.cpp
               src/alternative.cpp
               src/votingType.cpp)

add_executable(voting_admin
               src/mainAdmin.cpp
               src/administration.cpp
               src/stepInfo.cpp
               src/access.cpp
               src/postgresql.cpp
               src/panel.cpp
               src/passwCreation.cpp
               src/closing.cpp
               src/pending.cpp
               src/invitations.cpp
               src/voterList.cpp
               src/minister.cpp
               src/voteChoosing.cpp
               src/command.cpp)

add_executable(cast_vote
               src/mainCastVote.cpp
               src/castVote.cpp
               src/postgresql.cpp
               src/panel.cpp
               src/code.cpp
               src/sendingVote.cpp
               src/finish.cpp)



target_include_directories(voting_config PUBLIC include)

target_include_directories(voting_admin PUBLIC include)

target_include_directories(cast_vote PUBLIC include)



target_link_libraries(voting_config PUBLIC -lpqxx -lwthttp -lwt -lboost_random)

target_link_libraries(voting_admin PUBLIC -lpqxx -lwthttp -lwt -lboost_random)

target_link_libraries(cast_vote PUBLIC -lpqxx -lwthttp -lwt -lboost_random)
