#include "testcase_factory.hpp"
#include "util/factory.hpp"

using namespace std;
using namespace util;

void TestCaseFactory::registerTestFunctions()
{
    REGISTER_TEST_FUNCTION(TestCaseFactory, testFactory)
    REGISTER_TEST_FUNCTION(TestCaseFactory, testCloneFactory)
}

void TestCaseFactory::setUp()
{
}

void TestCaseFactory::tearDown()
{    
}

class Model
{
public:
    Model(const string& name, int value) :
        name_(name),
        value_(value)
    {}
    virtual ~Model() {}
    string getName() const { return name_; }
    void setName(const string& name) { name_ = name; }
    int getValue() const { return value_; }
    void setValue(int value) { value_ = value; }
private:
    string name_;
    int value_;
};

class A : public Model
{
public:
    A() : Model("A", 1) {}
};

class B : public Model
{
public:
    B() : Model("B", 2) {}
};

class C : public Model
{
public:
    C() : Model("C", 3) {}
};

void TestCaseFactory::testFactory()
{
    Factory<Model, string> factory;
    assertTrue(factory.Register("Model", [] () { return new Model("Model", 0); }), ASSERT_POSITION);
    assertTrue(factory.Register("A", [] () { return new A(); }), ASSERT_POSITION);
    assertTrue(factory.Register("B", [] () { return new B(); }), ASSERT_POSITION);
    assertTrue(factory.Register("C", [] () { return new C(); }), ASSERT_POSITION);
    
    UtilAutoPtr<Model> m(factory.createObject("Model"));
    assertNotNull(m.get(), ASSERT_POSITION);
    assertEquals<string>("Model", m->getName(), ASSERT_POSITION);
    assertEquals<int>(0, m->getValue(), ASSERT_POSITION);
    
    m.reset(factory.createObject("A"));
    assertNotNull(m.get(), ASSERT_POSITION);
    assertEquals<string>("A", m->getName(), ASSERT_POSITION);
    assertEquals<int>(1, m->getValue(), ASSERT_POSITION);
    
    m.reset(factory.createObject("B"));
    assertNotNull(m.get(), ASSERT_POSITION);
    assertEquals<string>("B", m->getName(), ASSERT_POSITION);
    assertEquals<int>(2, m->getValue(), ASSERT_POSITION);
    
    m.reset(factory.createObject("C"));
    assertNotNull(m.get(), ASSERT_POSITION);
    assertEquals<string>("C", m->getName(), ASSERT_POSITION);
    assertEquals<int>(3, m->getValue(), ASSERT_POSITION);
    
    m.reset(factory.createObject("D"));
    assertNull(m.get(), ASSERT_POSITION);
    
    assertFalse(factory.Register("A", [] () { return new A(); }), ASSERT_POSITION);
    assertFalse(factory.unRegister("D"), ASSERT_POSITION);
    assertTrue(factory.unRegister("A"), ASSERT_POSITION);

    m.reset(factory.createObject("A"));
    assertNull(m.get(), ASSERT_POSITION);
}

void TestCaseFactory::testCloneFactory()
{
    CloneFactory<Model> factory;
    
    assertTrue(factory.Register(typeid(Model), [] (Model* m) { auto p = new Model(m->getName(), m->getValue()); return p; }), ASSERT_POSITION);
    assertTrue(factory.Register(typeid(A), [] (Model* m) { auto p = new A(); p->setName(m->getName()); return p; }), ASSERT_POSITION);
    assertTrue(factory.Register(typeid(B), [] (Model* m) { auto p = new B(); p->setValue(m->getValue()); return p; }), ASSERT_POSITION);
    
    UtilAutoPtr<Model> model(new Model("ModelX", -1));
    UtilAutoPtr<Model> a(new A());
    a->setName("AX");
    a->setValue(22);
    UtilAutoPtr<Model> b(new B());
    b->setName("BX");
    b->setValue(33);
    UtilAutoPtr<Model> c(new C());
    c->setName("CX");
    c->setValue(44);
    
    UtilAutoPtr<Model> m(factory.createObject(model.get()));
    assertNotNull(m.get(), ASSERT_POSITION);
    assertEquals<string>("ModelX", m->getName(), ASSERT_POSITION);
    assertEquals<int>(-1, m->getValue(), ASSERT_POSITION);
    
    m.reset(factory.createObject(a.get()));
    assertNotNull(m.get(), ASSERT_POSITION);
    assertEquals<string>("AX", m->getName(), ASSERT_POSITION);
    assertEquals<int>(1, m->getValue(), ASSERT_POSITION);
    
    m.reset(factory.createObject(b.get()));
    assertNotNull(m.get(), ASSERT_POSITION);
    assertEquals<string>("B", m->getName(), ASSERT_POSITION);
    assertEquals<int>(33, m->getValue(), ASSERT_POSITION);
    
    m.reset(factory.createObject(c.get()));
    assertNull(m.get(), ASSERT_POSITION);
    
    assertFalse(factory.Register(typeid(A), [] (Model* m) { return new A(); }), ASSERT_POSITION);
    assertFalse(factory.unRegister(typeid(C)), ASSERT_POSITION);
    assertTrue(factory.unRegister(typeid(A)), ASSERT_POSITION);

    m.reset(factory.createObject(a.get()));
    assertNull(m.get(), ASSERT_POSITION);
}
