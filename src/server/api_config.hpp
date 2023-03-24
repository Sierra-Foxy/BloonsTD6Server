//
// Created by Rosier on 2023-03-01.
//

#ifndef BLOONSTOOLS_SRC_SERVER_API_CONFIG_HPP
#define BLOONSTOOLS_SRC_SERVER_API_CONFIG_HPP

#include <boost/asio.hpp>

#include <openssl/pem.h>

#include <set>
#include <map>
#include <string>
#include <regex>
#include <optional>

namespace net = boost::asio;

namespace server {

struct ModifiedResponse {
    std::regex Regex;
    std::string Replacement;

    ModifiedResponse(const std::string& RegexStr, const std::string& ReplacementStr);
};

class ApiConfig {
  public:
    void readConfig(const std::string& ConfigFile);

    [[nodiscard]] const std::map<std::string, std::string>& getMappedResponses() const;
    [[nodiscard]] const std::set<std::string>& getGeneratedResponses() const;
    [[nodiscard]] std::map<std::string, ModifiedResponse> getModifiedResponses() const;

    [[nodiscard]] std::optional<std::string> getMappedResponse(const std::string& Url) const;
    [[nodiscard]] bool getGeneratedResponse(const std::string& Url) const;
    [[nodiscard]] std::optional<ModifiedResponse> getModifiedResponse(const std::string& Url) const;

    [[nodiscard]] uint16_t getPort() const;
    [[nodiscard]] const net::ip::address& getAddress() const;

    [[nodiscard]] const std::string& getCaCertificateLocation() const;

  private:
    // Responses that map from an api endpoint to a file
    std::map<std::string, std::string> MappedResponses_;
    // Responses that are custom generated from the database
    std::set<std::string> GeneratedResponses_;
    // Responses that are modified with a regex
    std::map<std::string, ModifiedResponse> ModifiedResponses_;

    // Disables responses being generated, mapped or modified
    bool DisableCustomResponses_{false};

    uint16_t Port_{static_cast<uint16_t>(std::stoi("4430"))};
    net::ip::address Address_{net::ip::make_address("127.0.0.1")};

    std::string CaCertificateLocation_{USER_CONFIG_DIR"/bloonstools-ca"};
};

} // namespace server

#endif // BLOONSTOOLS_SRC_SERVER_API_CONFIG_HPP
