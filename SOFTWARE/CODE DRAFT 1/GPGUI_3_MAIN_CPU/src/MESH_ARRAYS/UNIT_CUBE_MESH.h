
float UNIT_CUBE_vertex_Array[8][3] =
{
{ -5.000 ,  -5.000 ,  -5.000 },
{ 5.000   ,  -5.000 ,  -5.000	},
{ -5.000 ,  -5.000 ,  5.000	},
{ 5.000   ,  -5.000 ,  5.000	},
{ -5.000 ,  5.000   ,  -5.000	},
{ 5.000   ,  5.000   ,  -5.000	},
{ -5.000 ,  5.000   ,  5.000	},
{5.000    ,  5.000   ,  5.000	}

};


//-1 from vertex array index
int UNIT_CUBE_faces[12][3] =
{
{ 1,  5, 6},
{ 6,  2, 1},
{ 2,  6, 8},
{ 8,  4, 2},
{ 4,  8, 7},
{ 7,  3, 4},
{ 3,  7, 5},
{ 5,  1, 3},
{ 5,  7, 8},
{ 8,  6, 5},
{ 3,  1, 2},
{ 2,  4, 3}
};
