#include "value-to-json.hh"
#include "value-to-xml.hh"
#include "value.hh"
#include "store-api.hh"

#include <gtest/gtest.h>
#include <sstream>

class ValueTest : public ::testing::Test {
    protected:
        static void SetUpTestCase() {
            nix::initGC();
        }

        static void TearDownTestCase() {}
};

class JSONValueTest : public ValueTest {};
class XMLValueTest : public ValueTest {};

namespace nix {


    class DummyStore : public Store {
    public:
        explicit DummyStore() : Store(Store::Params {}) {
        }

        std::string getUri() { return ""; }
        virtual void queryPathInfoUncached(const StorePath &) {}
        virtual void queryPathInfoUncached(const StorePath &, nix::Callback<std::shared_ptr<const nix::ValidPathInfo>>) noexcept {}
        std::optional<StorePath> queryPathFromHashPart(const std::string & hashPart) {
            return {};
        }
        StorePath addToStore(const std::string &, const std::string &, const StorePathSet&, nix::RepairFlag) {
            return StorePath::dummy.clone();
        }
        StorePath addToStore(const std::string &, const Path &, bool, nix::HashType, nix::PathFilter&, nix::RepairFlag) {
            return StorePath::dummy.clone();
        }
       StorePath addTextToStore(const std::string &, const std::string &, const StorePathSet&, nix::RepairFlag) {
                   return StorePath::dummy.clone();
        }

       void narFromPath(const StorePath&, Sink&) {}
       void ensurePath(const StorePath&) {}

       BuildResult buildDerivation(const StorePath &, const BasicDerivation &, BuildMode) {
            return BuildResult {};
       }
    };

    TEST_F(JSONValueTest, null) {
        std::stringstream ss;
        Value v;
        PathSet ps;
    auto store = std::make_shared<DummyStore>();
    EvalState s({}, ref<Store>(store), false);

        mkNull(v);
        printValueAsJSON(s, true, v, ss, ps);
        ASSERT_EQ(ss.str(), "null");
    }

    TEST_F(JSONValueTest, BoolFalse) {
        std::stringstream ss;
        auto store = std::make_shared<DummyStore>();
        EvalState s({ "." }, ref<Store>(store), false);
        Value v;
        PathSet ps;

        mkBool(v, false);
        printValueAsJSON(s, true, v, ss, ps);
        ASSERT_EQ(ss.str(), "false");
    }

    TEST_F(JSONValueTest, BoolTrue) {
        std::stringstream ss;
        auto store = std::make_shared<DummyStore>();
        EvalState s({ "." }, ref<Store>(store), false);
        Value v;
        PathSet ps;

        mkBool(v, true);
        printValueAsJSON(s, true, v, ss, ps);
        ASSERT_EQ(ss.str(), "true");
    }

    TEST_F(JSONValueTest, IntPositive) {
        std::stringstream ss;
        auto store = std::make_shared<DummyStore>();
        EvalState s({ "." }, ref<Store>(store), false);
        Value v;
        PathSet ps;

        mkInt(v, 100);
        printValueAsJSON(s, true, v, ss, ps);
        ASSERT_EQ(ss.str(), "100");
    }

    TEST_F(JSONValueTest, IntNegative) {
        std::stringstream ss;
        auto store = std::make_shared<DummyStore>();
        EvalState s({ "." }, ref<Store>(store), false);
        Value v;
        PathSet ps;

        mkInt(v, -100);
        printValueAsJSON(s, true, v, ss, ps);
        ASSERT_EQ(ss.str(), "-100");
    }

    TEST_F(JSONValueTest, String) {
        std::stringstream ss;
        auto store = std::make_shared<DummyStore>();
        EvalState s({ "." }, ref<Store>(store), false);
        Value v;
        PathSet ps;

        mkString(v, "test");
        printValueAsJSON(s, true, v, ss, ps);
        ASSERT_EQ(ss.str(), "\"test\"");
    }

    TEST_F(JSONValueTest, StringQuotes) {
        std::stringstream ss;
        auto store = std::make_shared<DummyStore>();
        EvalState s({ "." }, ref<Store>(store), false);
        Value v;
        PathSet ps;

        mkString(v, "test\"");
        printValueAsJSON(s, true, v, ss, ps);
        ASSERT_EQ(ss.str(), "\"test\\\"\"");
    }

    TEST_F(JSONValueTest, Path) {
        std::stringstream ss;
        auto store = std::make_shared<DummyStore>();
        EvalState s({ "." }, ref<Store>(store), false);
        Value v;
        PathSet ps;

        mkPath(v, "test");
        printValueAsJSON(s, true, v, ss, ps);
        ASSERT_EQ(ss.str(), "\"/nix/store/g1w7hy3qg1w7hy3qg1w7hy3qg1w7hy3q-x\"");
    }

