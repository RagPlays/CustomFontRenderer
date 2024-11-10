#include "enginepch.h"
#include "Layer.h"

namespace Engine
{
    Layer::Layer(const std::string& name)
        : m_DebugName{ name }
    {
    }

    const std::string& Layer::GetName() const
    {
        return m_DebugName;
    }
}