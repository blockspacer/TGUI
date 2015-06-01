#include <TGUI/VerticalLayout.hpp>
#include <numeric>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    VerticalLayout::VerticalLayout()
    {
        m_callback.widgetType = "VerticalLayout";
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    VerticalLayout::Ptr VerticalLayout::create()
    {
        return std::make_shared<VerticalLayout>();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    VerticalLayout::Ptr VerticalLayout::copy(ConstPtr layout)
    {
        if (layout)
            return std::make_shared<VerticalLayout>(*layout);
        else
            return nullptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void VerticalLayout::updateWidgetPositions()
    {
        float actualPosition = 0;
        const float sumRatio = std::accumulate(m_widgetsRatio.begin(), m_widgetsRatio.end(), 0);
        for (unsigned int i = 0; i < m_layoutWidgets.size(); ++i)
        {
            if (m_layoutWidgets[i])
            {
                m_layoutWidgets[i]->setSize(m_size.x, m_size.y * m_widgetsRatio[i] / sumRatio);
                m_layoutWidgets[i]->setPosition(0, m_size.y * actualPosition);
                actualPosition += m_widgetsRatio[i] / sumRatio;
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    tgui::Widget::Ptr VerticalLayout::clone()
    {
        return std::make_shared<VerticalLayout>(*this);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
