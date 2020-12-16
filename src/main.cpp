#include "SceneGraph/SceneGraph.hpp"

class QQ: public GameObject
{
private:

    virtual void onSetup() override
    {
        std::cout << "onSetup: " << name() << std::endl;
    }

    virtual void onUpdate(float const /*dt*/) override
    {
        std::cout << "onUpdate: " << name() << std::endl;
    }
};

int main()
{
    SceneGraph sg;

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
