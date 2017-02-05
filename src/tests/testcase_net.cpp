#include "testcase_net.hpp"
#include "util/net.hpp"

using namespace std;
using namespace util;

void TestCaseNet::registerTestFunctions()
{
    REGISTER_TEST_FUNCTION(TestCaseNet, testIpArrayToStrFail)
    REGISTER_TEST_FUNCTION(TestCaseNet, testIpArrayToStrSuccess)
    REGISTER_TEST_FUNCTION(TestCaseNet, testIpStrToArrayFail)
    REGISTER_TEST_FUNCTION(TestCaseNet, testIpStrToArraySuccess)
    REGISTER_TEST_FUNCTION(TestCaseNet, testNtohsAndHtons)
    REGISTER_TEST_FUNCTION(TestCaseNet, testNtohlAndHtonl)
    REGISTER_TEST_FUNCTION(TestCaseNet, testNtohllAndHtonll)
}

void TestCaseNet::setUp()
{
}

void TestCaseNet::tearDown()
{    
}

void TestCaseNet::testIpArrayToStrFail()
{
    unsigned char buf[20];
    memset(buf, 0, sizeof(buf));
    
    //ipv4
    assertTrue(ipArrayToStr(Family_IPv4, 0, 4).empty(), ASSERT_POSITION);
    assertTrue(ipArrayToStr(Family_IPv4, buf, 3).empty(), ASSERT_POSITION);
}

void TestCaseNet::testIpArrayToStrSuccess()
{    
    unsigned char buf[20];
    unsigned char dest[20];

    //ipv4
    memset(buf, 0, sizeof(buf));
    memset(dest, 0, sizeof(dest));
    //192.168.0.1
    buf[0] = 192;
    buf[1] = 168;
    buf[2] = 0;
    buf[3] = 1;
    auto str_address_v4 = ipArrayToStr(Family_IPv4, buf, 4);
    assertFalse(str_address_v4.empty(), ASSERT_POSITION);
    ipStrToArray(Family_IPv4, str_address_v4, dest, 4);
    for (size_t i=0; i<4; ++i)
    {
        assertEquals<unsigned char>(buf[i], dest[i], ASSERT_POSITION);
    }    
    
    //ipv6
    memset(buf, 0, sizeof(buf));
    memset(dest, 0, sizeof(dest));
    //2001:0000:3238:DFE1:63::FEFB
    buf[0] = 0x20;
    buf[1] = 0x01;
    buf[2] = 0x00;
    buf[3] = 0x00;

    buf[4] = 0x32;
    buf[5] = 0x38;
    buf[6] = 0xDF;
    buf[7] = 0xE1;

    buf[8] = 0x00;
    buf[9] = 0x63;
    buf[10] = 0x00;
    buf[11] = 0x00;    
    
    buf[12] = 0x00;
    buf[13] = 0x00;
    buf[14] = 0xFE;
    buf[15] = 0xFB;
    
    auto str_address_v6 = ipArrayToStr(Family_IPv6, buf, 16);
    assertFalse(str_address_v6.empty(), ASSERT_POSITION);
    ipStrToArray(Family_IPv6, str_address_v6, dest, 16);
    for (size_t i=0; i<16; ++i)
    {
        assertEquals<unsigned char>(buf[i], dest[i], ASSERT_POSITION);
    }
}

void TestCaseNet::testIpStrToArrayFail()
{
    //ipv4
    unsigned char buf[20];
    memset(buf, 0, sizeof(buf));
    assertNull(ipStrToArray(Family_IPv4, "abcdefg", buf, 4), ASSERT_POSITION);
    assertNull(ipStrToArray(Family_IPv4, "10.1.0.12.33", buf, 4), ASSERT_POSITION);
    
    //ipv6
    assertNull(ipStrToArray(Family_IPv6, "abcdefg", buf, 4), ASSERT_POSITION);
    assertNull(ipStrToArray(Family_IPv6, "2001:0000:3238:DFE1:63", buf, 20), ASSERT_POSITION);
}

void TestCaseNet::testIpStrToArraySuccess()
{
    //ipv4
    unsigned char buf[20];
    memset(buf, 0, sizeof(buf));
    assertNotNull(ipStrToArray(Family_IPv4, "10.1.0.12", buf, 4), ASSERT_POSITION);
    assertEquals<int>(10, (int)buf[0], ASSERT_POSITION);
    assertEquals<int>(1, (int)buf[1], ASSERT_POSITION);
    assertEquals<int>(0, (int)buf[2], ASSERT_POSITION);
    assertEquals<int>(12, (int)buf[3], ASSERT_POSITION);
    
    //ipv6
    assertNotNull(ipStrToArray(Family_IPv6, "2001:0000:3238:DFE1::63", buf, 20), ASSERT_POSITION);
}

void TestCaseNet::testNtohsAndHtons()
{
    uint16_t data = 0xab12;
    assertEquals<uint16_t>(0x12ab, u_ntohs(data), ASSERT_POSITION);
    assertEquals<uint16_t>(0x12ab, u_htons(data), ASSERT_POSITION);
}

void TestCaseNet::testNtohlAndHtonl()
{
    uint32_t data = 0xfe070034;
    assertEquals<uint32_t>(0x340007fe, u_ntohl(data), ASSERT_POSITION);
    assertEquals<uint32_t>(0x340007fe, u_htonl(data), ASSERT_POSITION);
}

void TestCaseNet::testNtohllAndHtonll()
{
    uint64_t data = 0x0011223344556677;
    assertEquals<uint64_t>(0x7766554433221100, u_ntohll(data), ASSERT_POSITION);
    assertEquals<uint64_t>(0x7766554433221100, u_htonll(data), ASSERT_POSITION);
}