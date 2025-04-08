#pragma once
#include <lvgl.h>
#include <functional>
#include <string>


class Navigator;
class PageBase {
public:
    Navigator* navigator = nullptr; // 注入式导航器指针

    // 纯虚函数 = 强制子类实现（接口部分）
    virtual void onCreate(lv_obj_t* parent) = 0;  // 创建页面对象
    virtual void onResume() = 0; // 显示
    virtual void onPause() = 0; // 暂停
    virtual void onDestroy() = 0;                // 清理资源

    // 虚函数 = 可选覆盖（抽象类部分）
    virtual void onEnter() {}    // 页面进入时触发
    virtual void onExit() {}     // 页面退出时触发
    virtual bool handleEvent(lv_event_t* e) { return false; } // 事件处理

    // 必须含 LVGL 根对象
    lv_obj_t* getRoot() const { return m_root; }

    // 页面路径
    const std::string& getPath() const {
        return m_path
            ;
    }

    // 禁止直接实例化（抽象类）
    virtual ~PageBase() = default;
protected:
    lv_obj_t* m_root = nullptr; // 所有页面必须持有 LVGL 对象
    // 页面路径
    std::string m_path;
};
