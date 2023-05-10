#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <string>
#include <vector>
#include <memory>
#include <Windows.h>
#include <map>
#include "Menu.h"

/// <summary>
/// 计算文本居中的坐标
/// </summary>
/// <param name="Min">左上</param>
/// <param name="Max">右下</param>
/// <param name="Text">文本</param>
/// <returns>计算出的坐标</returns>
ImVec2 Shadow::CalcTextPos( ImVec2 Min, ImVec2 Max, std::string Text )
{
    ImVec2 TextSize = ImGui::CalcTextSize( Text.c_str( ) );

    return { ( Min.x + Max.x ) / 2 - TextSize.x / 2, ( Min.y + Max.y ) / 2 - TextSize.y / 2 };
}

/// <summary>
/// 获取前景 & 背景的指针
/// </summary>
/// <param name=""></param>
void Shadow::IDL::GetIDL( void )
{
    F = ImGui::GetForegroundDrawList( );
    B = ImGui::GetBackgroundDrawList( );
}

/// <summary>
/// 绘制图片
/// </summary>
/// <param name=""></param>
void Shadow::MenuImage::DrawImage( void )
{
    GetIDL( );
    // B->AddImage( Image, Min, Max );
    B->AddRectFilled( Min, Max, ImColor{ 0, 0, 255 } );
}

/// <summary>
/// 绘制页面标题
/// </summary>
/// <param name=""></param>
void Shadow::MenuTitle::DrawTitle( void )
{
    GetIDL( );
    B->AddRectFilled( Min, Max, ImColor{ 0, 0, 0 } );
    F->AddText( CalcTextPos( Min, Max, Title.c_str( ) ), ImColor{ 255, 255, 255 }, Title.c_str( ) );
}

/// <summary>
/// 项基类 初始化名字和类型
/// </summary>
/// <param name="Name">项名字</param>
/// <param name="Type">项类型</param>
Shadow::ItemBase::ItemBase( std::string Name, Shadow::ITEMTYPE Type )
{
    this->Name = Name;
    this->Type = Type;
}

/// <summary>
/// 获取项名字
/// </summary>
/// <param name=""></param>
/// <returns>项名字</returns>
std::string Shadow::ItemBase::GetName( void )
{
    return Name;
}

/// <summary>
/// 获取项类型
/// </summary>
/// <param name=""></param>
/// <returns>项类型</returns>
Shadow::ITEMTYPE Shadow::ItemBase::GetType( void )
{
    return Type;
}

/// <summary>
/// 初始化页面项
/// </summary>
/// <param name="Name">页面名</param>
/// <param name="CurrentPage">当前页面索引</param>
/// <param name="TargetPage">目标页面索引</param>
Shadow::ItemPage::ItemPage( std::string Name, std::string& CurrentPage, std::string TargetPage, std::string BackPage, int& CurrentSelect, std::vector<int>& PageHierarchy ) : ItemBase( Name, Shadow::ITEMTYPE::PAGE ), CurrentPage( CurrentPage ), CurrentSelect( CurrentSelect ), PageHierarchy( PageHierarchy )
{
    this->TargetPage    = TargetPage;
    this->BackPage      = BackPage;
}

/// <summary>
/// 计算页面的尺寸
/// </summary>
/// <param name=""></param>
void Shadow::MenuPage::CalcSize( void )
{
    // item 数大于 11 直接 11 * 40 硬编码
    if ( Items.size( ) > 11 )
    {
        Max     = { Min.x + 380, Min.y + 440 };
        Width   = 380;
        Height  = 440;
    }
    else
    {
        Max     = { Min.x + 380, Min.y + Items.size( ) * 40 };
        Width   = 380;
        Height  = Items.size( ) * 40;
    }
}

