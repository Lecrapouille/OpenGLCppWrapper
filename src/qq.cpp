#  include <vector>
#  include <memory>
#  include <iostream>

//! \tparam T derived class deriving from Tree
template<class T>
class Tree
{
public:

    Tree<T>() = default;
    Tree<T>(const Tree<T>&) = delete;
    Tree<T>& operator=(const Tree<T>&) = delete;

    template <typename ...ArgsT>
    T& insert(ArgsT&&... args)
    {
        m_children.push_back(std::make_unique<T>(std::forward<ArgsT>(args)...));
        m_children.back()->m_parent = this;
        return *static_cast<T*>(m_children.back().get());
    }

    T& child(size_t const i)
    {
        return *static_cast<T*>(m_children[i].get());
    }

    T* parent()
    {
        return static_cast<T*>(m_parent);
    }

    Tree* m_parent = nullptr;
    std::vector<std::unique_ptr<Tree>> m_children;
};




class TreeInt: public Tree<TreeInt>
{
public:

    explicit TreeInt(int val = 0)
       : value(val)
    {}

    TreeInt& insert(int value)
    {
        TreeInt& t = Tree<TreeInt>::insert();
        t.value = value;
        return t;
    }

public:

    int value = 0;
};


class GameObject : public Tree<GameObject>
{
public:

    GameObject(std::string const& name)
        : m_name(name)
    {}

    GameObject& insert(std::string const& name)
    {
        GameObject& t = Tree<GameObject>::insert(name);
        return t;
    }

    std::string const& m_name;
};


int main()
{
   TreeInt t0;
   TreeInt& t1 = t0.insert(42);
   t1.insert(43);
   std::cout << t0.child(0).value << std::endl;
   std::cout << t0.child(0).child(0).value << std::endl;


   GameObject g0("AA");
   GameObject& g1 = g0.insert("BB");
   g1.insert("CC");
   std::cout << g0.child(0).m_name << std::endl;
   std::cout << g0.child(0).child(0).m_name << std::endl;

   return 0;
}
