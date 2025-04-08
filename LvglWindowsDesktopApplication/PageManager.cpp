
#include "PageManager.h"

#include "PageBase.h"


#include "Navigator.h"
#include <iostream>

void PageManager::pushPage(std::unique_ptr<Page> page)
{
    // 注入navigator
    page->navigator = this;
    std::string path = page->getPath();  // Page 提供 getPath()

    m_pathMap[path] = page.get();        // 加入路径映射
    m_pageStack.push_back(std::move(page));
}

void PageManager::registerPage(const std::string& path, std::function<std::unique_ptr<Page>()> creator)
{
    if (m_pageFactory.find(path) != m_pageFactory.end())
    {
        /*throw std::runtime_error("Page already registered with this path.");*/
        return;
    }
    m_pageFactory[path] = std::move(creator);
}

void PageManager::pop()
{
    if (m_pageStack.size() <= 1) return; // 不能弹出主页

    notifyPaused();
    notifyDestroyed();

    auto top = std::move(m_pageStack.back());
    m_pathMap.erase(top->getPath());
    m_pageStack.pop_back();

    notifyResumed();  // 恢复上一个页面
}


bool PageManager::navigate(const std::string& path)
{
    // 页面已存在：提取到顶部
    if (m_pathMap.find(path) != m_pathMap.end()) {
        auto* existingPage = m_pathMap[path];

        if (m_pageStack.back().get() == existingPage) {
            return true; // 已在顶部
        }

        notifyPaused();
        // 从页面栈中移除该页面
        auto it = std::find_if(m_pageStack.begin(), m_pageStack.end(),
            [&](const std::unique_ptr<Page>& p) {
                return p.get() == existingPage;
            });

        if (it != m_pageStack.end()) {
            // 找到了该页面
            std::unique_ptr<Page> pageToMove = std::move(*it);
            m_pageStack.erase(it);  // 移除该页面

            m_pageStack.push_back(std::move(pageToMove));  // 重新压入到栈顶
            notifyResumed();
            return true;
        }
        else {
            std::cout << "Page not found in stack: " << path << std::endl;
        }
    }

    // 页面不存在：查找工厂并创建
    auto it = m_pageFactory.find(path);
    if (it != m_pageFactory.end()) {
        notifyPaused(); // 暂停当前页面

        auto newPage = it->second();  // 创建页面
        newPage->navigator = this;
        m_pathMap[path] = newPage.get(); // 注册路径映射
        m_pageStack.push_back(std::move(newPage));

        notifyCreated();
        notifyResumed();
        return true;
    }
    // 页面不存在且未注册路径
    std::cout << "Page not found: " << path << std::endl;
    return false;
}

inline Page& PageManager::getCurrentPage() const
{
    if (m_pageStack.empty())
        throw std::runtime_error("No current page available.");
    return *m_pageStack.back();
}

inline size_t PageManager::getPageCount() const
{
    return this->m_pageStack.size();
}

inline bool PageManager::isEmpty() const
{
    return this->getPageCount() == 0;
}

void PageManager::notifyCreated()
{
    if (!m_pageStack.empty()) {
        m_pageStack.back()->onCreate(lv_screen_active());
    }
}

void PageManager::notifyResumed()
{
    if (!m_pageStack.empty()) {
        m_pageStack.back()->onResume();
    }
}

void PageManager::notifyPaused()
{
    if (!m_pageStack.empty()) {
        m_pageStack.back()->onPause();
    }
}

void PageManager::notifyDestroyed()
{
    if (!m_pageStack.empty()) {
        m_pageStack.back()->onDestroy();
    }
}
