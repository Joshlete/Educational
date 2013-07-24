'''
Created on Apr 29, 2012

@author: Cengiz
'''
from math import fabs
from Math.Function import Function


def graphMethod(func, stepsize=100, stepwidth=3, epsilon=1e-3, x0=0):
    current=func(x0)
    next1=func(x0+stepwidth)
    
    while stepsize!=0 and epsilon<(fabs(current-next1)):
        if next1==0 : 
            return x0+stepwidth
        
        if current/next1 < 0:
            stepwidth/=2
        else:
            x0=x0+stepwidth
            
        stepsize-=1
        current=func(x0)
        next1=func(x0+stepwidth)
        
    return x0


def Bisection(func, lvalue, rvalue, stepsize=100, epsilon=1e-3):
    if func(lvalue)==0:
        return lvalue
    elif func(rvalue)==0:
        return rvalue
    elif func(rvalue)/func(lvalue)>0:
        raise Exception("Verilen degerler kok testini gecemedi.")
    
    if lvalue>rvalue:
        lvalue, rvalue = rvalue, lvalue
    elif lvalue==rvalue:
        raise Exception("Verilen degerler birbirine esit olamaz.")
    
    current=func((lvalue+rvalue)/2)
    prev=func(lvalue)
    
    while stepsize!=0 and epsilon<fabs(current-prev):
        if current/func(lvalue)<0:
            rvalue=(lvalue+rvalue)/2
        else:
            lvalue=(lvalue+rvalue)/2
        
        prev=current
        current=func((rvalue+lvalue)/2)
        stepsize-=1
    return (lvalue+rvalue)/2


def regulaFalse(func, lvalue, rvalue, stepsize=100, epsilon=1e-3):
    
    if func(lvalue)==0:
        return lvalue
    if func(rvalue)==0:
        return rvalue
    if func(rvalue)/func(lvalue)>0:
        raise Exception("Verilen degerler kok testini gecemedi.")
    
    if lvalue>rvalue:
        lvalue, rvalue = rvalue, lvalue
    elif lvalue==rvalue:
        raise Exception("Verilen degerler birbirine esit olamaz.")


    mean=(lvalue*func(rvalue)-rvalue*func(lvalue))/(func(rvalue)-func(lvalue))
    current=func(mean)
    prev=func(lvalue)
    
    while stepsize!=0 and epsilon<fabs(current-prev):
        if current/func(lvalue)<0:
            rvalue=mean
        else:
            lvalue=mean
            
        prev=current
        mean=(lvalue*func(rvalue)-rvalue*func(lvalue))/(func(rvalue)-func(lvalue))
        current=func(mean)
        stepsize-=1
            
    return mean
            

def newtonRaphson(func, startpoint, stepsize=100, epsilon=1e-6):
    prev=startpoint
    next1=startpoint - (func(prev)/func.derivative(prev))
    
    while stepsize!=0 and epsilon<fabs(next1-prev):
        prev=next1
        next1=prev - (func(prev)/func.derivative(prev))
        stepsize-=1        
    return next1


        
    
        
        
        
            
        
        
        
        
    
    
