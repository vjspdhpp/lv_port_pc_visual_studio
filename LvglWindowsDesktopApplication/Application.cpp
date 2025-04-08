#include "Application.h"

#include "PageManager.h"
#include "MainPage.h"
#include "OptionsPage.h"

Application::Application()
{
    pm = std::make_unique<PageManager>();
    pm.get()->registerPage("/main", []() { return std::make_unique<MainPage>(); });
    pm.get()->registerPage("/options", []() { return std::make_unique<OptionsPage>(); });
    // 导航到首页
    pm->navigate("/main");
}

Application::~Application()
{
}

void Application::init()
{
}

void Application::run()
{
}