/// <summary>
/// 页面渲染
/// </summary>
/// <param name="CurrentSelect">当前选中项</param>
void Shadow::MenuPage::Render( int CurrentSelect )
{
    // 将开始和结束索引的内容放进临时容器
    std::vector<std::shared_ptr<ItemBase>> Temp;
    for ( int i = Start; i < End; i++ )
    {
        Temp.push_back( Items[ i ] );
    }

    for ( int i = 0; i < Temp.size( ); i++ )
    {
        GetIDL( );
        if ( Temp[ i ].get( )->GetType( ) == Shadow::ITEMTYPE::PAGE )
        {
            B->AddRectFilled( { Min.x, Min.y + i * 40 }, { Min.x + Width, Min.y + ( i + 1 ) * 40 }, ImColor{ 0, 0, 0, 80 } );
            F->AddText( { Max.x - 25, CalcTextPos( { Min.x, Min.y + i * 40 }, { Min.x, Min.y + ( i + 1 ) * 40 }, u8"\ueb03" ).y + 5 }, i == CurrentSelect ? ImColor{ 0, 0, 0 } : ImColor{ 255, 255, 255 }, u8"\ueb03" );
        }
        if ( i == CurrentSelect )
        {
            F->AddText( { Min.x + 15, CalcTextPos( { Min.x, Min.y + i * 40 }, { Min.x, Min.y + ( i + 1 ) * 40 }, Temp[ i ].get( )->GetName( ) ).y }, ImColor{ 0, 0, 0 }, Temp[ i ].get( )->GetName( ).c_str( ) );
            //B->AddRectFilled( { Min.x, Min.y + i * 40 }, { Min.x + Width, Min.y + ( i + 1 ) * 40 }, ImColor{ 255, 255, 255 } );
        }
        else
        {
            F->AddText( { Min.x + 15, CalcTextPos( { Min.x, Min.y + i * 40 }, { Min.x, Min.y + ( i + 1 ) * 40 }, Temp[ i ].get( )->GetName( ) ).y }, ImColor{ 255, 255, 255 }, Temp[ i ].get( )->GetName( ).c_str( ) );
        }
    }
}

/// <summary>
/// 页面输入
/// </summary>
/// <param name="CurrentSelect">当前选中项</param>
void Shadow::MenuPage::Input( int CurrentSelect )
{
    Items[ CurrentSelect + Start ].get( )->Input( );
}

/// <summary>
/// 绘制信息
/// </summary>
/// <param name=""></param>
void Shadow::MenuInfo::DrawInfo( void )
{
    GetIDL( );
    ImVec2 InfoPos      = CalcTextPos( Min, Max, Info );
    ImVec2 ItemNumPos   = CalcTextPos( Min, Max, ItemNum );
    B->AddRectFilled( Min, Max, ImColor{ 0, 0, 0 } );
    F->AddText( { Min.x + 15, InfoPos.y }, ImColor{ 255, 255, 255 }, Info.c_str( ) );
    F->AddText( { Max.x - ImGui::CalcTextSize( ItemNum.c_str( ) ).x - 15, ItemNumPos.y }, ImColor{ 255, 255, 255 }, ItemNum.c_str( ) );
}

