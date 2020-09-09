
#ifndef EX3_MTMEXCEPTIONS_H
#define EX3_MTMEXCEPTIONS_H

#include <exception>
#include <string>
#include <iostream>
#include "Auxilaries.h"
using std::string;

namespace MtmMath {
    namespace MtmExceptions {
        class MtmExceptions : public std::exception {
        public:
        virtual ~MtmExceptions() throw() {}
    };

        /*
         * Exception for illegal initialization of an object, needs to output
         * "MtmError: Illegal initialization values" in what() class function
         */
        class IllegalInitialization : public MtmExceptions {
            string message;
        public:
            IllegalInitialization(): message(){
                message="MtmError: Illegal initialization values";
            }
            const char *what() const noexcept override {
                return message.data();
            }
        };

        /*
         * Exception for Memory allocation failure for an object, needs to output
         * "MtmError: Out of memory" in what() class function
         */
        class OutOfMemory : public MtmExceptions {
            string message;
        public:
            OutOfMemory(): message(){
                message="MtmError: Out of memory";
            }
            const char *what() const noexcept override {
                return message.data();
            }

        };

        /*
         * Exception for dimension mismatch during a mathematical function, needs to output
         * "MtmError: Dimension mismatch: (<mat 1 row>,<mat 1 col>) (<mat 2 row>,<mat 2 col>)"
         * in what() class function
         */
        class DimensionMismatch : public MtmExceptions {
            Dimensions dim1, dim2;
            string message;
        public:
            DimensionMismatch(Dimensions dim1, Dimensions dim2) : MtmExceptions(), dim1(dim1), dim2(dim2), message() {
                string temp = "MtmError: Dimension mismatch: ";
                message= temp + dim1.to_string() +" "+dim2.to_string();
            }
            const char *what() const noexcept override {
                return message.data();
            }


        };

        /*
         * Exception for error for changing matrix/vector shape in reshape and resize, needs to output
         * "MtmError: Change matrix shape failed from: (<mat row>,<mat col>) (<new mat row>,<new mat col>)"
         * in what() class function
         */
        class ChangeMatFail : public MtmExceptions {
            Dimensions dim1, dim2;
            string message;

        public:
            ChangeMatFail(Dimensions dim1, Dimensions dim2) : MtmExceptions(), dim1(dim1), dim2(dim2), message() {
                string temp = "MtmError: Change matrix shape failed from ";
                message= temp + dim1.to_string()+" to "+ dim2.to_string();
            }

            const char *what() const noexcept override {
                return message.data();
            }

        };

        /*
         * Exception for accessing an illegal element in matrix or vector, needs to output
         * "MtmError: Attempt access to illegal element" in what() class function
         */
        class AccessIllegalElement : public MtmExceptions {
            string message;
        public:
            AccessIllegalElement(): message(){
                message="MtmError: Attempt access to illegal element";
            }
            const char *what() const noexcept override {
                return message.data();
            }

        };
    }

}




#endif //EX3_MTMEXCEPTIONS_H
