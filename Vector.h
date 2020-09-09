#ifndef EX3_MTMVEC_H
#define EX3_MTMVEC_H

#include <vector>
#include "MtmExceptions.h"
#include "Complex.h"


using std::ostream;
using std::size_t;
using std::vector;

namespace MtmMath {
    template<typename T>
    class MtmVec {
        vector<T> data;
        Dimensions dim;
        bool col;


    public:
        /*
         * Vector constructor, m is the number of elements in it and val is the initial value for the matrix elements
         */
        MtmVec(size_t m, const T &val = T()) try  :  data(
                vector<T>(m, val)), dim(Dimensions(m, 1)), col(true) {
            if (m<=0)
            {
                MtmExceptions::IllegalInitialization e;
                throw e;
            }
        }
        catch (std :: bad_alloc &e)
        {
            MtmExceptions::OutOfMemory o;
            throw o;
        }

        MtmVec(const MtmVec &v) : data(v.data), dim(v.dim), col(v.col){}

        ~MtmVec() {}

        int Cols() const
        {
            return static_cast<int>(dim.getCol());
        }
        int Rows() const
        {
            return static_cast<int>(dim.getRow());
        }

        MtmVec<T>& operator=(const MtmVec<T>& M)
        {
            data=M.data;
            dim=M.dim;
            return *this;
        }




        /*
         * Function that get function object f and uses it's () operator on each element in the vectors.
         * It outputs the function object's * operator after iterating on all the vector's elements
         */
        template <typename Func>
         T vecFunc(Func& f) const
        {
            Func g=f;
            for(int i=0;i< static_cast<int>(data.size());i++)
            {
                g(data[i]);
            }
            return *g;
        }

        /*
         * Resizes a vector to dimension dim, new elements gets the value val.
         * Notice vector cannot transpose through this method.
         */
        void resize(Dimensions dim, const T &val = T()) {
            if ((dim.getRow() != 1 && dim.getCol() != 1)||
                                (dim.getRow() <= 0 || dim.getCol() <= 0)){
                MtmExceptions::ChangeMatFail e(this->dim, dim);
                throw e; }
            if (dim.getCol() != this->dim.getCol() && dim.getRow() != this->dim.getRow()) {
                MtmExceptions::ChangeMatFail e(this->dim, dim);
                throw e; }
            if (dim.getRow() == 1) {
                int diff_col = (int) (dim.getCol() - this->dim.getCol());
                if (diff_col > 0) {
                    while (diff_col) {
                        this->data.push_back(val);
                        diff_col--;
                    }
                } else if (diff_col < 0) {
                    diff_col = -diff_col;
                    while (diff_col) {
                        this->data.pop_back();
                        diff_col--;
                    }}
            } else if (dim.getCol() == 1) {
                int diff_row = (int) (dim.getRow() - this->dim.getRow());
                if (diff_row < 0) {
                    diff_row = -diff_row;
                    while (diff_row) {
                        this->data.pop_back();
                        diff_row--;
                    }
                } else if (diff_row > 0) {
                    while (diff_row) {
                        this->data.push_back(val);
                        diff_row--;
                    }}} this->dim=dim;
        }

        T& operator[](int index)
        {
            if (index<0) {
                MtmExceptions::AccessIllegalElement ia;
                throw ia;
            }
            if (dim.getCol()==1) {
                if(index>=this->Rows())
                {
                    MtmExceptions::AccessIllegalElement ia;
                    throw ia;
                }
            } else  if (dim.getRow()==1)
            {
                if(index>=this->Cols())
                {
                    MtmExceptions::AccessIllegalElement ia;
                    throw ia;
                }
            }
            return data[index];
        }
        const T& operator[](int index) const
        {
            if (index<0) {
                MtmExceptions::AccessIllegalElement ia;
                throw ia;
            }
            if (dim.getCol()==1) {
                if(index>=this->Rows())
                {
                    MtmExceptions::AccessIllegalElement ia;
                    throw ia;
                }
            } else  if (dim.getRow()==1)
            {
                if(index>=this->Cols())
                {
                    MtmExceptions::AccessIllegalElement ia;
                    throw ia;
                }
            }
            return data[index];
        }

        /*
         * Performs transpose operation on matrix
         */
        void transpose() {
            dim.transpose();
            bool temp=(!(col));
            col=temp;
        }

        Dimensions getDim() const
        {
            return this->dim;
        }

        MtmVec<T>  operator-() const
        {
            if(dim.getCol()==1)
            {MtmVec<T> new_vec(dim.getRow());
            for (int i=0;i<this->Rows();i++)
            {
                    new_vec[i]=-this->data[i];
            }
            return new_vec;}
            else
            {MtmVec<T> new_vec(dim.getCol());
                for (int i=0;i<this->Cols();i++)
                {
                    new_vec[i]=-this->data[i];
                }
                new_vec.transpose();
                return new_vec;
            }
        }

        void pop_back()
        {
            data.pop_back();
            Dimensions new_dim(dim.getRow()-1,dim.getCol());
            dim=new_dim;
        }
        void push_back(const T& val)
        {
            data.push_back(val);
            Dimensions new_dim(dim.getRow()+1,dim.getCol());
            dim=new_dim;
        }

