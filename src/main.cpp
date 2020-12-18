#include "SceneGraph/SceneGraph.hpp"
#include <cassert>

int main()
{
    GameObject::Ptr g0 = GameObject::create("AA");
    std::cout << g0->size() << std::endl;
    assert(g0->parent == nullptr);
    assert(g0->children.size() == 0u);

    GameObject& g1 = g0->insert("BB");std::cout << g0->size() << std::endl;
    assert(g0->parent == nullptr);
    assert(g0->children.size() == 1u);
    assert(g1.parent == g0.get());
    assert(g1.children.size() == 0u);

    g1.insert("CC");
    std::cout << g0->children[0]->name() << std::endl;
    std::cout << g0->children[0]->children[0]->name() << std::endl;


    GameObject::Ptr g2 = GameObject::create("EE");
    g1.insert(std::move(g2));
    //g1.insert(GameObject::create("EE"));

    g0->debug();
    std::cout << g0->size() << std::endl;

    g0->clear();

    return 0;
}

#if 0

class QQ: public GameObject
{
public:

    using Ptr = std::shared_ptr<QQ>;

    static Ptr create(std::string const& name) { return make_shared<QQ>(name); }

private:

    virtual void onSetup() override
    {
        std::cout << "onSetup: " << name() << std::endl;

        //Foo::Ptr foo = Foo::create("foo");
        //insert(foo);
    }

    virtual void onUpdate(float const /*dt*/) override
    {
        std::cout << "onUpdate: " << name() << std::endl;
    }
};

int main()
{
    SceneGraph sg;

    QQ::Ptr qq = QQ::create("QQ1");
    sg.insert(qq);

    QQ::Ptr qq2 = QQ::create("QQ2");
    qq.insert(qq2);
    qq.transform.translate(Vector3f(1.0f));


    sg.root(QQ("AA"));
    SceneGraph::Node& n1 = sg.insert(sg.root(), "BB");
    sg.insert(n1, "CC");

    //GameObject& n2 = sg.attach(sg.root(), "DD");
    //sg.attach(n2, "EE");

    n1.data.transform.translate(Vector3f(1.0f));

    sg.update(0.1f);
    sg.print();

    return 0;
}

#endif
