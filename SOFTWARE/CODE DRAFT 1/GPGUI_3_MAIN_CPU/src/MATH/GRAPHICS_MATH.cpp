#include <Arduino.h>
#include "GRAPHICS_MATH.h"
#include <math.h>
#include <vector>

//VECTORS
vec3D GRAPHICS_MATH::vector_Cross_Product(vec3D& Va, vec3D& Vb)
{
    float Ax = Va.x;
    float Ay = Va.y;
    float Az = Va.z;

    float Bx = Vb.x;
    float By = Vb.y;
    float Bz = Vb.z;

  
    vec3D Vi;
    vec3D Vn;

    //WRITE NORMAL VECTOR TO Vi ADDRESS
    Vi.x = (Ay * Bz) - (Az * By);
    Vi.y = (Az * Bx) - (Ax * Bz);
    Vi.z = (Ax * By) - (Ay * Bx);



    //////////////////////////// normalize vector
    //CALC LENGTH OF NORMAL VECTOR
    float NL =
        sqrt
        (
            Vi.x * Vi.x +
            Vi.y * Vi.y +
            Vi.z * Vi.z
        );


    //NORMALIZE NORMAL VECTOR
    Vn.x = Vi.x / NL;
    Vn.y = Vi.y / NL;
    Vn.z = Vi.z / NL;
    /////////////////////////////////

    return Vn;
}

vec3D GRAPHICS_MATH::normalize_Vector(vec3D  &Vi)
{
    vec3D Vn;
    //FIND LENGHT OF VECTOR
    float NL =
        sqrt
        (
            Vi.x * Vi.x +
            Vi.y * Vi.y +
            Vi.z * Vi.z
        );


    //NORMALIZE NORMAL VECTOR BY DIVIDING BY THE LENGTH
    Vn.x = Vi.x / NL;
    Vn.y = Vi.y / NL;
    Vn.z = Vi.z / NL;
    /////////////////////////////////

    return Vn;
}

float GRAPHICS_MATH::vector_Dot_Product(vec3D &Va, vec3D &Vb)
{
   // vec3D Dp = (Va[0] * Vb[0]) + (Va[1] * Vb[1]) + (Va[2] * Vb[2]);
    float Dp = (Va.x * Vb.x) + (Va.y * Vb.y) + (Va.z * Vb.z);
    return Dp;
}

vec3D GRAPHICS_MATH::vector_Multiply(vec3D& Va, float scalar)
{
    vec3D Vc;
    Vc.x = Va.x * scalar;
    Vc.y = Va.y * scalar;
    Vc.z = Va.z * scalar;
    return Vc;
}

vec3D GRAPHICS_MATH::vector_Multiply_by_Vector(vec3D& Va, vec3D &Vb)
{
    vec3D Vc;
    Vc.x = Va.x * Vb.x;
    Vc.y = Va.y * Vb.y;
    Vc.z = Va.z * Vb.z;
    return Vc;
}

vec3D GRAPHICS_MATH::vector_Add(vec3D& Va, vec3D& Vb)
{
    vec3D Vc;
    Vc.x = Va.x + Vb.x;
    Vc.y = Va.y + Vb.y;
    Vc.z = Va.z + Vb.z;
    return Vc;
}

vec3D GRAPHICS_MATH::vector_Sub(vec3D& Va, vec3D& Vb)
{
    vec3D Vc;
    Vc.x = Va.x - Vb.x;
    Vc.y = Va.y - Vb.y;
    Vc.z = Va.z - Vb.z;
    return Vc;
}

vec3D GRAPHICS_MATH::vector_Rotate_Y(vec3D& Vi, float angle)
{
    float vi[3];

    vi[0] = Vi.x;
    vi[1] = Vi.y;
    vi[2] = Vi.z;

    y_Rotation_Matrix(angle, vi);

    vec3D Vn;

    Vn.x = vi[0];
    Vn.y = vi[1];
    Vn.z = vi[2];

    return Vn;
};

vec3D GRAPHICS_MATH::get_Plane_Normal_Vector(vec3D& Va, vec3D& Vb, vec3D& Vc)
{
    vec3D Av = vector_Sub(Vc, Va);
    vec3D Bv = vector_Sub(Vb, Va);

    vec3D Vn = vector_Cross_Product(Av, Bv);
    vec3D Vn_Norm = normalize_Vector(Vn);

    return Vn_Norm;
}







