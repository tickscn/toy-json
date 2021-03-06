#include <gtest/gtest.h>
#include <toy-json/toy-json.h>

TEST(Value, PARSE_LITERAL)
{
    using namespace TOYJSON;
    Value v;
    EXPECT_EQ(PARSE_OK, v.parse("true")) << "true parse failed";
    EXPECT_EQ(JSON_TRUE, v.getType()) << "true parse error";
    EXPECT_EQ(PARSE_OK, v.parse("  true")) << "true parse failed";
    EXPECT_EQ(JSON_TRUE, v.getType()) << "true parse error";
    EXPECT_EQ(PARSE_OK, v.parse("false")) << "false parse failed";
    EXPECT_EQ(JSON_FALSE, v.getType()) << "false parse error";
    EXPECT_EQ(PARSE_OK, v.parse("null")) << "null parse failed";
    EXPECT_EQ(JSON_NULL, v.getType()) << "null parse error";
    EXPECT_EQ(PARSE_INVALID_VALUE, v.parse("nul")) << "nul expect error";
    EXPECT_EQ(PARSE_EXPECT_SIGINAL_VALUE, v.parse("  null false"));
}

TEST(Value, PARSE_NUMBER)
{
    using namespace TOYJSON;
    Value v;
    EXPECT_EQ(PARSE_OK, v.parse("0"));
    EXPECT_DOUBLE_EQ(0.0, v.getNumber());
    EXPECT_EQ(PARSE_OK, v.parse("-0"));
    EXPECT_DOUBLE_EQ(0.0, v.getNumber());
    EXPECT_EQ(PARSE_OK, v.parse("-0.0"));
    EXPECT_DOUBLE_EQ(0.0, v.getNumber());
    EXPECT_EQ(PARSE_OK, v.parse("1"));
    EXPECT_DOUBLE_EQ(1.0, v.getNumber());
    EXPECT_EQ(PARSE_OK, v.parse("-1"));
    EXPECT_DOUBLE_EQ(-1.0, v.getNumber());
    EXPECT_EQ(PARSE_OK, v.parse("1.5"));
    EXPECT_DOUBLE_EQ(1.5, v.getNumber());
    EXPECT_EQ(PARSE_OK, v.parse("-1.5"));
    EXPECT_DOUBLE_EQ(-1.5, v.getNumber());
    EXPECT_EQ(PARSE_OK, v.parse("3.1416"));
    EXPECT_DOUBLE_EQ(3.1416, v.getNumber());
    EXPECT_EQ(PARSE_OK, v.parse("1E10"));
    EXPECT_DOUBLE_EQ(1E10, v.getNumber());
    EXPECT_EQ(PARSE_OK, v.parse("1e10"));
    EXPECT_DOUBLE_EQ(1E10, v.getNumber());
    EXPECT_EQ(PARSE_OK, v.parse("1E+10"));
    EXPECT_DOUBLE_EQ(1E10, v.getNumber());
    EXPECT_EQ(PARSE_OK, v.parse("1E-10"));
    EXPECT_DOUBLE_EQ(1E-10, v.getNumber());
    EXPECT_EQ(PARSE_OK, v.parse("-1E-10"));
    EXPECT_DOUBLE_EQ(-1E-10, v.getNumber());
    EXPECT_EQ(PARSE_OK, v.parse("-1e-10"));
    EXPECT_DOUBLE_EQ(-1E-10, v.getNumber());
    EXPECT_EQ(PARSE_OK, v.parse("-1E+10"));
    EXPECT_DOUBLE_EQ(-1E10, v.getNumber());
    EXPECT_EQ(PARSE_OK, v.parse("-1E-10"));
    EXPECT_DOUBLE_EQ(-1E-10, v.getNumber());
    EXPECT_EQ(PARSE_OK, v.parse("1.234E+10"));
    EXPECT_DOUBLE_EQ(1.234E+10, v.getNumber());
    EXPECT_EQ(PARSE_OK, v.parse("1.234E-10"));
    EXPECT_DOUBLE_EQ(1.234E-10, v.getNumber());
    /* 特殊值 */
    EXPECT_EQ(PARSE_OK, v.parse("1.0000000000000002"));        // 1 + 2^{-52}
    EXPECT_DOUBLE_EQ(1.0000000000000002, v.getNumber());       // 0x 3f f0 00 00 00 00 00 01
    EXPECT_EQ(PARSE_OK, v.parse("4.9406564584124654e-324"));   // 2^{-1022-52}
    EXPECT_DOUBLE_EQ(4.9406564584124654e-324, v.getNumber());  // 0x 00 00 00 00 00 00 00 01
    EXPECT_EQ(PARSE_OK, v.parse("2.2250738585072009e-308"));   // 2^{-1022}-2^{-1074}
    EXPECT_DOUBLE_EQ(2.2250738585072009e-308, v.getNumber());  // 0x 00 0f ff ff ff ff ff ff
    EXPECT_EQ(PARSE_OK, v.parse("1.7976931348623157e+308"));   // 2^{1024}-2^{971}
    EXPECT_DOUBLE_EQ(1.7976931348623157e+308, v.getNumber());  // 0x 7f ef ff ff ff ff ff ff
}

