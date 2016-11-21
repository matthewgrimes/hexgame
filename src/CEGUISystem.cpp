#include "CEGUISystem.h"



CEGUISystem::CEGUISystem()
{
}


CEGUISystem::~CEGUISystem()
{
}

void CEGUISystem::Initialize()
{
    CEGUI::OpenGL3Renderer& CEGUITempRenderer = CEGUI::OpenGL3Renderer::bootstrapSystem();
    renderer = &CEGUITempRenderer;
    initCEGUI();

    windowManager = CEGUI::WindowManager::getSingletonPtr();
    // load font
    CEGUI::FontManager::getSingleton().createFromFile("Jura-18.font");
    CEGUI::FontManager::getSingleton().createFromFile("Jura-13.font");
}

void CEGUISystem::Cleanup()
{
    static_cast<CEGUI::OpenGL3Renderer*>(renderer)->destroySystem();
}

void CEGUISystem::HandleEvent(SDL_Event event)
{
    // get the context
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();

    // we use a switch to determine the event type
    switch (event.type)
    {
    // mouse motion handler
    case SDL_MOUSEMOTION:
        // we inject the mouse position directly.
        context.injectMousePosition(
            static_cast<float>(event.motion.x),
            static_cast<float>(event.motion.y)
        );

        break;

    // mouse down handler
    case SDL_MOUSEBUTTONDOWN:
        // let a special function handle the mouse button down event
        switch (event.button.button)
        {
        // handle real mouse buttons
        case SDL_BUTTON_LEFT:
            context.injectMouseButtonDown(CEGUI::LeftButton);
            break;
        case SDL_BUTTON_MIDDLE:
            context.injectMouseButtonDown(CEGUI::MiddleButton);
            break;
        case SDL_BUTTON_RIGHT:
            context.injectMouseButtonDown(CEGUI::RightButton);
            break;

            // handle the mouse wheel
            /*case SDL_BUTTON_WHEELDOWN:
            CEGUI::System::getSingleton().injectMouseWheelChange( -1 );
            break;
            case SDL_BUTTON_WHEELUP:
            CEGUI::System::getSingleton().injectMouseWheelChange( +1 );
            break;*/
        }
        break;

    // mouse up handler
    case SDL_MOUSEBUTTONUP:
        // let a special function handle the mouse button up event
    {
        switch (event.button.button)
        {
        case SDL_BUTTON_LEFT:
            context.injectMouseButtonUp(CEGUI::LeftButton);
            break;
        case SDL_BUTTON_MIDDLE:
            context.injectMouseButtonUp(CEGUI::MiddleButton);
            break;
        case SDL_BUTTON_RIGHT:
            context.injectMouseButtonUp(CEGUI::RightButton);
            break;
        }
    }
    break;

    // mouse wheel handler
    case SDL_MOUSEWHEEL:
    {
    }
    break;


        // Still need to deal with keyboard input

    }
}

void CEGUISystem::subscribeFunction(std::string windowName, std::function<void(void)> function)
{

    auto CEGUIfunction = [function](const CEGUI::EventArgs& e)
    {
        function();
        return true;
    };

    rootWindow->getChild(windowName)->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUIfunction);
}


void CEGUISystem::subscribeFunction(std::string parentName, std::string windowName, std::function<void(void)> function)
{
    auto CEGUIfunction = [function](const CEGUI::EventArgs& e)
    {
        function();
        return true;
    };

    rootWindow->getChild(parentName)->getChild(windowName)->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUIfunction);
}

