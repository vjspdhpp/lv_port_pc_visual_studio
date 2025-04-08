#pragma once
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <memory>
#include <functional>
#include "Navigator.h"

class PageBase;
typedef PageBase Page;

class PageManager : public Navigator
{
public:
    // 页面管理基本操作

    void registerPage(const std::string& path, std::function<std::unique_ptr<Page>()> creator); // 注册页面工厂
    void pop() override;                 // 弹出当前页面
    bool navigate(const std::string& path) override;

    // 获取页面信息
    inline Page& getCurrentPage() const;    // 获取当前页面
    inline size_t getPageCount() const;     // 获取页面栈大小
    inline bool isEmpty() const;            // 判断页面栈是否为空

    // 生命周期管理（调用页面的 onCreate/onResume 等）
    void notifyCreated();            // 通知当前页面 onCreate
    void notifyResumed();            // 通知当前页面 onResume
    void notifyPaused();             // 通知当前页面 onPause
    void notifyDestroyed();          // 通知当前页面 onDestroy

    // 禁止拷贝和赋值
    PageManager(const PageManager&) = delete;
    PageManager& operator=(const PageManager&) = delete;

    // 构造/析构
    explicit PageManager() = default;
    ~PageManager() = default;

private:
    std::vector<std::unique_ptr<Page>> m_pageStack; // 页面栈
    std::unordered_map<std::string, Page*> m_pathMap; // 快速判断是否存在页面
    std::function<void(const std::string&)> m_navigateCallback; // 导航回调函数
    std::function<void()>  m_popupCallback; // popup回调注入

    std::unordered_map<std::string, std::function<std::unique_ptr<Page>()>> m_pageFactory; // 页面工厂

    // 内部生命周期管理
    void pushPage(std::unique_ptr<Page> page); // 压入页面
};

// PageManager.h