        class iterator  {
        protected:
            MtmVec<T>& vector;
            int i;

        public :
            explicit iterator (MtmVec& vec,int i=0): vector(vec), i(i) {}

            virtual iterator& operator ++()
            {
               i++;
                return *this;
            }

            T& operator*()
            {
                return vector[i];
            }
            const T& operator*() const
            {
                return vector[i];
            }
            bool operator==(const iterator& i1) const
            {
                return (i1.i==this->i);
            }

            bool operator!=(const iterator& i1) const
            {
                return !(i1==*this);
            }

        };


        iterator begin()
        {
            return iterator(*this);
        }

        iterator end()
        {
            iterator it=this->begin();
            for (int i=0;i< static_cast<int>(this->data.size());i++)
            {
                ++it;
            }
            return it;
        }

        class nonzero_iterator : public iterator
        {
        public:
            explicit nonzero_iterator(MtmVec& matrix,int i=0): iterator(matrix,i) {}
            nonzero_iterator& operator ++() override
            {

                if (this->i> static_cast<int>(this->vector.data.size()-1))
                {
                    return *this;
                }

                if (this->i== static_cast<int>(this->vector.data.size()-1))
                {
                    this->i++;
                    return *this;
                }
                this->i++;
                while (this->vector.data[this->i]==0) {
                    if (this->i== static_cast<int>(this->vector.data.size()-1))
                    {
                        this->i++;
                        return *this;
                    }
                    this->i++;
                    }
                    return *this;
                }
                int getI()const {
                return this->i;
            }

        };
        nonzero_iterator nzbegin()
        {
            nonzero_iterator nz_it(*this);
            while(*nz_it==0)
            {
                ++nz_it;
                if (nz_it.getI()== static_cast<int>(this->data.size()))
                {
                    return nz_it;
                }
            }
            return nz_it;
        }

        nonzero_iterator nzend()
        {
            nonzero_iterator it=this->nzbegin();
            for (int i=0;i<static_cast<int>(data.size())-(this->num_of_zero());i++)
            {
                ++it;
            }
            return it;
        }

        int num_of_zero() const
        {
            int num=0;
            for (int i=0;i< static_cast<int>(data.size());i++)
            {
                if (this->data[i]==0)
                    {
                        num++;
                    }
            }
            return num;
        }



        const bool getColBool()const
        {
            return col;
        }


    };


    template <class T>
    MtmVec<T>  operator+(const MtmVec<T>& A, const T& val)
    {
            if(A.Cols()==1) {
                MtmVec<T> new_vec(A.getDim().getRow());
                for (int i = 0; i < A.Rows(); i++) {
                    new_vec[i] = A[i] + val;
                }
                return new_vec;
            }
       else  {
                MtmVec<T> new_vec(A.getDim().getCol());
                for (int i = 0; i < A.Cols(); i++) {
                    new_vec[i] = A[i] + val;
                }
                new_vec.transpose();
                return new_vec;
            }
    }
    template <class T>
    MtmVec<T>  operator-(const MtmVec<T>& A, const T& val)
    {
        return A+(-val);
    }
    template <class T>
    MtmVec<T>  operator+(const T& val,const MtmVec<T>& A)
    {
        return A+val;
    }
    template <class T>
    MtmVec<T>  operator-(const T& val,const MtmVec<T>& A)
    {
        return -A+val;
    }

    template <class T>
    MtmVec<T>  operator+(const MtmVec<T>& A, const MtmVec<T>& B)
    {
        if (A.getDim()!=B.getDim()) {
            MtmExceptions::DimensionMismatch e(A.getDim(),B.getDim());
            throw e;
        }
        if (A.getColBool() != B.getColBool()) {
            MtmExceptions::DimensionMismatch e(A.getDim(),B.getDim());
            throw e;
        }
        if(A.getDim().getCol()==1) {
            MtmVec<T> new_vec(A.getDim().getRow());
            for (int i = 0; i < A.Rows(); i++) {
                new_vec[i] = A[i] + B[i];
            }
            return new_vec;
        }
        else
        {
            MtmVec<T> new_vec(A.getDim().getCol());
            for (int i = 0; i < A.Cols(); i++) {
                new_vec[i] = A[i] + B[i];
            }
            new_vec.transpose();
            return new_vec;
        }
    }
    template <class T>
    MtmVec<T>  operator-(const MtmVec<T>& A, const MtmVec<T>& B)
    {
        return A+(-B);

    }

    template <class T>
    MtmVec<T>  operator*(const T& val, const MtmVec<T>& A) {

        if (A.getDim().getCol()==1) {
            MtmVec<T> Mul(A.getDim().getRow());

        for (int i=0;i<A.Rows();i++)
        {
                Mul[i] = val * A[i];

        }
        return Mul;
        }
        else
        {
            MtmVec<T> Mul(A.getDim().getCol());

            for (int i=0;i<A.Cols();i++)
            {
                Mul[i] = val * A[i];

            }
            Mul.transpose();
            return Mul;
        }
    }
    template <class T>
    MtmVec<T>  operator*(const MtmVec<T>& A,const T& val) {
        return val*A;
    }


}


#endif //EX3_MTMVEC_H