//MATRIX TRANSFORMS

void GRAPHICS_MATH::mat4x4(float mat[4][4], float vertex[3])
{
    //ROW MAJOR FORM, LEFT TO RIGHT

    float x = vertex[0];
    float y = vertex[1];
    float z = vertex[2];
    float w = 1;



    vertex[0] =
        (x * mat[0][0]) +
        (y * mat[0][1]) +
        (z * mat[0][2]) +
        (w * mat[0][3]);


    vertex[1] =
        (x * mat[1][0]) +
        (y * mat[1][1]) +
        (z * mat[1][2]) +
        (w * mat[1][3]);

    vertex[2] =
        (x * mat[2][0]) +
        (y * mat[2][1]) +
        (z * mat[2][2]) +
        (w * mat[2][3]);

    if (w != 0.0)//go from homogeouns space to cartisian space
    {
        x /= w;
        y /= w;
        z /= w;
    }
}

void GRAPHICS_MATH::translate_Matrix(float dx, float dy, float dz, float vertex[3])
{
    float translate[4][4] =
    {
        {1, 0, 0, dx},
        {0, 1, 0, dy},
        {0, 0, 1, dz},
        {0, 0, 0,  1}
    };

    mat4x4(translate, vertex);
}

void GRAPHICS_MATH::scaling_Matrix(float sf, float vertex[3])

{
    float xsf = sf;
    float ysf = sf;
    float zsf = sf;



    float scale[4][4] =
    {
        {xsf,   0,   0, 0},
        {0,   ysf,   0, 0},
        {0,     0, zsf, 0},
        {0,     0,   0, 1}
    };
    mat4x4(scale, vertex);
}

void GRAPHICS_MATH::projection_Matrix(float h, float w, float f, float n, float FOV, float vertex[3])

{
   

    if (vertex_Transform_Debug)
    {
        Serial.println("WORLD SPACE TO CLIP SPACE TRANSFORM");
        Serial.print("Vi x in = "); Serial.println(vertex[0]);
        Serial.print("Vi y in = "); Serial.println(vertex[1]);
        Serial.print("Vi z in = "); Serial.println(vertex[2]);
        Serial.println();
    }


    //float h = 240;
    //float w = 320;
    //float f = -10000;//far plane
    //float n = -10;//near plane
    //float FOV = deg_To_Rad(60);

    float fov = deg_To_Rad(FOV);

    float Projection[4][4] =
    {
        {h / w * 1 / tan(fov / 2),   0,             0,                0},
        {0,                     1 / tan(fov / 2),  0,                0},
        {0,                     0,             f / (f - n),          1},
        {0,                     0,             (-f * n) / (f - n), 0}
    };
    mat4x4(Projection, vertex);


}

void GRAPHICS_MATH::x_Rotation_Matrix(float anx, float vertex[3])
{
    float a = deg_To_Rad(anx);

    float rotate_X[4][4] =
    {
        {1,       0,        0,   0},
        {0,  cos(a),  -sin(a),   0},
        {0,  sin(a),   cos(a),   0},
        {0,       0,        0,   1}
    };


    mat4x4(rotate_X, vertex);

}

void GRAPHICS_MATH::y_Rotation_Matrix(float any, float vertex[3])
{
    float a = deg_To_Rad(any);
    float rotate_Y[4][4] =
    {
        { cos(a),   0, sin(a),    0},
        {      0,   1,      0,    0},
        {-sin(a),   0, cos(a),    0},
        {      0,   0,      0,    1}
    };
    mat4x4(rotate_Y, vertex);
}

void GRAPHICS_MATH::z_Rotation_Matrix(float anz, float vertex[3])
{
    float a = deg_To_Rad(anz);

    float rotate_Z[4][4] =
    {
        {   cos(a),   -sin(a),   0,   0},
        {   sin(a),    cos(a),   0,   0},
        {        0,         0,   1,   0},
        {        0,         0,   0,   1}
    };

    mat4x4(rotate_Z, vertex);

}

void GRAPHICS_MATH::look_At_Matrix(vec3D &A, vec3D &B, vec3D &C, vec3D &T, float vertex_N[3])

