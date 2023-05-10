#pragma once

namespace Shadow
{
    // 计算文本居中位置
    ImVec2 CalcTextPos( ImVec2 Min, ImVec2 Max, std::string Text );

    /// <summary>
    /// 用于绘制使用
    /// </summary>
    class IDL
    {
    public:
        ImDrawList* F; // 前景指针
        ImDrawList* B; // 背景指针

        void GetIDL( void ); // 获取前景&背景指针
    };

    /// <summary>
    /// 组件模块基础数据
    /// </summary>
    class MenuBase
    {
    public:
        ImVec2  Min;    // 左上角
        ImVec2  Max;    // 右下角
        float   Width;  // 宽度
        float   Height; // 高度
    };

    /// <summary>
    /// 图片模块
    /// </summary>
    class MenuImage : public MenuBase, IDL
    {
    public:
        ImTextureID Image;  // 图片数据

        void DrawImage( void ); // 绘制图片
    };

    /// <summary>
    /// 标题模块
    /// </summary>
    class MenuTitle : public MenuBase, IDL
    {
    public:
        std::string Title;  // 标题

        void DrawTitle( void ); // 绘制标题
    };

    /// <summary>
    /// 项类型枚举
    /// </summary>
    enum class ITEMTYPE
    {
        PAGE    // 页
    };

    /// <summary>
    /// 项基类
    /// </summary>
    class ItemBase
    {
    public:
        ItemBase( std::string Name, ITEMTYPE Type );    // 初始化项名和项类型

        std::string GetName( void );                    // 获取项名
        ITEMTYPE    GetType( void );                    // 获取项类型

        virtual void Input( void )                      = 0;    // 输入

    private:
        std::string Name;   // 项名
        ITEMTYPE    Type;   // 项类型
    };

    /// <summary>
    /// 项页面
    /// </summary>
    class ItemPage : public ItemBase
    {
    public:
        ItemPage( std::string Name, std::string& CurrentPage, std::string TargetPage, std::string BackPage, int& CurrentSelect, std::vector<int>& PageHierarchy );

        virtual void Input( void ) override
        {
            if ( GetAsyncKeyState( VK_SPACE ) & 1 )
            {
                PageHierarchy.push_back( CurrentSelect );
                CurrentSelect   = 0;
                CurrentPage     = TargetPage;
            }
            else if ( GetAsyncKeyState( VK_BACK ) & 1 )
            {
                if ( PageHierarchy.size( ) )
                {
                    CurrentSelect = PageHierarchy.back( );
                    PageHierarchy.pop_back( );
                }
                CurrentPage = BackPage;
            }
        }

    private:
        std::string&        CurrentPage;    // 当前显示页面
        std::string         TargetPage;     // 本页面
        std::string         BackPage;       // 上一层页面
        int&                CurrentSelect;  // 当前选项
        std::vector<int>&   PageHierarchy;  // 页面层级
    };

    /// <summary>
    /// 页面模块
    /// </summary>
    class MenuPage : public MenuBase, IDL
    {
    public:
        std::vector<std::shared_ptr<ItemBase>>  Items;      // items
        int                                     Start;      // 开始索引
        int                                     End;        // 结束索引

        void CalcSize( void );              // 计算页面尺寸
        void Render( int CurrentSelect );   // 渲染
        void Input( int CurrentSelect );    // 输入
    };

    /// <summary>
    /// 信息模块
    /// </summary>
    class MenuInfo : public MenuBase, IDL
    {
    public:
        std::string Info;   // 菜单信息
        std::string ItemNum;// 项数量

        void DrawInfo( void ); // 绘制信息
    };

    /// <summary>
    /// 核心
    /// </summary>
    class Yorktown
    {
    public:
        Yorktown( void );   // 将一些重要数据初始化

        void Core( void );  // 核心

    private:
        bool                            IsMenuActive;   // 判断鼠标是否按住菜单
        MenuBase                        MenuData;       // 菜单的数据
        std::string                     CurrentPage;    // 当前显示页面
        int                             CurrentSelect;  // 当前选中项
        int                             MaxItem;        // 最大 item 数量
        std::vector<int>                PageHierarchy;  // 页面层级
        ImVec2                          CurSelectPos;   // 当前选中位置
        MenuImage                       Image;          // 图片模块
        MenuTitle                       Title;          // 标题模块
        std::map<std::string, MenuPage> Pages;          // 页面模块
        MenuInfo                        Info;           // 信息模块

        ImVec2 EaseMovement( ImVec2 Start, ImVec2 End, float Speed );   // 非线性移动
        void Render( void );                                            // 渲染出菜单
        void Input( void );                                             // 处理输入
        void UpData( void );                                            // 更新数据
    };
}
