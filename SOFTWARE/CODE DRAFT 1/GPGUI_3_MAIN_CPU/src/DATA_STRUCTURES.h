struct slide_Bar_Int
{
    int xStart;
    int yStart;
    int output_Min;
    int output_Max;
    int boarder_Color;
    int back_Color;
    int bar_Color;
    int slide_Color;
};



struct button
{
    bool state;
    int xStart;
    int yStart;
    int xSize;
    int ySize;
    int onColor;
    int offColor;
    int id;
    char* text;
    int icon[16][16];
    bool fill_Type;
    int texSize;
    bool text_Enable;
    int icon_Index_X;
    int icon_Index_Y;
    int window_Ref_Id;
};

struct window
{
    int xStart;
    int yStart;
    int xSize;
    int ySize;
    int index_X;
    int index_Y;
    int title_Bar_Color;
    int w_bar_Color;
    int w_border_Color;
    int background_Color;
    int id;
    bool priority;
    char* window_Name;
    bool open_Close;
};




struct vertex3D
{
    float x;
    float y;
    float z;
    float u;
    float v;
    int   c;
    int  id;
    int mesh_Type;
};


struct line
{
    vertex3D p1, p2;
    int id;
    int c;
};

struct triangle
{
    vertex3D
        p1,
        p2,
        p3;
    int face_Color;
    int id;
    float Nx;
    float Ny;
    float Nz;
    float Dp_Camera;
    float Dp_Light;
    int pixel_X1;
    int pixel_Y1;
    int pixel_X2;
    int pixel_Y2;
    int pixel_X3;
    int pixel_Y3;
};


char button_Inst_Map[20] =
{
    7,
    8,
    9,
    10,
    11,
    12,
    13,
    14,
    46,
    47,
    105,
    106,
    107,
    108,
    112,
    113,
    114,
    115,
    24,
    4
};