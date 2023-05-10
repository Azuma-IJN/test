#pragma once

namespace Shadow
{
    // �����ı�����λ��
    ImVec2 CalcTextPos( ImVec2 Min, ImVec2 Max, std::string Text );

    /// <summary>
    /// ���ڻ���ʹ��
    /// </summary>
    class IDL
    {
    public:
        ImDrawList* F; // ǰ��ָ��
        ImDrawList* B; // ����ָ��

        void GetIDL( void ); // ��ȡǰ��&����ָ��
    };

    /// <summary>
    /// ���ģ���������
    /// </summary>
    class MenuBase
    {
    public:
        ImVec2  Min;    // ���Ͻ�
        ImVec2  Max;    // ���½�
        float   Width;  // ���
        float   Height; // �߶�
    };

    /// <summary>
    /// ͼƬģ��
    /// </summary>
    class MenuImage : public MenuBase, IDL
    {
    public:
        ImTextureID Image;  // ͼƬ����

        void DrawImage( void ); // ����ͼƬ
    };

    /// <summary>
    /// ����ģ��
    /// </summary>
    class MenuTitle : public MenuBase, IDL
    {
    public:
        std::string Title;  // ����

        void DrawTitle( void ); // ���Ʊ���
    };

    /// <summary>
    /// ������ö��
    /// </summary>
    enum class ITEMTYPE
    {
        PAGE    // ҳ
    };

    /// <summary>
    /// �����
    /// </summary>
    class ItemBase
    {
    public:
        ItemBase( std::string Name, ITEMTYPE Type );    // ��ʼ��������������

        std::string GetName( void );                    // ��ȡ����
        ITEMTYPE    GetType( void );                    // ��ȡ������

        virtual void Input( void )                      = 0;    // ����

    private:
        std::string Name;   // ����
        ITEMTYPE    Type;   // ������
    };

    /// <summary>
    /// ��ҳ��
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
        std::string&        CurrentPage;    // ��ǰ��ʾҳ��
        std::string         TargetPage;     // ��ҳ��
        std::string         BackPage;       // ��һ��ҳ��
        int&                CurrentSelect;  // ��ǰѡ��
        std::vector<int>&   PageHierarchy;  // ҳ��㼶
    };

    /// <summary>
    /// ҳ��ģ��
    /// </summary>
    class MenuPage : public MenuBase, IDL
    {
    public:
        std::vector<std::shared_ptr<ItemBase>>  Items;      // items
        int                                     Start;      // ��ʼ����
        int                                     End;        // ��������

        void CalcSize( void );              // ����ҳ��ߴ�
        void Render( int CurrentSelect );   // ��Ⱦ
        void Input( int CurrentSelect );    // ����
    };

    /// <summary>
    /// ��Ϣģ��
    /// </summary>
    class MenuInfo : public MenuBase, IDL
    {
    public:
        std::string Info;   // �˵���Ϣ
        std::string ItemNum;// ������

        void DrawInfo( void ); // ������Ϣ
    };

    /// <summary>
    /// ����
    /// </summary>
    class Yorktown
    {
    public:
        Yorktown( void );   // ��һЩ��Ҫ���ݳ�ʼ��

        void Core( void );  // ����

    private:
        bool                            IsMenuActive;   // �ж�����Ƿ�ס�˵�
        MenuBase                        MenuData;       // �˵�������
        std::string                     CurrentPage;    // ��ǰ��ʾҳ��
        int                             CurrentSelect;  // ��ǰѡ����
        int                             MaxItem;        // ��� item ����
        std::vector<int>                PageHierarchy;  // ҳ��㼶
        ImVec2                          CurSelectPos;   // ��ǰѡ��λ��
        MenuImage                       Image;          // ͼƬģ��
        MenuTitle                       Title;          // ����ģ��
        std::map<std::string, MenuPage> Pages;          // ҳ��ģ��
        MenuInfo                        Info;           // ��Ϣģ��

        ImVec2 EaseMovement( ImVec2 Start, ImVec2 End, float Speed );   // �������ƶ�
        void Render( void );                                            // ��Ⱦ���˵�
        void Input( void );                                             // ��������
        void UpData( void );                                            // ��������
    };
}
