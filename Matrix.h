//
// Created by sasso on 12/27/2018.
//


#ifndef EX3_MTMMAT_H
#define EX3_MTMMAT_H





using namespace MtmMath;
using std::size_t;
using std :: vector;
using std ::ostream;



namespace MtmMath {

    template <typename T>
    class MtmMat {
    protected:
       vector<MtmVec<T>> data;
        Dimensions dim;


    public:
        /*
         * Matrix constructor, dim_t is the dimension of the matrix and val is the initial value for the matrix elements
         */
        explicit MtmMat(Dimensions dim_t, const T &val = T()) try : data(
                vector<MtmVec<T>>(dim_t.getRow(), MtmVec<T>(dim_t.getCol(), val))), dim(dim_t)
        {
           if (dim_t.getRow()<=0 || dim_t.getCol()==0)
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

        MtmMat(const MtmMat &m) :
                data(vector<MtmVec<T>>(m.dim.getRow(), MtmVec<T>(m.dim.getCol()))), dim(m.dim)
        {
            data = m.data;
        }

        class check{
            int x;
            MtmMat<T>& matrix;
        public:
                check (MtmMat& mat,int i) : x(i), matrix(mat){}
                T& operator[](int y){
                if(matrix.safe(x,y)){
                    return matrix.data[x][y];
                }
                else
                {
                    MtmExceptions::AccessIllegalElement ia;
                    throw ia;
                }
            }
        };


        virtual check operator[](int x){
            return check(*this,x);
        }

        virtual bool safe(int x,int y) {
        if (x>=this->Rows()||y>=this->Cols() ||x<0 || y<0)
        {
            MtmExceptions::AccessIllegalElement ia;
            throw ia;
        }
        return true;
        }




        const MtmVec<T> &operator[](int i) const {
            if (i>=this->Rows())
            {
                MtmExceptions::AccessIllegalElement ai;
                throw ai;
            }
            return data[size_t(i)];
        };

        ~MtmMat(){}



        class iterator  {
        protected:
            MtmMat<T>& matrix;
            int i;
            int j;

        public :
            explicit iterator (MtmMat& matrix,int i=0,int j=0 ):
                    matrix(matrix), i(i), j(j) {}

            virtual iterator& operator ++()
            {
                if (i==matrix.Rows()-1 && j==matrix.Cols()-1)
                {
                    i=0;
                    j=matrix.Cols();
                    return *this;
                }
                if (i==matrix.Rows()-1)
                {
                    i=0;
                    j++;
                    return *this;
                }
                i++;
                return *this;
            }

            T& operator*()
            {
                return matrix[i][j];
            }
            const T& operator*() const
            {
                return matrix[i][j];
            }
            bool operator==(const iterator& i1) const
            {
                return (i1.i==this->i && i1.j==this->j);
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
            for (int i=0;i<this->Rows()*this->Cols();i++)
            {
                ++it;
            }
            return it;
        }

        class nonzero_iterator : public iterator
        {
        public:
            explicit nonzero_iterator(MtmMat& matrix,int i=0,int j=0 ):
                    iterator(matrix,i,j) {}
            nonzero_iterator& operator ++() override {
                if (this->i ==0 && this->j >= this->matrix.Cols())
                {
                    return *this;
                }
                if (this->i == this->matrix.Rows() - 1 && this->j == this->matrix.Cols() - 1) {
                    this->i = 0;
                    this->j = this->matrix.Cols();
                    return *this;
                }
                if (this->i == this->matrix.Rows() - 1) {
                    this->i = 0;
                    this->j++;
                    while (this->matrix.data[this->i][this->j] == 0) {
                        if (this->i == this->matrix.Rows() - 1 && this->j == this->matrix.Cols() - 1) {
                            this->i = 0;
                            this->j = this->matrix.Cols();
                            return *this;
                        }
                        if (this->i == this->matrix.Rows() - 1) {
                            this->i = 0;
                            this->j++;
                        } else { this->i++; }
                    } return *this;
                } else {
                    this->i++;
                    while (this->matrix.data[this->i][this->j] == 0) {
                        if (this->i == this->matrix.Rows() - 1 && this->j == this->matrix.Cols() - 1) {
                            this->i = 0;
                            this->j = this->matrix.Rows();
                            return *this;
                        }
                        if (this->i == this->matrix.Rows() - 1) {
                            this->i = 0;
                            this->j++;
                        }
                        else {this->i++;}
                    }
                    return *this;
                }
            }
            int getI()
            {
                return this->i;
            }
            int getJ()
            {
                return this->j;
            }
        };
        nonzero_iterator nzbegin()
        {
            nonzero_iterator nz_it(*this);
            while(*nz_it==0)
            {
                ++nz_it;
                if(nz_it.getI()==0 && nz_it.getJ()==this->Cols())
                {
                    return nz_it;
                }
            }
            return nz_it;
        }

        nonzero_iterator nzend()
        {
            nonzero_iterator it=this->nzbegin();
            for (int i=0;i<this->Rows()*this->Cols()-(this->num_of_zero());i++)
            {
                ++it;
            }
            return it;
        }

        int num_of_zero() const
        {
            int num=0;
            for (int i=0;i<this->Rows();i++)
            {
                for (int j=0;j<this->Cols();j++)
                {
                    if (this->data[i][j]==0)
                    {
                        num++;
                    }
                }
            }
            return num;
        }


        /*
         * Function that get function object f and uses it's () operator on each element in the matrix columns.
         * It outputs a vector in the size of the matrix columns where each element is the final output
         * by the function object's * operator
         */
        template <typename Func>
        MtmVec<T> matFunc(Func& f) const
        {
            Func g=f;
            MtmVec<T> res(dim.getCol());
            for (int j=0;j<this->Cols();j++)
            {     g=f;
                for (int i = 0; i < this->Rows(); i++)
                {
                    g(this->data[i][j]);
                }
                res[j]=*g;
            }
            return res;
        }

        /*
         * resizes a matrix to dimension dim, new elements gets the value val.
         */
        virtual void resize(Dimensions dim, const T& val=T())
        {
            if (!(dim == this->dim)) {
                size_t new_col = dim.getCol();
                size_t new_row = dim.getRow();
                int diff_col = (int) (new_col - this->dim.getCol());
                int diff_row = (int) (new_row - this->dim.getRow());
                if (diff_row < 0) {
                    diff_row *= -1;
                    while (diff_row) {
                        data.pop_back();
                        diff_row--;
                    }
                } else {
                    while (diff_row) {
                        data.push_back(MtmVec<T>(this->dim.getCol(), val));
                        diff_row--;
                    }
                }
                if (diff_col < 0) {
                    for (int i = 0; i < static_cast<int>(data.size()); i++) {
                        MtmVec<T> &temp = data[i];
                        int temp_diff = -diff_col;
                        while (temp_diff) {
                            temp.pop_back();
                            temp_diff--;
                        }
                    }
                } else {
                    for (int i = 0; i < static_cast<int>(data.size()); i++) {
                        MtmVec<T> &temp = data[i];
                        int temp_diff = diff_col;
                        while (temp_diff) {
                            temp.push_back(val);
                            temp_diff--;
                        }
                    }
                }
                this->dim = dim;
            }
        }


        int Cols() const
        {
            return static_cast<int>(dim.getCol());
        }
        int Rows() const
        {
            return static_cast<int>(dim.getRow());
        }
        MtmMat<T>& operator=(const MtmMat<T>& M)
        {

            int diff = M.Rows()-this->Rows();
            if (diff<0)
            {   diff=-diff;
                while (diff)
                {
                    this->data.pop_back();
                    diff--;
                }
            }
            else if (diff>0) {
                while (diff) {
                    this->data.push_back(MtmVec<T>(M.dim.getCol()));
                    diff--;
                }
            }

            for (int i=0;i<M.Rows();i++)
            {
                data[i]=M.data[i];
            }
            dim=M.dim;
            return *this;
        }



        /*
         * reshapes matrix so linear elements value are the same without changing num of elements.
         */
        virtual void reshape(Dimensions newDim)
        {
            if (newDim.getCol()*newDim.getRow()!=dim.getCol()*dim.getRow())
            {
                MtmExceptions::ChangeMatFail e(dim,newDim);
                throw e;
            }
            MtmMat new_mat(newDim,0);
            int new_i,new_j,i=0,j=0;
            for (new_j=0;new_j<new_mat.Cols();new_j++)
            {
                for (new_i = 0; new_i < new_mat.Rows(); new_i++)
                {
                    new_mat[new_i][new_j]=data[i][j];
                    i++;
                    if(i==this->Rows()) {
                        if (j == this->Cols()-1) { break; }
                        else {
                            i = 0;
                            j++;
                        }
                    }
                }
            }

            *this=new_mat;
        }

        /*
         * Performs transpose operation on matrix
         */
        virtual void transpose()
        {
            MtmMat new_mat(Dimensions(dim.getCol(),dim.getRow()),0);
            for (int j=0;j<this->Cols();j++)
            {
                MtmVec<T> &temp = new_mat.data[j];
                for (int i = 0; i < this->Rows(); i++)
                {
                    temp[i]=(this->data[i][j]);
                }
            }
            *this=new_mat;
        }

        Dimensions getDim() const {
            return this->dim;
        }
        MtmMat<T>  operator-() const
        {
            MtmMat<T> new_matrix(dim);
            for (int i=0;i<this->Rows();i++)
            {
                for (int j=0;j<this->Cols();j++)
                {
                    new_matrix[i][j]=-this->data[i][j];
                }
            }
            return new_matrix;
        }


    };

    template <class T>
    MtmMat<T>  operator+(const MtmMat<T>& A, const MtmMat<T>& B)
    {
        if (A.getDim()!=B.getDim())
        {
            MtmExceptions::DimensionMismatch e(A.getDim(),B.getDim());
            throw e;
        }
        MtmMat<T> Sum(A.getDim());
        for (int i=0;i<Sum.Rows();i++)
        {
            for (int j=0;j<Sum.Cols();j++)
            {
                Sum[i][j]=A[i][j]+B[i][j];
            }
        }
        return Sum;
    }

    template <class T>
    MtmMat<T>  operator-(const MtmMat<T>& A, const MtmMat<T>& B)
    {
        MtmMat<T> res=A+(-B);
        return res;
    }

    template <class T>
    MtmMat<T>  operator*(const MtmMat<T>& A, const MtmMat<T>& B)
    {

        if ( B.getDim().getRow()!=A.getDim().getCol() )
        {
            MtmExceptions::DimensionMismatch e(A.getDim(),B.getDim());
            throw e;
        }
        MtmMat<T> Mul(Dimensions(A.getDim().getRow(),B.getDim().getCol()));
        for (int i=0;i<Mul.Rows();i++)
        {
            for (int j=0;j<Mul.Cols();j++)
            {
                for (int k = 0; k < A.Cols(); k++) {
                    Mul[i][j] += A[i][k] * B[k][j];
                }
            }
        }
        return Mul;
    }
    template <class T>
    MtmMat<T>  operator*(const T& val, const MtmMat<T>& A) {

        MtmMat<T> Mul(Dimensions(A.getDim().getRow(), A.getDim().getCol()));

        for (int i=0;i<Mul.Rows();i++)
        {
            for (int j=0;j<Mul.Cols();j++)
            {
                Mul[i][j] = val * A[i][j];
            }
        }
        return Mul;
    }

    template <class T>
    MtmMat<T>  operator*(const MtmMat<T>& A,const T& val) {
        return val*A;
    }


    template <class T>
    MtmMat<T>  operator+(const MtmMat<T>& A, const T& val)
    {

        MtmMat<T> Sum(A.getDim());
        for (int i=0;i<Sum.Rows();i++)
        {
            for (int j=0;j<Sum.Cols();j++)
            {
                Sum[i][j]=A[i][j]+val;
            }
        }
        return Sum;
    }

    template <class T>
    MtmMat<T>  operator+( const T& val, const MtmMat<T>& A) {
        return A+val;
    }
    template <class T>
    MtmMat<T>  operator-(const MtmMat<T>& A, const T& val)
    {
        return operator+(A,-val);
    }
    template <class T>
    MtmMat<T>  operator-( const T& val,const MtmMat<T>& A)
    {
        return (-A)+val;
    }




    template <class T>
    MtmMat<T>  operator+(const MtmMat<T>& A, const MtmVec<T>& B)
    {
        if (A.getDim()!=B.getDim())
        {
            MtmExceptions::DimensionMismatch e(A.getDim(),B.getDim());
            throw e;
        }
        MtmMat<T> sum(A.getDim());

        if (A.Cols()==1)
        {
            for (int i = 0; i < A.Rows(); i++) {
                sum[i][0] = A[i][0] + B[i];
            }
        }
        else if (A.Rows()==1)
        {
            for (int i = 0; i < A.Cols(); i++) {
                sum[0][i] = A[0][i] + B[i];
            }
        }
        return sum;

    }
    template <class T>
    MtmMat<T>  operator-(const MtmMat<T>& A, const MtmVec<T>& B)
    {
        return A+(-B);

    }
    template <class T>
    MtmMat<T>  operator+(const MtmVec<T>& A,const MtmMat<T>& B)
    {
        if (A.getDim()!=B.getDim())
        {
            MtmExceptions::DimensionMismatch e(A.getDim(),B.getDim());
            throw e;
        }
        MtmMat<T> sum(A.getDim());

        if (A.Cols()==1)
        {
            for (int i = 0; i < A.Rows(); i++) {
                sum[i][0] = B[i][0] + A[i];
            }
        }
        else if (A.Rows()==1)
        {
            for (int i = 0; i < A.Cols(); i++) {
                sum[0][i] = B[0][i] + A[i];
            }
        }
        return sum;

    }
    template <class T>
    MtmMat<T>  operator-(const MtmVec<T>& A,const MtmMat<T>& B)
    {
        return A+(-B);
    }
    template <class T>
    MtmMat<T>  operator*(const MtmMat<T>& A, const MtmVec<T>& B) {

        if (B.getDim().getRow() != A.getDim().getCol()) {
            MtmExceptions::DimensionMismatch e(A.getDim(), B.getDim());
            throw e;
        }
        MtmMat<T> Mul(Dimensions(A.getDim().getRow(), B.getDim().getCol()));
        for (int i = 0; i < Mul.Rows(); i++) {
            for (int j = 0; j < Mul.Cols(); j++) {
                for (int k = 0; k < A.Cols(); k++) {
                    Mul[i][j] += A[i][k] * B[k];
                }
            }
        }
        return Mul;
    }

    template <class T>
    MtmMat<T>  operator*(const MtmVec<T>& A, const MtmMat<T>& B)
    {

        if ( B.getDim().getRow()!=A.getDim().getCol() )
        {
            MtmExceptions::DimensionMismatch e(A.getDim(),B.getDim());
            throw e;
        }
        MtmMat<T> Mul(Dimensions(A.getDim().getRow(),B.getDim().getCol()));
        for (int i=0;i<Mul.Rows();i++)
        {
            for (int j=0;j<Mul.Cols();j++)
            {
                for (int k = 0; k < A.Cols(); k++) {
                    Mul[i][j] += A[k] * B[k][j];
                }
            }
        }
        return Mul;
    }
    template <class T>
    MtmMat<T>  operator*(const MtmVec<T>& A, const MtmVec<T>& B) {

        if (B.getDim().getRow() != A.getDim().getCol()) {
            MtmExceptions::DimensionMismatch e(A.getDim(), B.getDim());
            throw e;
        }
        MtmMat<T> Mul(Dimensions(A.getDim().getRow(), B.getDim().getCol()));
        if (A.Rows()==1) {
            {
                for (int k = 0; k < A.Cols(); k++) {
                    Mul[0][0] += A[k] * B[k];
                }
            }
        }
        else if (A.Cols()==1)
        {
            for (int i=0;i<Mul.Rows();i++)
            {
                for (int j=0;j<Mul.Cols();j++)
                {
                    Mul[i][j] = A[i] * B[j];
                }
            }
        }
        return Mul;
    }



}


#endif //EX3_MTMMAT_H