void CEGUISystem::set_CEGUI_paths()
{

    // Initialises the required directories for the DefaultResourceProvider:
    //
    CEGUI::DefaultResourceProvider & defaultResProvider =
        *static_cast<CEGUI::DefaultResourceProvider*>
        (CEGUI::System::getSingleton().getResourceProvider());

    // For each resource type, sets a corresponding resource group directory:

    defaultResProvider.setResourceGroupDirectory("schemes",
            "data/schemes/");

    defaultResProvider.setResourceGroupDirectory("imagesets",
            "data/imagesets/");

    defaultResProvider.setResourceGroupDirectory("fonts",
            "data/fonts/");

    defaultResProvider.setResourceGroupDirectory("layouts",
            "data/layouts/");

    defaultResProvider.setResourceGroupDirectory("looknfeels",
            "data/looknfeel/");

    defaultResProvider.setResourceGroupDirectory("lua_scripts",
            "data/lua_scripts/");

    defaultResProvider.setResourceGroupDirectory("schemas",
            "data/xml_schemas/");

    defaultResProvider.setResourceGroupDirectory("animations",
            "data/animations/");

    // Sets the default resource groups to be used:

    CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
    CEGUI::Font::setDefaultResourceGroup("fonts");
    CEGUI::Scheme::setDefaultResourceGroup("schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
    CEGUI::WindowManager::setDefaultResourceGroup("layouts");
    CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");
    CEGUI::AnimationManager::setDefaultResourceGroup("animations");

    CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");

    // Set-up default group for validation schemas:
    CEGUI::XMLParser * parser = CEGUI::System::getSingleton().getXMLParser();
    if (parser->isPropertyPresent("SchemaDefaultResourceGroup"))
    {
        parser->setProperty("SchemaDefaultResourceGroup", "schemas");
    }
}

void CEGUISystem::initCEGUI()
{

    // load schemes and set up defaults
    set_CEGUI_paths();
    //CEGUI::SchemeManager::getSingleton().create( "TaharezLook.scheme" ) ;
    CEGUI::SchemeManager::getSingleton().createFromFile("WindowsLook.scheme");
    //CEGUI::System::getSingleton().setDefaultMouseCursor( "TaharezLook", "MouseArrow" ) ;
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("WindowsLook/MouseArrow");
    // set tool tip
    CEGUI::System::getSingleton().getDefaultGUIContext().setDefaultTooltipType("TaharezLook/Tooltip");
}

void CEGUISystem::enableWindow(std::string parentName, std::string name)
{
    rootWindow->getChild(parentName)->getChild(name)->enable();
}

void CEGUISystem::disableWindow(std::string parentName, std::string name)
{
    rootWindow->getChild(parentName)->getChild(name)->disable();
}

void CEGUISystem::hideWindow(std::string name)
{
    rootWindow->getChild(name)->hide();
}

void CEGUISystem::showWindow(std::string name)
{
    rootWindow->getChild(name)->show();
}

void CEGUISystem::setProperty(const std::string & windowName, const std::string & propertyName, const std::string & value)
{
    rootWindow->getChild(windowName)->setProperty(propertyName, value);
}

void CEGUISystem::injectDeltaTime(const float deltaTime)
{
    CEGUI::System::getSingleton().getDefaultGUIContext().injectTimePulse(deltaTime);//!< Inject time pulse
}

void CEGUISystem::Render()
{
    CEGUI::System::getSingleton().renderAllGUIContexts();
}


/*void CEGUISystem::subscribeFunction(void * window, std::function<void(void)> function)
{
	auto CEGUIfunction = [function](const CEGUI::EventArgs& e) { function(); return true; };
	static_cast<CEGUI::Window*>(window)->subscribeEvent(CEGUI::PushButton::EventClicked,
		CEGUIfunction);
}*/

void CEGUISystem::addChildFromFile(std::string layout)
{
    CEGUI::Window* newWindow = windowManager->loadLayoutFromFile(layout);
    rootWindow->addChild(newWindow);
}

void CEGUISystem::addRootFromFile(std::string file)
{
    rootWindow = windowManager->createWindow("DefaultWindow", "Root");
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(rootWindow);
}

std::string CEGUISystem::getNameOfWindowContainingMouse()
{
    return CEGUI::System::getSingleton().getDefaultGUIContext().getWindowContainingMouse()->getName().c_str();
}
