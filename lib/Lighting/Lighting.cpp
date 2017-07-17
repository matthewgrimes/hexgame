#include "Lighting.h"
#include "ShaderManager.h"

/*
 *
 * 	Copyright 2011 Etay Meiri
 *
 * 	    This program is free software: you can redistribute it and/or modify
 * 	        it under the terms of the GNU General Public License as published by
 * 	            the Free Software Foundation, either version 3 of the License, or
 * 	                (at your option) any later version.
 *
 * 	                    This program is distributed in the hope that it will be useful,
 * 	                        but WITHOUT ANY WARRANTY; without even the implied warranty of
 * 	                            MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * 	                                GNU General Public License for more details.
 *
 * 	                                    You should have received a copy of the GNU General Public License
 * 	                                        along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 	                                        */

#include <stdio.h>
#include <string.h>

#include "math_utils.h"

LightingTechnique::LightingTechnique()
{
}

GLuint LightingTechnique::GetUniformLocation(const char* Name)
{
    return glGetUniformLocation(ShaderManager::getInstance()->getProgram(),Name);
}

bool LightingTechnique::Init()
{
    m_dirLightLocation.Color = glGetUniformLocation(ShaderManager::getInstance()->getProgram(),"gDirectionalLight.Base.Color");
    m_dirLightLocation.AmbientIntensity = glGetUniformLocation(ShaderManager::getInstance()->getProgram(),"gDirectionalLight.Base.AmbientIntensity");
    m_dirLightLocation.Direction = glGetUniformLocation(ShaderManager::getInstance()->getProgram(),"gDirectionalLight.Direction");
    m_dirLightLocation.DiffuseIntensity = glGetUniformLocation(ShaderManager::getInstance()->getProgram(),"gDirectionalLight.Base.DiffuseIntensity");

    if (m_dirLightLocation.AmbientIntensity == 0xFFFFFFFF ||
            m_dirLightLocation.Color == 0xFFFFFFFF ||
            m_dirLightLocation.DiffuseIntensity == 0xFFFFFFFF ||
            m_dirLightLocation.Direction == 0xFFFFFFFF)
    {
        return false;
    }

    m_numPointLightsLocation = GetUniformLocation("gNumPointLights");
    for (unsigned int i = 0 ; i < sizeof(m_pointLightsLocation)/sizeof(m_pointLightsLocation[0]) ; i++)
    {
        char Name[128];
        memset(Name, 0, sizeof(Name));
        SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Base.Color", i);
        m_pointLightsLocation[i].Color = GetUniformLocation(Name);

        SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Base.AmbientIntensity", i);
        m_pointLightsLocation[i].AmbientIntensity = GetUniformLocation(Name);

        SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Position", i);
        m_pointLightsLocation[i].Position = GetUniformLocation(Name);

        SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Base.DiffuseIntensity", i);
        m_pointLightsLocation[i].DiffuseIntensity = GetUniformLocation(Name);

        SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Atten.Constant", i);
        m_pointLightsLocation[i].Atten.Constant = GetUniformLocation(Name);

        SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Atten.Linear", i);
        m_pointLightsLocation[i].Atten.Linear = GetUniformLocation(Name);

        SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Atten.Exp", i);
        m_pointLightsLocation[i].Atten.Exp = GetUniformLocation(Name);

    }

    m_numSpotLightsLocation = GetUniformLocation("gNumSpotLights");
    for (unsigned int i = 0 ; i < sizeof(m_spotLightsLocation)/sizeof(m_spotLightsLocation[0]) ; i++)
    {
        char Name[128];
        memset(Name, 0, sizeof(Name));
        SNPRINTF(Name, sizeof(Name), "gSpotLights[%d].Base.Base.Color", i);
        m_spotLightsLocation[i].Color = GetUniformLocation(Name);

        SNPRINTF(Name, sizeof(Name), "gSpotLights[%d].Base.Base.AmbientIntensity", i);
        m_spotLightsLocation[i].AmbientIntensity = GetUniformLocation(Name);

        SNPRINTF(Name, sizeof(Name), "gSpotLights[%d].Base.Position", i);
        m_spotLightsLocation[i].Position = GetUniformLocation(Name);

        SNPRINTF(Name, sizeof(Name), "gSpotLights[%d].Direction", i);
        m_spotLightsLocation[i].Direction = GetUniformLocation(Name);

        SNPRINTF(Name, sizeof(Name), "gSpotLights[%d].Cutoff", i);
        m_spotLightsLocation[i].Cutoff = GetUniformLocation(Name);

        SNPRINTF(Name, sizeof(Name), "gSpotLights[%d].Base.Base.DiffuseIntensity", i);
        m_spotLightsLocation[i].DiffuseIntensity = GetUniformLocation(Name);

        SNPRINTF(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Constant", i);
        m_spotLightsLocation[i].Atten.Constant = GetUniformLocation(Name);

        SNPRINTF(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Linear", i);
        m_spotLightsLocation[i].Atten.Linear = GetUniformLocation(Name);

        SNPRINTF(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Exp", i);
        m_spotLightsLocation[i].Atten.Exp = GetUniformLocation(Name);


        if (m_spotLightsLocation[i].Color == 0xFFFFFFFF ||
                m_spotLightsLocation[i].AmbientIntensity == 0xFFFFFFFF ||
                m_spotLightsLocation[i].Position == 0xFFFFFFFF ||
                m_spotLightsLocation[i].Direction == 0xFFFFFFFF ||
                m_spotLightsLocation[i].Cutoff == 0xFFFFFFFF ||
                m_spotLightsLocation[i].DiffuseIntensity == 0xFFFFFFFF ||
                m_spotLightsLocation[i].Atten.Constant == 0xFFFFFFFF ||
                m_spotLightsLocation[i].Atten.Linear == 0xFFFFFFFF ||
                m_spotLightsLocation[i].Atten.Exp == 0xFFFFFFFF)
        {
            printf("Error initializing spotlights\n");
            return false;
        }

    }
    return true;
}

void LightingTechnique::bindDirectionalLight(const DirectionalLight& light)
{
    glUniform3f(m_dirLightLocation.Color, light.Color.x, light.Color.y, light.Color.z);
    glUniform1f(m_dirLightLocation.AmbientIntensity, light.AmbientIntensity);

    glUniform3f(m_dirLightLocation.Direction, light.Direction.x, light.Direction.y, light.Direction.z);
    glUniform1f(m_dirLightLocation.DiffuseIntensity, light.DiffuseIntensity);
}
void LightingTechnique::bindPointLight(unsigned int i, const PointLight* pointLight)
{
    glUniform3f(m_pointLightsLocation[i].Color, pointLight->Color.x, pointLight->Color.y, pointLight->Color.z);
    glUniform1f(m_pointLightsLocation[i].AmbientIntensity, pointLight->AmbientIntensity);
    glUniform1f(m_pointLightsLocation[i].DiffuseIntensity, pointLight->DiffuseIntensity);
    glUniform3f(m_pointLightsLocation[i].Position, pointLight->Position.x, pointLight->Position.y, pointLight->Position.z);
    glUniform1f(m_pointLightsLocation[i].Atten.Constant, pointLight->Attenuation.Constant);
    glUniform1f(m_pointLightsLocation[i].Atten.Linear, pointLight->Attenuation.Linear);
    glUniform1f(m_pointLightsLocation[i].Atten.Exp, pointLight->Attenuation.Exp);
}
void LightingTechnique::bindPointLights(unsigned int numLights, const PointLight* pointLights)
{
    setNumPointLights(numLights);

    for (unsigned int i = 0; i < numLights; i++)
    {
        bindPointLight(i, &pointLights[i]);
    }
}
void LightingTechnique::bindSpotLight(unsigned int i, const SpotLight* spotLight)
{
    glUniform3f(m_spotLightsLocation[i].Color, spotLight->Color.x, spotLight->Color.y, spotLight->Color.z);
    glUniform1f(m_spotLightsLocation[i].AmbientIntensity, spotLight->AmbientIntensity);
    glUniform1f(m_spotLightsLocation[i].DiffuseIntensity, spotLight->DiffuseIntensity);
    glUniform3f(m_spotLightsLocation[i].Position, spotLight->Position.x, spotLight->Position.y, spotLight->Position.z);
    glUniform3f(m_spotLightsLocation[i].Direction, spotLight->Direction.x, spotLight->Direction.y, spotLight->Direction.z);
    glUniform1f(m_spotLightsLocation[i].Cutoff, cosf(ToRadian(spotLight->Cutoff)));
    glUniform1f(m_spotLightsLocation[i].Atten.Constant, spotLight->Attenuation.Constant);
    glUniform1f(m_spotLightsLocation[i].Atten.Linear, spotLight->Attenuation.Linear);
    glUniform1f(m_spotLightsLocation[i].Atten.Exp, spotLight->Attenuation.Exp);
}
void LightingTechnique::setNumPointLights(unsigned int numPointLights)
{
    glUniform1i(m_numPointLightsLocation, numPointLights);
}
void LightingTechnique::setNumSpotLights(unsigned int numSpotLights)
{
    glUniform1i(m_numSpotLightsLocation, numSpotLights);
}
void LightingTechnique::bindSpotLights(unsigned int numLights, const SpotLight* spotLights)
{
    setNumSpotLights(numLights);

    for (unsigned int i = 0; i < numLights; i++)
    {
        bindSpotLight(i, &spotLights[i]);
    }
}
void LightingTechnique::bindPointLights(const std::vector<PointLight>& pointLights)
{
    setNumPointLights(pointLights.size());
    for (unsigned int i = 0; i < pointLights.size(); i++)
    {
        bindPointLight(i,&pointLights[i]);
    }
}
void LightingTechnique::bindSpotLights(const std::vector<SpotLight>& spotLights)
{
    setNumSpotLights(spotLights.size());
    for (unsigned int i = 0; i < spotLights.size(); i++)
    {
        bindSpotLight(i,&spotLights[i]);
    }
}
