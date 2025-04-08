#pragma once
#include <memory>

class PageManager;

class Application
{
public:
    // 禁止拷贝和赋值
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    // 初始化
    void init();
    // 运行应用程序
    void run();

    static Application& instance()
    {
        static Application app;
        return app;
    }
private:
    // 构造/析构
    explicit Application();
    ~Application();

    std::unique_ptr<PageManager> pm;
};

