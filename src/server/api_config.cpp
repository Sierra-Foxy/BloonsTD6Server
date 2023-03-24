//
// Created by Rosier on 2023-03-01.
//

#include "api_config.hpp"

#include "toml.hpp"

#include <fstream>
#include <sstream>

using namespace server;

ModifiedResponse::ModifiedResponse(const std::string& RegexStr,
                                   const std::string& ReplacementStr)
    : Regex(RegexStr)
{
    std::ifstream File(ReplacementStr);

    // Checks if files exists and sets the replacement to the file if it does.
    // Otherwise, sets the replacement to the given string
    if (File.fail()) {
        Replacement = ReplacementStr;
    } else {
        std::ostringstream Buf;
        Buf << File.rdbuf();
        Replacement = Buf.str();
    }
}

void ApiConfig::readConfig(const std::string& ConfigFile) {
    auto Config = toml::parse_file(ConfigFile);

    DisableCustomResponses_ = Config.at_path("Api.Disable_Custom_Responses").value_or(false);

    if (!DisableCustomResponses_) {
        if (Config.at_path("Api.Mapped_Responses")) {
            for (auto& R : *Config["Api"]["Mapped_Responses"].as_table()) {
                std::ifstream File(R.second.as_string()->get());
                if (File.fail()) {
                    throw std::runtime_error("Response file does not exist");
                }
                std::ostringstream Buf;
                Buf << File.rdbuf();
                MappedResponses_.emplace(R.first.str(), Buf.str());
            }
        }

        if (Config.at_path("Api.Generated_Responses")) {
            for (auto& R : *Config["Api"]["Generated_Responses"].as_array()) {
                GeneratedResponses_.emplace(R.as_string()->get());
            }
        }

        if (Config.at_path("Api.Modified_Responses")) {
            for (auto& R : *Config["Api"]["Modified_Responses"].as_array()) {
                ModifiedResponses_.emplace(
                    R.as_table()->get("Url")->as_string()->get(),
                    ModifiedResponse{
                        R.as_table()->get("Regex")->as_string()->get(),
                        R.as_table()->get("Replacement")->as_string()->get()});
            }
        }
    }

}

const std::map<std::string, std::string>&
ApiConfig::getMappedResponses() const {
    return MappedResponses_;
}

const std::set<std::string>&
ApiConfig::getGeneratedResponses() const {
    return GeneratedResponses_;
}

std::map<std::string, ModifiedResponse>
ApiConfig::getModifiedResponses() const {
    return ModifiedResponses_;
}

std::optional<std::string>
ApiConfig::getMappedResponse(const std::string& Url) const {
    auto Itr = MappedResponses_.find(Url);
    if (Itr == MappedResponses_.end())
    {
        return std::nullopt;
    }
    return Itr->second;
}

bool ApiConfig::getGeneratedResponse(const std::string& Url) const {
    if (GeneratedResponses_.find(Url) != GeneratedResponses_.end()) {
        return true;
    }
    return false;
}

std::optional<ModifiedResponse>
ApiConfig::getModifiedResponse(const std::string& Url) const {
    auto Itr = ModifiedResponses_.find(Url);
    if (Itr == ModifiedResponses_.end()) {
        return std::nullopt;
    }
    return Itr->second;
}

uint16_t ApiConfig::getPort() const {
    return Port_;
}

const net::ip::address& ApiConfig::getAddress() const {
    return Address_;
}

const std::string& ApiConfig::getCaCertificateLocation() const {
    return CaCertificateLocation_;
}