    TEST_F(JSONValueTest, PathNoCopy) {
        std::stringstream ss;
        auto store = std::make_shared<DummyStore>();
        EvalState s({ "." }, ref<Store>(store), false);
        Value v;
        PathSet ps;

        mkPathNoCopy(v, "test");
        printValueAsJSON(s, true, v, ss, ps);
        ASSERT_EQ(ss.str(), "\"/nix/store/g1w7hy3qg1w7hy3qg1w7hy3qg1w7hy3q-x\"");
    }


    /*
     * Value to XMl tests
     */

#define XML(v) ("<?xml version='1.0' encoding='utf-8'?>\n<expr>\n" v "\n</expr>\n")

    TEST_F(XMLValueTest, null) {
        std::stringstream ss;
        Value v;
        PathSet ps;
    auto store = std::make_shared<DummyStore>();
    EvalState s({}, ref<Store>(store), false);

        mkNull(v);
        printValueAsXML(s, true, true, v, ss, ps);
        ASSERT_EQ(ss.str(), XML("  <null />"));
    }

    TEST_F(XMLValueTest, BoolFalse) {
        std::stringstream ss;
        auto store = std::make_shared<DummyStore>();
        EvalState s({ "." }, ref<Store>(store), false);
        Value v;
        PathSet ps;

        mkBool(v, false);
        printValueAsXML(s, true, true, v, ss, ps);
        ASSERT_EQ(ss.str(), XML("  <bool value=\"false\" />"));
    }

    TEST_F(XMLValueTest, BoolTrue) {
        std::stringstream ss;
        auto store = std::make_shared<DummyStore>();
        EvalState s({ "." }, ref<Store>(store), false);
        Value v;
        PathSet ps;

        mkBool(v, true);
        printValueAsXML(s, true, true, v, ss, ps);
        ASSERT_EQ(ss.str(), XML("  <bool value=\"true\" />"));
    }

    TEST_F(XMLValueTest, IntPositive) {
        std::stringstream ss;
        auto store = std::make_shared<DummyStore>();
        EvalState s({ "." }, ref<Store>(store), false);
        Value v;
        PathSet ps;

        mkInt(v, 100);
        printValueAsXML(s, true, true, v, ss, ps);
        ASSERT_EQ(ss.str(), XML("  <int value=\"100\" />"));
    }

    TEST_F(XMLValueTest, IntNegative) {
        std::stringstream ss;
        auto store = std::make_shared<DummyStore>();
        EvalState s({ "." }, ref<Store>(store), false);
        Value v;
        PathSet ps;

        mkInt(v, -100);
        printValueAsXML(s, true, true, v, ss, ps);
        ASSERT_EQ(ss.str(), XML("  <int value=\"-100\" />"));
    }

    TEST_F(XMLValueTest, String) {
        std::stringstream ss;
        auto store = std::make_shared<DummyStore>();
        EvalState s({ "." }, ref<Store>(store), false);
        Value v;
        PathSet ps;

        mkString(v, "test-value");
        printValueAsXML(s, true, true, v, ss, ps);
        ASSERT_EQ(ss.str(), XML("  <string value=\"test-value\" />"));
    }

    TEST_F(XMLValueTest, StringQuotes) {
        std::stringstream ss;
        auto store = std::make_shared<DummyStore>();
        EvalState s({ "." }, ref<Store>(store), false);
        Value v;
        PathSet ps;

        mkString(v, "test-value\"");
        printValueAsXML(s, true, true, v, ss, ps);
        ASSERT_EQ(ss.str(), XML("  <string value=\"test-value&quot;\" />"));
    }

    /*
     * FIXME: This function returns the original input path while the JSON version of the same
     * actually touches the store to create a /nix/store path.
     */
    TEST_F(XMLValueTest, Path) {
        std::stringstream ss;
        auto store = std::make_shared<DummyStore>();
        EvalState s({ "." }, ref<Store>(store), false);
        Value v;
        PathSet ps;

        mkPath(v, "some-path");
        printValueAsXML(s, true, true, v, ss, ps);
        ASSERT_EQ(ss.str(),
                XML("  <path value=\"some-path\" />")
        );
    }

    /*
     * FIXME: This function returns the original input path while the JSON version of the same
     * actually touches the store to create a /nix/store path.
     */
    TEST_F(XMLValueTest, PathNoCopy) {
        std::stringstream ss;
        auto store = std::make_shared<DummyStore>();
        EvalState s({ "." }, ref<Store>(store), false);
        Value v;
        PathSet ps;

        mkPathNoCopy(v, "some-other-path");
        printValueAsXML(s, true, true, v, ss, ps);
        ASSERT_EQ(ss.str(),
                XML("  <path value=\"some-other-path\" />")
        );
    }

}