/// <summary>
/// 初始化一些必要数据
/// </summary>
/// <param name=""></param>
Shadow::Yorktown::Yorktown( void )
{
    IsMenuActive    = false;
    MenuData.Min    = { 100, 100 };
    MenuData.Width  = 380;
    CurrentPage     = "Home";
    CurrentSelect   = 0;
    MaxItem         = 11;
    Image.Min       = MenuData.Min;
    Image.Max       = { Image.Min.x + MenuData.Width, Image.Min.y + 140 };
    Image.Width     = MenuData.Width;
    Image.Height    = 140;
    Image.Image     = nullptr;
    Title.Min       = { Image.Min.x, Image.Max.y };
    Title.Max       = { Title.Min.x + MenuData.Width, Title.Min.y + 40 };
    Title.Width     = MenuData.Width;
    Title.Height    = 40;
    Title.Title     = CurrentPage;
    CurSelectPos    = { Title.Min.x, Title.Max.y + CurrentSelect * 40 };

    MenuPage temp;
    temp.Items.push_back( std::make_shared<ItemPage>( "No.1", CurrentPage, "Weapon", "Home", CurrentSelect, PageHierarchy ) );
    temp.Items.push_back( std::make_shared<ItemPage>( "No.2", CurrentPage, "Weapon", "Home", CurrentSelect, PageHierarchy ) );
    temp.Items.push_back( std::make_shared<ItemPage>( "No.3", CurrentPage, "Weapon", "Home", CurrentSelect, PageHierarchy ) );
    temp.Items.push_back( std::make_shared<ItemPage>( "No.4", CurrentPage, "Weapon", "Home", CurrentSelect, PageHierarchy ) );
    temp.Items.push_back( std::make_shared<ItemPage>( "No.5", CurrentPage, "Weapon", "Home", CurrentSelect, PageHierarchy ) );
    temp.Items.push_back( std::make_shared<ItemPage>( "No.6", CurrentPage, "Weapon", "Home", CurrentSelect, PageHierarchy ) );
    temp.Items.push_back( std::make_shared<ItemPage>( "No.7", CurrentPage, "Weapon", "Home", CurrentSelect, PageHierarchy ) );
    temp.Items.push_back( std::make_shared<ItemPage>( "No.8", CurrentPage, "Weapon", "Home", CurrentSelect, PageHierarchy ) );
    temp.Items.push_back( std::make_shared<ItemPage>( "No.9", CurrentPage, "Weapon", "Home", CurrentSelect, PageHierarchy ) );
    temp.Items.push_back( std::make_shared<ItemPage>( "No.10", CurrentPage, "Weapon", "Home", CurrentSelect, PageHierarchy ) );
    temp.Items.push_back( std::make_shared<ItemPage>( "No.11", CurrentPage, "Weapon", "Home", CurrentSelect, PageHierarchy ) );
    temp.Items.push_back( std::make_shared<ItemPage>( "No.12", CurrentPage, "Weapon", "Home", CurrentSelect, PageHierarchy ) );
    temp.Items.push_back( std::make_shared<ItemPage>( "No.13", CurrentPage, "Weapon", "Home", CurrentSelect, PageHierarchy ) );
    temp.Items.push_back( std::make_shared<ItemPage>( "No.14", CurrentPage, "Weapon", "Home", CurrentSelect, PageHierarchy ) );
    temp.Items.push_back( std::make_shared<ItemPage>( "No.15", CurrentPage, "Weapon", "Home", CurrentSelect, PageHierarchy ) );
    temp.Items.push_back( std::make_shared<ItemPage>( "No.16", CurrentPage, "Weapon", "Home", CurrentSelect, PageHierarchy ) );
    temp.Items.push_back( std::make_shared<ItemPage>( "No.17", CurrentPage, "Weapon", "Home", CurrentSelect, PageHierarchy ) );
    temp.Items.push_back( std::make_shared<ItemPage>( "No.18", CurrentPage, "Weapon", "Home", CurrentSelect, PageHierarchy ) );
    temp.Items.push_back( std::make_shared<ItemPage>( "No.19", CurrentPage, "Weapon", "Home", CurrentSelect, PageHierarchy ) );
    Pages[ "Home" ] = temp;

    MenuPage temp1;
    temp1.Items.push_back( std::make_shared<ItemPage>( "No.1", CurrentPage, "AK47", "Home", CurrentSelect, PageHierarchy ) );
    temp1.Items.push_back( std::make_shared<ItemPage>( "No.2", CurrentPage, "AK47", "Home", CurrentSelect, PageHierarchy ) );
    temp1.Items.push_back( std::make_shared<ItemPage>( "No.3", CurrentPage, "AK47", "Home", CurrentSelect, PageHierarchy ) );
    Pages[ "Weapon" ] = temp1;

    MenuPage temp2;
    temp2.Items.push_back( std::make_shared<ItemPage>( "No.1", CurrentPage, "AK47", "Weapon", CurrentSelect, PageHierarchy ) );
    temp2.Items.push_back( std::make_shared<ItemPage>( "No.2", CurrentPage, "AK47", "Weapon", CurrentSelect, PageHierarchy ) );
    temp2.Items.push_back( std::make_shared<ItemPage>( "No.3", CurrentPage, "AK47", "Weapon", CurrentSelect, PageHierarchy ) );
    temp2.Items.push_back( std::make_shared<ItemPage>( "No.4", CurrentPage, "AK47", "Weapon", CurrentSelect, PageHierarchy ) );
    temp2.Items.push_back( std::make_shared<ItemPage>( "No.5", CurrentPage, "AK47", "Weapon", CurrentSelect, PageHierarchy ) );
    temp2.Items.push_back( std::make_shared<ItemPage>( "No.6", CurrentPage, "AK47", "Weapon", CurrentSelect, PageHierarchy ) );
    temp2.Items.push_back( std::make_shared<ItemPage>( "No.7", CurrentPage, "AK47", "Weapon", CurrentSelect, PageHierarchy ) );
    temp2.Items.push_back( std::make_shared<ItemPage>( "No.8", CurrentPage, "AK47", "Weapon", CurrentSelect, PageHierarchy ) );
    temp2.Items.push_back( std::make_shared<ItemPage>( "No.9", CurrentPage, "AK47", "Weapon", CurrentSelect, PageHierarchy ) );
    Pages[ "AK47" ] = temp2;

    Pages[ "Home" ].Start   = 0;
    Pages[ "Home" ].End     = Pages[ "Home" ].Items.size() > 11 ? 11 : Pages[ "Home" ].Items.size( );
    Pages[ "Weapon" ].Start = 0;
    Pages[ "Weapon" ].End   = Pages[ "Weapon" ].Items.size( ) > 11 ? 11 : Pages[ "Weapon" ].Items.size( );
    Pages[ "AK47" ].Start = 0;
    Pages[ "AK47" ].End = Pages[ "AK47" ].Items.size( ) > 11 ? 11 : Pages[ "AK47" ].Items.size( );

    Info.Min        = { Pages[ CurrentPage ].Min.x, Pages[ CurrentPage ].Max.y };
    Info.Max        = { Info.Min.x + MenuData.Width, Info.Min.y + 40 };
    Info.Width      = MenuData.Width;
    Info.Height     = 40;
    Info.Info       = "Yorktown Menu";
    Info.ItemNum    = std::to_string( CurrentSelect + 1 + Pages[ CurrentPage ].Start ) + " / " + std::to_string( Pages[ CurrentPage ].Items.size( ) );

    MenuData.Max    = { MenuData.Min.x + MenuData.Width, MenuData.Min.y + Image.Height + Title.Height + Pages[ CurrentPage ].Height + Info.Height };
    MenuData.Height = Image.Height + Title.Height + Pages[ CurrentPage ].Height + Info.Height;
}

