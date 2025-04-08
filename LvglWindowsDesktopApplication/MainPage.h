#pragma once
#include "PageBase.h"
#include <lvgl.h>
#include <iostream>

#include <Navigator.h>

class MainPage :
    public PageBase
{
public:

    // 构造函数
    MainPage() {
        m_path = "/main"; // 设置页面路径
    }

    static void onLabelClicked(lv_event_t* e)
    {
        auto* page = static_cast<PageBase*>(lv_event_get_user_data(e));
        if (page && page->navigator) {
            page->navigator->navigate("/options");
        }
    }

    // 创建页面对象
    void onCreate(lv_obj_t* parent) override {
        std::cout << "MainPage onCreate" << std::endl;

        m_root = lv_obj_create(parent); // 创建页面对象
        lv_obj_set_size(m_root, LV_HOR_RES, LV_VER_RES); // 设置大小
        lv_obj_t* label = lv_label_create(m_root); // 创建标签
        lv_label_set_text(label, "Welcome to LVGL!"); // 设置标签文本
        lv_obj_center(label); // 居中显示
        lv_obj_set_style_text_font(label, &lv_font_montserrat_14, 0); // 设置字体
        lv_obj_set_style_text_color(label, lv_color_hex(0x333333), 0); // 设置字体颜色
        
        lv_obj_t* button = lv_btn_create(m_root); // 创建按钮
        lv_obj_set_size(button, 100, 50); // 设置按钮大小
        lv_obj_align(button, LV_ALIGN_CENTER, 0, 50); // 设置按钮位置
        lv_obj_add_event_cb(button, MainPage::onLabelClicked, LV_EVENT_CLICKED, this);
    }
    // 显示页面
    void onResume() override {
        lv_obj_clear_flag(m_root, LV_OBJ_FLAG_HIDDEN); // 显示
        std::cout << "MainPage onResume" << std::endl;
    }
    // 暂停页面
    void onPause() override {
        lv_obj_add_flag(m_root, LV_OBJ_FLAG_HIDDEN); // 隐藏
        std::cout << "MainPage onPause" << std::endl;
    }
    // 清理资源
    void onDestroy() override {
        std::cout << "MainPage onDestroy" << std::endl;
        lv_obj_del(m_root);
        m_root = nullptr;
    }
};

