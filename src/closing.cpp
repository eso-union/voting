#include "closing.h"
#include "command.h"

Closing::Closing(const Postgresql &db): Panel(db)
{
    settingType_= TYPE_ADMIN;
    step_= STEP_4;
    description_= "Close voting";

    // addWidget(std::make_unique<Wt::WText>("<h3>Terminate Voting</h3>"));
    setTitle();

    wTerminate_= addWidget(std::make_unique<Wt::WPushButton>("Result and Terminate"));
    wTerminate_->clicked().connect(this, &Closing::terminate);

    wOutput_= addWidget(std::make_unique<Wt::WTextArea>());
    wOutput_->setColumns(40);
    wOutput_->setRows(20);
    wOutput_->setReadOnly(true);

    Wt::WFont mono;
    mono.setFamily(Wt::FontFamily::Monospace, "'Courier New'");
    mono.setSize(12);
    wOutput_->decorationStyle().setFont(mono);

    Wt::WColor bgColor(10, 10, 10);
    wOutput_->decorationStyle().setBackgroundColor(bgColor);

    Wt::WColor fgColor(245, 245, 245);
    wOutput_->decorationStyle().setForegroundColor(fgColor);
}

void Closing::setup(
    const int &type,
    const int &value)
{
    if(type == SELECTED)
    {
        idxVoting_= value;
    }

    // Show the result, i.e. the query, in the output box
    // if the voting is terminated.
}

void Closing::terminate()
{
    wTerminate_->disable();
    generateResult();
    notify(COMPLETED, id_);
}

void Closing::generateResult()
{
    // ******************************************
    std::map<int, std::string> alt;

    Wt::WString selAlternat=
        "SELECT idx, value "
        "FROM alternative "
        "WHERE idx_general={1} "
        "ORDER BY idx ASC";

    selAlternat.arg(idxVoting_);

    Wt::log("info") << "selAlternat: " << selAlternat;

    pqxx::result ansAlternat= db_.query(selAlternat.toUTF8());
    for(auto row: ansAlternat)
    {
        int idx= row[0].as<int>();
        std::string value= row[1].as<std::string>();
        alt[idx]= value;
    }

    // ******************************************

    const std::string RETURN= ""; //"\n";

    std::string sentence= "SELECT code.code, ";

    // SELECT
    int index= 0;
    std::string comma= "";
    for(auto it= alt.begin(); it != alt.end(); ++it)
    {
        Wt::WString piece= "A{1}.value AS {2}";
        piece
            .arg(index)
            .arg(it->second);
        sentence+= comma + piece.toUTF8();
        comma= ", ";
        index++;
    }
    sentence+= " " + RETURN;

    // FROM
    sentence+= "FROM code, general";
    for(int i=0; i<alt.size(); i++)
    {
        Wt::WString piece= ", vote A{1}";
        piece.arg(i);
        sentence+= piece.toUTF8();
    }
    sentence+= " " + RETURN;

    // WHERE
    sentence+= "WHERE general.idx=code.idx_general ";
    sentence+= RETURN;

    // AND
    sentence+= "AND general.idx=" + std::to_string(idxVoting_) + " ";
    sentence+= RETURN;

    // AND
    index= 0;
    for(auto it= alt.begin(); it != alt.end(); ++it)
    {
        Wt::WString piece= "AND A{1}.idx_alternative={2}";
        piece
            .arg(index)
            .arg(it->first);
        sentence+= piece.toUTF8() + " ";
        sentence+= RETURN;
        index++;
    }

    // AND
    const std::string pAnd= "AND ";
    const std::string pEq= "=";
    std::string pieceA= "code.idx";
    for(int i=0; i<alt.size(); i++)
    {
        Wt::WString pieceB= "A{1}.idx_code";
        pieceB.arg(i);
        sentence+= pAnd + pieceA + pEq + pieceB.toUTF8() + " ";
        pieceA= pieceB.toUTF8();
        sentence+= RETURN;
    }

    // ORDER
    sentence+= "ORDER BY code.code ASC;";
    sentence+= RETURN;

    // *********************************************

    const std::string RET= "\n";
    std::string command;
    std::string result;

    command= "mkdir -p /tmp/voting/";
    wOutput_->setText(wOutput_->text() + RET + command);

    result= Command::execute(command);
    wOutput_->setText(wOutput_->text() + RET + result + RET);

    command= "psql -d voting00 -c \"" + sentence + "\"";
    wOutput_->setText(wOutput_->text() + RET + command + RET);

    result= Command::execute(command);
    wOutput_->setText(wOutput_->text() + RET + result + RET);
}
