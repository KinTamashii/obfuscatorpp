#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Infos.hpp>
#include <curlpp/Options.hpp>
#include <sstream>

#include <util/timer.hpp>

#include <future>
#include <thread>
#include <google.hpp>
#include <batch/parse.hpp>


#include "execute.hpp"

#include <util/arrayToInt.hpp>
#include <util/parse.hpp>
#define FMT_HEADER_ONLY
#include <fmt/format.h>


#include <chrono>



inline size_t secondsSinceMidnight() {
    std::chrono::time_point now = std::chrono::system_clock::now();
    std::chrono::time_point today = floor<std::chrono::days>(now);
    return duration_cast<std::chrono::seconds>(now - today).count();
}

batchExecute::batchExecute(const std::string &url) {
    path = url; // Base url where the batchexecute endpoint is located.
    getParameters(); // Get the required info for the url parameters from the site.
    id = secondsSinceMidnight(); // Initialize the id.
}
void batchExecute::getParameters() {
    curlpp::Easy request;
    std::ostringstream stream;
    request.setOpt(new curlpp::options::WriteStream(&stream));
    request.setOpt(new curlpp::options::Url(path));
    request.setOpt(new curlpp::options::Timeout(25));
    request.perform();
    const std::string &str = stream.str();
    const char *cur = &str[0], *end = &str[str.size()];
    
    findArray<"\"FdrFJe\"">(cur, end);
    findString(cur, end);
    csrfToken = getString(cur, end);

    findArray<"\"cfb2h\"">(cur, end);
    findString(cur, end);
    backend = getString(cur, end);

    findArray<"\"qwAQke\"">(cur, end);
    findString(cur, end);
    uiName = getString(cur, end);
}
std::string batchExecute::execute(const std::string &payload, const std::string &functions) {
    std::string url = fmt::format("{}/_/{}/data/batchexecute?rpcids={}&f.sid={}&bl={}&_reqid={}&hl=en",
        path,
        uiName,
        functions,
        csrfToken,
        backend,
        getID()
    );
    curlpp::Easy request;
    std::ostringstream stream;
    request.setOpt(new curlpp::options::WriteStream(&stream));
    request.setOpt(new curlpp::options::Url(url));
    request.setOpt(new curlpp::options::Post(1) );
    request.setOpt(new curlpp::options::PostFields( fmt::format("f.req=[[{}]]", payload) ));
    request.setOpt(new curlpp::options::Timeout(50));
    
    try {
        request.perform();
        long status;
        if ((status = curlpp::infos::ResponseCode::get(request)) != google::status::OK) {
            std::cerr << "Failed: Status Code " << status << '\n';
            return "";
        }
    } catch (curlpp::RuntimeError &err) {
        std::cerr << "Failed: " << err.what() << '\n';
        return "";
    }
    return stream.str();
}

