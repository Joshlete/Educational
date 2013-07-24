'''
Created on May 27, 2012

@author: Cengiz
'''


import sys
from PyQt4 import QtCore, QtGui

from GUI.Interface import *
from Math.Function import *
from Math.Matrix import *
from Algorithm.Algorithm import *
from math import *

class NAP(QtGui.QMainWindow):
        def __init__(self, parent=None):
            QtGui.QWidget.__init__(self, parent)
            self.ui=Ui_MainWindow()
            self.ui.setupUi(self)
            
            self.ui.pushBRoot.clicked.connect(self.RootCallback)
            self.ui.pushBMatrix.clicked.connect(self.MatrixCallback)
            self.ui.pushBTurev.clicked.connect(self.TurevCallback)
            
            self.ui.konsolTemizle.clicked.connect(self.konsolTemizle)
            self.ui.matrixTemizle.clicked.connect(self.matrisTemizle)
            self.ui.kokTemizle.clicked.connect(self.kokTemizle)
            self.ui.turevTemizle.clicked.connect(self.turevTemizle)

        def RootCallback(self):
            
            try:
                F=Function(self.ui.RootFIn.text())
                bas=float(eval(str(self.ui.Rootx0.text())))
                son=float(eval(str(self.ui.Rootx1.text())))
                maxiter=float(eval(str(self.ui.RootMaxIter.text())))
                epsilon=float(eval(str(self.ui.RootEpsilon.text())))
            except:
                self.ui.textBrowser.append("Lutfen degerleri dogru giriniz. Kullanmayacaginiz bir degisken varsa degerini 0 olarak birakiniz.")
                return
            
            try:
                
                if self.ui.radioRoot.isChecked():
                    value=graphMethod(F, stepsize=maxiter, stepwidth=son, epsilon=epsilon, x0=bas)
                    self.ui.textBrowser.append("Grafik Yontemi ile bulunan deger : " + str(value))
                    
                elif self.ui.radioRoot2.isChecked():
                    value=regulaFalse(func=F, lvalue=bas, rvalue=son, stepsize=maxiter, epsilon=epsilon)
                    self.ui.textBrowser.append("Regula False ile bulunan deger : " + str(value))
                    
                elif self.ui.radioRoot3.isChecked():
                    value=Bisection(stepsize=maxiter, epsilon=epsilon, lvalue=bas, rvalue=son, func=F)
                    self.ui.textBrowser.append("Yariya Bolme ile bulunan deger : " + str(value))
                    
                elif self.ui.radioRoot4.isChecked():
                    value=newtonRaphson(func=F, startpoint=bas, stepsize=maxiter, epsilon=epsilon)
                    self.ui.textBrowser.append("Newton-Raphson ile bulunan deger : " + str(value))
                    
            except Exception as exc:
                self.ui.textBrowser.append("Hata : " + exc.args[0])
        
        def MatrixCallback(self):
            matr=self.ui.MatrixIn.toPlainText()
            matr=str(matr).split()
            matr=[float(element) for element in matr]
            
            dt=self.ui.DTIn.text()
            dt=str(dt).split(" ")
            dt=[float(element) for element in dt]
            
            
            if sqrt(len(matr))-int(sqrt(len(matr)))!=0 or  matr[0]=='':
                self.ui.textBrowser.append("Girilen matris kare ve bos olmayan bir matris olmalidir.")
                return
            
            matr=Matrix(int(sqrt(len(matr))), matr)
            try:
                if self.ui.radioMatrix.isChecked():
                    self.ui.textBrowser.append("Cozum takimi : " + str(matr.gaussElimination(dt)))
                
                elif self.ui.radioMatrix2.isChecked():
                    self.ui.textBrowser.append("Invers Matris :")
                    for row in matr.inverse().matrix:
                        self.ui.textBrowser.append(str(row))
                    
                elif self.ui.radioMatrix3.isChecked():
                    self.ui.textBrowser.append("Determinant : " + str(matr.determinant()))
            except Exception as exc:
                self.ui.textBrowser.append(exc.args[0])

        def TurevCallback(self):
            
            try:
                f=Function(str(self.ui.TurevFIn.text()))
                bas=float(eval(str(self.ui.Turevx0.text())))
                son=float(eval(str(self.ui.Turevx1.text())))
                epsilon=float(eval(str(self.ui.TurevEpsilon.text())))
                maxiter=int(eval(str(self.ui.TurevMaxIter.text())))
            except Exception as exc:
                self.ui.textBrowser.append("Lutfen degerleri dogru giriniz. Kullanmayacaginiz bir deger varsa 0 olarak birakiniz")
                self.ui.textBrowser.append("Hata : " + exc.args[0])
                return
                
            try:    
                if self.ui.radioTurev.isChecked():
                    self.ui.textBrowser.append("Yamuk Methodu ile hesaplanan deger : " + str(f.trapezoidmethod(bas, son, maxiter)))
                elif self.ui.radioTurev2.isChecked():
                    self.ui.textBrowser.append("Simpson Methodu ile hesaplanan deger : " + str(f.simpsonmethod(maxiter, bas, son)))
                elif self.ui.radioInt.isChecked():
                    self.ui.textBrowser.append("Geri turev ile hesaplanan deger : " + str(f.backwardDiff(bas, epsilon)))
                elif self.ui.radioInt2.isChecked():
                    self.ui.textBrowser.append("Merkezi fark ile hesaplanan deger : " + str(f.centralDiff(bas, epsilon)))
                elif self.ui.radioInt3.isChecked():
                    self.ui.textBrowser.append("Ileri turev ile hesaplanan deger : " + str(f.forwardDiff(bas, epsilon)))
            except Exception as exc:
                self.ui.textBrowser.append("Hata : " + exc.args[0])

        def konsolTemizle(self):
            self.ui.textBrowser.clear()
        
        def matrisTemizle(self):
            self.ui.MatrixIn.setText("")
            self.ui.DTIn.setText('0')
            
        def kokTemizle(self):
            self.ui.RootEpsilon.setText('0')
            self.ui.RootFIn.setText('0')
            self.ui.RootMaxIter.setText('0')
            self.ui.Rootx0.setText('0')
            self.ui.Rootx1.setText('0')
        
        def turevTemizle(self):
            self.ui.TurevFIn.setText('0')
            self.ui.TurevEpsilon.setText('0')
            self.ui.TurevMaxIter.setText('0')
            self.ui.Turevx0.setText('0')
            self.ui.Turevx1.setText('0')
            
app=QtGui.QApplication(sys.argv)
myapp=NAP()
myapp.show()
app.exec_()
