
#include "PluginImport.h"
#include <string>

PluginImport::PluginImport(std::string pluginName, std::string functionName)
{
	this->pluginName = pluginName;
	this->functionName = functionName;
}

std::string PluginImport::getPluginName()
{
	return this->pluginName;
}

std::string PluginImport::getFunctionName()
{
        return this->functionName;
}

bool PluginImport::operator==(const PluginImport& other) const
{
        return this->pluginName == other.pluginName && this->functionName == other.functionName;
}