{
    bool debug = false;
    if (debug)
    {
        Serial.println("WORLD TO CAMERA SPACE TRANSFORM");
        Serial.print("Vi x in = "); Serial.println(vertex_N[0]);
        Serial.print("Vi y in = "); Serial.println(vertex_N[1]);
        Serial.print("Vi z in = "); Serial.println(vertex_N[2]);
        Serial.println();
    }





    float AX = A.x;//foword vector
    float AY = A.y;
    float AZ = A.z;

    float BX = B.x;//right vector
    float BY = B.y;
    float BZ = B.z;

    float CX = C.x;//up vector
    float CY = C.y;
    float CZ = C.z;
    float TA = - vector_Dot_Product(T,A); // INVERSE DOT PRODUCT OF T.d A
    float TB = - vector_Dot_Product(T,B); // INVERSE DOT PRODUCT OF T.d B
    float TC = - vector_Dot_Product(T,C); // INVERSE DOT PRODUCT OF T.d C


    float look_At[4][4] =
    {
        {AX,   BX,   CX, 0},
        {AY,   BY,   CY, 0},
        {AZ,   BZ,   CZ, 0},
        {TA,   TB,   TC, 1}
    };

    mat4x4(look_At, vertex_N);

    if (debug)
    {
        Serial.print("Vi x out = "); Serial.println(vertex_N[0]);
        Serial.print("Vi y out = "); Serial.println(vertex_N[1]);
        Serial.print("Vi z out = "); Serial.println(vertex_N[2]);
        Serial.println();
        Serial.println();
    }
}

void GRAPHICS_MATH::point_At_Matrix(vec3D &A, vec3D &B, vec3D &C, vec3D &T, float vertex_N[3])

{
    float AX = A.x;//foword vector
    float AY = A.y;
    float AZ = A.z;

    float BX = B.x;//right vector
    float BY = B.y;
    float BZ = B.z;

    float CX = C.x;//up vector
    float CY = C.y;
    float CZ = C.z;

    float TX = T.x; 
    float TY = T.y; 
    float TZ = T.z; 







    float point_At[4][4] =
    {
        {AX,   AY,   AZ, 0},
        {BX,   BY,   BZ, 0},
        {CX,   CY,   CZ, 0},
        {TX,   TY,   TZ, 1}
    };
    mat4x4(point_At, vertex_N);
}

void GRAPHICS_MATH::create_Camera_Origin(float origin[3], float angle_X, float angle_Y, float angle_Z, float vA[3], float vB[3], float vC[3])
{
    static float a_Vector[3] = { 0,0,1};
    static float b_Vector[3];
    static float c_Vector[3];
    



}

void GRAPHICS_MATH::world_Space_To_Camera_Space_Vertex_Translate(vec3D &camera_Pos, vec3D& camera_Up, vec3D& camera_Dir, float vertex[3])
{
    bool camera_Axis_Debug = false;

    //CAMERA ORIGIN VECTOR A FOWARD
    vec3D newForwad = vector_Sub(camera_Dir, camera_Pos);
    newForwad = normalize_Vector(newForwad);
    if (camera_Axis_Debug)
    {
        Serial.println();
        Serial.print("FORWARD X = "); Serial.println(newForwad.x);
        Serial.print("FORWARD Y = "); Serial.println(newForwad.y);
        Serial.print("FORWARD Z = "); Serial.println(newForwad.z);
        Serial.println();
    }
    

    //CAMERA ORIGIN VECTOR B UP
    vec3D a = vector_Multiply(newForwad, vector_Dot_Product(camera_Up, newForwad));
    vec3D newUp = vector_Sub(camera_Up, a);
    if (camera_Axis_Debug)
    {
        Serial.println();
        Serial.print("UP X = "); Serial.println(newUp.x);
        Serial.print("UP Y = "); Serial.println(newUp.y);
        Serial.print("UP Z = "); Serial.println(newUp.z);
        Serial.println();
    }

    //CAMERA ORIGIN VECTOR C RIGHT
    vec3D newRight = vector_Cross_Product(newUp, newForwad);
    if (camera_Axis_Debug)
    {
        Serial.println();
        Serial.print("RIGHT X = "); Serial.println(newRight.x);
        Serial.print("RIGHT Y = "); Serial.println(newRight.y);
        Serial.print("RIGHT Z = "); Serial.println(newRight.z);
        Serial.println();
    }
    //TRANSLATE TO CAMERA SPACE
    point_At_Matrix(newForwad, newRight, newUp, camera_Dir, vertex);
    look_At_Matrix(newForwad, newRight, newUp, camera_Dir, vertex);

}