TEST(Value, PARSE_INVILID)
{
    using namespace TOYJSON;
    Value v;
    EXPECT_EQ(PARSE_INVALID_VALUE, v.parse("+5.0"));
    EXPECT_EQ(PARSE_INVALID_VALUE, v.parse("5."));
    EXPECT_EQ(PARSE_INVALID_VALUE, v.parse(".5"));
    EXPECT_EQ(PARSE_INVALID_VALUE, v.parse("1.e2"));
    EXPECT_EQ(PARSE_INVALID_VALUE, v.parse("1.2e"));
    EXPECT_EQ(PARSE_EXPECT_SIGINAL_VALUE, v.parse("00123"));
    EXPECT_EQ(PARSE_INVALID_VALUE, v.parse("?"));
    EXPECT_EQ(PARSE_INVALID_VALUE, v.parse("/"));
    EXPECT_EQ(PARSE_INVALID_VALUE, v.parse("treu"));
    EXPECT_EQ(PARSE_INVALID_VALUE, v.parse("fales"));
}

inline bool stringEQ(const char* expect, const char* json, size_t length)
{
    for (int i = 0; i < length; ++i)
    {
        if (expect[i] != json[i]) return false;
    }
    return true;
}

#define TEST_STRING(expect, json)                                       \
    do                                                                  \
    {                                                                   \
        EXPECT_EQ(PARSE_OK, v.parse(json));                             \
        EXPECT_EQ(JSON_STRING, v.getType());                            \
        EXPECT_TRUE(stringEQ(expect, v.getString(), v.getStrLength())); \
    } while (0)

TEST(Value, PARSE_STRING)
{
    using namespace TOYJSON;
    Value v;
    TEST_STRING("hello, world", "\"hello, world\"");
    TEST_STRING("", "\"\"");
    TEST_STRING("hello\nworld", "\"hello\\nworld\"");
    TEST_STRING("\" \\ / \b \f \n \r \t", "\"\\\" \\\\ \\/ \\b \\f \\n \\r \\t\"");

    TEST_STRING("Hello\0World", "\"Hello\\u0000World\"");
    TEST_STRING("\x24", "\"\\u0024\"");                    /* Dollar sign U+0024 */
    TEST_STRING("\xC2\xA2", "\"\\u00A2\"");                /* Cents sign U+00A2 */
    TEST_STRING("\xE2\x82\xAC", "\"\\u20AC\"");            /* Euro sign U+20AC */
    TEST_STRING("\xF0\x9D\x84\x9E", "\"\\uD834\\uDD1E\""); /* G clef sign U+1D11E */
    TEST_STRING("\xF0\x9D\x84\x9E", "\"\\ud834\\udd1e\""); /* G clef sign U+1D11E */
}

TEST(Value, SET_VALUE)
{
    using namespace TOYJSON;
    Value v;
    v.setBoolen(true);
    EXPECT_EQ(JSON_TRUE, v.getType());
    v.setNull();
    EXPECT_EQ(JSON_NULL, v.getType());
    v.setBoolen(false);
    EXPECT_EQ(JSON_FALSE, v.getType());
    v.setString("hello, world", 12);
    EXPECT_EQ(JSON_STRING, v.getType());
    EXPECT_STREQ("hello, world", v.getString());
    v.setNull();
    v.setString("", 0);
    EXPECT_STREQ("", v.getString());
}

TEST(Value, PARSE_ARRAY)
{
    using namespace TOYJSON;
    Value v;
    EXPECT_EQ(PARSE_OK, v.parse("[ ]"));
    EXPECT_EQ(JSON_ARRAY, v.getType());
    EXPECT_TRUE(v.getArray(0) == nullptr && v.getArrEnd() == nullptr);

    EXPECT_EQ(PARSE_OK, v.parse("[ null, true , false, 123, \"abc\"]"));
    EXPECT_EQ(JSON_ARRAY, v.getType());
    arrList* p = v.getArray(0);
    for (int i = 0; i < 3; ++i)
    {
        EXPECT_EQ(i, p->v.getType());
        p = p->next;
    }
    EXPECT_EQ(JSON_NUMBER, p->v.getType());
    EXPECT_DOUBLE_EQ(123, p->v.getNumber());
    p = p->next;
    EXPECT_EQ(JSON_STRING, p->v.getType());
    EXPECT_STREQ("abc", p->v.getString());
}

TEST(Value, PARSE_OBJECT)
{
    using namespace TOYJSON;
    Value v;
    EXPECT_EQ(PARSE_OK, v.parse(" { } "));
    EXPECT_EQ(JSON_OBJECT, v.getType());

    EXPECT_EQ(PARSE_OK, v.parse("{ \"n\":null, \"t\":true, \"f\":false, \"i\": 123, \"s\":\"abc\", \"a\":[null,123,false],\"o\":{\"1\":1,\"2\":2,\"3\":3}}"));
    EXPECT_EQ(JSON_OBJECT, v.getType());
    EXPECT_EQ(JSON_NULL, v.getObject("n").getType());
    EXPECT_EQ(JSON_TRUE, v.getObject("t").getType());
    EXPECT_EQ(JSON_FALSE, v.getObject("f").getType());
    EXPECT_EQ(JSON_NUMBER, v.getObject("i").getType());
    EXPECT_EQ(123, v.getObject("i").getNumber());
    EXPECT_EQ(JSON_STRING, v.getObject("s").getType());
    EXPECT_STREQ("abc", v.getObject("s").getString());
    EXPECT_EQ(JSON_ARRAY, v.getObject("a").getType());
    EXPECT_EQ(JSON_OBJECT, v.getObject("o").getType());
}
