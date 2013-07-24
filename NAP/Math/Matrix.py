'''
Created on May 25, 2012

@author: Cengiz
'''

from Error.BadInputError import *
from math import *

class Matrix:
    
    def __init__(self, width, matrix):
        
        self.width=width
        self.matrix=[]
                    
        
        if (len(matrix)!=self.width**2):
            print("Matrix uzunlugu yanlis. Len.Matrix={}, width={}".format(len(self.matrix), self.width**2))
            raise BadInputError("Hata!")
        
        try:
            temp=[]
            for i in xrange(self.width**2):
                temp.append(float(matrix[i]))
                if (i+1) % self.width == 0:
                    self.matrix.append(temp)
                    temp=[]
        except:
            raise BadInputError("Matriste sayi olmayan bir eleman var")
                
    def printMatrix(self):
        for row in self.matrix:
            print(row)
            
    def multiplyRow(self, row, value):
        for i in xrange(self.width):
            self.matrix[row][i]*=value
                
    def multiplyColumn(self, column, value):
        for i in xrange(self.width):
            self.matrix[i][column]*=value
            
    def addRow2Row(self, firstrow, secondrow, coeff=1):
        for i in xrange(self.width):
            self.matrix[secondrow][i]+=self.matrix[firstrow][i]*coeff
            
    def addCol2Col(self, firstcol, secondcol, coeff=1):
        for i in range(self.width):
            self.matrix[i][secondcol]+=self.matrix[i][firstcol]*coeff
            
    def subMatrix(self, cancelledRow, cancelledColumn):
        temp=[]
        for row in xrange(self.width):
            if row==cancelledRow :
                continue
            for column in xrange(self.width):
                if column==cancelledColumn :
                    continue
                else:
                    temp.append(self.matrix[row][column])
        return Matrix(self.width-1, temp)
                   
    def determinant(self):
        if self.width==1:
            return self.matrix[0][0]
        else:
            sum1=0
            signflag=0
            for i in xrange(self.width):
                signflag = 1 if i%2==0 else -1
                sum1 += signflag*self.matrix[0][i]*self.subMatrix(0, i).determinant()
            return sum1

    def gaussElimination(self, ec):
        if self.determinant()==0:
            raise TypeError("Bu denklem sisteminin tek bir cozumu yoktur.")
        elif len(ec)!=self.width:
            raise TypeError("Denklem sabiti sayisi yanlis.")
        else:                
            clone=self
            equationConstants=ec
            
            for i in xrange(clone.width):
                if clone.matrix[i][i]==0:
                    for j in xrange(clone.width-i):
                        if clone.matrix[i+j][i]!=0:
                            clone.addRow2Row(i+j,i)
                            equationConstants[i]+=equationConstants[i+j]
                            break
                        
                for j in xrange(1, clone.width-i):
                    equationConstants[i+j]+=equationConstants[i]*(float(-clone.matrix[i+j][i])/float(clone.matrix[i][i]))
                    clone.addRow2Row(i, i+j, coeff=(float(-clone.matrix[i+j][i])/float(clone.matrix[i][i])))
                
                equationConstants[i]*=1/float(clone.matrix[i][i])
                clone.multiplyRow(i, 1/float(clone.matrix[i][i]))
                
            for i in xrange(clone.width):
                for j in xrange(1, clone.width-i):
                    equationConstants[clone.width-i-j-1]+=equationConstants[clone.width-i-1]*(float(-clone.matrix[clone.width-i-j-1][clone.width-i-1])/
                                                                  float(clone.matrix[clone.width-i-1][clone.width-i-1]))
                    clone.addRow2Row(clone.width-i-1, clone.width-i-j-1, coeff=(float(-clone.matrix[clone.width-i-j-1][clone.width-i-1])/
                                                                            float(clone.matrix[clone.width-i-1][clone.width-i-1])))
            
        
        return equationConstants
    
    def transpose(self):
        temp=[]
        for i in xrange(self.width):
            for j in xrange(self.width):
                temp.append(self.matrix[j][i])
        return Matrix(self.width, temp)

    def inverse(self):
        determinant=self.determinant()
        if determinant==0 : raise ValueError("Bu matrisin tersi yoktur.")
        temp=[]
        flag=0
        for i in xrange(self.width):
            for j in xrange(self.width):
                flag= 1 if i%2==j%2 else -1
                temp.append(flag*self.subMatrix(i,j).determinant()/determinant)
        return Matrix(self.width, temp).transpose()
    
    
    
        

    
