#ifndef EX3_MTMMATREC_H
#define EX3_MTMMATREC_H

#include <vector>
#include "MtmExceptions.h"
#include "Auxilaries.h"
#include "MtmMat.h"

using std::size_t;

namespace MtmMath {

    template <typename T>
    class MtmMatSq : public MtmMat<T>{
    public:
        /*
         * Rectangular Matrix constructor, m is the number of rows and columns in the matrix
         * and val is the initial value for the matrix elements
         */
        MtmMatSq (size_t m, const T& val=T()): MtmMat<T>(Dimensions(m,m),val){}

        MtmMatSq (const MtmMatSq<T>& m): MtmMat<T>(m){}

        ~MtmMatSq(){}

        MtmMatSq (const MtmMat<T>& m): MtmMat<T>(m)
                {
                    if (m.getDim().getRow()!=m.getDim().getCol())
                    {
                        MtmExceptions::IllegalInitialization e;
                        throw e;
                    }
                }
        void resize(Dimensions dim, const T& val=T()) {
            if (dim.getCol()!=dim.getRow()) {
                MtmExceptions::ChangeMatFail e(this->dim,dim);
                throw e; }

                if (dim.getCol()==dim.getRow()) {
                    if (!(dim == this->dim)) {
                        size_t new_dim = dim.getCol();
                        int diff = (int) (new_dim - this->dim.getCol());
                        if (diff < 0) {
                            diff *= -1;
                            while (diff) {
                                this->data.pop_back();
                                diff--; }
                } else {
                    while (diff) {
                       this->data.push_back(MtmVec<T>(this->dim.getCol(), val));
                        diff--; }
                }
                diff = (int) (new_dim - this->dim.getCol());
                if (diff < 0) {
                    for (int i = 0; i < static_cast<int>(dim.getRow()); i++) {
                        MtmVec<T> &temp = this->data[i];
                        int temp_diff = -diff;
                        while (temp_diff) {
                            temp.pop_back();
                            temp_diff--; }
                    }
                } else if(diff>0) {
                    for (int i = 0; i < static_cast<int>(dim.getRow()); i++) {
                        MtmVec<T> &temp = this->data[i];
                        int temp_diff = (int) (new_dim - this->dim.getCol());
                        while (temp_diff) {
                            temp.push_back(val);
                            temp_diff--;
                        }
                    }
                }
                this->dim=dim;
            }
        }
    }



        void reshape(Dimensions newDim)
        {
            MtmExceptions::ChangeMatFail e(this->dim,newDim);
            throw e;
        }

    };



}

#endif //EX3_MTMMATREC_H
