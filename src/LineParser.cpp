#include "LineParser.h"

std::vector<std::string> explode(std::string const & s, char delim)
{
    std::vector<std::string> result;
    std::istringstream iss(s);

    for (std::string token; std::getline(iss, token, delim); )
    {
        result.push_back(std::move(token));
    }

    return result;
}

void separateLineHeader(std::string line, std::string* header, std::string* footer)
{
    std::size_t spacemarker;
    spacemarker = line.find_first_of(" ");
    *header = line.substr(0,spacemarker);
    *footer = line.substr(spacemarker+1);
}

int ParseInt(const std::string& line)
{
    return atoi(line.c_str());
}

float ParseFloat(const std::string& line)
{
    return (float)(atof(line.c_str()));
}

std::string ParseString(const std::string& line)
{
    return line;
}

void ParseDamageEffect(std::string const & line, damageEffect* Effect)
{
    std::vector<std::string> data = explode(line.c_str(),' ');
    Effect->setBaseHealthEffectAmount( ParseInt( data[0] ) );
    Effect->setAOE( ParseInt( data[1] ) );
}
