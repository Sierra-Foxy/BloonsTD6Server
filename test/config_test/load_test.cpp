//
// Created by Rosier on 2023-03-02.
//
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "api_config.hpp"

BOOST_AUTO_TEST_SUITE(ConfigTestSuite)
    BOOST_AUTO_TEST_CASE(LoadConfig)
    {
        server::ApiConfig Config;
        Config.readConfig("test_config.toml");
        server::ModifiedResponse ModifiedResponse = Config.getModifiedResponse("fast-static-api.nkstatic.com/storage/static/11/638c61427b1a2590bc72514f/public-stats").value();
        BOOST_CHECK(Config.getGeneratedResponse("fast-static-api.nkstatic.com/storage/static/11/ffffffffffffffffffffffff/public-stats"));
        BOOST_CHECK_EQUAL(Config.getMappedResponse("api.ninjakiwi.com/utility/time").value(), "{\"error\":null,\"data\":\"{\\\"date\\\":{\\\"time\\\":1670713522085}}\",\"sig\":\"26c2d48034f674a359bbe2e23f1a20b2b493b7da\"}");
        BOOST_CHECK_EQUAL(ModifiedResponse.Replacement, "100");
        // Might want to add a test for the regex, not sure how to do that though.
    }
BOOST_AUTO_TEST_SUITE_END()