void GRAPHICS_MATH::world_plane_To_Screen_Plane_Vertex_Translate
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
)
{
    int x_Screen_Space;
    int y_Screen_Space;
    float XNDC;
    float YNDC;
    float ZNDC;
    float temp[3];
    float view_Plane_Width = (wp2_x - wp1_x) / 2;
    float x_Zdivide = 0.0;
    float y_Zdivide = 0.0;

 




    if (vertex_Transform_Debug)
    {
        Serial.print("WORLD PLANE = "); Serial.println(wp2_x - wp1_x);
        Serial.print("FAR PLANE   = "); Serial.println(f);
        Serial.print("NEAR PLANE  = "); Serial.println(n);
        Serial.print("FOV         = "); Serial.println(FOV);
        if (projection_Type == 0) Serial.print("ORTHOGRAPHIC");
        if (projection_Type == 1) Serial.println("PERSPECTIVE");







        Serial.println("VERTEX Vi");
        Serial.print("x = "); Serial.println(vertex[0]);
        Serial.print("y = "); Serial.println(vertex[1]);
        Serial.print("z = "); Serial.println(vertex[2]);
        Serial.println();
    }



    //MAP VERTEX TO -1 to 1  NDC SPACE
    normalise_Range_To_1_0_1(vertex[0], -view_Plane_Width, view_Plane_Width, &XNDC);
    normalise_Range_To_1_0_1(vertex[1], -view_Plane_Width, view_Plane_Width, &YNDC);
    normalise_Range_To_1_0_1(vertex[2], -view_Plane_Width, view_Plane_Width, &ZNDC);
    //
    temp[0] = XNDC;
    temp[1] = YNDC;
    temp[2] = ZNDC;
    //

    if (vertex_Transform_Debug)
    {
        Serial.println("WORLD TO NDC -1 TO 1 SPACE");
        Serial.print("x = "); Serial.println(temp[0]);
        Serial.print("y = "); Serial.println(temp[1]);
        Serial.print("z = "); Serial.println(temp[2]);
        Serial.println();
    }




    //PROJECT TO CLIP SPACE
    projection_Matrix(h, w, f, n, FOV, temp);
    //
    if (vertex_Transform_Debug)
    {
        Serial.println("NDC SPACE TO PROJRCTED CLIP SPACE");
        Serial.print("x = "); Serial.println(temp[0]);
        Serial.print("y = "); Serial.println(temp[1]);
        Serial.print("z = "); Serial.println(temp[2]);
        Serial.println();
    }
    if (projection_Type == 1)//PERSPECTIVE
    {
        x_Zdivide = temp[0] / 1 / temp[2];//Z DIVIDE FOR PERSPECTIVE
        y_Zdivide = temp[1] / 1 / temp[2];
    }

    if (projection_Type == 0)//ORTHOGRAPHIC
    {
        x_Zdivide = temp[0];
        y_Zdivide = temp[1];
    }


    if (vertex_Transform_Debug)
    {
        Serial.println("CLIP SPACE PERSPECTIVE DIVIDE");
        Serial.print("x = "); Serial.println(x_Zdivide);
        Serial.print("y = "); Serial.println(y_Zdivide);
        Serial.println();
    }

    //MAP TO SCREEN SPACE FROM CLIP SPACE
    map_Value_To_Range(x_Zdivide, sp1_x, sp2_x, -1, 1, &x_Screen_Space);
    map_Value_To_Range(y_Zdivide, sp1_y, sp2_y, -1, 1, &y_Screen_Space);

    if (vertex_Transform_Debug)
    {
        Serial.println("CLIP SPACE TO SCREEN SPACE");
        Serial.print("x = "); Serial.println(x_Screen_Space);
        Serial.print("y = "); Serial.println(y_Screen_Space);
        Serial.println();
        Serial.println();
        Serial.println();
        Serial.println();
        Serial.println();
    }


    *Xn = x_Screen_Space;
    *Yn = y_Screen_Space;
}


