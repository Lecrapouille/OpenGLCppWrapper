// Original code source https://stackoverflow.com/questions/44105058/how-does-unitys-getcomponent-work
// Quote: "I'm wondering how the GetComponent() method that Unity implements works."

#ifndef OPENGLCPPWRAPPER_SCENEGRAPH_COMPONENTS_HPP
#  define OPENGLCPPWRAPPER_SCENEGRAPH_COMPONENTS_HPP

#  include "Common/NonCppStd.hpp"
#  include <algorithm> // find_if
#  include <vector>

// *****************************************************************************
//
// *****************************************************************************
class Component
{
public:

    // -------------------------------------------------------------------------
    virtual ~Component() = default;

    // -------------------------------------------------------------------------
    //! \brief
    // -------------------------------------------------------------------------
    virtual bool isClassType(const uint32_t classType) const
    {
        return classType == Type;
    }

public:

    static const uint32_t Type = "Component"_hash;
};

// *****************************************************************************
//
// *****************************************************************************
class Components
{
public:

    // -------------------------------------------------------------------------
    //! \brief
    // -------------------------------------------------------------------------
    template<class ComponentType, typename... Args>
    void addComponent(Args&&... params)
    {
        m_components.emplace_back(
            std::make_unique<ComponentType>(std::forward<Args>(params)...));
    }

    // -------------------------------------------------------------------------
    //! \brief
    // -------------------------------------------------------------------------
    template<class ComponentType>
    ComponentType& getComponent()
    {
        for (auto && component: m_components)
        {
            if (component->isClassType(ComponentType::Type))
                return *static_cast<ComponentType*>(component.get());
        }

        return *std::unique_ptr<ComponentType>(nullptr);
    }

    // -------------------------------------------------------------------------
    //! \brief
    // -------------------------------------------------------------------------
    template<class ComponentType>
    bool removeComponent()
    {
        if (m_components.empty())
            return false;

        auto index =
                std::find_if(m_components.begin(),
                             m_components.end(),
                             [](std::unique_ptr<Component> & component)
                             {
                                 return component->isClassType(ComponentType::Type);
                             });

        bool success = index != m_components.end();
        if (success)
            m_components.erase(index);

        return success;
    }

    // -------------------------------------------------------------------------
    //! \brief
    // -------------------------------------------------------------------------
    template<class ComponentType>
    std::vector<ComponentType*> getComponents()
    {
        std::vector<ComponentType*> componentsOfType;

        for (auto && component : m_components)
        {
            if (component->isClassType(ComponentType::Type))
                componentsOfType.emplace_back(static_cast<ComponentType*>(component.get()));
        }

        return componentsOfType;
    }

    // -------------------------------------------------------------------------
    //! \brief
    // -------------------------------------------------------------------------
    template<class ComponentType>
    size_t removeComponents()
    {
        if (m_components.empty())
            return 0;

        size_t numRemoved = 0u;
        bool success = false;

        do
        {
            auto index =
                    std::find_if(m_components.begin(),
                                 m_components.end(),
                                 [](std::unique_ptr<Component> & component)
                                 {
                                     return component->isClassType(ComponentType::Type);
                                 });

            success = (index != m_components.end());
            if (success)
            {
                m_components.erase(index);
                ++numRemoved;
            }
        }
        while (success);

        return numRemoved;
    }

private:

    std::vector<std::unique_ptr<Component>> m_components;
};

#endif // OPENGLCPPWRAPPER_SCENEGRAPH_COMPONENTS_HPP
