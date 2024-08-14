//
// Created by root on 7/31/24.
//

#ifndef MARKETSHARKCPP_PROXYREQUEST_H
#define MARKETSHARKCPP_PROXYREQUEST_H

#include <string>
#include <mclib/common/internal/json.hpp>

using json = nlohmann::json;

class ProxyRequest {
private:
    bool uploadEnabled;
    std::string id;
    std::string url;

public:
    ProxyRequest() {}

    ProxyRequest(bool uploadEnabled, const std::string& id, const std::string& url)
            : uploadEnabled(uploadEnabled), id(id), url(url) {}

    std::string getId() const {
        return id;
    }

    std::string getUrl() const {
        return url;
    }

    bool isUploadEnabled() const {
        return uploadEnabled;
    }

    friend void to_json(json& j, const ProxyRequest& p) {
        j = json{{"upload", p.uploadEnabled}, {"id", p.id}, {"url", p.url}};
    }

    friend void from_json(const json& j, ProxyRequest& p) {
        j.at("upload").get_to(p.uploadEnabled);
        j.at("id").get_to(p.id);
        j.at("url").get_to(p.url);
    }
};

#endif //MARKETSHARKCPP_PROXYREQUEST_H
