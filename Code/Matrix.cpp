#include "Matrix.h"
#include "bits/stdc++.h"

using namespace std;

Matrix::Matrix(int r,int c)
    : row(r),column(c)
{
    for(int i=0;i<c*r;i++)
        elem.push_back(0.0f);
}

Matrix::Matrix(const Matrix &ref)
    : row(ref.row),column(ref.column)
{
    int i;

    for(i=0;i<row*column;i++)
        elem[i] = ref.elem[i];
}

Matrix::~Matrix()
{
    elem.clear();
}

void Matrix::ShowMatrix()
{
    int i,j;
    for(i=0;i<row;i++)
    {
        for(j=0;j<column;j++)
            printf("%f ",GetElem(i,j));
        printf("\n");
    }
}

void Matrix::SetElem(int x,int y,double val)
{
    elem.at(x+y*column) = val;
}

double Matrix::GetElem(int x,int y)  const
{
    x = min(x,row-1);
    x = max(x,0);

    y = min(y,row-1);
    y = max(y,0);

    return elem[x+y*column];
}
Matrix Matrix::Transpose()
{
    Matrix ans(column,row);

    int i,j;
    for(i=0;i<row;i++)
        for(j=0;j<column;j++)
            ans.SetElem(j,i,GetElem(i,j));

    return ans;
}

Matrix Matrix::operator+(const Matrix& ref)
{
    if(row != ref.row || column != ref.column)
    {
        printf("Error : Matrix_operator+");
        exit(-1);
    }

    Matrix ans(row,column);
    int i,j;
    for(i=0;i<row;i++)
    {
        for(j=0;j<column;j++)
            ans.SetElem(i,j,(GetElem(i,j)+ref.GetElem(i,j)));
    }

    return ans;
}

Matrix Matrix::operator-(const Matrix& ref)
{
    if(row != ref.row || column != ref.column)
    {
        printf("Error : Matrix_operator+");
        exit(-1);
    }

    Matrix ans(row,column);
    int i,j;
    for(i=0;i<row;i++)
    {
        for(j=0;j<column;j++)
            ans.SetElem(i,j,GetElem(i,j) - ref.GetElem(i,j));
    }

    return ans;
}

Matrix Matrix::operator*(const Matrix& ref)
{
    if(column != ref.row)
    {
        printf("Error : Matrix_operator*");
        exit(-1);
    }

    Matrix ans(row,ref.column);

    int i,j,k;
    int ir = row,jr = ref.column;
    double sum = 0;
    for(i=0;i<ir;i++)
    {
        for(j=0;j<jr;j++)
        {
            sum = 0;
            for(k=0;k<column;k++)
            {
                sum += GetElem(i,k) * ref.GetElem(k,j);
            }
            ans.SetElem(i,j,sum);
        }
    }

    return ans;
}