void GRAPHICS_MATH::cast_Ray
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
)
{
    static float t = 0.0f;
    static float xyz[3] = { ray.direction.x, ray.direction.y, ray.direction.z };
    static bool set_Ray_Angle = true;
    static float ray_Length = 0.0f;
    static bool rotated = true;
    static float current_X;
    static bool HIT = false;

    if (!cast_Enable)
    {
        ray_Length = 0.0f;
        t = 0.0f;
        set_Ray_Angle = true;
        xyz[0] = ray.direction.x;
        xyz[1] = ray.direction.y;
        xyz[2] = ray.direction.z;
    }

    if (set_Ray_Angle)
    {
        x_Rotation_Matrix(dir_Angle_X, xyz);
        y_Rotation_Matrix(dir_Angle_Y, xyz);
        z_Rotation_Matrix(dir_Angle_Z, xyz);  
        set_Ray_Angle = false;
    }

  

    if (cast_Enable)//PER STEP
    {
        //STEP IN RAY DIRECTION 
        if (ray_Length < max_Length)
        {
            t += step_Length;       
            current_Tail.x = xyz[0] * t - ray.origin.x;
            current_Tail.y = xyz[1] * t - ray.origin.y;
            current_Tail.z = xyz[2] * t - ray.origin.z;
            ray_Length = t;
            *current_Length = ray_Length;  



            //check for hits here
            if ( current_Tail.x >= 2.00 && current_Tail.x <= 3 && current_Tail.z < 5)//5 = view plane
            {
                HIT = true;
                *hit = HIT;
                cast_Enable = false;
            }
            else
            {
                cast_Enable = true;          
            } 
        }
    } 
}


void GRAPHICS_MATH::cast_Ray_Into_Scene
(
    RAY ray,
    vec3D & current_Tail,
    float *current_Length,
    float max_Length,
    float step_Size,
    float ray_Angle_X,
    float ray_Angle_Y,
    float ray_Angle_Z,
    bool *hit,
    bool cast_Enable
)
{
     float current_L;
     bool HIT;
    while (1)
    {                                 
        if (current_L > max_Length) cast_Enable = false;
                            
                              




        //CHECK AGAINST OBJECT VECTOR HERE


        cast_Ray
        (
            ray,
            current_Tail,
            step_Size,
            &current_L,
            max_Length,
            ray_Angle_X,
            ray_Angle_Y,
            ray_Angle_Z,
            &HIT,
            cast_Enable
        );

        *current_Length = current_L;

        if (!cast_Enable) { *hit = HIT; break; }
    }
}

void GRAPHICS_MATH::normalise_Range_To_1_0_1(float xi, float xmin, float xmax, float* xn)
{
     *xn = (2 * (xi - xmin) / (xmax - xmin)) - 1;
}

void GRAPHICS_MATH::map_Value_To_Range(float Vi, float aMin, float aMax, float bMin, float bMax, int* Vn)
{
    
    float pn = (aMax - aMin) * ((Vi - bMin) / (bMax - bMin)) + aMin;
    *Vn = (int)pn;
   
    //range a bigger than range b

}

void GRAPHICS_MATH::normalize(int xi, int xMin, int xMax, float* ui)
{

    if (xi < xMin) xi = xMin;
    if (xi > xMax) xi = xMax;

    *ui = ((float)xi - (float)xMin) / ((float)xMax - (float)xMin);
} 

void GRAPHICS_MATH::normalize_Range_To_0_1(float xi, float xMin, float xMax, float* ui)
{

    if (xi < xMin) xi = xMin;
    if (xi > xMax) xi = xMax;

    *ui = (xi -xMin) / (xMax -xMin);
}

void GRAPHICS_MATH::interp(int X1, int X2, float P, int* PX)
{
    if (P > 1) P = 1;
    if (P < 0) P = 0;


    float DX = X2 - X1;
    *PX = X1 + (DX * P);
}

void GRAPHICS_MATH::point_Liner_Interpolation(int X1, int Y1, int X2, int Y2, float P, int *PX, int *PY)//(P1   P2)
{
    float DX = X2 - X1;
    float DY = Y2 - Y1;

    *PY =  Y1 + (DY * P);
    *PX =  X1 + (DX * P);
    
}