/// <summary>
/// 非线性移动
/// </summary>
/// <param name="CurrentPos">当前选中位置</param>
/// <param name="TargetPos">目标点</param>
/// <param name="Speed">速度</param>
/// <returns>计算出的坐标</returns>
ImVec2 Shadow::Yorktown::EaseMovement( ImVec2 Start, ImVec2 End, float Speed )
{
    ImVec2  Dir = { End.x - Start.x, End.y - Start.y };
    float   Dis = sqrtf( Dir.x * Dir.x + Dir.y * Dir.y );
    if ( Dis <= Speed )
    {
        return End;
    }
    else
    {
        ImVec2 DirNormalized = { Dir.x / Dis, Dir.y / Dis };
        return { Start.x + DirNormalized.x * Dis * Speed, Start.y + DirNormalized.y * Dis * Speed };
    }
}

/// <summary>
/// 获取输入
/// </summary>
/// <param name=""></param>
void Shadow::Yorktown::Input( void )
{
    // 判断鼠标是否在菜单中拖动
    if ( ImGui::IsMouseHoveringRect( MenuData.Min, MenuData.Max, false ) && ImGui::IsMouseDragging( ImGuiMouseButton_Left ) )
    {
        IsMenuActive = true;
    }
    if ( IsMenuActive )
    {
        MenuData.Min.x += ImGui::GetIO( ).MouseDelta.x;
        MenuData.Min.y += ImGui::GetIO( ).MouseDelta.y;
    }
    if ( ImGui::IsMouseReleased( ImGuiMouseButton_Left ) )
    {
        IsMenuActive = false;
    }

    // 上移
    if ( GetAsyncKeyState( VK_UP ) & 1 )
    {
        if ( Pages[ CurrentPage ].Items.size( ) <= 11 )
        {
            if ( CurrentSelect != 0 )
            {
                CurrentSelect--;
            }
            else
            {
                CurrentSelect = Pages[ CurrentPage ].End - 1;
            }
        }
        else
        {
            if ( Pages[ CurrentPage ].Start == 0 )
            {
                if ( CurrentSelect != 0 )
                {
                    CurrentSelect--;
                }
                else if ( CurrentSelect == 0 )
                {
                    CurrentSelect               = 10;
                    Pages[ CurrentPage ].End    = Pages[ CurrentPage ].Items.size( );
                    Pages[ CurrentPage ].Start  = Pages[ CurrentPage ].End - 11;
                }
            }
            else
            {
                if ( CurrentSelect != 0 )
                {
                    CurrentSelect--;
                }
                else if ( CurrentSelect == 0 )
                {
                    Pages[ CurrentPage ].Start--;
                    Pages[ CurrentPage ].End--;
                }
            }
        }
    }

    // 下移
    if ( GetAsyncKeyState( VK_DOWN ) & 1 )
    {
        if ( Pages[ CurrentPage ].Items.size( ) <= 11 )
        {
            if ( CurrentSelect != Pages[ CurrentPage ].End - 1 )
            {
                CurrentSelect++;
            }
            else
            {
                CurrentSelect = 0;
            }
        }
        else
        {
            if ( CurrentSelect == 10 && Pages[ CurrentPage ].End == Pages[ CurrentPage ].Items.size( ) )
            {
                CurrentSelect               = 0;
                Pages[ CurrentPage ].End    = 11;
                Pages[ CurrentPage ].Start  = 0;
            }
            else if ( Pages[ CurrentPage ].Start != 0 )
            {
                if ( CurrentSelect == 10 )
                {
                    Pages[ CurrentPage ].Start++;
                    Pages[ CurrentPage ].End++;
                }
                else
                {
                    CurrentSelect++;
                }
            }
            else if ( Pages[ CurrentPage ].Start == 0 )
            {
                if ( CurrentSelect == 10 )
                {
                    Pages[ CurrentPage ].Start++;
                    Pages[ CurrentPage ].End++;
                }
                else
                {
                    CurrentSelect++;
                }
            }
        }
    }

    Pages[ CurrentPage ].Input( CurrentSelect );
}

