#include <TGUI/HorizontalLayout.hpp>
#include <numeric>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    HorizontalLayout::HorizontalLayout()
    {
        m_callback.widgetType = "HorizontalLayout";
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    HorizontalLayout::Ptr HorizontalLayout::create()
    {
        return std::make_shared<HorizontalLayout>();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    HorizontalLayout::Ptr HorizontalLayout::copy(ConstPtr layout)
    {
        if (layout)
            return std::make_shared<HorizontalLayout>(*layout);
        else
            return nullptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void HorizontalLayout::updateWidgetPositions()
    {
        float actualPosition = 0;
        const float sumRatio = std::accumulate(m_widgetsRatio.begin(), m_widgetsRatio.end(), 0);
        for (unsigned int i = 0; i < m_layoutWidgets.size(); ++i)
        {
            if (m_layoutWidgets[i])
            {
                m_layoutWidgets[i]->setSize(m_size.x * m_widgetsRatio[i] / sumRatio, m_size.y);
                m_layoutWidgets[i]->setPosition(m_size.x * actualPosition, 0);
                actualPosition += m_widgetsRatio[i] / sumRatio;
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    tgui::Widget::Ptr HorizontalLayout::clone()
    {
        return std::make_shared<HorizontalLayout>(*this);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