byte GRAPHICS_MATH::check_Line_Cross_p1_p3(int x1, int y1, int x3, int y3, int x2, int y2)
{
    int pos_X;
    int pos_Y;
    static byte side = 1;

    for (float pos = 0.00f; pos < 1.00f; pos += 0.01f)
    {
        point_Liner_Interpolation(x1, y1, x3, y3, pos, &pos_X, &pos_Y);

        if (x2 <= pos_X && y2 <= pos_Y) { side = 1; }
        if (x2 >  pos_X && y2 > pos_Y)   { side = 2; }
    
    

    }

    return side;
}

void GRAPHICS_MATH::Swap_XY(int x_In1, int y_In1, int x_In2, int y_In2, int* x_Out1, int* y_Out1, int* x_Out2, int* y_Out2)
{
    *x_Out1 = x_In2;
    *y_Out1 = y_In2;
    *x_Out2 = x_In1;
    *y_Out2 = y_In1;
}
void GRAPHICS_MATH::Swap_XY_Float(float x_In1, float y_In1, float x_In2, float y_In2, float* x_Out1, float* y_Out1, float* x_Out2, float* y_Out2)
{
    *x_Out1 = x_In2;
    *y_Out1 = y_In2;
    *x_Out2 = x_In1;
    *y_Out2 = y_In1;
}

void GRAPHICS_MATH::rotate_Point(int* x_Orign, int* y_Orign, float AN, bool direction)
{
    float Radius = *x_Orign;//read from refrence address
    AN = AN * 3.144 / 180;



    float clockwise_Rotation_Matrix[2][2] =
    {
    {cos(AN),   -sin(AN)},
    {sin(AN),    cos(AN)}
    };

    float counter_Clockwise_Rotation_Matrix[2][2] =
    {
    {cos(AN),   sin(AN)},
    {-sin(AN),  cos(AN)}
    };


    //RUN ORIGN THRUGH ROTATION MATRIX


    if (direction)
    {
        *x_Orign = (int)Radius * counter_Clockwise_Rotation_Matrix[0][0] + counter_Clockwise_Rotation_Matrix[0][1];//
        *y_Orign = (int)Radius * counter_Clockwise_Rotation_Matrix[1][0] - counter_Clockwise_Rotation_Matrix[1][1];//
    }

    if (!direction)
    {
        *x_Orign = (int)Radius * clockwise_Rotation_Matrix[0][0] + clockwise_Rotation_Matrix[0][1];//
        *y_Orign = (int)Radius * clockwise_Rotation_Matrix[1][0] - clockwise_Rotation_Matrix[1][1];//
    }
}

void GRAPHICS_MATH::get_Point_Vector(int PX1, int PY1, int R, float AN, bool rotation_Dir, int* px2, int* py2)
{
    int PX2 = R;
    int PY2 = 0;

    rotate_Point(&PX2, &PY2, AN, rotation_Dir);
    PX2 += PX1;
    PY2 += PY1;

    *px2 = PX2;//return end point of vector
    *py2 = PY2;
}

void GRAPHICS_MATH::angled_Point_Step(float angle, float step_Size, int x1, int y1, int* X_Next, int* Y_Next)
{
    //USES LENGTH OF HYPOTENUSE AND ANGLE TO GET X AND Y STEP SIZE
    angle = 30 * 3.144 / 180;
    float XL = step_Size;
    //
    float Yinc = sin(angle) * XL;
    float Xinc = sqrt((XL * XL) - (Yinc * Yinc));
    //
    float P1X = x1;
    float P1Y = y1;
    //
    float P2X = P1X + Xinc;
    float P2Y = P1Y - Yinc;
    //
    *X_Next = (int)P2X;
    *Y_Next = (int)P2Y;

}

float GRAPHICS_MATH::deg_To_Rad(float Ai)
{
    float An = Ai * 3.144 / 180;
    return An;
}

void GRAPHICS_MATH::triangle_CW_Sort(int &x1,int &y1,int &x2,int &y2,int &x3,int &y3)
{
    //FIND THE HIGHEST Y
    if (y2 < y1)   Swap_XY(x2, y2, x1, y1, &x2, &y2, &x1, &y1);
    if (y3 < y1)   Swap_XY(x3, y3, x1, y1, &x3, &y3, &x1, &y1);  
    if(y2 > y3)    Swap_XY(x2, y2, x3, y3, &x2, &y2, &x3, &y3);
    
}





