/// <summary>
/// 数据更新
/// </summary>
/// <param name=""></param>
void Shadow::Yorktown::UpData( void )
{
    Image.Min                   = MenuData.Min;
    Image.Max                   = { Image.Min.x + MenuData.Width, Image.Min.y + 140 };
    Title.Min                   = { Image.Min.x, Image.Max.y };
    Title.Max                   = { Title.Min.x + MenuData.Width, Title.Min.y + 40 };
    CurSelectPos                = EaseMovement( CurSelectPos, { Title.Min.x, Title.Max.y + CurrentSelect * 40 }, 0.1 );
    Pages[ CurrentPage ].Min    = { Title.Min.x, Title.Max.y };
    Pages[ CurrentPage ].CalcSize( );
    Info.Min                    = { Pages[ CurrentPage ].Min.x, Pages[ CurrentPage ].Max.y };
    Info.Max                    = { Info.Min.x + MenuData.Width, Info.Min.y + 40 };
    Info.ItemNum                = std::to_string( CurrentSelect + Pages[ CurrentPage ].Start + 1 ) + " / " + std::to_string( Pages[ CurrentPage ].Items.size( ) );
    MenuData.Max                = { MenuData.Min.x + MenuData.Width, MenuData.Min.y + Image.Height + Title.Height + Pages[ CurrentPage ].Height };
    MenuData.Height             = Image.Height + Title.Height + Pages[ CurrentPage ].Height;
    Title.Title                 = CurrentPage;
}

/// <summary>
/// 渲染菜单
/// </summary>
/// <param name=""></param>
void Shadow::Yorktown::Render( void )
{
    Image.DrawImage( );
    Title.DrawTitle( );
    Pages[ CurrentPage ].Render( CurrentSelect );
    IDL T;
    T.GetIDL( );
    T.B->AddRectFilled( { Title.Min.x, CurSelectPos.y }, { Title.Max.x, CurSelectPos.y + 40 }, ImColor{ 255, 255, 255 } );
    Info.DrawInfo( );
    if ( Pages[ CurrentPage ].Start == 0 )
    {
        T.F->AddText( { CalcTextPos( { Info.Min.x, Info.Min.y }, { Info.Max.x, Info.Max.y }, u8"\ueb03" ).x, CalcTextPos( { Info.Min.x, Info.Min.y }, { Info.Max.x, Info.Max.y }, u8"\ueb05" ).y + 10 }, ImColor{ 255, 255, 255 }, u8"\ueb05" );
    }
    else if ( Pages[ CurrentPage ].Start != 0 && Pages[ CurrentPage ].End != Pages[ CurrentPage ].Items.size( ) )
    {
        T.F->AddText( { CalcTextPos( { Info.Min.x, Info.Min.y }, { Info.Max.x, Info.Max.y }, u8"\ueb03" ).x, CalcTextPos( { Info.Min.x, Info.Min.y }, { Info.Max.x, Info.Max.y }, u8"\ueb13" ).y + 3 }, ImColor{ 255, 255, 255 }, u8"\ueb13" );
    }
    else if ( Pages[ CurrentPage ].End == Pages[ CurrentPage ].Items.size( ) )
    {
        T.F->AddText( { CalcTextPos( { Info.Min.x, Info.Min.y }, { Info.Max.x, Info.Max.y }, u8"\ueb03" ).x, CalcTextPos( { Info.Min.x, Info.Min.y }, { Info.Max.x, Info.Max.y }, u8"\ueb04" ).y }, ImColor{ 255, 255, 255 }, u8"\ueb04" );
    }
}

/// <summary>
/// 核心
/// </summary>
/// <param name=""></param>
void Shadow::Yorktown::Core( void )
{
    Input( );
    UpData( );
    Render( );
}
