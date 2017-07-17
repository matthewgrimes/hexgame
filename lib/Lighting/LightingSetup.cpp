#include "LightingSetup.h"

LightingSetup::LightingSetup()
{
    //ctor
    m_PointLights.clear();
    m_SpotLights.clear();
}

LightingSetup::~LightingSetup()
{
    //dtor
}

void LightingSetup::Init()
{
    m_LightingTechnique.Init();
}
void LightingSetup::Bind()
{
    m_LightingTechnique.bindPointLights(m_PointLights);
    m_LightingTechnique.bindSpotLights(m_SpotLights);

}

bool LightingSetup::loadFromFile(const std::string& fileName)
{
    LightingSetupFileLoader Loader(fileName);
    Loader.setLightingSetup(this);
    Loader.parseFile();
    return 0;
}
void LightingSetupFileLoader::parseLine(const std::string& field, const std::string& data)
{
    if( field == "spotlights")
    {
        m_LSetup->m_SpotLights.resize(ParseInt(data));
    }
    else if ( field == "pointlights" )
    {
        m_LSetup->m_PointLights.resize(ParseInt(data));
    }
    else if ( field == "pointlight" )
    {
        std::vector<std::string> specs = explode(data.c_str(),' ');
        if ( ParseInt(specs[0]) >= m_LSetup->m_PointLights.size() )
        {
            return;
        }
        if ( specs[1] == "diffuse" )
        {
            m_LSetup->m_PointLights[ParseInt(specs[0])].DiffuseIntensity = ParseFloat(specs[2]);
        }
        else if ( specs[1] == "color" )
        {
            m_LSetup->m_PointLights[ParseInt(specs[0])].Color = Vector3f(ParseFloat(specs[2]),ParseFloat(specs[3]),ParseFloat(specs[4]));
        }
        else if ( specs[1] == "position" )
        {
            m_LSetup->m_PointLights[ParseInt(specs[0])].Position = Vector3f(ParseFloat(specs[2]),ParseFloat(specs[3]),ParseFloat(specs[4]));

        }
        else if ( specs[1] == "attenuationlinear" )
        {
            m_LSetup->m_PointLights[ParseInt(specs[0])].Attenuation.Linear = ParseFloat(specs[2]);
        }
    }
    else if ( field == "spotlight" )
    {
        std::vector<std::string> specs = explode(data.c_str(),' ');
        if ( ParseInt(specs[0]) >= m_LSetup->m_SpotLights.size() )
        {
            return;
        }
        if ( specs[1] == "diffuse" )
        {
            m_LSetup->m_SpotLights[ParseInt(specs[0])].DiffuseIntensity = ParseFloat(specs[2]);
        }
        else if ( specs[1] == "color" )
        {
            m_LSetup->m_SpotLights[ParseInt(specs[0])].Color = Vector3f(ParseFloat(specs[2]),ParseFloat(specs[3]),ParseFloat(specs[4]));
        }
        else if ( specs[1] == "position" )
        {
            m_LSetup->m_SpotLights[ParseInt(specs[0])].Position = Vector3f(ParseFloat(specs[2]),ParseFloat(specs[3]),ParseFloat(specs[4]));

        }
        else if ( specs[1] == "direction" )
        {
            m_LSetup->m_SpotLights[ParseInt(specs[0])].Direction = Vector3f(ParseFloat(specs[2]),ParseFloat(specs[3]),ParseFloat(specs[4])).Normalize();

        }
        else if ( specs[1] == "attenuationlinear" )
        {
            m_LSetup->m_SpotLights[ParseInt(specs[0])].Attenuation.Linear = ParseFloat(specs[2]);
        }
        else if ( specs[1] == "cutoff" )
        {
            m_LSetup->m_SpotLights[ParseInt(specs[0])].Cutoff = ParseFloat(specs[2]);
        }
    }
};
