'''
Created on Apr 28, 2012

@author: Cengiz
'''
from math import *
from Error.BadInputError import BadInputError

class Function(object):
    
    step=1e-5
    
    def __init__(self,polynomial):
        exec("self.function = lambda x : {0}".format(polynomial)) 
        
    def __call__(self,arg): 
        return self.function(arg) 
    
    def derivative(self,arg):
        return (self.function(arg+self.step)-self.function(arg))/self.step \
    
    def trapezoidmethod(self, startpoint, endpoint, stepCount):
        if startpoint>endpoint:
            startpoint, endpoint=endpoint, startpoint
        if stepCount <= 0:
            raise TypeError("Adim genisligini duzgun giriniz")
        
        sum=0
        stepsize=(endpoint-startpoint)/float(stepCount)
        prev=startpoint
        next=startpoint+stepsize
        
        for i in xrange(stepCount):
            sum+=stepsize*(self(prev)+self(next))/2.0
            prev, next=next, next+stepsize
        
        return sum
    
    def simpsonmethod(self, enterpolationCount, lvalue, rvalue):
        
        if lvalue>rvalue:
            lvalue, rvalue = rvalue, lvalue
        elif lvalue==rvalue:
            raise BadInputError("Aralik degerleri birbirine esit olamaz.")
        if enterpolationCount<=0:
            raise BadInputError("Enterpolasyon Sayisi (Adim sayisi) sifirdan kucuk veya esit olamaz.")
        
        stepsize=(rvalue-lvalue)/(enterpolationCount*2)
        sum=0
        enterpolationIndex=lvalue
        
        for i in xrange(enterpolationCount):
            print self(enterpolationIndex), self(enterpolationIndex+stepsize), self(enterpolationIndex+stepsize*2)
            sum+= (self(enterpolationIndex)+self(enterpolationIndex+stepsize)*4+self(enterpolationIndex+stepsize*2))*(stepsize/3)
            enterpolationIndex+=stepsize*2
        
        return sum
   
    def centralDiff(self, point, epsilon):
        return (self(point+epsilon)-self(point-epsilon))/(2*epsilon)
    
    def forwardDiff(self, point, epsilon):
        return (self(point+epsilon)-self(point))/epsilon
    
    def backwardDiff(self, point, epsilon):
        return (self(point)-self(point-epsilon))/epsilon
    
