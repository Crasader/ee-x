//
//  FacebookGraphRequest.hpp
//  ee_x_mobile_facebook
//
//  Created by eps on 3/22/18.
//

#ifndef EE_X_FACEBOOK_GRAPH_REQUEST_HPP
#define EE_X_FACEBOOK_GRAPH_REQUEST_HPP

#include <map>
#include <string>

#include "ee/FacebookFwd.hpp"

namespace ee {
namespace facebook {
class GraphRequest {
private:
    using Self = GraphRequest;

public:
    GraphRequest();

    Self& setPath(const std::string& path);
    Self& setParameter(const std::string& key, const std::string& value);
    
    std::string toString() const;

private:
    friend Bridge;

    std::string path_;
    std::map<std::string, std::string> parameters_;
};
} // namespace facebook
} // namespace ee

#endif /* EE_X_FACEBOOK_GRAPH_REQUEST_HPP */
