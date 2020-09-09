#ifndef EX3_MTMMATTRIAG_H
#define EX3_MTMMATTRIAG_H


#include <vector>
#include "MtmExceptions.h"
#include "Auxilaries.h"
#include "MtmMatSq.h"

using std::size_t;

namespace MtmMath {


    template <typename T>
    class MtmMatTriag : public MtmMatSq<T>{
        bool is_upper;
    public:

        /*
         * Triangular Matrix constructor, m is the number of rows and columns in the matrix,
         * val is the initial value for the matrix elements and isUpper_ is whether it is upper
         * Rectangular matrix (true means it is)
         */
        MtmMatTriag (size_t m, const T& val=T(), bool isUpper_t=true):
                MtmMatSq<T>(m,val),is_upper(isUpper_t)
                {
                    int size = static_cast<int>(m);
                    if (isUpper_t)
                    {

                        for (int j=0;j<size;j++)
                        {
                            for (int i=j+1;i<size;i++)
                            {
                                this->data[i][j]=0;
                            }
                        }
                    }
                    else
                    {
                        for (int i=0;i<size;i++)
                        {
                            for (int j=i+1;j<size;j++)
                            {
                                this->data[i][j]=0;
                            }
                        }
                    }
                }

        MtmMatTriag (const MtmMatSq<T>& m): MtmMatSq<T>(m)
        { if (m.getDim().getRow()!=m.getDim().getCol()) {
                MtmExceptions::IllegalInitialization e;
                throw e; }
            bool tri_up=1;
            for (int j=0;j<m.Cols();j++) {
                if(tri_up==0) {
                    break; }
                for (int i=j+1;i<m.Rows();i++) {
                    if(this->data[i][j]!=0) {
                        tri_up=0;
                        break;
                    }
                }
            }if (tri_up==1) {
                is_upper=1;
            } else
            { bool tri_down=1;
                for (int i=0;i<m.Rows();i++) {
                    if(tri_down==0) {
                        break; }
                    for (int j=i+1;j<m.Cols();j++) {
                        if(this->data[i][j]!=0) {
                            tri_down=0;
                            break;
                        }
                    }
                } if (tri_down==1) {
                    is_upper=0;
                } else {
                    MtmExceptions::IllegalInitialization e;
                    throw e; }
            }
        }


        void reshape(Dimensions newDim)
        {
            MtmExceptions::ChangeMatFail e(this->dim,newDim);
            throw e;
        }

        void resize(Dimensions dim, const T& val=T())
        {
            if (dim.getCol()!=dim.getRow()) {
                MtmExceptions::ChangeMatFail e(this->dim,dim);
                throw e; }
            int diff = (int) (dim.getCol() - this->dim.getCol());
            if (diff<0) {
                diff *= -1;
                    while (diff) {
                        this->data.pop_back();
                        diff--; }
                    diff = (int) (dim.getCol() - this->dim.getCol());
                    for (int i = 0; i < static_cast<int>(dim.getRow()); i++) {
                        MtmVec<T> &temp = this->data[i];
                        int temp_diff = -diff;
                        while (temp_diff) {
                            temp.pop_back();
                            temp_diff--;
                        }
                    }
            } else if (diff>0) {
                while (diff) {
                    this->data.push_back(MtmVec<T>(this->dim.getCol(),val));
                    diff--; }
                for (int i = 0; i < static_cast<int>(dim.getRow()); i++) {
                    MtmVec<T> &temp = this->data[i];
                    int temp_diff = (int) (dim.getCol() - this->dim.getCol());
                    while (temp_diff) {
                        temp.push_back(val);
                        temp_diff--; }
                }
                if(this->is_upper) {
                    for (int j=0;j<static_cast<int>(dim.getCol());j++) {
                        for (int i=j+1;i<static_cast<int>(dim.getRow());i++) {
                            this->data[i][j]=0; }
                    }
                } else {
                    for (int i=0;i<static_cast<int>(dim.getRow());i++) {
                        for (int j=i+1;j<static_cast<int>(dim.getCol());j++) {
                            this->data[i][j]=0;
                        }
                    }
                }
            }this->dim=dim;
        }

        void transpose() override
        {
            MtmMatTriag new_mat(this->dim.getCol(),0,this->is_upper);
            for (int j=0;j<this->Cols();j++)
            {
                MtmVec<T> &temp = new_mat.data[j];
                for (int i = 0; i < this->Rows(); i++)
                {
                    temp[i]=(this->data[i][j]);
                }
            }
            *this=new_mat;
            this->is_upper=!(is_upper);
        }

        bool safe(int x,int y){
            if (x>=this->Rows()||y>=this->Cols() || x<0 || y<0)
            {
                MtmExceptions::AccessIllegalElement ia;
                throw ia;
            }
            if (is_upper)
            {
                if (x>y)
                {
                    return false;
                }
                else return true;
            }
            else {
                if (y>x)
                {
                    return false;
                }
                else return true;
            }
        }

    };

}


#endif //EX3_MTMMATTRIAG_H
