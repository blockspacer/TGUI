/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus's Graphical User Interface
// Copyright (C) 2012-2015 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef TGUI_WIDGETS_HPP
#define TGUI_WIDGETS_HPP


#include <TGUI/Global.hpp>
#include <TGUI/Signal.hpp>
#include <TGUI/Transformable.hpp>
#include <TGUI/Texture.hpp>
#include <TGUI/Color.hpp>
#include <TGUI/Font.hpp>
#include <TGUI/Loading/Deserializer.hpp>
#include <TGUI/RendererDefines.hpp>

#include <TGUI/Aurora/SmartPtr/CopiedPtr.hpp>
#include <TGUI/Aurora/Tools/Downcast.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    class Container;
    class WidgetRenderer;
    struct RendererData;

    enum class ShowAnimationType;
    namespace priv
    {
        class Animation;
    }


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief The parent class for every widget.
    ///
    /// Signals
    ///     - PositionChanged (Position of the widget has changed)
    ///         * Optional parameter sf::Vector2f: New widget position
    ///         * Uses Callback member 'position'
    ///
    ///     - SizeChanged (Size of the widget has changed)
    ///         * Optional parameter sf::Vector2f: New widget size
    ///         * Uses Callback member 'size'
    ///
    ///     - Focused (Widget gained focus)
    ///     - Unfocused (Widget lost focus)
    ///     - MouseEntered (Mouse cursor entered in the Widget area)
    ///     - MouseLeft (Mouse cursor left the Widget area)
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class TGUI_API Widget : public sf::Drawable, public Transformable, public SignalWidgetBase, public std::enable_shared_from_this<Widget>
    {
    public:

        typedef std::shared_ptr<Widget> Ptr; ///< Shared widget pointer
        typedef std::shared_ptr<const Widget> ConstPtr; ///< Shared constant widget pointer


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Default constructor
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Widget();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Copy constructor
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Widget(const Widget&);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Default move constructor
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Widget(Widget&&);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Destructor
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ~Widget();

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Overload of copy assignment operator
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Widget& operator=(const Widget&);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Default move assignment
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Widget& operator=(Widget&&);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Set a new renderer for the widget. The renderer determines how the widget looks.
        ///
        /// @param rendererData  new renderer data
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setRenderer(std::shared_ptr<RendererData> rendererData);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the renderer, which gives access to functions that determine how the widget is displayed
        ///
        /// @return Temporary pointer to the renderer
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        WidgetRenderer* getRenderer() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief set the position of the widget
        ///
        /// This function completely overwrites the previous position.
        /// See the move function to apply an offset based on the previous position instead.
        /// The default position of a transformable widget is (0, 0).
        ///
        /// @param position New position
        ///
        /// @see move, getPosition
        ///
        /// Usage examples:
        /// @code
        /// // Place the widget on an exact position
        /// widget->setPosition({40, 30});
        ///
        /// // Place the widget 50 pixels below another widget
        /// widget->setPosition(otherWidget->getPosition() + sf::Vector2f{0, otherWidget->getSize().y + 50});
        ///
        /// // Place the widget 50 pixels below another widget and automatically move it when the other widget moves
        /// widget->setPosition({tgui::bindLeft(otherWidget), tgui::bindBottom(otherWidget) + 50});
        /// @endcode
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void setPosition(const Layout2d& position) override;
        using Transformable::setPosition;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the size of the widget.
        ///
        /// @param size  Size of the widget
        ///
        /// Usage examples:
        /// @code
        /// // Give the widget an exact size
        /// widget->setSize({40, 30});
        ///
        /// // Make the widget 50 pixels higher than some other widget
        /// widget->setSize(otherWidget->getSize() + sf::Vector2f{0, 50});
        ///
        /// // Make the widget 50 pixels higher than some other widget and automatically resize it when the other widget resizes
        /// widget->setSize(tgui::bindSize(otherWidget) + sf::Vector2f{0, 50});
        /// @endcode
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void setSize(const Layout2d& size) override;
        using Transformable::setSize;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Get the absolute position of the widget on the screen
        ///
        /// @return Position of the widget
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual sf::Vector2f getAbsolutePosition() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Shows the widget.
        ///
        /// The widget is visible by default.
        ///
        /// @see hide
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void show();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Shows the widget by introducing it with an animation.
        ///
        /// The animation will also be played if the widget was already visible.
        ///
        /// During the animation the position, size and/or opacity may change. Once the animation is done the widget will
        /// be back in the state in which it was when this function was called.
        ///
        /// @param type     Type of the animation
        /// @param duration Duration of the animation
        ///
        /// @see hideWithEffect
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void showWithEffect(ShowAnimationType type, sf::Time duration);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Hides the widget.
        ///
        /// The widget won't receive events nor will it be drawn when hidden. The widget is visible by default.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void hide();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Shows the widget by introducing it with an animation.
        ///
        /// If the widget is already hidden then the animation will still play but you will not see it until you show the widget.
        ///
        /// During the animation the position, size and/or opacity may change. Once the animation is done the widget will
        /// be back in the state in which it was when this function was called.
        ///
        /// @param type     Type of the animation
        /// @param duration Duration of the animation
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void hideWithEffect(ShowAnimationType type, sf::Time duration);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns true when the widget is visible.
        ///
        /// @return Is the widget visible?
        ///
        /// If this function returns false then the widget is hidden, which means that it won't receive events and it won't be drawn.
        /// All widgets are visible by default.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool isVisible() const
        {
            return m_visible;
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Enables the widget.
        ///
        /// The widget will receive events and send callbacks again.
        /// All widgets are enabled by default.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void enable();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Disables the widget.
        ///
        /// The widget will no longer receive events and it will thus no longer send callbacks.
        /// All widgets are enabled by default.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void disable();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns true when the widget is enabled.
        ///
        /// @return Is the widget enabled?
        ///
        /// If this function returns false then the widget is disabled and will longer receive events and it will thus no longer send callbacks.
        /// All widgets are enabled by default.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool isEnabled() const
        {
            return m_enabled;
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Focus the widget.
        ///
        /// The previously focused widget will be unfocused.
        ///
        /// @see unfocus
        /// @see focusNextWidget
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void focus();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Unfocus the widget.
        ///
        /// @see focus
        /// @see focusNextWidget
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void unfocus();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns true when the widget is focused and false otherwise.
        ///
        /// @return Is the widget focused?
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool isFocused() const
        {
            return m_focused;
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the type of the widget.
        ///
        /// @return Type of the widget
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const std::string& getWidgetType() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns a pointer to the parent widget.
        ///
        /// @return Pointer to the parent.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Container* getParent() const
        {
            return m_parent;
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Places the widget before all other widgets.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void moveToFront();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Places the widget behind all other widgets.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void moveToBack();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Sets the tool tip that should be displayed when hovering over the widget
        ///
        /// @param toolTip  Any widget that you want to use as a tool tip (usually a Label)
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setToolTip(Widget::Ptr toolTip);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the tool tip that is displayed when hovering over the widget
        ///
        /// @return The widget that is used as tool tip or nullptr when no tool tip has been set
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Widget::Ptr getToolTip();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the font of the text in the widget.
        ///
        /// @param font  The new font.
        ///
        /// When you don't call this function then the font from the parent widget will be used.
        ///
        /// Some widget don't need a font and won't do anything when this function is called.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void setFont(const Font& font);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the font associated with the widget (if any)
        ///
        /// @return Font used by widget
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        std::shared_ptr<sf::Font> getFont() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the distance between the position where the widget is drawn and where the widget is placed
        ///
        /// This is basically the width and height of the optional borders drawn around widgets.
        ///
        /// @return Offset of the widget
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual sf::Vector2f getWidgetOffset() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /// This function is called when the widget is added to a container.
        /// You should not call this function yourself.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void setParent(Container* parent);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /// This function is called every frame with the time passed since the last frame.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void update(sf::Time elapsedTime);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual bool mouseOnWidget(float x, float y) const = 0;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void leftMousePressed(float x, float y);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void leftMouseReleased(float x, float y);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void mouseMoved(float x, float y);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void keyPressed(const sf::Event::KeyEvent& event);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void textEntered(sf::Uint32 key);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void mouseWheelMoved(int delta, int x, int y);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void widgetFocused();

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void widgetUnfocused();

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void mouseNoLongerOnWidget();

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void mouseNoLongerDown();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        // Show the tool tip when the widget is located below the mouse.
        // Returns its tool tip or the tool tip from a child widget if the mouse is on top of the widget.
        // A nullptr is returned when the mouse is not on top of the widget or when the tool tip is empty.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual Widget::Ptr askToolTip(sf::Vector2f mousePos);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Makes a copy of the widget if you don't know its exact type
        ///
        /// This function should only be used when you don't know the type of the widget.
        /// If you know what kind of widget you are copying, you should use the copy function.
        ///
        /// @return Copy of the widget
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual Widget::Ptr clone() const = 0;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    protected:

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Function called when one of the properties of the renderer is changed
        ///
        /// @param property  Lowercase name of the property that was changed
        /// @param value     New value of the property
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void rendererChanged(const std::string& property, ObjectConverter&& value);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief This function is called when the mouse enters the widget.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void mouseEnteredWidget();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief This function is called when the mouse leaves the widget.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void mouseLeftWidget();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Draw the borders around the widget
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void drawBorders(sf::RenderTarget& target, const sf::RenderStates& states, const Borders& borders,
                         const sf::Vector2f& position, const sf::Vector2f& size, const sf::Color& color) const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    private:


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Callback function which is called on a renderer change and which calls the virtual rendererChanged function
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void rendererChangedCallback(const std::string& property, ObjectConverter&& value);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      protected:

        // When a widget is disabled, it will no longer receive events
        bool m_enabled = true;

        // Is the widget visible? When it is invisible it will not receive events and it won't be drawn.
        bool m_visible = true;

        // This will point to our parent widget. If there is no parent then this will be nullptr.
        Container* m_parent = nullptr;

        // Is the mouse on top of the widget? Did the mouse go down on the widget?
        bool m_mouseHover = false;
        bool m_mouseDown = false;

        // Are you focused on the widget?
        bool m_focused = false;

        // Can the widget be focused?
        bool m_allowFocus = false;

        // Keep track of the elapsed time.
        sf::Time m_animationTimeElapsed;

        // This is set to true for widgets that have something to be dragged around (e.g. sliders and scrollbars)
        bool m_draggableWidget = false;

        // This is set to true for widgets that store other widgets inside them
        bool m_containerWidget = false;

        // The tool tip connected to the widget
        Widget::Ptr m_toolTip = nullptr;

        // The font that the widget can use
        std::shared_ptr<sf::Font> m_font = nullptr;

        // Renderer of the widget
        aurora::CopiedPtr<WidgetRenderer> m_renderer = aurora::makeCopied<WidgetRenderer>();

        // Show animations
        std::vector<std::shared_ptr<priv::Animation>> m_showAnimations;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        friend class Container;
        friend class BaseTheme;
        friend class WidgetRenderer;
    };


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Data used in renderer classes
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    struct RendererData
    {
        std::map<std::string, ObjectConverter> propertyValuePairs;
        std::map<void*, std::function<void(const std::string& property, ObjectConverter&& value)>> observers;
    };


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Base class for all renderer classes
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class TGUI_API WidgetRenderer
    {
    public:

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Virtual destructor
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual ~WidgetRenderer() {};


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the opacity of the widget.
        ///
        /// @param opacity  The opacity of the widget. 0 means completely transparent, while 1 (default) means fully opaque.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void setOpacity(float opacity);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the opacity of the widget.
        ///
        /// @return The opacity of the widget. 0 means completely transparent, while 1 (default) means fully opaque.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        float getOpacity() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Change a property of the renderer
        ///
        /// @param property  The property that you would like to change
        /// @param value     The new value that you like to assign to the property.
        ///                  The ObjectConverter is implicitly constructed from the possible value types.
        ///
        /// @throw Exception for unknown properties or when value was of a wrong type.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setProperty(const std::string& property, ObjectConverter&& value);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Retrieve the value of a certain property
        ///
        /// @param property  The property that you would like to retrieve
        ///
        /// @return The value inside a ObjectConverter object which you can extract with the correct get function or
        ///         an ObjectConverter object with type ObjectConverter::Type::None when the property did not exist.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ObjectConverter getProperty(const std::string& property) const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Get a map with all properties and their values
        ///
        /// @return Property-value pairs of the renderer
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const std::map<std::string, ObjectConverter>& getPropertyValuePairs() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Subscribe a callback function to changes in the renderer
        ///
        /// @param id       Unique identifier for this callback function so that you can unsubscribe it later
        /// @param function Callback function to call when the renderer changes
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void subscribe(void* id, const std::function<void(const std::string& property, ObjectConverter&& value)>& function);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Subscribe a callback function to changes in the renderer
        ///
        /// @param id  Unique identifier used when subscribing the callback function
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void unsubscribe(void* id);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the renderer data.
        ///
        /// @return Data that is shared between the renderers.
        ///
        /// @warning You should not make changed to this data directly. Instead, use the function from the renderer classes.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        std::shared_ptr<RendererData> getData() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Get a clone of the renderer data
        ///
        /// You can pass this to a widget with the setRenderer function to have a separate non-shared copy of this renderer.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        std::shared_ptr<RendererData> clone() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    protected:
        std::shared_ptr<RendererData> m_data = std::make_shared<RendererData>();

        friend class Widget; // Only Widget is allowed to replace the m_data variable

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    };



    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // TGUI_WIDGETS_HPP
