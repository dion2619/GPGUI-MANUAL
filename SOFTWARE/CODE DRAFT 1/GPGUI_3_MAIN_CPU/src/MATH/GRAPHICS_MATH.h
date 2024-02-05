#include <Arduino.h>
#include "debug_Flags.h"
#include <vector>




struct vec3D
{
    float x;
    float y;
    float z;
};



struct RAY
{
    vec3D direction;
    vec3D origin;
};






class  GRAPHICS_MATH
{
  
public:


    //VECTORS/////////////////////////////////////////////////
    vec3D normalize_Vector(vec3D& Vi);
    vec3D vector_Multiply(vec3D& Va, float scalar);
    vec3D vector_Add(vec3D& Va, vec3D& Vb);
    vec3D vector_Sub(vec3D& Va, vec3D& Vb);
    vec3D vector_Cross_Product(vec3D& Va, vec3D& Vb);
    float vector_Dot_Product(vec3D& Va, vec3D& Vb);
    vec3D vector_Rotate_Y(vec3D& Vi, float angle);
    vec3D vector_Multiply_by_Vector(vec3D& Va, vec3D& Vb);
    vec3D get_Plane_Normal_Vector(vec3D& Va, vec3D& Vb, vec3D& Vc);


    //MATRICIES////////////////////////////////////////////
     void mat4x4(float mat[4][4], float vertex[3]);
     void translate_Matrix(float dx, float dy, float dz, float vertex[3]);
     void scaling_Matrix(float sf, float vertex[3]);
     void x_Rotation_Matrix(float anx, float vertex[3]);
     void y_Rotation_Matrix(float any, float vertex[3]);
     void z_Rotation_Matrix(float anz, float vertex[3]);
     void projection_Matrix(float h, float w, float f, float n, float FOV, float vertex[3]);
     void look_At_Matrix (vec3D& A, vec3D& B, vec3D& C, vec3D& T, float vertex_N[3]);
     void point_At_Matrix(vec3D& A, vec3D& B, vec3D& C, vec3D& T, float vertex_N[3]);
     //////////////////////////////////////////////////////
     void create_Camera_Origin(float origin[3], float angle_X, float angle_Y, float angle_Z, float vA[3], float vB[3], float vC[3]);
     void Swap_XY_Float(float x_In1, float y_In1, float x_In2, float y_In2, float* x_Out1, float* y_Out1, float* x_Out2, float* y_Out2);
     void normalize_Range_To_0_1(float xi, float xMin, float xMax, float* ui);
     void normalize(int xi, int xMin, int xMax, float* ui);
     void normalise_Range_To_1_0_1(float xi, float xmin, float xmax, float* xn);
     void interp(int X1, int X2, float P, int* PX);
     void point_Liner_Interpolation(int X1, int Y1, int X2, int Y2, float P, int* PX, int* PY);
     void Swap_XY(int x_In1, int y_In1, int x_In2, int y_In2, int* x_Out1, int* y_Out1, int* x_Out2, int* y_Out2);
     byte check_Line_Cross_p1_p3(int x1, int y1, int x3, int y3, int x2, int y2);
     void rotate_Point(int* x_Orign, int* y_Orign, float AN, bool direction);
     void get_Point_Vector(int PX1, int PY1, int R, float AN, bool rotation_Dir, int* px2, int* py2);
     void angled_Point_Step(float angle, float step_Size, int x1, int y1, int* X_Next, int* Y_Next);
     void map_Value_To_Range(float Vi, float aMin, float aMax, float bMin, float bMax, int *Vn);
     float deg_To_Rad(float Ai);
     void triangle_CW_Sort
     (
         int &x1,
         int &y1,
         int &x2,
         int &y2,
         int &x3,
         int &y3
     );


     void world_plane_To_Screen_Plane_Vertex_Translate
     (
         float wp1_x,
         float wp1_y,
         float wp2_x,
         float wp2_y,
         float sp1_x,
         float sp1_y,
         float sp2_x,
         float sp2_y,
         float vertex[3],
         int projection_Type,
         float h,
         float w,
         float f,
         float n,
         float FOV,
         int* Xn,
         int* Yn
     );

     void world_Space_To_Camera_Space_Vertex_Translate(vec3D& camera_Pos, vec3D& camera_Up, vec3D& camera_Dir, float vertex[3]);


     void cast_Ray
     (
         RAY ray,
         vec3D& current_Tail,
         float step_Length,
         float* current_Length,
         float max_Length,
         float dir_Angle_X,
         float dir_Angle_Y,
         float dir_Angle_Z,
         bool* hit,
         bool cast_Enable
     );


     void cast_Ray_Into_Scene
     (
         RAY ray,
         vec3D& current_Tail,
         float *current_Length,
         float max_Length,
         float step_Size,
         float ray_Angle_X,
         float ray_Angle_Y,
         float ray_Angle_Z,
         bool *hit,
         bool cast_Enable
     );

     ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
};




